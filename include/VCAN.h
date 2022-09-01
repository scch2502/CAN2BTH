#ifndef VCAN_H_
#define VCAN_H_

#include <stdio.h>
#include <stdlib.h>
#include <Arduino.h>
#include "driver/can.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "esp_err.h"
#include "esp_log.h"
//#include "LD_BTH.h"
#include "LD_CQueue.h"
#include "LD_Config.h"
using namespace std;


extern int WantedCanIDs[8];

int Generate_Virtual_CAN_Messages(can_message_t  &p_rx_message);


#endif