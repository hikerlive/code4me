/**
 * This is a demo.
 */

#include "SSCommon.h"

#include "SSRpcChannelProxy.h"
#include "SSRpcController.h"

#include "ssdatadomain.pb.h"
#include "SSDataDomainSvcSyncStub.h"

using namespace std;
using namespace SSDataDomain;
using namespace google::protobuf;

void async_call_back(SSRpcController* controller, CreateAccountResponse* response)
{
	SSTRACEMETHOD(("void async_call_back(SSRpcController* controller, CreateAccountResponse* response)"));
	SSTRACE("async call response result: " << response->result().code());
	delete response;
	delete controller;
}

int main(int argc, char** argv)
{
#ifdef USE_LOG_MODULE
	// initialize log module
	SSLogModule::initialize();
#endif

	SSTRACE(("int main(int argc, char** argv)"));

	SSRpcChannelProxy rpc_channel;
	rpc_channel.AddChannel(SSEndpoint("127.0.0.1", 9161));
	rpc_channel.AddChannel(SSEndpoint("127.0.0.1", 9161));
	rpc_channel.AddChannel(SSEndpoint("127.0.0.1", 9161));
	rpc_channel.AddChannel(SSEndpoint("127.0.0.1", 9161));
	rpc_channel.AddChannel(SSEndpoint("127.0.0.1", 9161));
	
	// asynchronized call demo
	SSRpcController* async_controller = new SSRpcController();
	CreateAccountRequest* async_request = new CreateAccountRequest();
	CreateAccountResponse* async_response = new CreateAccountResponse();

	async_request->set_userid("userid_1234567890");
	async_request->set_email("yaosw@channelsoft.com");
	async_request->set_nickname("yaosw");
	async_request->set_password("123456");

	SSDataDomainSvc::Stub stub(&rpc_channel);
	stub.CreateAccount(async_controller,
		async_request,
		async_response,
		NewCallback<SSRpcController*, CreateAccountResponse*>(async_call_back, async_controller, async_response));
	delete async_request;

	// synchronized call demo
	SSRpcController sync_controller;
	CreateAccountRequest sync_request;
	CreateAccountResponse* sync_response;

	sync_request.set_userid("userid_1234567890");
	sync_request.set_email("yaosw@channelsoft.com");
	sync_request.set_nickname("yaosw");
	sync_request.set_password("123456");

	SSDataDomain::SSDataDomainSvcSyncStub sync_stub(&rpc_channel);
	sync_response = sync_stub.CreateAccount(&sync_controller, &sync_request);
	SSTRACE("sync call response result: " << sync_response->result().code());
	delete sync_response;

#ifdef USE_LOG_MODULE
	// dispose log module
	SSLogModule::dispose();
#endif
	return 0;
}