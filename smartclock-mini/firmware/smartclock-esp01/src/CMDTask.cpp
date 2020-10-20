#include "CMDTask.h"

char msg[256];
uint8_t count = 0;

void CMDTask::sendMSG(char *msg)
{
    Serial.println(msg);
}

void CMDTask::setup()
{
    Serial.begin(9600);
    count = 0;
    msg[0] = '\0';
}

void CMDTask::loop()
{
    if (Serial.available())
    {
        char ch = Serial.read();
        if (count > 255)
        {
            count = 0;
            msg[count] = '\0';
            Serial.println("Buffer overrun!!!");
        }
        else
        {
            if ((ch != '\n') && (ch != '\r'))
            {
                msg[count] = ch;
                count++;
            }
            else
            {
                if (ch == '\n')
                {
                    msg[count] = '\0';
                    recvMSG(msg);
                    count = 0;
                }
            }
        }
    }
    yield();
}