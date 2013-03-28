#ifndef __SSRPCCHANNELPROXY_H__
#define __SSRPCCHANNELPROXY_H__
#include <hash_map>
#include <list>

#include "google/protobuf/service.h"
#include "SSEndpoint.h"
#include "SSRpcChannel.h"

namespace SSDataDomain
{

class SSRpcChannelProxy : public google::protobuf::RpcChannel
{
private:
	list<SSRpcChannel*> channels;

	//bool destroyed;
	//uintptr_t work_thread;

public:
	SSRpcChannelProxy();
	virtual ~SSRpcChannelProxy();

public:
	bool IsConnected();
	void AddChannel(SSEndpoint& endpoint);
	void AddChannel(SSRpcChannel* channel);

public:
	// Call the given method of the remote service.  The signature of this
	// procedure looks the same as Service::CallMethod(), but the requirements
	// are less strict in one important way:  the request and response objects
	// need not be of any specific class as long as their descriptors are
	// method->input_type() and method->output_type().
	virtual void CallMethod(const google::protobuf::MethodDescriptor* method,
							google::protobuf::RpcController* controller,
							const google::protobuf::Message* request,
							google::protobuf::Message* response,
							google::protobuf::Closure* done);

private:
	//static void static_work_thread_proc(SSRpcChannelProxy* self);
	//void work_thread_proc();
	//
	//struct CallData
	//{
	//	unsigned int retry_time;
	//	MethodDescriptor* method_descriptor;
	//	RpcController* controller;
	//	Message* request;
	//	Message* response;
	//	Closure* done;
	//};
	//CONDITION_VARIABLE list_not_empty;
	//SSLock call_pending_list_lock;
	//list<CallData*> call_pending_list;

	bool InnerCallMethod(const google::protobuf::MethodDescriptor* method,
						google::protobuf::RpcController* controller,
						const google::protobuf::Message* request,
						google::protobuf::Message* response,
						google::protobuf::Closure* done);

#ifdef _DEBUG
public:
	bool CanExit() {
		//for(list<SSRpcChannel*>::const_iterator it = this->channels.cbegin();
		//	it != this->channels.cend(); it++) {
		//		if (!(*it)->CanExit())
		//			return false;
		//}
		for(list<SSRpcChannel*>::iterator it = this->channels.begin();
			it != this->channels.end(); it++) {
				if (!(*it)->CanExit())
					return false;
		}
		//this->call_pending_list_lock.lock();
		//if (!this->call_pending_list.empty())
		//	return false;
		//this->call_pending_list_lock.unlock();
		return true;
	}
#endif
};

//inline void SSRpcChannelProxy::static_work_thread_proc(SSRpcChannelProxy* self)
//{
//	self->work_thread_proc();
//	_endthread();
//}

} // namespace

#endif