#include <unity.h>
#include "UriUtils.h"

void test_parse_ws_with_path() {
  UriParts parts;
  TEST_ASSERT_TRUE(parseMqttUri("ws://broker.example.com:8080/mqtt", parts));
  TEST_ASSERT_EQUAL_STRING("ws", parts.scheme.c_str());
  TEST_ASSERT_EQUAL_STRING("broker.example.com", parts.host.c_str());
  TEST_ASSERT_EQUAL_UINT16(8080, parts.port);
  TEST_ASSERT_TRUE(parts.isWebSocket());
  TEST_ASSERT_FALSE(parts.isSecure());
  TEST_ASSERT_EQUAL_STRING("/mqtt", parts.path.c_str());
}

void test_parse_wss_default_port_and_path_default() {
  UriParts parts;
  TEST_ASSERT_TRUE(parseMqttUri("wss://iot.example.com", parts));
  TEST_ASSERT_EQUAL_STRING("wss", parts.scheme.c_str());
  TEST_ASSERT_EQUAL_STRING("iot.example.com", parts.host.c_str());
  TEST_ASSERT_EQUAL_UINT16(443, parts.port);
  TEST_ASSERT_TRUE(parts.isWebSocket());
  TEST_ASSERT_TRUE(parts.isSecure());
  TEST_ASSERT_EQUAL_STRING("/", parts.path.c_str());
}

void test_parse_mqtt_no_path() {
  UriParts parts;
  TEST_ASSERT_TRUE(parseMqttUri("mqtt://localhost:1883", parts));
  TEST_ASSERT_EQUAL_STRING("mqtt", parts.scheme.c_str());
  TEST_ASSERT_EQUAL_STRING("localhost", parts.host.c_str());
  TEST_ASSERT_EQUAL_UINT16(1883, parts.port);
  TEST_ASSERT_FALSE(parts.isWebSocket());
  TEST_ASSERT_EQUAL_STRING("", parts.path.c_str());
}

void test_build_ws_uri() {
  UriParts parts;
  parts.scheme = "ws";
  parts.host = "broker.example.com";
  parts.port = 8080;
  parts.path = "/mqtt";
  auto uri = buildMqttUri(parts);
  TEST_ASSERT_EQUAL_STRING("ws://broker.example.com:8080/mqtt", uri.c_str());
}

int main(int argc, char **argv) {
  UNITY_BEGIN();
  RUN_TEST(test_parse_ws_with_path);
  RUN_TEST(test_parse_wss_default_port_and_path_default);
  RUN_TEST(test_parse_mqtt_no_path);
  RUN_TEST(test_build_ws_uri);
  return UNITY_END();
}
