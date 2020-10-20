#ifndef img_h
#define img_h

#include <Arduino.h>

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

const uint32_t pacman_ghost[] PROGMEM =
    {
        0xFF000000, 0xFF000000, 0xFF000000, 0xFF0000FF, 0xFF0000FF, 0xFF000000, 0xFF000000, 0xFF000000,
        0xFF000000, 0xFF0000FF, 0xFF0000FF, 0xFF0000FF, 0xFF0000FF, 0xFF0000FF, 0xFF0000FF, 0xFF000000,
        0xFF000000, 0xFFFFFFFF, 0xFF0000FF, 0xFF0000FF, 0xFF0000FF, 0xFFFFFFFF, 0xFF0000FF, 0xFF000000,
        0xFFFFFFFF, 0xFFFF0000, 0xFFFFFFFF, 0xFF0000FF, 0xFFFFFFFF, 0xFFFF0000, 0xFFFFFFFF, 0xFF0000FF,
        0xFF0000FF, 0xFFFFFFFF, 0xFF0000FF, 0xFF0000FF, 0xFF0000FF, 0xFFFFFFFF, 0xFF0000FF, 0xFF0000FF,
        0xFF0000FF, 0xFF0000FF, 0xFF0000FF, 0xFF0000FF, 0xFF0000FF, 0xFF0000FF, 0xFF0000FF, 0xFF0000FF,
        0xFF0000FF, 0xFF0000FF, 0xFF0000FF, 0xFF0000FF, 0xFF0000FF, 0xFF0000FF, 0xFF0000FF, 0xFF0000FF,
        0xFF0000FF, 0xFF000000, 0xFF0000FF, 0xFF000000, 0xFF000000, 0xFF0000FF, 0xFF000000, 0xFF0000FF};

const uint32_t alien1[] PROGMEM =
    {
        0xFF000000, 0xFF000000, 0xFF000000, 0xFF00FF00, 0xFF00FF00, 0xFF000000, 0xFF000000, 0xFF000000,
        0xFF000000, 0xFF00FF00, 0xFF00FF00, 0xFF00FF00, 0xFF00FF00, 0xFF00FF00, 0xFF00FF00, 0xFF000000,
        0xFF00FF00, 0xFF000000, 0xFF000000, 0xFF00FF00, 0xFF00FF00, 0xFF000000, 0xFF000000, 0xFF00FF00,
        0xFF00FF00, 0xFF00FF00, 0xFF00FF00, 0xFF00FF00, 0xFF00FF00, 0xFF00FF00, 0xFF00FF00, 0xFF00FF00,
        0xFF00FF00, 0xFF00FF00, 0xFF00FF00, 0xFF00FF00, 0xFF00FF00, 0xFF00FF00, 0xFF00FF00, 0xFF00FF00,
        0xFF000000, 0xFF000000, 0xFF00FF00, 0xFF000000, 0xFF000000, 0xFF00FF00, 0xFF000000, 0xFF000000,
        0xFF000000, 0xFF00FF00, 0xFF000000, 0xFF00FF00, 0xFF00FF00, 0xFF000000, 0xFF00FF00, 0xFF000000,
        0xFF00FF00, 0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000, 0xFF00FF00};

