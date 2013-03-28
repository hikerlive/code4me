// **************************************************************************
// workerthread.cpp
//
// Description:
//	Implements a WorkerThread class. This is the document part of our MFC
//	SDI document/view architecture.
//
// DISCLAIMER:
//	This programming example is provided 'hikerlive'.
//  makes no claims to the worthiness of the code and does not warranty
//	the code to be error free.It is provided freely and can be used in
//	your own projects.
// **************************************************************************

#include "workerthread.h"
#include <process.h>

// **************************************************************************
// RunWorkerThread ()
//
// Description:
//	Creates a worker thread to perform a specified task.  This function will
//	block until worker thread completes its task.
//
// Parameters:
//	WORKERTHREADARG	*pArg	Pointer to a structure that contains information
//							  about task to be performed.
//
// Returns:
//  void
// **************************************************************************
void XWorkerThread::RunWorkerThread (WORKERTHREADARG *pArg)
{
	// Load standard arrow and small hourglass cursor reource:
	HCURSOR hCursor = ::LoadCursor (NULL, IDC_APPSTARTING);
	ASSERT (hCursor);

	// Set the hourglass cursor:
	::SetCursor (hCursor);

	// Lock document to prevent edits while worker thread is doing its thing:
	// SetLocked (true);

	// Start the worker thread:
	unsigned int uAddress = 0;
	HANDLE hThread = (HANDLE) _beginthreadex (NULL, 0, WorkerThread, pArg, 0, &uAddress);
	
	// If thread was created successfully, we need to wait for it to complete
	// its task.  This will guarentee that pArg will remain valid while the
	// thread completes its task.  We will need to process the message queue
	// while the thread runs to keep everybody up to date.
	if (hThread)
		{
		MSG stMsg;

		// Proccess messages while waiting for the thread to stop.  
		// hThread event will be set when thread stops.  Timeout every
		// 10 ms to process message queue, and then resume wait.
		while (WaitForSingleObject (hThread, 10) == WAIT_TIMEOUT)
			{
			// Process message queue by removing the next message, and then
			// dispatching it.  This will insure that important Windows 
			// notifiactions get processed by their target windows while this
			// thread is running.
			if (PeekMessage (&stMsg, NULL, 0, 0, PM_REMOVE))
				DispatchMessage (&stMsg);

			// Make sure hourglass cursor stays:
			::SetCursor (hCursor);
			}

		// When we break out of look, we know worker thread has stopped.
		// We can now close the thread handle:
		CloseHandle (hThread);
		}

	// We can now unlock document for edits:
	// SetLocked (false);

	// Replace normal cursor:
	::DestroyCursor (hCursor);
}

// **************************************************************************
// WorkerThread ()
//
// Description:
//	Worker thread function.
//
// Parameters:
//	void		*pvArgs		Pointer to a WORKERTHREADARG structure which
//							  describes task to be performed.
//
// Returns:
//  unsigned int - 0
// **************************************************************************
unsigned _stdcall XWorkerThread::WorkerThread (void *pvArgs)
{
	// Cast argument to proper type so we can extract the required data
	// about the task we are to perform:
	WORKERTHREADARG *pWTA = (WORKERTHREADARG *)pvArgs;
	ASSERT (pWTA);

	// Execute specified task:
	switch (pWTA->eTask)
	{
	case WORKERTHREADARG::ACTION_1:
		break;

	case WORKERTHREADARG::ACTION_2:
		break;

	case WORKERTHREADARG::ACTION_3:
		break;

	case WORKERTHREADARG::ACTION_4:
		break;

	default:
		// unhandled task ?
		ASSERT (FALSE);
		break;
	}

	return (0);
}
