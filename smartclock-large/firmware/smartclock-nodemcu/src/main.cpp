#include <Arduino.h>
#include "WiFiTask.h"
#include <IotWebConf.h>
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
int state;
bool do_scroll, state_done;
os_timer_t eventTimer, scrollTimer;
String currentTime, currentDate, MName, Message;
int posX;
uint32_t scroll_color;

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
void scrollText(uint32_t colour)
{
  if (matrixClient.ready == 1)
  {
    matrixClient.ready = 0;
    scroll_color = colour;
    do_scroll = true;
    matrixClient.ready = 1;
//    os_timer_arm(&scrollTimer, 100, true);
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
        Message = String(msg);
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

void getMonthName(uint8_t M)
{
  switch (M)
  {
  case 1:
    MName = F("Ene");
    break;
  case 2:
    MName = F("Feb");
    break;
  case 3:
    MName = F("Mar");
    break;
  case 4:
    MName = F("Abr");
    break;
  case 5:
    MName = F("May");
    break;
  case 6:
    MName = F("Jun");
    break;
  case 7:
    MName = F("Jul");
    break;
  case 8:
    MName = F("Ago");
    break;
  case 9:
    MName = F("Sep");
    break;
  case 10:
    MName = F("Oct");
    break;
  case 11:
    MName = F("Nov");
    break;
  case 12:
    MName = F("Dic");
    break;
  default:
    MName = F("---");
    break;
  }
}

void updateTimeDate(unsigned long t)
{
  String NTPtime = "";
  String NTPdate = "";
  uint8_t M, D, h, m;
  struct tm *ptm = gmtime((time_t *)&t);
  M = ptm->tm_mon + 1;
  D = ptm->tm_mday;
  h = ptm->tm_hour;
  m = ptm->tm_min;
  if (h > 12)
  {
    h -= 12;
  }
  if (h == 0)
  {
    h = 12;
  }
  if (h < 10)
  {
    NTPtime += "0";
  }
  NTPtime += h;
  NTPtime += ":";
  if (m < 10)
  {
    NTPtime += "0";
  }
  NTPtime += m;
  if (currentTime != NTPtime)
  {
    currentTime = NTPtime;
  }

  if (D < 10)
  {
    NTPdate += "0";
  }
  NTPdate += D;
  NTPdate += "/";
  if (M < 10)
  {
    NTPdate += "0";
  }
  NTPdate += M;
/*  getMonthName(M);
  NTPdate += MName;*/
  if (currentDate != NTPdate)
  {
    currentDate = NTPdate;
  }
}

// Actualiza el display con el icono y el texto recibidos
void renderDisplay(String icon, String text)
{
  if (matrixClient.ready == 1)
  {
    matrixClient.clear();
    showIcon(0, 0, icon);
    showText(9, 1, text, random(0xFFFFFF));
  }
}

void scrollTimerCallback(void *pArg)
{
  if (do_scroll)
  {
    matrixClient.ready = 0;
    if (--posX > (int)(Message.length() * -4))
    {
      matrixClient.clear();
      matrixClient.setText(posX, 5, Message, scroll_color);
    }
    else
    {
      do_scroll = false;
      state_done = true;
      matrixClient.ready = 1;
//      os_timer_disarm(&scrollTimer);
    }
  }
  else
  {
    posX = DISPLAY_MAX_X;
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

void stateProcess()
{
  switch (state)
  {
  case START_STATE:
    Serial.println("Starting Wifi");
    renderDisplay("/wifi_7.bmp", "Start");
    break;
  case CLOCK_STATE:
    Serial.println(currentTime);
    renderDisplay("/clock.bmp", currentTime);
    break;
  case DATE_STATE:
    Serial.println(currentDate);
    renderDisplay("/poop.bmp", currentDate);
    break;
  case MSG_STATE:
    Serial.println("Mensaje");
    matrixClient.clear();
    showIcon(0, 0, "/message.bmp");
    if (Message.length() > 0)
    {
      state_done = false;
      scrollText(random(0xFFFFFF));
    }
    break;
  default:
    Serial.println("Abrazos");
    renderDisplay("/heart.bmp", "Abrazos");
    break;
  }
}

void eventProcess(int8_t event)
{
  switch (event)
  {
  case CLOCK_EVENT:
    Serial.printf("CLOCK EVENT\n");
    if (state_done)
    {
      if (wifiClient.WiFiState() == IOTWEBCONF_STATE_ONLINE)
      {
        if (state >= NUM_STATES)
        {
          state = 1;
        }
        else
        {
          state++;
        }
      }
      else
      {
        state = 0;
      }
      stateProcess();
    }
    break;
  case MQTT_EVENT:
    Serial.printf("MQTT EVENT\n");
    mqttUpdate();
    break;
  case NTP_EVENT:
    Serial.printf("NTP EVENT: %lu\n", ntpClient.epochTime);
    updateTimeDate(ntpClient.epochTime);
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
  do_scroll = false;
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

  os_timer_setfn(&eventTimer, timerCallback, NULL);
  os_timer_arm(&eventTimer, 5000, true);

  os_timer_setfn(&scrollTimer, scrollTimerCallback, NULL);
  os_timer_arm(&scrollTimer, 100, true);

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
