#include <Arduino.h>
#include "WiFiTask.h"
#include <IotWebConf.h>
#include <FS.h>
#include <LittleFS.h>
#include "MQTTTask.h"
#include "NTPTask.h"
#include "CMDTask.h"
#include "main.h"

WiFiTask wifiClient;
NTPTask ntpClient;
MQTTTask mqttTaskClient;
CMDTask cmdClient;

int fs_enabled;
int state;
bool state_done;
os_timer_t eventTimer;
String currentTime, currentDate, MName, Message;

void eventProcess(int8_t event);

void clockUpdate()
{
}

void cmdUpdate()
{
}

void ntpUpdate(unsigned long t)
{
  char msg[256];
  snprintf(msg, 256, "NTP %lu", t);
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
    else if (strcmp(token, "IPADDR") == 0)
    {
      snprintf(msg, MSG_BUFFER_SIZE, "%s", WiFi.localIP().toString().c_str());
      mqttTaskClient.publish(title, msg);
    }
    else if (strcmp(token, "RESET") == 0)
    {
      if (rest != NULL)
      {
        snprintf(msg, MSG_BUFFER_SIZE, "%X", ESP.getChipId());
        if (strcmp(msg, rest) == 0)
        {
          ESP.restart();
        }
      }
    }
  }
}

void notifyClock(void *pArg)
{
  eventProcess(CLOCK_EVENT);
}

void notifyCMD(char *msg)
{
  char title[50];
  snprintf(title, 50, "sansila/smartclock/smartclock-%X/salida", ESP.getChipId());
  mqttTaskClient.publish(title, msg);
  eventProcess(CMD_EVENT);
}

void notifyMQTT()
{
  eventProcess(MQTT_EVENT);
}

void notifyNTP()
{
  eventProcess(NTP_EVENT);
}

void eventProcess(int8_t event)
{
  switch (event)
  {
  case CLOCK_EVENT:
    Serial.printf("CLOCK EVENT\n");
    clockUpdate();
    break;
  case MQTT_EVENT:
    Serial.printf("MQTT EVENT\n");
    mqttUpdate();
    break;
  case NTP_EVENT:
    Serial.printf("NTP EVENT: %lu\n", ntpClient.epochTime);
    ntpUpdate(ntpClient.epochTime);
    break;
  case CMD_EVENT:
    Serial.printf("CMD EVENT: %lu\n", ntpClient.epochTime);
    cmdUpdate();
    break;
  default:
    break;
  }
}

void setup()
{
  Serial.begin(115200);
  delay(1000);
  pinMode(BEEP_PIN, OUTPUT);
  state = 0;
  state_done = true;
  Message = "";
  if (!LittleFS.begin())
  {
    fs_enabled = false;
  }
  else
  {
    fs_enabled = true;
  }
  Serial.println(F("Smartclock starting..."));
  currentTime = "--:--";
  currentDate = "--/--";

  digitalWrite(BEEP_PIN, LOW);
  delay(100);
  digitalWrite(BEEP_PIN, HIGH);

  os_timer_setfn(&eventTimer, notifyClock, NULL);
  os_timer_arm(&eventTimer, 5000, true);

  Scheduler.start(&wifiClient);
  ntpClient.notify = &notifyNTP;
  Scheduler.start(&ntpClient);
  mqttTaskClient.notify = &notifyMQTT;
  Scheduler.start(&mqttTaskClient);
  cmdClient.recvMSG = &notifyCMD;
  Scheduler.start(&cmdClient);
  Scheduler.begin();
}

void loop()
{
}
