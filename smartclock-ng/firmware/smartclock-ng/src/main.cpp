#include <Arduino.h>
#include <WiFiManager.h>
#include <TaskScheduler.h>
#include <ArduinoOTA.h>
#include <WiFiUdp.h>
#include <NTPClient.h>
#include <FastLED.h>        //https://github.com/FastLED/FastLED
#include <LEDMatrix.h>      //https://github.com/Jorgen-VikingGod/LEDMatrix

// Change the next defines to match your matrix type and size
#define DATA_PIN            2
#define COLOR_ORDER         GRB
#define CHIPSET             WS2812B

// initial matrix layout (to get led strip index by x/y)
#define MATRIX_WIDTH        16
#define MATRIX_HEIGHT       16
#define MATRIX_TYPE         HORIZONTAL_ZIGZAG_MATRIX
#define MATRIX_SIZE         (MATRIX_WIDTH*MATRIX_HEIGHT)
#define NUMPIXELS           MATRIX_SIZE
#define NUM_LEDS            (MATRIX_WIDTH*MATRIX_HEIGHT)


void tickLEDCallback();
bool setupOTACallback();
void handleOTACallback();
bool ntpSetupCallback();
void ntpUpdateCallback();
bool setupLEDCallback();

Scheduler ts;

Task timeLEDTick(1000, TASK_FOREVER, &tickLEDCallback, &ts, true, &setupLEDCallback);
Task tOTA(TASK_IMMEDIATE, TASK_FOREVER, &handleOTACallback, &ts, false, &setupOTACallback);
Task tupdateTime(TASK_MINUTE, TASK_FOREVER, &ntpUpdateCallback, &ts, false, &ntpSetupCallback);

WiFiManager wm;

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", (-5 * 60 * 60));
unsigned long epochTime;

cLEDMatrix<MATRIX_WIDTH, MATRIX_HEIGHT, MATRIX_TYPE> leds;
int led_x_pos, led_y_pos;

void connectWiFi() {
  WiFi.mode(WIFI_STA);
  bool res;
  char ssid[23];
  snprintf(ssid, 23, "smartclock-ng-%04X", ESP.getChipId());
  wm.setConfigPortalBlocking(false);
  res = wm.autoConnect(ssid, "password");
  if (!res) {
    Serial.println("Failed to connect");
  } else {
    Serial.println("Configportal running");
  }
}

void setup() {
  Serial.begin(115200);
  connectWiFi();
  Serial.println("Connected!!!");
  tOTA.enable();
  tupdateTime.enable();
}

void loop() {
  wm.process();
  ts.execute();
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
  Serial.println(WiFi.SSID());
}

void matrix_clear() {
    //FastLED[1].clearLedData();
    // clear does not work properly with multiple matrices connected via parallel inputs
    memset(leds[0], 0, NUM_LEDS*3);
}

bool setupLEDCallback() {
  led_x_pos = 0;
  led_y_pos = 0;
  // initial LEDs
  FastLED.addLeds<CHIPSET, DATA_PIN, COLOR_ORDER>(leds[0], leds.Size()).setCorrection(TypicalSMD5050);
  FastLED.setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(32);
  FastLED.clear(true);
  return true;
}

void tickLEDCallback() {
  leds.DrawPixel(led_x_pos, led_y_pos, CRGB(0, 0, 0));
  if (led_x_pos == 15) {
    led_x_pos = 0;
    if (led_y_pos == 15) {
      led_y_pos = 0;
    } else {
      led_y_pos++;
    }
  } else {
    led_x_pos++;
  }
  leds.DrawPixel(led_x_pos, led_y_pos, CRGB(80, 160, 240));
}
