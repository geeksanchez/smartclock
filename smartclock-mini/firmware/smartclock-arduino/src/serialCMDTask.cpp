#include <SoftwareSerial.h>
#include "serialCMDTask.h"

SoftwareSerial espSerial(2, 3);

serialCMDTask::serialCMDTask() : Task()
{
    espSerial.begin(9600);
}

bool serialCMDTask::canRun(uint32_t now)
{
    return espSerial.available() > 0;
}

void serialCMDTask::run(uint32_t now)
{
    while (espSerial.available() > 0)
    {
        char ch = espSerial.read();
        if (byteCount > 255)
        {
            byteCount = 0;
            msg[byteCount] = '\0';
        }
        else
        {
            if ((ch != '\n') && (ch != '\r'))
            {
                msg[byteCount] = ch;
                byteCount++;
            }
            else
            {
                if (ch == '\n')
                {
                    msg[byteCount] = '\0';
                    new_message = true;
                    strcpy(last_msg, msg);
                    byteCount = 0;
                }
            }
        }
    }
}

String serialCMDTask::espRead()
{
    return String(last_msg);
}