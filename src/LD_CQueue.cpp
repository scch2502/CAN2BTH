#include "LD_CQueue.h"


CQueue::CQueue(): mCurrentSize(0), mHeadIndex(0), mTailIndex(0) {};


bool CQueue::add(can_message_t &pMSG)
{
	if (mCurrentSize == QUEUESIZE)
	{
		return false;
	}

	mQueue[mTailIndex] = pMSG;
	mTailIndex++;

	if (mTailIndex > QUEUESIZE - 1)
	{
		mTailIndex = 0;
	};

	mCurrentSize++;
	return true;
};


bool CQueue::get(can_message_t &pMSG)
{
	if (mCurrentSize == 0)		//in case empty
	{
		return false;
	}

	pMSG = mQueue[mHeadIndex];
	mCurrentSize--;
	mHeadIndex++;

	if (mHeadIndex > QUEUESIZE - 1)
	{
		mHeadIndex = 0;
	}

	return true;
};
