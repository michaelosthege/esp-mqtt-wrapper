#pragma once

#include <stdint.h>
#include <string>

// Lightweight URI parsing/building focused on MQTT and WebSocket transports
// Supported schemes: mqtt, mqtts, ws, wss

struct UriParts {
  std::string scheme;   // "mqtt", "mqtts", "ws", "wss"
  std::string host;     // hostname or IP
  uint16_t port = 0;    // port; defaults based on scheme if 0
  std::string path;     // optional path for ws/wss (e.g., "/mqtt")

  bool isWebSocket() const { return scheme == "ws" || scheme == "wss"; }
  bool isSecure() const { return scheme == "mqtts" || scheme == "wss"; }
};

// Parse a URI into parts. Returns true on success.
bool parseMqttUri(const std::string &uri, UriParts &out);

// Build a URI string from parts. If path is set for ws/wss, it's appended.
std::string buildMqttUri(const UriParts &parts);
