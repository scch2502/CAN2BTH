#ifndef LD_TCAN_H_
#define LD_TCAN_H_

#include <stdio.h>
#include <stdlib.h>
#include <Arduino.h>
#include "driver/can.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "esp_err.h"
#include "esp_log.h"
#include "LD_CQueue.h"
#include "LD_Config.h"


class LD_TCAN
{
    public:
        LD_TCAN(SemaphoreHandle_t sem, CQueue Queue, bool deviceConnected)
        {
            this->sem = sem;
            this->GlobalQueue = Queue;
            this->deviceConnected = deviceConnected;
        }

        void T_CAN_Code(void *parameter);


    private:
        SemaphoreHandle_t sem;
        CQueue GlobalQueue;
        int WantedCanIDs[8] ={59, 209, 210, 211, 212, 257, 273, 518};    //only CAN-ID's that should be included in the queue <- TBD! Make ist as #define
        bool deviceConnected;
};


#endif /*LD_TCAN_H_*/