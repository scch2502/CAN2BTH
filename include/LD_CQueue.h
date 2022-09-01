#ifndef LD_CQUEUE_H_
#define LD_CQUEUE_H_

#include <iostream>
#include <cstdlib>
#include "Arduino.h"
#include "esp_err.h"
#include "esp_log.h"
#include "driver/can.h"
#include "LD_Config.h" //Benötigt für: mQueue[SIZE]
using namespace std;


class CQueue
{
public:
	CQueue();
	bool add(can_message_t &pMSG);
	bool get(can_message_t &pMSG);

private:	
	can_message_t mQueue[QUEUESIZE];		//ptr auf Queue
	int mCurrentSize;		//number of Messages
	int mHeadIndex;			//Read Ptr
	int mTailIndex;			//Write Ptr

};

#endif /*LD_CQUEUE_H*/