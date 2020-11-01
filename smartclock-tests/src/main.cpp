#include <Arduino.h>
#include "WiFiTask.h"
#include "OTATask.h"
#include "MatrixTask.h"
#include <FS.h>
#include <LittleFS.h>
#include "MQTTTask.h"
#include "NTPTask.h"
#include "main.h"

WiFiTask wifiClient;
OTATask otaTaskClient;
MatrixTask matrixClient;
MQTTTask mqttTaskClient;
NTPTask ntpClient;

void eventProcess(int8_t event);

int fs_enabled;
int ntp_updated;
int state;
os_timer_t myTimer;
String currentTime, currentDate;

// Visualiza un texto en la posicion (x, y)
void showText(uint16_t x, uint16_t y, String text, uint32_t colour)
{
  if (matrixClient.ready == 1)
  {
    matrixClient.ready = 0;
    matrixClient.setText(x, y + 5, text, colour);
    matrixClient.ready = 1;
  }
}

// Hace scroll del texto a mostrar
void scrollText(uint16_t x, uint16_t y, String text, uint32_t colour)
{
  if (matrixClient.ready == 1)
  {
    matrixClient.ready = 0;
    uint16_t step, x1, y1;
    for (step = 0; step < (DISPLAY_MAX_X - x); step++)
    {
      for (x1 = x; x1 < DISPLAY_MAX_X; x1++)
        for (y1 = y; y1 < DISPLAY_MAX_Y; y1++)
        {
          yield();
          matrixClient.setPixel(x1, y1, 0);
        }
      matrixClient.setText(DISPLAY_MAX_X - step, y, text, colour);
      delay(150);
    }
    matrixClient.ready = 1;
  }
}

// Visualiza un icono (BMP de 8x8) en la posicion (x, y)
void showIcon(int x, int y, String icon)
{
  uint8_t pos;

  if (matrixClient.ready == 1)
  {
    matrixClient.ready = 0;

    File bmpImage = LittleFS.open(icon, "r");

    uint8_t fHeader[sizeof(bmp_file_header_t)];
    bmpImage.read(fHeader, BMP_FILE_HEADER_SIZE);

    bmp_file_header_t fileHeader;
    pos = 0;

    fileHeader.signature = fHeader[pos] + (fHeader[pos + 1] << 8);
    pos += sizeof(fileHeader.signature);
    fileHeader.file_size = (fHeader[pos] << 0) + (fHeader[pos + 1] << 8) + (fHeader[pos + 2] << 16) + (fHeader[pos + 3] << 24);
    pos += sizeof(fileHeader.file_size);
    fileHeader.reserved = (fHeader[pos] << 0) + (fHeader[pos + 1] << 8) + (fHeader[pos + 2] << 16) + (fHeader[pos + 3] << 24);
    pos += sizeof(fileHeader.reserved);
    fileHeader.image_offset = (fHeader[pos] << 0) + (fHeader[pos + 1] << 8) + (fHeader[pos + 2] << 16) + (fHeader[pos + 3] << 24);
    pos += sizeof(fileHeader.image_offset);

    if (fileHeader.signature == 0x4D42)
    {
      uint8_t iHeader[sizeof(bmp_image_header_t)];
      bmpImage.read(iHeader, BMP_IMAGE_HEADER_SIZE);

      bmp_image_header_t imageHeader;
      pos = 0;

      imageHeader.header_size = (iHeader[pos] << 0) + (iHeader[pos + 1] << 8) + (iHeader[pos + 2] << 16) + (iHeader[pos + 3] << 24);
      pos += sizeof(imageHeader.header_size);
      imageHeader.image_width = (iHeader[pos] << 0) + (iHeader[pos + 1] << 8) + (iHeader[pos + 2] << 16) + (iHeader[pos + 3] << 24);
      pos += sizeof(imageHeader.image_width);
      imageHeader.image_height = (iHeader[pos] << 0) + (iHeader[pos + 1] << 8) + (iHeader[pos + 2] << 16) + (iHeader[pos + 3] << 24);
      pos += sizeof(imageHeader.image_height);
      imageHeader.color_planes = (iHeader[pos] << 0) + (iHeader[pos + 1] << 8);
      pos += sizeof(imageHeader.color_planes);
      imageHeader.bits_per_pixel = (iHeader[pos] << 0) + (iHeader[pos + 1] << 8);
      pos += sizeof(imageHeader.bits_per_pixel);
      imageHeader.compression_method = (iHeader[pos] << 0) + (iHeader[pos + 1] << 8) + (iHeader[pos + 2] << 16) + (iHeader[pos + 3] << 24);
      pos += sizeof(imageHeader.compression_method);
      imageHeader.image_size = (iHeader[pos] << 0) + (iHeader[pos + 1] << 8) + (iHeader[pos + 2] << 16) + (iHeader[pos + 3] << 24);
      pos += sizeof(imageHeader.image_size);
      imageHeader.horizontal_resolution = (iHeader[pos] << 0) + (iHeader[pos + 1] << 8) + (iHeader[pos + 2] << 16) + (iHeader[pos + 3] << 24);
      pos += sizeof(imageHeader.horizontal_resolution);
      imageHeader.vertical_resolution = (iHeader[pos] << 0) + (iHeader[pos + 1] << 8) + (iHeader[pos + 2] << 16) + (iHeader[pos + 3] << 24);
      pos += sizeof(imageHeader.vertical_resolution);
      imageHeader.colors_in_palette = (iHeader[pos] << 0) + (iHeader[pos + 1] << 8) + (iHeader[pos + 2] << 16) + (iHeader[pos + 3] << 24);
      pos += sizeof(imageHeader.colors_in_palette);
      imageHeader.important_colors = (iHeader[pos] << 0) + (iHeader[pos + 1] << 8) + (iHeader[pos + 2] << 16) + (iHeader[pos + 3] << 24);
      pos += sizeof(imageHeader.important_colors);

      bmpImage.seek(fileHeader.image_offset);

      uint8_t bitmap[imageHeader.image_size];
      bmpImage.read(bitmap, imageHeader.image_size);
      matrixClient.showBitmap(x + imageHeader.image_width - 1, y + imageHeader.image_height - 1, bitmap, imageHeader.image_width, imageHeader.image_height);
    }

    bmpImage.close();
    matrixClient.ready = 1;
  }
}

