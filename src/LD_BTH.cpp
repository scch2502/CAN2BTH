#include "LD_BTH.h"
#include "LD_Config.h"



int LD_BTH::BLESetup()
{
    // Create the BLE Device
    BLEDevice::init(DEVICENAME);

    // Create the BLE Server
    BLEServer *server = BLEDevice::createServer(); 
    server->setCallbacks(new ServerCallbacks(this->deviceConnected));

    // Create BLE Services 
    BLEService *service_s1 = server->createService(SERVICE_UUID_S1);
    BLEService *service_s2 = server->createService(SERVICE_UUID_S2);

 
    // Create BLE Characteristic for sending data
    this->CHARACTERISTIC_Disp_SupportMode = service_s1->createCharacteristic(CHARACTERISTIC_UUID_Disp_SupportMode, BLECharacteristic::PROPERTY_NOTIFY);
    this->CHARACTERISTIC_Disp_SupportMode->addDescriptor(new BLE2902());

    this->CHARACTERISTIC_Mot_BikeSpeed  = service_s1->createCharacteristic(CHARACTERISTIC_UUID_Mot_BikeSpeed, BLECharacteristic::PROPERTY_NOTIFY);
    this->CHARACTERISTIC_Mot_BikeSpeed->addDescriptor(new BLE2902());

    this->CHARACTERISTIC_Mot_DriverCadence = service_s1->createCharacteristic(CHARACTERISTIC_UUID_Mot_DriverCadence, BLECharacteristic::PROPERTY_NOTIFY);
    this->CHARACTERISTIC_Mot_DriverCadence->addDescriptor(new BLE2902());
    
    this->CHARACTERISTIC_Mot_MotorData  = service_s1->createCharacteristic(CHARACTERISTIC_UUID_Mot_MotorData , BLECharacteristic::PROPERTY_NOTIFY);
    this->CHARACTERISTIC_Mot_MotorData->addDescriptor(new BLE2902());

    this->CHARACTERISTIC_Mot_MotorPower  = service_s2->createCharacteristic(CHARACTERISTIC_UUID_Mot_MotorPower , BLECharacteristic::PROPERTY_NOTIFY);
    this->CHARACTERISTIC_Mot_MotorPower->addDescriptor(new BLE2902());

    this->CHARACTERISTIC_Bat_ElectricData1  = service_s2->createCharacteristic(CHARACTERISTIC_UUID_Bat_ElectricData1 , BLECharacteristic::PROPERTY_NOTIFY);
    this->CHARACTERISTIC_Bat_ElectricData1->addDescriptor(new BLE2902());

    this->CHARACTERISTIC_Bat_ElectricData2  = service_s2->createCharacteristic(CHARACTERISTIC_UUID_Bat_ElectricData2 , BLECharacteristic::PROPERTY_NOTIFY);
    this->CHARACTERISTIC_Bat_ElectricData2->addDescriptor(new BLE2902());

    this->CHARACTERISTIC_Bat_EnergyData  = service_s2->createCharacteristic(CHARACTERISTIC_UUID_Bat_EnergyData , BLECharacteristic::PROPERTY_NOTIFY);
    this->CHARACTERISTIC_Bat_EnergyData->addDescriptor(new BLE2902());

    this->CHARACTERISTIC_Mot_DriverInput  = service_s2->createCharacteristic(CHARACTERISTIC_UUID_Mot_DriverInput , BLECharacteristic::PROPERTY_NOTIFY);
    this->CHARACTERISTIC_Mot_DriverInput->addDescriptor(new BLE2902());


    // Start the service
    service_s1->start();    
    service_s2->start();  
 
    // Start advertising 
    server->getAdvertising()->start();

    return 0;  
};


void CharacteristicCallbacks::onWrite(BLECharacteristic *characteristic)
{
    std::string rxValue = characteristic->getValue(); 
    //Serial.print(rxValue);  //print out received data
}


void ServerCallbacks::onConnect(BLEServer* pServer)
{
    this->deviceConnected = true;
};


void ServerCallbacks::onDisconnect(BLEServer* pServer) 
{
    this->deviceConnected = false;
};


int LD_BTH::Send_BLE_MSG(String pMessageString, BLECharacteristic *pBLECharacteristic)
{
    char mValue[(pMessageString.length()+1)];    //creates a char array depending on the size of pMessageString
    strcpy(mValue, pMessageString.c_str());      //the message string is copied into the array
    
    if(deviceConnected)     //only if a device is coupled via BLE, data is sent, otherwise not
    {   
        pBLECharacteristic->setValue(mValue);       //A string is sent via BLE containing all sensor signals of a CAN message
        pBLECharacteristic->notify();          
        delay(8);                            
    }

    return 0;
};


