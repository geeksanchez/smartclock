#ifndef MatrixTask_h
#define MatrixTask_h

#include <Scheduler.h>

class MatrixTask : public Task
{

    private:
        const uint8_t *Font;
        void clearText();
        void writePixel(unsigned int bX, unsigned int bY, uint32_t colour);
        void drawLine(int x1, int y1, int x2, int y2, uint32_t colour);
        void drawCircle(int xCenter, int yCenter, int radius, uint32_t colour);
        void drawCircleSub(int cx, int cy, int x, int y, uint32_t colour);
        void drawBox(int x1, int y1, int x2, int y2, uint32_t colour);
        void drawFilledBox(int x1, int y1, int x2, int y2, uint32_t colour);
        void selectFont(const uint8_t *font);
        byte charWidth(const char letter);
        byte drawChar(const int bX, const int bY, const char letter, uint32_t fgcolour, uint32_t bgcolour);
        void drawString(int bX, int bY, const char *bChars, byte length, uint32_t fgcolour, uint32_t bgcolour);

    protected:
        void setup();
        void loop();

    public:
        void setBrightness(uint8_t lux);
        void show();
        void DrawIcon(uint8_t icon);
        void DrawMsg(char *msg, byte length, uint8_t mode);
        void ScrollMsg(char *msg, byte length);
        void (*notifyMatrix)(char *msg);
};

#endif