#pragma once

#ifndef __SSRPCCHANNEL_H__
#define __SSRPCCHANNEL_H__

#ifdef _WIN32
	#include <winsock.h>
#else
	#include <sys/socket.h>
	#include <netinet/in.h>
	#include <arpa/in.h>
#endif

#include <hash_map>

#include "google/protobuf/service.h"
#include "SSEndpoint.h"

//using namespace ::google::protobuf;

namespace SSDataDomain
{

struct ResponseCallbackData;
class SSRpcChannel : public google::protobuf::RpcChannel
{
private:
	static long id_generator;
	long id;
	SSEndpoint endpoint;

	SSLock socket_lock;
	SOCKET socket;
	bool connected;

#pragma pack(push)
#pragma pack(4)
#if(_WIN32_WINNT >= 0x0502)
	unsigned long busy;
	unsigned long sequence;
#else
	long busy;
	long sequence;
#endif
#pragma pack(pop)
	
	bool destroyed;
	uintptr_t recieve_thread;

	struct ResponseCallbackData
	{
		long id;
		google::protobuf::RpcController* controller;
		google::protobuf::Message* request;
		google::protobuf::Message* response;
		google::protobuf::Closure* done;
		google::protobuf::Closure* cancel_callback;
	};
	SSLock map_lock;
	hash_map<long, ResponseCallbackData> callback_map; // Is it thread-safe?

public:
	SSRpcChannel();
	SSRpcChannel(SSEndpoint& endpoint);
	virtual ~SSRpcChannel();

	long Id() const { return id; }

	bool Start();
	void End();
	bool IsConnected();
	unsigned long IsBusy();
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

	virtual void HandleRecievedData(char* buf, int len);

	virtual void HandleRecievedMessage(google::protobuf::Message* response);

private:
	static void static_recieve_thread_proc(SSRpcChannel* channel);
	void recieve_thread_proc();
	
	void OnRpcCancel(long seq_id);
	
	SSLock send_lock;
	SSLock recv_lock;
	int send (const char FAR * buf, int len, int flags);
	int recv (char FAR * buf, int len, int flags);

	void Done(google::protobuf::Closure* done);

private:
	class auto_busy {
#if(_WIN32_WINNT >= 0x0502)
	private: unsigned long* busy;
#else
	private: long* busy;
#endif
	public:
#if(_WIN32_WINNT >= 0x0502)
        auto_busy(unsigned long* busy)
#else
        auto_busy(long* busy)
#endif
        { this->busy = busy; ::InterlockedIncrement(this->busy); }
		~auto_busy() { ::InterlockedDecrement(this->busy); }
	};

#ifdef _DEBUG
public:
	bool CanExit() {
		SYNCHRONIZED_SCOPE_WITH(&this->map_lock);
		//for (hash_map<long, ResponseCallbackData>::const_iterator it = this->callback_map.cbegin();
		//	it != this->callback_map.cend();
		//	it++) {
		//		SSTRACE("channel [" << this->id << "] callback data: " << it->first);
		//}
		//for (hash_map<long, ResponseCallbackData>::iterator it = this->callback_map.begin();
		//	it != this->callback_map.end();
		//	it++) {
		//		SSTRACE("channel [" << this->id << "] callback data: " << it->first);
		//}
		return this->callback_map.size() == 0;
	}
#endif
};

inline bool SSRpcChannel::IsConnected()
{
	return this->connected;
}

inline unsigned long SSRpcChannel::IsBusy()
{
	return ::InterlockedCompareExchange(&busy, 0, 0);
}

#pragma pack(push)
#pragma pack(1)
typedef struct _RPCFRAME
{
	int size;
	char buffer[1];
} RPCFRAME, *LPRPCFRAME;
#pragma pack(pop)

} // namespace

#endif
