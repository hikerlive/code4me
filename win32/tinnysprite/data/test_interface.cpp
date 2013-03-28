/**
 * ²âÊÔQueryAndUpdate½Ó¿Ú
 */
#include "SSCommon.h"
#include <process.h>
#include "SSRpcChannelProxy.h"
#include "SSRpcController.h"

#include "ssdatadomain.pb.h"
#include "SSDataDomainSvcSyncStub.h"

using namespace std;
using namespace SSDataDomain;
using namespace google::protobuf;

#define MAX_SEQ_LENGTH 10000
#define MAX_TASK_COUNT 4
#define TASK_SEQ_LENGTH (MAX_SEQ_LENGTH/MAX_TASK_COUNT)

#pragma pack(push)
#pragma pack(4)
	unsigned long sequence[MAX_SEQ_LENGTH+1];
#pragma pack(pop)

typedef struct _task_info {
	int start;
	int end;
	RpcChannel* rpc_channel;
}task_info, *p_task_info;

void async_call_back(SSRpcController* controller, QueryAndUpdateResponse* response)
{
	SSTRACE("async call response result: " << response->result().code() << "; " << response->has_value());
	assert(response->result().code() == 0);
	assert(!controller->Failed());
	if (response->has_value()) {
		string r = response->value();
		assert(r.size() == sizeof(unsigned long));
		unsigned long o = *(unsigned long*)r.data();
		assert(sequence[o] == 0);

		/*if (sequence[o] != 0) {
			char t[256];
			char p[256];
			memset(t, 0, 256);
			memset(p, 0, 256);
			itoa(o, t, 10);
			itoa(::GetCurrentThreadId(), p, 10);
			MessageBoxA(0, t, p, 0);
		}*/
		sequence[o] = o;
	}
	delete response;
	delete controller;
}

void async_call_thread(void* parameter)
{
	p_task_info p_task_info = (task_info*)parameter;
	RpcChannel* rpc_channel = (RpcChannel*)p_task_info->rpc_channel;
	SSDataDomainSvc::Stub stub(rpc_channel);

	for (long i = p_task_info->start; i < p_task_info->end; i++)
	{
		SSRpcController* async_controller = new SSRpcController();
		QueryAndUpdateRequest* async_request = new QueryAndUpdateRequest();
		QueryAndUpdateResponse* async_response = new QueryAndUpdateResponse();
		
		//unsigned long o = ::InterlockedCompareExchange(&sequence[i], 0, i);
		unsigned long o = sequence[i];
		assert(o == i);
		sequence[i] = 0;
		async_request->set_key("aaabbb");
		async_request->set_is_update(true);
		async_request->set_update_value(&o, sizeof(unsigned long));
		
		stub.QueryAndUpdate(
			async_controller,
			async_request,
			async_response,
			NewCallback<SSRpcController*, QueryAndUpdateResponse*>(
				async_call_back,
				async_controller,
				async_response));
		delete async_request;
	}

	SSTRACE("async call thread is exited.");
	_endthread();
}

void sync_call_thread(void* parameter)
{
	p_task_info p_task_info = (task_info*)parameter;
	RpcChannel* rpc_channel = (RpcChannel*)p_task_info->rpc_channel;
	SSDataDomain::SSDataDomainSvcSyncStub sync_stub(rpc_channel);

	for (int i = p_task_info->start; i < p_task_info->end; i++)
	{
		SSRpcController sync_controller;
		QueryAndUpdateRequest sync_request;
		QueryAndUpdateResponse* sync_response;
		
		//unsigned long o = ::InterlockedCompareExchange(&sequence[i], 0, i);
		unsigned long o = sequence[i];
		assert(o == i);
		sequence[i] = 0;
		sync_request.set_key("aaabbb");
		sync_request.set_is_update(true);
		sync_request.set_update_value(&o, sizeof(unsigned long));

		sync_response = sync_stub.QueryAndUpdate(&sync_controller, &sync_request);
		SSTRACE("sync call response result: " << sync_response->result().code());
		assert(sync_response->has_value());
		if (sync_response->has_value()) {
			string r = sync_response->value();
			assert(r.size() == sizeof(unsigned long));
			o = *(unsigned long*)r.data();
			//::InterlockedCompareExchange(&sequence[o], o, 0);
			assert(sequence[o] == 0);
			sequence[o] = o;
		}
		delete sync_response;
	}
	
	SSTRACE("sync call thread is exited.");
	_endthread();
}

