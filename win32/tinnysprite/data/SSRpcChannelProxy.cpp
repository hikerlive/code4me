#include "SSCommon.h"
#include <process.h>

#include "SSRpcChannelProxy.h"

using namespace SSDataDomain;
using namespace google::protobuf;

SSRpcChannelProxy::SSRpcChannelProxy()
{
	//this->destroyed = false;
	//this->work_thread = _beginthread((void (_cdecl*)(void*))static_work_thread_proc, 0, this);
}

SSRpcChannelProxy::~SSRpcChannelProxy()
{
	//this->destroyed = true;
	//WakeConditionVariable(&this->list_not_empty);
	//::WaitForSingleObject((HANDLE)work_thread, INFINITE);
	
	//list<SSRpcChannel*>::const_iterator it = this->channels.cbegin();
	//while (it != this->channels.cend()) {
	//	SSRpcChannel* channel = *it;
	//	delete channel;
	//	it++;
	//}
	list<SSRpcChannel*>::iterator it = this->channels.begin();
	while (it != this->channels.end()) {
		SSRpcChannel* channel = *it;
		delete channel;
		it++;
	}
}

bool SSRpcChannelProxy::IsConnected()
{
	//list<SSRpcChannel*>::const_iterator it = this->channels.cbegin();
	//while (it != this->channels.cend()) {
	//	SSRpcChannel* channel = *it;
	//	if (channel->IsConnected()) {
	//		return true;
	//	}
	//	it++;
	//}
	list<SSRpcChannel*>::iterator it = this->channels.begin();
	while (it != this->channels.end()) {
		SSRpcChannel* channel = *it;
		if (channel->IsConnected()) {
			return true;
		}
		it++;
	}
	return false;
}

void SSRpcChannelProxy::AddChannel(SSEndpoint& endpoint)
{
	SSRpcChannel* channel = new SSRpcChannel(endpoint);
	this->AddChannel(channel);
}

void SSRpcChannelProxy::AddChannel(SSRpcChannel* channel)
{
	this->channels.push_back(channel);
	channel->Start();
}

bool SSRpcChannelProxy::InnerCallMethod(const MethodDescriptor * method, RpcController * controller, const Message * request, Message * response, Closure * done)
{
	unsigned long busy = ~(unsigned long)0;
	SSRpcChannel* least_busy_channel = NULL;

	//list<SSRpcChannel*>::const_iterator ci = this->channels.cbegin();
	//while (ci != this->channels.cend()) {
	list<SSRpcChannel*>::iterator ci = this->channels.begin();
	while (ci != this->channels.end()) {
		if (!(*ci)->IsConnected()) {
			ci++;
			continue;
		}

		unsigned long ib = (*ci)->IsBusy();
		if (!ib) {
			(*ci)->CallMethod(method, controller, request, response, done);
			return true;
		}

		if (ib < busy) {
			busy = ib;
			least_busy_channel = *ci;
			assert((least_busy_channel->Id() <= 3 && least_busy_channel->Id() >= 0));
		}
		
		ci++;
	}

	if (least_busy_channel != NULL) {
		least_busy_channel->CallMethod(method, controller, request, response, done);
		return true;
	}

	return false;
}

void SSRpcChannelProxy::CallMethod(const MethodDescriptor * method, RpcController * controller, const Message * request, Message * response, Closure * done)
{
	if (InnerCallMethod(method, controller, request, response, done))
		return;

	string failed_message("No connection.");
	controller->SetFailed(failed_message);
	if (done != NULL) {
		try {
			done->Run();
		} catch(...){
			SSTRACE("something wrong when done.");
		}
	}

	//CallData* cd = new CallData();
	//cd->method_descriptor = (MethodDescriptor*)method;
	//cd->controller = controller;
	//cd->request = (Message*)request;
	//cd->response = response;
	//cd->done = done;

	//this->call_pending_list_lock.lock();
	//this->call_pending_list.push_back(cd);
	//this->call_pending_list_lock.unlock();
	//WakeConditionVariable(&this->list_not_empty);
}

//void SSRpcChannelProxy::work_thread_proc()
//{
//	while (!this->destroyed) {
//		this->call_pending_list_lock.lock();
//		if (this->call_pending_list.empty()) {
//			SleepConditionVariableCS(&this->list_not_empty, this->call_pending_list_lock.get_critical_section(), INFINITE);
//		}
//
//		if (this->destroyed) {
//			this->call_pending_list_lock.unlock();
//			break;
//		}
//
//		CallData* cd = this->call_pending_list.front();
//		this->call_pending_list.pop_front();
//		if (cd == NULL) {
//			this->call_pending_list_lock.unlock();
//			continue;
//		}
//		
//		if (cd->retry_time < 3) {
//			cd->retry_time++;
//			if (!InnerCallMethod(cd->method_descriptor, cd->controller, cd->request, cd->response, cd->done)) {
//				this->call_pending_list.push_back(cd);
//			}
//		}
//		this->call_pending_list_lock.unlock();
//	}
//}



