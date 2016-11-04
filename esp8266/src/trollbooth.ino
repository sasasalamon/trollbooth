#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <ArduinoOTA.h>
#include <PubSubClient.h>

static const char* hostname = "esp-trollbooth";
static const char* mqttTopic = "trollbooth";
static const char* ssid = "xx77aBs";
static const char* password = "3KRgC7Gr9q84CheW";

static const int RAMP_BUTTON_PIN = 14;
static int triggerButton = 0;
static unsigned long lastTriggerMillis = 0;
WiFiClient wifiClient;
IPAddress mqttIP(192, 168, 1, 44);
PubSubClient mqttClient(wifiClient, mqttIP);

void (*loop_pointer)(void);
void safe_loop();
void normal_loop();

void setup_ota() {
  ArduinoOTA.setHostname(hostname);
  ArduinoOTA.onStart([]() { Serial.println("Start"); });
  ArduinoOTA.onEnd([]() { Serial.println("\nEnd"); });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });

  ArduinoOTA.begin();
}

void setup()
{
  Serial.begin(9600);
  WiFi.setAutoReconnect(true);
  WiFi.persistent(true);
  WiFi.mode(WIFI_STA);
  WiFi.hostname(hostname);
  WiFi.begin(ssid, password);

  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Booting in AP mode...");
    WiFi.softAP("ESP Uploader");
    Serial.print("AP IP address: ");
    Serial.println(WiFi.softAPIP());
    setup_ota();
    loop_pointer = safe_loop;
    return;
  }

  setup_ota();
  pinMode(RAMP_BUTTON_PIN, OUTPUT);
  loop_pointer = normal_loop;
}

void mqttCallback(const MQTT::Publish& pub) {
  if (pub.topic() == mqttTopic && pub.payload_string() == "1") {
    triggerButton = 1;
  }
}

void safe_loop() {
  ArduinoOTA.handle();
}

void normal_loop() {
  ArduinoOTA.handle();

  if (!mqttClient.connected()) {
    if (mqttClient.connect(hostname)) {
      mqttClient.set_callback(mqttCallback);
      mqttClient.subscribe(mqttTopic);
    }
  } else {
    mqttClient.loop();
  }

  if (triggerButton) {
    triggerButton = 0;
    lastTriggerMillis = millis();
    digitalWrite(RAMP_BUTTON_PIN, HIGH);
  }
  if (lastTriggerMillis && millis() - lastTriggerMillis > 200) {
    lastTriggerMillis = 0;
    digitalWrite(RAMP_BUTTON_PIN, LOW);
  }
}

void loop() { loop_pointer(); }