int main(int argc, char** argv)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF|_CRTDBG_LEAK_CHECK_DF);
	_CrtSetReportMode(_CRT_ERROR, _CRTDBG_REPORT_MODE);

	GOOGLE_PROTOBUF_VERIFY_VERSION;

	SSTRACE(("int main()"));

	SSRpcChannelProxy* rpc_channel = new SSRpcChannelProxy();
	//rpc_channel->AddChannel(SSEndpoint("127.0.0.101", 9161));
	//rpc_channel->AddChannel(SSEndpoint("127.0.0.102", 9161));
	//rpc_channel->AddChannel(SSEndpoint("127.0.0.103", 9161));
	rpc_channel->AddChannel(SSEndpoint("192.168.1.100", 29161));
	rpc_channel->AddChannel(SSEndpoint("192.168.1.101", 29161));
	rpc_channel->AddChannel(SSEndpoint("192.168.1.102", 29161));
	
	for(unsigned long i = 0; i < MAX_SEQ_LENGTH+1; i++) {
		sequence[i] = i;
	}

	{
		SSRpcController sync_controller;
		QueryAndUpdateRequest sync_request;
		QueryAndUpdateResponse* sync_response;
		
		//unsigned long o = ::InterlockedCompareExchange(&sequence[i], 0, i);
		unsigned long o = sequence[MAX_SEQ_LENGTH];
		sequence[MAX_SEQ_LENGTH] = 0;
		sync_request.set_key("aaabbb");
		sync_request.set_is_update(true);
		sync_request.set_update_value(&o, sizeof(unsigned long));

		SSDataDomain::SSDataDomainSvcSyncStub sync_stub(rpc_channel);
		sync_response = sync_stub.QueryAndUpdate(&sync_controller, &sync_request);
		SSTRACE("sync call response result: " << sync_response->result().code());
		delete sync_response;
	}

	HANDLE threads[MAX_TASK_COUNT];
	task_info parameters[MAX_TASK_COUNT];
	
	int count = 0;
	for (int i = 0; i < MAX_TASK_COUNT; i++) {
		parameters[i].rpc_channel = rpc_channel;
		parameters[i].start = i*TASK_SEQ_LENGTH;
		parameters[i].end = (i+1)*TASK_SEQ_LENGTH;
		threads[i] = (HANDLE)_beginthread(async_call_thread, 0, &parameters[i]);
		count = i;
	}
	count += 1;
	//threads[count] = (HANDLE)_beginthread(sync_call_thread, 0, &parameters[count]);count++;
	//threads[count] = (HANDLE)_beginthread(async_call_thread, 0, &parameters[count]);count++;
	//threads[count] = (HANDLE)_beginthread(async_call_thread, 0, &parameters[count]);count++;
	//threads[count] = (HANDLE)_beginthread(async_call_thread, 0, &parameters[count]);count++;
	//threads[count] = (HANDLE)_beginthread(async_call_thread, 0, &parameters[count]);count++;

	::WaitForMultipleObjects(count, threads, TRUE, INFINITE);

#ifdef _DEBUG
	while (!rpc_channel->CanExit())
		::Sleep(1000);
#else
	::MessageBox(0,0,0,0);
#endif
	
	{
		SSRpcController sync_controller;
		QueryAndUpdateRequest sync_request;
		QueryAndUpdateResponse* sync_response;
		
		sync_request.set_key("aaabbb");
		sync_request.set_is_update(false);

		SSDataDomain::SSDataDomainSvcSyncStub sync_stub(rpc_channel);
		sync_response = sync_stub.QueryAndUpdate(&sync_controller, &sync_request);
		SSTRACE("sync call response result: " << sync_response->result().code());
		assert(sync_response->has_value());
		if (sync_response->has_value()) {
			string r = sync_response->value();
			assert(r.size() == sizeof(unsigned long));
			unsigned long o = *(unsigned long*)r.data();
			assert(sequence[o] == 0);
			sequence[o] = o;
		}
		delete sync_response;
	}
	delete rpc_channel;

	for (unsigned long i = 0; i < MAX_SEQ_LENGTH; i++) {
		assert(sequence[i] == i);
	}

	SSTRACE(("main exit"));
	// dispose log module
#ifdef USE_LOG_MODULE
	SSLogModule::dispose();
#endif
	return 0;
}
