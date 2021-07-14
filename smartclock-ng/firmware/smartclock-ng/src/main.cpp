#include <Arduino.h>
#include <WiFiManager.h>
#include <TaskScheduler.h>
#include <ArduinoOTA.h>
#include <WiFiUdp.h>
#include <NTPClient.h>

void tickSecondCallback();
bool setupOTACallback();
void handleOTACallback();
bool ntpSetupCallback();
void ntpUpdateCallback();

Scheduler ts;

Task timeTick(1000, TASK_FOREVER, &tickSecondCallback, &ts, true);
Task tOTA(TASK_IMMEDIATE, TASK_FOREVER, &handleOTACallback, &ts, false, &setupOTACallback);
Task tupdateTime(TASK_MINUTE, TASK_FOREVER, &ntpUpdateCallback, &ts, false, &ntpSetupCallback);

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", (-5 * 60 * 60));
unsigned long epochTime;

int connectWiFi() {
  WiFi.mode(WIFI_STA);
  WiFiManager wm;
  bool res;
  char ssid[23];
  snprintf(ssid, 23, "smartclock-ng-%04X", ESP.getChipId());
  res = wm.autoConnect(ssid, "password");
  if (!res) {
    Serial.println("Failed to connect");
    ESP.restart();
  }
  return 1;
}

void setup() {
  Serial.begin(115200);
  if (connectWiFi() == 1) {
    Serial.println("Connected!!!");
    tOTA.enable();
    tupdateTime.enable();
  };
}

void loop() {
  ts.execute();
}

void tickSecondCallback() {
}

bool setupOTACallback() {
  char ssid[23];
  snprintf(ssid, 23, "smartclock-ng-%04X", ESP.getChipId());
  ArduinoOTA.setPort(8266);
  ArduinoOTA.setHostname(ssid);
  ArduinoOTA.setPassword(ssid);

  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH) {
      type = "sketch";
    } else { // U_FS
      type = "filesystem";
    }

    // NOTE: if updating FS this would be the place to unmount FS using FS.end()
    Serial.println("Start updating " + type);
  });

  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });

  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });

  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) {
      Serial.println("Auth Failed");
    } else if (error == OTA_BEGIN_ERROR) {
      Serial.println("Begin Failed");
    } else if (error == OTA_CONNECT_ERROR) {
      Serial.println("Connect Failed");
    } else if (error == OTA_RECEIVE_ERROR) {
      Serial.println("Receive Failed");
    } else if (error == OTA_END_ERROR) {
      Serial.println("End Failed");
    }
  });

  ArduinoOTA.begin();
  Serial.println("OTA Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  return true;
}

void handleOTACallback() {
  ArduinoOTA.handle();
}

bool ntpSetupCallback() {
  timeClient.begin();
  return true;
};

void ntpUpdateCallback() {
  if (timeClient.update()) {
    epochTime = timeClient.getEpochTime();
    Serial.println(timeClient.getFormattedTime());
    
  }
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
  Serial.printf("smartclock-ng-%04X\n", ESP.getChipId());
  Serial.println(WiFi.localIP());
}