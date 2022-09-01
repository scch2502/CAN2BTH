#include "VCAN.h"

unsigned long mstart, mend, mdiff;
int i=0;


int Generate_Virtual_CAN_Messages(can_message_t  &p_rx_message)
{

    
    p_rx_message.identifier= WantedCanIDs[(rand()%7+0)];
    p_rx_message.data[0] = (rand()%125);
    p_rx_message.data[1] = (rand()%125);
    p_rx_message.data[2] = (rand()%125);
    p_rx_message.data[3] = (rand()%125);
    p_rx_message.data[4] = (rand()%125);
    p_rx_message.data[5] = (rand()%125);
    p_rx_message.data[6] = (rand()%125);
    p_rx_message.data[7] = (rand()%125);
    p_rx_message.data_length_code = 8;
    
    Serial.println("VCAN Message Created");

    delay(50);
    return 0;
}