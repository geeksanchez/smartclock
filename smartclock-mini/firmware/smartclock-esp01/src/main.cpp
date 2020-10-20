#include "WiFiTask.h"
#include "NTPTask.h"
#include "MQTTTask.h"
#include "CMDTask.h"

#define VERSION "0.5s"

WiFiTask wifiClient;
NTPTask ntpClient;
MQTTTask mqttTaskClient;
CMDTask cmdClient;

void ntpUpdate();
void mqttUpdate();

void ntpUpdate()
{
  char msg[256];
  snprintf(msg, 256, "NTP %lu", ntpClient.epochTime);
  cmdClient.sendMSG(msg);
}

void mqttUpdate()
{
  cmdClient.sendMSG(mqttTaskClient.command);
  char title[50];
  char msg[256];
  char *rest = mqttTaskClient.command;
  char *token = strtok_r(rest, " ", &rest);
  if (token != NULL)
  {
    snprintf(title, 50, "sansila/smartclock/smartclock-%X/salida", ESP.getChipId());
    if (strcmp(token, "WIFI") == 0)
    {
      snprintf(msg, 256, "SSID: %s", wifiClient.wifi->getWifiSsidParameter()->valueBuffer);
      mqttTaskClient.publish(title, msg);
    }
    else if (strcmp(token, "TIME") == 0)
    {
      snprintf(msg, 256, "TIME: %lu", ntpClient.epochTime);
      mqttTaskClient.publish(title, msg);
    }
    else if (strcmp(token, "INFO") == 0)
    {
      snprintf(msg, 256, "INFO: UPTIME:%lums CPU:%dMHz %s", millis(), ESP.getCpuFreqMHz(), ESP.getFullVersion().c_str());
      mqttTaskClient.publish(title, msg);
    }
  }
}

void cmdUpdate(char *msg)
{
  char title[50];
  snprintf(title, 50, "sansila/smartclock/smartclock-%X/salida", ESP.getChipId());
  mqttTaskClient.publish(title, msg);
}

void setup()
{
  Scheduler.start(&wifiClient);
  ntpClient.notifyNTP = &ntpUpdate;
  Scheduler.start(&ntpClient);
  mqttTaskClient.notifyMQTT = &mqttUpdate;
  Scheduler.start(&mqttTaskClient);
  cmdClient.recvMSG = &cmdUpdate;
  Scheduler.start(&cmdClient);
  Scheduler.begin();
}

void loop()
{
}
