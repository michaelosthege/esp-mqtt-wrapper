#include "UriUtils.h"

#include <algorithm>

static inline std::string toLower(std::string s) {
  std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c) { return (char)tolower(c); });
  return s;
}

static uint16_t defaultPortForScheme(const std::string &scheme) {
  if (scheme == "mqtt") return 1883;
  if (scheme == "mqtts") return 8883;
  if (scheme == "ws") return 80;
  if (scheme == "wss") return 443;
  return 0;
}

bool parseMqttUri(const std::string &uri, UriParts &out) {
  // Expect scheme://host[:port][/path]
  auto posScheme = uri.find("://");
  if (posScheme == std::string::npos) return false;

  out.scheme = toLower(uri.substr(0, posScheme));

  std::string rest = uri.substr(posScheme + 3);

  // Find path start
  auto posSlash = rest.find('/');
  std::string authority = posSlash == std::string::npos ? rest : rest.substr(0, posSlash);
  out.path = posSlash == std::string::npos ? std::string() : rest.substr(posSlash);

  // Split host:port
  auto posColon = authority.rfind(':');
  if (posColon != std::string::npos) {
    out.host = authority.substr(0, posColon);
    std::string portStr = authority.substr(posColon + 1);
    out.port = (uint16_t)std::max(0, std::stoi(portStr));
  } else {
    out.host = authority;
    out.port = 0; // will set default later
  }

  if (out.port == 0) {
    out.port = defaultPortForScheme(out.scheme);
  }

  // Normalize empty path
  if (!out.isWebSocket()) {
    out.path.clear();
  } else {
    if (out.path.empty()) out.path = "/"; // WebSocket path defaults to "/"
  }

  return !out.scheme.empty() && !out.host.empty();
}

std::string buildMqttUri(const UriParts &parts) {
  std::string uri;
  uri.reserve(32 + parts.host.size() + parts.path.size());
  uri += parts.scheme;
  uri += "://";
  uri += parts.host;
  if (parts.port != 0 && parts.port != defaultPortForScheme(parts.scheme)) {
    uri += ":";
    uri += std::to_string(parts.port);
  }
  if (parts.isWebSocket()) {
    if (!parts.path.empty()) {
      if (parts.path[0] != '/') uri += "/"; // ensure leading slash
      uri += parts.path;
    } else {
      uri += "/";
    }
  }
  return uri;
}
