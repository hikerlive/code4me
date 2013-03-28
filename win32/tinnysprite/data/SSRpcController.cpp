#include "SSCommon.h"
#include "SSRpcController.h"

using namespace SSDataDomain;
using namespace google::protobuf;

SSRpcController::SSRpcController()
{
	this->reason.clear();
	this->failed = false;
	this->canceled = false;
	this->callback = NULL;
}

SSRpcController::~SSRpcController()
{
	RpcController::~RpcController();
}

void SSRpcController::Reset()
{
	this->reason.clear();
	this->failed = false;
	this->canceled = false;
	this->callback = NULL;
}

bool SSRpcController::Failed() const
{
	return this->failed;
}

string SSRpcController::ErrorText() const
{
	return this->reason;
}

void SSRpcController::StartCancel()
{
	this->canceled = true;
	if (this->callback != NULL) {
		try {
			this->callback->Run();
		} catch(...) {
			SSTRACE("something error when starting cancel rpc.");
		}
	}
}

void SSRpcController::SetFailed(const string& reason)
{
	this->reason = reason;
	this->failed = true;
}

bool SSRpcController::IsCanceled() const
{
	return this->canceled;
}

void SSRpcController::NotifyOnCancel(Closure* callback)
{
	this->callback = callback;
	if (this->canceled)	 {
		if (this->callback != NULL) {
			try {
				this->callback->Run();
			} catch(...) {
				SSTRACE("something error when starting cancel rpc.");
			}
		}
	}
}