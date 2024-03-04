//
// ESPressIoT Controller for Espresso Machines
// 2016-2021 by Roman Schmitz
//
// MQTT integration
//

#ifdef ENABLE_MQTT

#include <SPI.h>
#include <Ethernet.h>
#include <WiFiClient.h>
#include <PubSubClient.h>

WiFiClient espClient;
PubSubClient client(espClient);

const char* mqttConfigTopic = MQTT_TOPIC "/config/#";
const char* mqttStatusTopic = MQTT_TOPIC "/status";

void MQTT_reconnect() {
  if (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    String clientId = "ESP8266Client-" + String(random(0xffff), HEX);
    if (client.connect(clientId.c_str(), MQTT_USER, MQTT_PASS)) {
      Serial.println("connected");
      client.subscribe(mqttConfigTopic, 1);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
    }
  }
}

void MQTT_callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] '");

  String msg = "";
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
    msg += (char)payload[i];
  }
  Serial.println("'");

  double val = msg.toFloat();
  Serial.println(val);

  if (strstr(topic, "/config/tset")) {
    if (val > 1e-3) gTargetTemp = val;
  } else if (strstr(topic, "/config/toggle")) {
    poweroffMode = !poweroffMode;
  }
}

void setupMQTT() {
  client.setServer(MQTT_HOST, MQTT_PORT);
  client.setCallback(MQTT_callback);
}

void loopMQTT() {
  for (int i = 0; i < MAX_CONNECTION_RETRIES && !client.connected(); i++) {
    MQTT_reconnect();
    Serial.print(".");
    MQTT_reconnect();
  }

  client.loop();
  client.publish(mqttStatusTopic, gStatusAsJson.c_str());
}

#endif // ENABLE_MQTT
