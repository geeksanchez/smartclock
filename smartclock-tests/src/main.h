#ifndef main_h
#define main_h

#include <Arduino.h>

#define BEEP_PIN D4

#define CLOCK_EVENT 0
#define MQTT_EVENT  1
#define NTP_EVENT   2

#define VERSION "0.20"
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

#endif