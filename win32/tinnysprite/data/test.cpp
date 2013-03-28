/**
 * ≤‚ ‘≤¢∑¢
 */
#include "SSCommon.h"
#include <process.h>
#include "SSRpcChannelProxy.h"
#include "SSRpcController.h"

#include "ssdatadomain.pb.h"
#include "SSDataDomainSvcSyncStub.h"

using namespace std;
using namespace SSDataDomain;

#define CALL_TIMES  100

void async_call_back(SSRpcController* controller, CreateAccountResponse* response)
{
	SSTRACEMETHOD(("void async_call_back(SSRpcController* controller, CreateAccountResponse* response)"));
	SSTRACE("async call response result: " << response->result().code());
	delete response;
	delete controller;
}

void async_call_thread(void* parameter)
{
	SSRpcChannel* rpc_channel = (SSRpcChannel*)parameter;

	for (int i = 0; i < CALL_TIMES; i++)
	{
		SSRpcController* async_controller = new SSRpcController();
		CreateAccountRequest* async_request = new CreateAccountRequest();
		CreateAccountResponse* async_response = new CreateAccountResponse();

		async_request->set_userid("userid_1234567890");
		async_request->set_email("yaosw@channelsoft.com");
		async_request->set_nickname("yaosw");
		async_request->set_password("123456");

		SSDataDomainSvc::Stub stub(rpc_channel);
		stub.CreateAccount(async_controller,
			async_request,
			async_response,
			NewCallback<SSRpcController*, CreateAccountResponse*>(async_call_back, async_controller, async_response));
		delete async_request;
	}

	SSTRACE("async call thread is exited.");
	_endthread();
}

void sync_call_thread(void* parameter)
{
	SSRpcChannel* rpc_channel = (SSRpcChannel*)parameter;

	for (int i = 0; i < CALL_TIMES; i++)
	{
		SSRpcController sync_controller;
		CreateAccountRequest sync_request;
		//shared_ptr<CreateAccountResponse> sync_response;
		CreateAccountResponse* sync_response;

		sync_request.set_userid("userid_1234567890");
		sync_request.set_email("yaosw@channelsoft.com");
		sync_request.set_nickname("yaosw");
		sync_request.set_password("123456");

		SSDataDomain::SSDataDomainSvcSyncStub sync_stub(rpc_channel);
		sync_response = sync_stub.CreateAccount(&sync_controller, &sync_request);
		SSTRACE("sync call response result: " << sync_response->result().code());
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

	SSTRACE(("int main(int argc, char** argv)"));

	SSRpcChannelProxy* rpc_channel = new SSRpcChannelProxy();
	rpc_channel->AddChannel(SSEndpoint("127.0.0.1", 9161));
	rpc_channel->AddChannel(SSEndpoint("127.0.0.1", 9161));
	rpc_channel->AddChannel(SSEndpoint("127.0.0.1", 9161));
	
	HANDLE threads[4];
	int count = 0;
	threads[count++] = (HANDLE)_beginthread(async_call_thread, 0, rpc_channel);
	threads[count++] = (HANDLE)_beginthread(async_call_thread, 0, rpc_channel);
	threads[count++] = (HANDLE)_beginthread(async_call_thread, 0, rpc_channel);
	threads[count++] = (HANDLE)_beginthread(sync_call_thread, 0, rpc_channel);

	::WaitForMultipleObjects(count, threads, TRUE, INFINITE);

#ifdef _DEBUG
	while (!rpc_channel->CanExit())
		::Sleep(1000);
#else
	::MessageBox(0,0,0,0);
#endif

	delete rpc_channel;

	SSTRACE(("main exit"));
	// dispose log module
#ifdef USE_LOG_MODULE
	SSLogModule::dispose();
#endif
	return 0;
}