const uint32_t alien2[] PROGMEM =
    {
        0xFF000000, 0xFF000000, 0xFF000000, 0xFF00FF00, 0xFF00FF00, 0xFF000000, 0xFF000000, 0xFF000000,
        0xFF000000, 0xFF00FF00, 0xFF00FF00, 0xFF00FF00, 0xFF00FF00, 0xFF00FF00, 0xFF00FF00, 0xFF000000,
        0xFF00FF00, 0xFF000000, 0xFF000000, 0xFF00FF00, 0xFF00FF00, 0xFF000000, 0xFF000000, 0xFF00FF00,
        0xFF00FF00, 0xFF00FF00, 0xFF00FF00, 0xFF00FF00, 0xFF00FF00, 0xFF00FF00, 0xFF00FF00, 0xFF00FF00,
        0xFF00FF00, 0xFF00FF00, 0xFF00FF00, 0xFF00FF00, 0xFF00FF00, 0xFF00FF00, 0xFF00FF00, 0xFF00FF00,
        0xFF000000, 0xFF00FF00, 0xFF000000, 0xFF00FF00, 0xFF00FF00, 0xFF000000, 0xFF00FF00, 0xFF000000,
        0xFF000000, 0xFF00FF00, 0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000, 0xFF00FF00, 0xFF000000,
        0xFF000000, 0xFF000000, 0xFF00FF00, 0xFF00FF00, 0xFF00FF00, 0xFF00FF00, 0xFF000000, 0xFF000000};

const uint32_t mail[] PROGMEM =
    {
        0xFFFF6666, 0xFFFF6666, 0xFFFF6666, 0xFFFF6666, 0xFFFF6666, 0xFFFF6666, 0xFFFF6666, 0xFFFF6666,
        0xFFFF6666, 0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000, 0xFFFF6666,
        0xFFFF6666, 0xFFFF6666, 0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000, 0xFFFF6666, 0xFFFF6666,
        0xFFFF6666, 0xFF000000, 0xFFFF6666, 0xFF000000, 0xFF000000, 0xFFFF6666, 0xFF000000, 0xFFFF6666,
        0xFFFF6666, 0xFF000000, 0xFF000000, 0xFFFF6666, 0xFFFF6666, 0xFF000000, 0xFF000000, 0xFFFF6666,
        0xFFFF6666, 0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000, 0xFFFF6666,
        0xFFFF6666, 0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000, 0xFFFF6666,
        0xFFFF6666, 0xFFFF6666, 0xFFFF6666, 0xFFFF6666, 0xFFFF6666, 0xFFFF6666, 0xFFFF6666, 0xFFFF6666};

const uint32_t ghost[] PROGMEM =
    {
        0xFF000000, 0xFFA0A0A0, 0xFFCDE0E2, 0xFFCDE0E2, 0xFFCDE0E2, 0xFFCDE0E2, 0xFFA0A0A0, 0xFF000000,
        0xFFA0A0A0, 0xFFCDE0E2, 0xFFCDE0E2, 0xFFCDE0E2, 0xFFCDE0E2, 0xFFCDE0E2, 0xFFCDE0E2, 0xFFA0A0A0,
        0xFFCDE0E2, 0xFF000000, 0xFFCDE0E2, 0xFFCDE0E2, 0xFFCDE0E2, 0xFFCDE0E2, 0xFF000000, 0xFFCDE0E2,
        0xFFCDE0E2, 0xFF000000, 0xFFCDE0E2, 0xFFCDE0E2, 0xFFCDE0E2, 0xFFCDE0E2, 0xFF000000, 0xFFCDE0E2,
        0xFFCDE0E2, 0xFFCDE0E2, 0xFFCDE0E2, 0xFFCDE0E2, 0xFFCDE0E2, 0xFFCDE0E2, 0xFFCDE0E2, 0xFFCDE0E2,
        0xFFCDE0E2, 0xFFCDE0E2, 0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000, 0xFFCDE0E2, 0xFFCDE0E2,
        0xFFCDE0E2, 0xFFCDE0E2, 0xFFCDE0E2, 0xFFCDE0E2, 0xFFCDE0E2, 0xFFCDE0E2, 0xFFCDE0E2, 0xFFCDE0E2,
        0xFFCDE0E2, 0xFFCDE0E2, 0xFF000000, 0xFFCDE0E2, 0xFFCDE0E2, 0xFF000000, 0xFFCDE0E2, 0xFFCDE0E2};

const uint32_t *const dib[] PROGMEM =
    {
        pacman_ghost, alien1, alien2, mail, ghost};

#endif