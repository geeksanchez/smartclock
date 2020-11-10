#include <Arduino.h>
#include "TimeLib.h"
#include "Task.h"
#include "TaskScheduler.h"
#include "serialCMDTask.h"
#include "displayTask.h"
#include "Debugger.h"
#include "main.h"

void notifySerialCMD()
{
}

void loop()
{
}

void setup()
{

#ifdef DEBUG
  Debugger debugger;
  serialCMDTask taskSerialCMD(&debugger);
  taskSerialCMD.notify = notifySerialCMD;
  displayTask taskDisplay(&debugger);
#else
  taskSerialCMD.notify = notifySerialCMD;
  serialCMDTask taskSerialCMD();
  displayTask taskDisplay();
#endif

#ifdef DEBUG
  Task *tasks[] = {
      &taskSerialCMD,
      &taskDisplay,
      &debugger,
  };
#else
  Task *tasks[] = {
      &taskSerialCMD,
      &taskDisplay,
  };
#endif

  TaskScheduler scheduler(tasks, NUM_TASKS(tasks));

  scheduler.runTasks();
}
