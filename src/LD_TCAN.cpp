#include "LD_TCAN.h"
//#include "VCAN.h"   //comment/uncomment if needet

void LD_TCAN::T_CAN_Code(void *parameter)
{
    for(;;) 
    {
        static can_message_t rx_message;                       //create new can_message object on Stack
        can_receive(&rx_message, portMAX_DELAY);               //listen to CAN and receive a message
        //Generate_Virtual_CAN_Messages(rx_message);             //ONLY for BLE Testing without physical CAN-Bus -> #include "VCAN.h"

        //Filter for wanted ID's, only these are stored in the queue
        bool exists = find(begin(this->WantedCanIDs),end(this->WantedCanIDs),rx_message.identifier)!=end(this->WantedCanIDs);

        if (exists)
        {   
            xSemaphoreTake(this->sem, portMAX_DELAY); //take the semaphores to prevent collisions when reading and writing on the queue
    
            if(this->deviceConnected)                 //only if mobile device is connected, messages get stored in queue
            {
                this->GlobalQueue.add(rx_message);    //add message to queue
            }
            xSemaphoreGive(this->sem);                //give back semaphore 
        }
    }

    //Uninstall CAN driver
    ESP_ERROR_CHECK(can_stop());
    ESP_ERROR_CHECK(can_driver_uninstall());
    vTaskDelete(NULL);
}