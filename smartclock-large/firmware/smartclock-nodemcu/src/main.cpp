#include "constants.h"
#include "SmartclockTask.h"
#include <ESP8266WiFi.h>
#include "MatrixTask.h"
#include "WiFiTask.h"
#include "NTPTask.h"
#include "MQTTTask.h"
#include "OTATask.h"

#define VERSION "0.10"

SmartclockTask smartclockClient;
MatrixTask matrixClient;
WiFiTask wifiClient;
NTPTask ntpClient;
MQTTTask mqttTaskClient;
OTATask otaTaskClient;

enum ESTADOS {STILL, REINICIO, RELOJ, FECHA, MENSAJE, ICON, LUX};

char lastTime[10];

void matrixUpdate();
void ntpUpdate();
void mqttUpdate();
void otaUpdate() ;

void smartclockUpdate(uint8_t state, char *msg, int len)
{
  switch (state)
  {
  case REINICIO:
    matrixClient.DrawIcon(REINICIO);
    matrixClient.show();
    break;
  case RELOJ:
    matrixClient.DrawMsg(msg, len, TIME_MODE);
    matrixClient.DrawIcon(RELOJ);
    matrixClient.show();
    break;
  case FECHA:
    matrixClient.DrawMsg(msg, len, DATE_MODE);
    matrixClient.DrawIcon(FECHA);
    matrixClient.show();
    break;
  case ICON:
    matrixClient.DrawIcon(atoi(msg));
    matrixClient.show();
    break;
  case MENSAJE:
    matrixClient.DrawMsg(msg, len, TEXT_MODE);
    matrixClient.DrawIcon(MENSAJE);
    matrixClient.show();
    break;
  case LUX:
    matrixClient.setBrightness(atoi(msg));
    matrixClient.show();
    break;
  default:
    break;
  }
}

void matrixUpdate(char *msg)
{
}

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
    smartclockUpdate(RELOJ, msg, 4);
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
    else if (strcmp(token, "MSG") == 0)
    {
      if (rest != NULL)
      {
        int len = snprintf(msg, MSG_BUFFER_SIZE, "%s", rest);
        smartclockUpdate(MENSAJE, msg, len);
      }
    }
    else if (strcmp(token, "TIME") == 0)
    {
      if (rest != NULL)
      {
        int len = snprintf(msg, MSG_BUFFER_SIZE, "%s", rest);
        smartclockUpdate(RELOJ, msg, len);
      }
    }
    else if (strcmp(token, "ICON") == 0)
    {
      if (rest != NULL)
      {
        snprintf(msg, MSG_BUFFER_SIZE, "%s", rest);
        smartclockUpdate(ICON, msg, 1);
      }
    }
    else if (strcmp(token, "LUX") == 0)
    {
      if (rest != NULL)
      {
        snprintf(msg, MSG_BUFFER_SIZE, "%s", rest);
        smartclockUpdate(LUX, msg, 1);
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

void otaUpdate(char *msg)
{
  char title[50];
  snprintf(title, 50, "sansila/smartclock/smartclock-%X/salida", ESP.getChipId());
  mqttTaskClient.publish(title, msg);
}

void setup()
{
  Serial.begin(115200);

  snprintf(lastTime, 10, "0000");

  matrixClient.notifyMatrix = &matrixUpdate;
  Scheduler.start(&matrixClient);
  Scheduler.start(&wifiClient);
  ntpClient.notifyNTP = &ntpUpdate;
  Scheduler.start(&ntpClient);
  mqttTaskClient.notifyMQTT = &mqttUpdate;
  Scheduler.start(&mqttTaskClient);
  otaTaskClient.notifyOTA = &otaUpdate;
  Scheduler.start(&otaTaskClient);
  smartclockClient.notifySmartclock = smartclockUpdate;
  Scheduler.start(&smartclockClient);

  Scheduler.begin();
}

void loop()
{
}