// Analiza los comandos recibidos por MQTT y ejecuta las acciones pertinentes
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
        snprintf(msg, MSG_BUFFER_SIZE, "%s", rest);
        scrollText(9, 2, msg, (random(0x00FFFFFF)));
      }
    }
    else if (strcmp(token, "ICON") == 0)
    {
      if (rest != NULL)
      {
        snprintf(msg, MSG_BUFFER_SIZE, "%s", rest);
        showIcon(0, 0, msg);
      }
    }
    else if (strcmp(token, "LUX") == 0)
    {
      if (rest != NULL)
      {
        snprintf(msg, MSG_BUFFER_SIZE, "%s", rest);
        matrixClient.lux(atoi(msg));
      }
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

void updateTime(unsigned long t)
{
  String NTPtime = "";

  if (((t % 86400L) / 3600) > 12)
  {
    t -= 43200;
  }
  if (((t % 86400L) / 3600) < 10)
  {
    NTPtime += "0";
  }
  NTPtime += (t % 86400L) / 3600;
  NTPtime += ":";
  if (((t % 3600) / 60) < 10)
  {
    NTPtime += "0";
  }
  NTPtime += (t % 3600) / 60;
  if (currentTime != NTPtime)
  {
    currentTime = NTPtime;
    ntp_updated = 1;
  }
}

// Actualiza el display con el icono y el texto recibidos
void renderDisplay(String icon, String text)
{
  if ((matrixClient.ready == 1) && (state != 0))
  {
    if (state != 0)
    {
      matrixClient.clear();
      showIcon(0, 0, icon);
      scrollText(10, 1, text, random(0xFFFFFF));
    }
    else
    {
      matrixClient.clear();
      showIcon(0, 0, icon);
      showText(10, 1, text, random(0xFFFFFF));
      state = 1;
    }
  }
}

// Evento de reloj que ocurre cada 1 segundo
void timerCallback(void *pArg)
{
  eventProcess(CLOCK_EVENT);
}

// Evento de red MQTT que ocurre cada que se recibe un mensaje
void notifyMQTT()
{
  eventProcess(MQTT_EVENT);
}

// Evento de red NTP que ocurre cada vez que ha pasado 1 segundo
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
    if (state == 0)
    {
      renderDisplay("tongue.bmp", "start");
    } else
    {
      if (ntp_updated == 1)
      {
        ntp_updated = 0;
        renderDisplay("/clock.bmp", currentTime);
      }
    }

    break;
  case MQTT_EVENT:
    Serial.printf("MQTT EVENT\n");
    mqttUpdate();
    break;
  case NTP_EVENT:
    Serial.printf("NTP EVENT: %lu\n", ntpClient.epochTime);
    updateTime(ntpClient.epochTime);
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
  if (!LittleFS.begin())
  {
    fs_enabled = false;
  }
  else
  {
    fs_enabled = true;
  }
  Serial.println("Smartclock starting...");
  currentTime = "--:--";

  digitalWrite(BEEP_PIN, LOW);
  delay(100);
  digitalWrite(BEEP_PIN, HIGH);

  os_timer_setfn(&myTimer, timerCallback, NULL);
  os_timer_arm(&myTimer, 1000, true);

  Scheduler.start(&matrixClient);
  Scheduler.start(&wifiClient);
  Scheduler.start(&otaTaskClient);
  mqttTaskClient.notify = notifyMQTT;
  Scheduler.start(&mqttTaskClient);
  ntpClient.notify = notifyNTP;
  Scheduler.start(&ntpClient);

  Scheduler.begin();
}

void loop()
{
}
