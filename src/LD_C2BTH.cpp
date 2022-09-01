#include "LD_C2BTH.h"

LD_C2BTH::LD_C2BTH()
{
    t_config = CAN_TIMING_CONFIG_500KBITS();            //t_config is set to 500kbps 
    
    g_config = {.mode = CAN_MODE_LISTEN_ONLY, 
                .tx_io = TX_GPIO_NUM, 
                .rx_io = RX_GPIO_NUM,
                .clkout_io =  GPIO_NUM_25, 
                .bus_off_io =  GPIO_NUM_25,
                .tx_queue_len = 0, 
                .rx_queue_len = 10,
                .alerts_enabled = CAN_ALERT_NONE,
                .clkout_divider = 0};                   //mode is set to listenOnly//set tx queue length to 0 due to listen only mode//rest is default configuration
    
    f_config = CAN_FILTER_CONFIG_ACCEPT_ALL();           //no filter applied

    deviceConnected = false;

    TCAN = new LD_TCAN(sem, GlobalQueue, deviceConnected);
    BTH = new LD_BTH(sem, GlobalQueue, deviceConnected);
}


void LD_C2BTH::TaskCAN(void *parameter)
{
    LD_C2BTH::TCAN->T_CAN_Code(parameter);
}


void LD_C2BTH::TaskBLE(void *parameter)
{
    LD_C2BTH::BTH->T_MAP_Code(parameter);
}