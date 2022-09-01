#ifndef LD_BTH_H_
#define LD_BTH_H_

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <BLE2902.h>
#include <stdio.h>
#include <stdlib.h>
#include <Arduino.h>

#include "LD_CQueue.h"

class CharacteristicCallbacks: public BLECharacteristicCallbacks 
{
    public:
        CharacteristicCallbacks(bool deviceConnected)
        {
            this->deviceConnected = deviceConnected;
        }
        bool deviceConnected;
        void onWrite(BLECharacteristic *characteristic);
};

class ServerCallbacks: public BLEServerCallbacks 
{
    public:
        ServerCallbacks(bool deviceConnected)
        {
            this->deviceConnected = deviceConnected;
        }
        bool deviceConnected;
        void onConnect(BLEServer* pServer);
        void onDisconnect(BLEServer* pServer);
};


class LD_BTH
{
    public:
        LD_BTH(SemaphoreHandle_t sem, CQueue Queue, bool deviceConnected)
        {
            this->sem = sem;
            this->GlobalQueue = Queue;
            this->deviceConnected = deviceConnected;

            this->BLESetup();
        }

        void T_MAP_Code(void *parameter);

    private:
        int BLESetup();
        int Send_BLE_MSG(String pMessageString, BLECharacteristic *pBLECharacteristic);
        String Get_CAN_Values(can_message_t &p_rx_message, int p_start_bit, int p_data_length, bool p_little_endian, bool p_unsigned, float p_scale, float p_offset, String pUnit);

        SemaphoreHandle_t sem;
        CQueue GlobalQueue;
        int WantedCanIDs[8] ={59, 209, 210, 211, 212, 257, 273, 518};    //only CAN-ID's that should be included in the queue <- TBD! Make ist as #define
        bool deviceConnected;

        //SERVICE_UUID_S1
        BLECharacteristic *CHARACTERISTIC_Disp_SupportMode;     //CAN-ID: 059
        BLECharacteristic *CHARACTERISTIC_Mot_BikeSpeed;        //CAN-ID: 209
        BLECharacteristic *CHARACTERISTIC_Mot_DriverCadence;    //CAN-ID: 210
        BLECharacteristic *CHARACTERISTIC_Mot_MotorData;        //CAN-ID: 211

        //SERVICE_UUID_S1
        BLECharacteristic *CHARACTERISTIC_Mot_MotorPower;       //CAN-ID: 212
        BLECharacteristic *CHARACTERISTIC_Bat_ElectricData1;    //CAN-ID: 257> for bat_mode and current_A
        BLECharacteristic *CHARACTERISTIC_Bat_ElectricData2;    //CAN-ID: 257-> for voltage_v and poer_output_w
        BLECharacteristic *CHARACTERISTIC_Bat_EnergyData;       //CAN-ID: 273
        BLECharacteristic *CHARACTERISTIC_Mot_DriverInput;      //CAN-ID: 518

};




#endif /*LD_BTH_H_*/