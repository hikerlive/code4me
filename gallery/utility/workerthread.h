// **************************************************************************
// workerthread.h
//
// Description:
//	Defines a WorkerThread class. This is the document part of our MFC
//	SDI document/view architecture.
//
// DISCLAIMER:
//	This programming example is provided 'hikerlive'.
//  makes no claims to the worthiness of the code and does not warranty
//	the code to be error free.It is provided freely and can be used in
//	your own projects.
// **************************************************************************

#ifndef __WORKDERTHREAD_H
#define __WORKDERTHREAD_H

class XWorkerThread
{
public:
	typedef struct _tagWORKERTHREADARG
	{
		enum TASK
		{
			ACTION_1,
			ACTION_2,
			ACTION_3,
			ACTION_4,
		};
		
		_tagWORKERTHREADARG ()
		{
			pvObjectA = NULL;
			pvObjectB = NULL;
		}
		
		TASK eTask;
		void *pvObjectA;
		void *pvObjectB;
	} WORKERTHREADARG;
	
	void RunWorkerThread (WORKERTHREADARG *pArg);
	static unsigned _stdcall WorkerThread (void *pvArgs);
};

#endif //__WORKDERTHREAD_H