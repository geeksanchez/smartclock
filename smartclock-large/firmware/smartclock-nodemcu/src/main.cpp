//#include "MicSensorTask.h"
//#include "LuxSensorTask.h"
//#include "RTCTask.h"
#include "SmartclockTask.h"
#include <ESP8266WiFi.h>
#include "MatrixTask.h"
#include "WiFiTask.h"
#include "NTPTask.h"
#include "MQTTTask.h"
#include "OTATask.h"

#define VERSION "0.9"

SmartclockTask smartclockClient;
MatrixTask matrixClient;
//MicSensorTask micSensor;
//LuxSensorTask luxSensor;
//RTCTask rtcClient;
WiFiTask wifiClient;
NTPTask ntpClient;
MQTTTask mqttTaskClient;
OTATask otaTaskClient;

char lastTime[10];

void matrixUpdate();
//void clapUpdate();
//void luxUpdate();
//void rtcUpdate();
void ntpUpdate();
void mqttUpdate();
void otaUpdate() ;

void smartclockUpdate(uint8_t state)
{
  if (state == 1)
  {
    matrixClient.DrawIcon(1);
  }
}

void matrixUpdate(char *msg)
{
  Serial.printf(msg);
}
/*
void clapUpdate()
{
  char title[50];
  snprintf(title, 50, "sansila/smartclock/smartclock-%X/salida", ESP.getChipId());
  mqttTaskClient.publish(title, "CLAP");
}
*/
/*
void luxUpdate()
{
  Serial.printf("Lux: %d\n", luxSensor.lux);
}
*/
/*
void rtcUpdate()
{
  char msg[10];
  if (rtcClient.h < 12)
  {
    if (rtcClient.h == 0)
    {
      rtcClient.h = 12;
    }
    if (rtcClient.m < 10)
    {
      snprintf(msg, TIME_BUFFER_SIZE, "%d0%d", rtcClient.h, rtcClient.m);
    }
    else
    {
      snprintf(msg, TIME_BUFFER_SIZE, "%d%d", rtcClient.h, rtcClient.m);
    }
  }
  else
  {
    if (rtcClient.h == 12)
    {
      if (rtcClient.m < 10)
      {
        snprintf(msg, TIME_BUFFER_SIZE, "%d0%d", rtcClient.h, rtcClient.m);
      }
      else
      {
        snprintf(msg, TIME_BUFFER_SIZE, "%d%d", rtcClient.h, rtcClient.m);
      }
    }
    else
    {
      if (rtcClient.m < 10)
      {
        snprintf(msg, TIME_BUFFER_SIZE, "%d0%d", (rtcClient.h - 12), rtcClient.m);
      }
      else
      {
        snprintf(msg, TIME_BUFFER_SIZE, "%d%d", (rtcClient.h - 12), rtcClient.m);
      }
    }
  }
  if (strcmp(lastTime, msg) != 0)
  {
    matrixClient.DrawTime(msg);
    char title[50];
    snprintf(title, 50, "sansila/smartclock/smartclock-%X/salida", ESP.getChipId());
    mqttTaskClient.publish(title, msg);
    strcpy(lastTime, msg);
  }

  char MO[3], DA[3];
  if (rtcClient.D < 10)
  {
    snprintf(DA, 3, "0%d", rtcClient.D);
  }
  else
  {
    snprintf(DA, 3, "%d", rtcClient.D);
  }
  if (rtcClient.M < 10)
  {
    snprintf(MO, 3, "0%d", rtcClient.M);
  }
  else
  {
    snprintf(MO, 3, "%d", rtcClient.M);
  }
  snprintf(msg, DATE_BUFFER_SIZE, "%s%s%d", DA, MO, rtcClient.Y);
}
*/
void ntpUpdate()
{
  char title[50];
  char msg[256];
  snprintf(msg, MSG_BUFFER_SIZE, "%02d%02d", ntpClient.h, ntpClient.m);
  if (strcmp(lastTime, msg) != 0)
  {
    snprintf(title, 50, "sansila/smartclock/smartclock-%X/salida", ESP.getChipId());
    mqttTaskClient.publish(title, msg);
    snprintf(lastTime, 10, "%s", msg);
    matrixClient.DrawIcon(2);
    matrixClient.DrawTime(msg);
  }
}

void mqttUpdate()
{
  char title[50];
  char msg[256];
  char *rest = mqttTaskClient.command;
  char *token = strtok_r(rest, " ", &rest);
  if (token != NULL)
  {
    snprintf(title, 50, "sansila/smartclock/smartclock-%X/salida", ESP.getChipId());
    if (strcmp(token, "INFO") == 0)
    {
      snprintf(msg, MSG_BUFFER_SIZE, "INFO: VERSION=%s UPTIME=%lums CPU=%dMHz %s", VERSION, millis(), ESP.getCpuFreqMHz(), ESP.getFullVersion().c_str());
      mqttTaskClient.publish(title, msg);
    }
    else if (strcmp(token, "WIFI") == 0)
    {
      snprintf(msg, MSG_BUFFER_SIZE, "SSID: %s", wifiClient.wifi->getWifiSsidParameter()->valueBuffer);
      mqttTaskClient.publish(title, msg);
    }
    else if (strcmp(token, "ICON") == 0)
    {
      if (rest != NULL)
      {
        snprintf(msg, MSG_BUFFER_SIZE, "%s", rest);
        matrixClient.DrawIcon(atoi(msg));
      }
    }
    else if (strcmp(token, "LUX") == 0)
    {
      if (rest != NULL)
      {
        snprintf(msg, MSG_BUFFER_SIZE, "%s", rest);
        matrixClient.setBrightness(atoi(msg));
      }
    }
    else if (strcmp(token, "TIME") == 0)
    {
      if (rest != NULL)
      {
        snprintf(msg, MSG_BUFFER_SIZE, "%s", rest);
        matrixClient.DrawTime(msg);
      }
    }
    else if (strcmp(token, "IPADDR") == 0)
    {
      snprintf(msg, MSG_BUFFER_SIZE, "%s", WiFi.localIP().toString().c_str());
      mqttTaskClient.publish(title, msg);
    }
    else if (strcmp(token, "RESET") == 0)
    {
      snprintf(msg, MSG_BUFFER_SIZE, "%X", ESP.getChipId());
      if (strcmp(msg, rest) == 0)
      {
        ESP.restart();
      }
    }
  }
}

void otaUpdate()
{
  Serial.println(otaTaskClient.msg);
}

void setup()
{
  Serial.begin(115200);

  snprintf(lastTime, 10, "0000");

  smartclockClient.notifySmartclock = smartclockUpdate;
  Scheduler.start(&smartclockClient);
  matrixClient.notifyMatrix = &matrixUpdate;
  Scheduler.start(&matrixClient);
//  micSensor.setPin(D4);
//  micSensor.notifyClap = &clapUpdate;
//  Scheduler.start(&micSensor);
//  luxSensor.notifyLux = &luxUpdate;
//  luxSensor.lux = 10;
//  Scheduler.start(&luxSensor);
//  rtcClient.notifyRTC = &rtcUpdate;
//  Scheduler.start(&rtcClient);
  Scheduler.start(&wifiClient);
  ntpClient.notifyNTP = &ntpUpdate;
  Scheduler.start(&ntpClient);
  mqttTaskClient.notifyMQTT = &mqttUpdate;
  Scheduler.start(&mqttTaskClient);
  otaTaskClient.notifyOTA = &otaUpdate;
  Scheduler.start(&otaTaskClient);
  
  Scheduler.begin();
}

void loop()
{
}