void LD_BTH::T_MAP_Code(void *parameter)
{
    for(;;)
    {
        static can_message_t p_rx_message;

        xSemaphoreTake(this->sem, portMAX_DELAY);     //take semaphore


        if (this->GlobalQueue.get(p_rx_message) == false)     //Independent retrieval of a message from the queue
        {
            xSemaphoreGive(this->sem);                
            vTaskDelay(pdMS_TO_TICKS(50));      //if queue should be empty -> wait
        }

        else
        {
            xSemaphoreGive(sem);    

            String temp_Str;        //temp_Str to temporarily store string

            switch (p_rx_message.identifier)    //Select by CAN-ID
            {
                case 59:
                {
                    temp_Str = this->Get_CAN_Values(p_rx_message,7,8,false,false,1,0,"-");
                    
                    this->Send_BLE_MSG(temp_Str, CHARACTERISTIC_Disp_SupportMode);
                    break;
                }

                case 209: 
                {
                    temp_Str = this->Get_CAN_Values(p_rx_message,7,16,false,false,0.01,0,"km/h");     //mot_speed_kmh

                    this->Send_BLE_MSG(temp_Str, CHARACTERISTIC_Mot_BikeSpeed);

                    break;
                };

                case 210:
                {
                    temp_Str = this->Get_CAN_Values(p_rx_message,15,8,false,false,1,0,"rpm");     //SG_ driver_cadence_rpm
                    
                    this->Send_BLE_MSG(temp_Str, CHARACTERISTIC_Mot_DriverCadence);

                    break;
                };

                case 211:
                {
                    temp_Str = this->Get_CAN_Values(p_rx_message,7,16,false,true,0.001,0,"Nm");                           //mot_actual_torque_Nm
                    temp_Str = temp_Str + DELIMITER + this->Get_CAN_Values(p_rx_message,23,16,false,true,0.001,0,"Nm");   //mot_desired_torque_Nm
                    temp_Str = temp_Str + DELIMITER + this->Get_CAN_Values(p_rx_message,39,16,false,true,1,0,"rpm");      //mot_bike_rpm

                    this->Send_BLE_MSG(temp_Str, CHARACTERISTIC_Mot_MotorData);

                    break;
                };

                case 212:
                {  
                    temp_Str = this->Get_CAN_Values(p_rx_message,7,16,false,false,0.1,0,"-");     //mot_power_output_W

                    this->Send_BLE_MSG(temp_Str, CHARACTERISTIC_Mot_MotorPower);

                    break;
                };

                case 257: 
                {
                    temp_Str = this->Get_CAN_Values(p_rx_message,7,16,false,false,1.0,0,"-");                             //bat_operating_mode
                    temp_Str = temp_Str + DELIMITER + this->Get_CAN_Values(p_rx_message,23,16,false,true,0.001,0,"A");    //bat_current_A

                    this->Send_BLE_MSG(temp_Str, CHARACTERISTIC_Bat_ElectricData1);

                    temp_Str =this-> Get_CAN_Values(p_rx_message,39,16,false,true,0.1,0,"W");                             //bat_power_output_W
                    temp_Str = temp_Str + DELIMITER + this->Get_CAN_Values(p_rx_message,55,16,false,false,0.001,0,"V");   //mot_bat_voltage_V

                    this->Send_BLE_MSG(temp_Str, CHARACTERISTIC_Bat_ElectricData2);


                    break;
                };

                case 273:
                {
                    temp_Str = this->Get_CAN_Values(p_rx_message,47,8,false,false,0.1,0,"Ah");                        //bat_last_charge_capacity_Ah
                    temp_Str = temp_Str + DELIMITER + this->Get_CAN_Values(p_rx_message,48,8,true,false,1,0,"%");     //bat_soc

                    this->Send_BLE_MSG(temp_Str, CHARACTERISTIC_Bat_EnergyData);

                    break;
                };

                case 518:
                {
                    temp_Str = this->Get_CAN_Values(p_rx_message,23,16,false,false,0.1,0,"Nm");                       //mot_rider_torque
                    temp_Str = temp_Str + DELIMITER + this->Get_CAN_Values(p_rx_message,7,16,false,true,0.1,0,"W");   //mot_rider_power_W

                    this->Send_BLE_MSG(temp_Str, CHARACTERISTIC_Mot_DriverInput);

                    break;
                };  
                
                default:
                {};
            };
        };

    };

    vTaskDelete(NULL);
};

String LD_BTH::Get_CAN_Values(can_message_t &p_rx_message, int p_start_bit, int p_data_length, bool p_little_endian, bool p_unsigned, float p_scale, float p_offset, String pUnit)
{
  float temp, data = 0;
  int Start_Byte = p_start_bit*0.125;     //StartByte is calculated (0.125 = 1/8)
  int Byte_Count = p_data_length*0.125;   //calculate length of message (in Byte)
  //int j = Byte_Count;                     //power for calculation

  /* Utdated
  if(Byte_Count <2)   
  {
      j=0;    //set power of 16 to 0, if message is only one byte long
  }*/
  
  //little endian
  if(p_little_endian == true)    
  {
    for (int i = (Start_Byte); i>=(Start_Byte-Byte_Count+1); i--) //go through each byte
    {

      temp = p_rx_message.data[i];            //store value from data-byte temporarily

      if(temp >127 && p_unsigned==true)       //if there are any signed values
      {   
          temp = temp-256;                    //calculate signed value e.g. 512 signed = -128
      };

      data = data + temp*pow(16,((Start_Byte-i)*2));           
    };
  }

  //big endian
  else    
  {
    for (int i = Start_Byte; i<(Start_Byte+Byte_Count); i++)
    {
      temp = p_rx_message.data[i]; 

      if(temp >127 && p_unsigned==true)
      {
          temp = temp-256;
      };

      data = data + temp*pow(16,((i-Start_Byte)*2));
    };
  }
      
  data = (p_offset+ (p_scale*data));      //add and multiply scale and offset to data


  if(cmd_send_units == 1)
  {
    return (String(data)+pUnit); 
  }
  else
  {
    return String(data); //Return as string, to send later merged via BLE
  }

};

