#ifndef LD_CONFIG_H_
#define LD_CONFIG_H_

#define DEVICENAME              "CAN-2-BTH"
#define DELIMITER               ";"
#define cmd_send_units          0   //=0 no units will be send in string; =1 units will be send (LD_PARS.cpp). !WARNING: Sending the units may result in data loss (actual float values) due to the string length

#define TX_GPIO_NUM             GPIO_NUM_21              
#define RX_GPIO_NUM             GPIO_NUM_22              
#define RX_TASK_PRIO            9     
#define QUEUESIZE               100   //Number of messages able to store       

#define SERVICE_UUID_S1                                 "ab0828b1-198e-4351-b779-901fa0e0371e"
#define SERVICE_UUID_S2                                 "fc49fc5e-5264-433d-9f77-01aa715ee06d"
//#define SERVICE_UUID_SERV3                              "8478639e-282e-41e3-a319-43654409bf13" //Diehier als Backup, falls weitere gebraucht werden
//#define SERVICE_UUID_SERV4                              "2c861576-75c4-4499-9f22-519c2db4a96c" //Diehier als Backup, falls weitere gebraucht werden

#define CHARACTERISTIC_UUID_Disp_SupportMode            "b5c75c21-5ef6-419b-93ee-e633ddab2853"  //CAN-ID: 059
#define CHARACTERISTIC_UUID_Mot_BikeSpeed               "9f450ac2-56c0-486c-935a-28ef05b242d3"  //CAN-ID: 209
#define CHARACTERISTIC_UUID_Mot_DriverCadence           "df08fd73-ed6d-45a0-bef2-b69e18921743"  //CAN-ID: 210
#define CHARACTERISTIC_UUID_Mot_MotorData               "e6fd50d9-483d-4b72-a5cb-ceeb629b2a1e"  //CAN-ID: 211
#define CHARACTERISTIC_UUID_Mot_MotorPower              "e6b7bcca-d803-4d7c-8fe0-85a26299903d"  //CAN-ID: 212
#define CHARACTERISTIC_UUID_Bat_ElectricData1           "944fe094-c1a3-4969-824b-7965921908d3"  //CAN-ID: 257-> for bat_mode and current_A
#define CHARACTERISTIC_UUID_Bat_ElectricData2           "944fe094-c1a3-4969-824b-7965921908d4"  //CAN-ID: 257-> for voltage_v and poer_output_w
#define CHARACTERISTIC_UUID_Bat_EnergyData              "432dcbe9-5537-4f50-b92a-f5d10e09f507"  //CAN-ID: 273
#define CHARACTERISTIC_UUID_Mot_DriverInput             "d7ec4aa0-5c02-43fb-ae6f-89166cf66eff"  //CAN-ID: 518

//#define WANTEDCANIDS[8]                                 {59, 209, 210, 211, 212, 257, 273, 518} // <- TBD!

#endif /*LD_CONFIG_H_*/