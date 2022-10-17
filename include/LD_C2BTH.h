#include "LD_BTH.h"
#include "LD_CQueue.h"
#include "LD_Config.h"
#include "LD_TCAN.h"


class LD_C2BTH
{
    public:
        LD_C2BTH();

        static void TaskCAN(void *parameter);
        static void TaskBLE(void *parameter);

        SemaphoreHandle_t sem;
        TaskHandle_t T_CAN;
        TaskHandle_t T_MAP;

        static can_timing_config_t t_config;    //= CAN_TIMING_CONFIG_500KBITS(); //t_config is set to 500kbps 
        static can_general_config_t g_config;   //= {.mode = CAN_MODE_LISTEN_ONLY, .tx_io = TX_GPIO_NUM, .rx_io = RX_GPIO_NUM,.clkout_io =  GPIO_NUM_25, .bus_off_io =  GPIO_NUM_25,.tx_queue_len = 0, .rx_queue_len = 10,.alerts_enabled = CAN_ALERT_NONE,.clkout_divider = 0}; //mode is set to listenOnly//set tx queue length to 0 due to listen only mode//rest is default configuration
        static can_filter_config_t f_config;    //= CAN_FILTER_CONFIG_ACCEPT_ALL(); //no filter applied

        static LD_C2BTH *self; 


    private:

        CQueue GlobalQueue;
        bool deviceConnected;

        LD_TCAN *TCAN;
        LD_BTH *BTH;

};