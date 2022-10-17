/*************************************************************************************************;
* Project           : 19ws_RK_CAN2Bluetooth
*
* Program name      : 20220901_CAN2BLEV24
*
* Author            :           Andreas Bank,       M.Sc.        
*                               Christian Schmidt,  B.Eng.   
*                               Jonas Gentner,      B.Eng.
*
* Date created      : 20200428
* Date modified     : 20220901
*
* Purpose           : Receive, calculate and send data from eBike CAN over BLE to mobile device
*------------------------------------------------------------------------------------------------
* Changes           :
* 20200726          :   A string is sent via BLE containing all sensor signals of a CAN message.
                        "Send_BLE_MSG"-> changed 'pValue' from float to string 
                        float -> (std::__cxx11::basic_string<char>)
* 20220901          :   OOP

*************************************************************************************************/;

#include "LD_C2BTH.h"

LD_C2BTH *CAN2BTH = new LD_C2BTH;
LD_C2BTH *LD_C2BTH::self{CAN2BTH};

void setup()
{
    Serial.begin(115200);
    
    //Install CAN driver
    ESP_ERROR_CHECK(can_driver_install(&(CAN2BTH->g_config), &(CAN2BTH->t_config), &(CAN2BTH->f_config)));
    ESP_ERROR_CHECK(can_start());
    Serial.println("CAN driver installed");

    //Create Services and Charactaristics
    Serial.println("BLE installed");        //done by constructor of "LD_BTH.h"

    //Create multicore Task 
    CAN2BTH->sem = xSemaphoreCreateBinary();
    xTaskCreatePinnedToCore(CAN2BTH->TaskCAN,
                            "CANReader",
                            4096,
                            NULL,
                            8,
                            &(CAN2BTH->T_CAN),
                            0);

    xTaskCreatePinnedToCore(CAN2BTH->TaskBLE,
                            "Mapping",
                            4096,
                            NULL,
                            9,
                            &(CAN2BTH->T_MAP),
                            1);

    xSemaphoreGive(CAN2BTH->sem);
}

void loop(){}


