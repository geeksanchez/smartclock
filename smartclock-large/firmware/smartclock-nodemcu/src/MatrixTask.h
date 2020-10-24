#ifndef MatrixTask_h
#define MatrixTask_h

#include <Scheduler.h>

#ifndef MSG_BUFFER_SIZE
#define MSG_BUFFER_SIZE (256)
#endif

#define TIME_BUFFER_SIZE (12)
#define DATE_BUFFER_SIZE (12)

class MatrixTask : public Task
{

    private:
        char mtxMsg[MSG_BUFFER_SIZE];
        const uint8_t *Font;
        void clearText();
        void writePixel(unsigned int bX, unsigned int bY, uint32_t colour);
        void selectFont(const uint8_t *font);
        byte charWidth(const char letter);
        byte drawChar(const int bX, const int bY, const char letter, uint32_t fgcolour, uint32_t bgcolour);
        void drawString(int bX, int bY, const char *bChars, byte length, uint32_t fgcolour, uint32_t bgcolour);

    protected:
        void setup();
        void loop();

    public:
        void setBrightness(uint8_t lux);
        void DrawIcon(uint8_t icon);
        void DrawMsg(char *msg, byte length, uint8_t mode);
        void ScrollMsg(char *msg, byte length);
        void (*notifyMatrix)(char *msg);
};

#endif