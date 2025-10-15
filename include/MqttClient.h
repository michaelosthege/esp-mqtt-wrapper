#pragma once

#include <functional>
#include "esp_event.h"
#include "mqtt_client.h"

typedef std::function<void(const char* topic, const char* payload, size_t length)> MessageCallback;
typedef std::function<void()> SimpleCallback;

class MqttClient {
public:
  static MqttClient* getInstance();

  void begin(const char* brokerUri);
  void setServer(const char* host, uint16_t port);
  void setCredentials(const char* username, const char* password);
  void setKeepalive(uint16_t keepalive);
  void setProtocolFallback(bool enableFallback); // Enable v3.1.1 fallback if v5 fails

  bool connect(const char* clientId);
  void disconnect();
  bool isConnected() const;

  // Communication
  int publish(const char* topic, const char* payload, bool retain = false);
  int subscribe(const char* topic, int qos = 0);
  int unsubscribe(const char* topic);

  // Event callbacks
  void onMessage(MessageCallback cb);
  void onConnect(SimpleCallback cb);
  void onDisconnect(SimpleCallback cb);

  // Processing
  void loop(); // No-op for event-driven esp-mqtt

  ~MqttClient();

private:
  MqttClient();
  static MqttClient* _instance;

  void* _client; // esp_mqtt_client_handle_t
  char* _host;
  uint16_t _port;
  char* _username;
  char* _password;
  char* _clientId;
  uint16_t _keepalive;
  bool _connected;

  // Fallback configuration
  bool _enableFallback;
  bool _usingFallback;

  bool connectWithProtocol(esp_mqtt_protocol_ver_t protocol);
  void reconnectWithFallback();

  MessageCallback _messageCallback;
  SimpleCallback _connectCallback;
  SimpleCallback _disconnectCallback;

  void parseUriComponents(const char* uri);
  void handleMessage(const char* topic, const char* payload);

public:
  void onConnectedInternal();
  void onDisconnectedInternal();
  void onDataInternal(const char* topic, const char* data, int data_len);
};