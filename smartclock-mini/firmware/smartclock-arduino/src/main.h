#ifndef main_h
#define main_h

#include <Arduino.h>

#define BEEP_PIN D4


#define CLOCK_EVENT 0
#define MQTT_EVENT  1
#define NTP_EVENT   2
#define NUM_EVENTS  3

#define START_STATE 0
#define CLOCK_STATE 1
#define DATE_STATE  2
#define MSG_STATE   3
#define NUM_STATES  4

#define VERSION "0.21"
#define BMP_FILE_HEADER_SIZE 14
#define BMP_IMAGE_HEADER_SIZE 40

struct bmp_file_header_t
{
    uint16_t signature;
    uint32_t file_size;
    uint32_t reserved;
    uint32_t image_offset;
};

struct bmp_image_header_t
{
    uint32_t header_size;
    uint32_t image_width;
    uint32_t image_height;
    uint16_t color_planes;
    uint16_t bits_per_pixel;
    uint32_t compression_method;
    uint32_t image_size;
    uint32_t horizontal_resolution;
    uint32_t vertical_resolution;
    uint32_t colors_in_palette;
    uint32_t important_colors;
};

const uint8_t minutosY[] PROGMEM =
    {
        255, 254, 249, 243, 233, 221, 206, 190, 171, 150, 128, 104, 79, 53, 27,
        0, 27, 53, 79, 104, 128, 150, 171, 190, 206, 221, 233, 243, 249, 254,
        255, 254, 249, 243, 233, 221, 206, 190, 171, 150, 128, 104, 79, 53, 27,
        0, 27, 53, 79, 104, 128, 150, 171, 190, 206, 221, 233, 243, 249, 254};

const uint8_t minutosX[] PROGMEM =
    {
        0, 27, 53, 79, 104, 128, 150, 171, 190, 206, 221, 233, 243, 249, 254,
        255, 254, 249, 243, 233, 221, 206, 190, 171, 150, 128, 104, 79, 53, 27,
        0, 27, 53, 79, 104, 128, 150, 171, 190, 206, 221, 233, 243, 249, 254,
        255, 254, 249, 243, 233, 221, 206, 190, 171, 150, 128, 104, 79, 53, 27};

const uint8_t horasY[] PROGMEM =
    {
        255, 221, 128, 0, 128, 221, 255, 221, 128, 0, 128, 221, 255, 221, 128, 0, 128, 221, 255, 221, 128, 0, 128, 221};

const uint8_t horasX[] PROGMEM =
    {
        0, 128, 221, 255, 221, 128, 0, 128, 221, 255, 221, 128, 0, 128, 221, 255, 221, 128, 0, 128, 221, 255, 221, 128};

#endif