#include "SSCommon.h"
#include "SSDataDomainSvcSyncStub.h"

using namespace SSDataDomain;
using namespace google::protobuf;
SSDataDomainSvcSyncStub::SSDataDomainSvcSyncStub(RpcChannel* channel)
	: stub(channel)
{
	timeout = INFINITE;
	//this->event_pool.SetMinSize(10);
	//this->event_pool.SetMaxSize(500);
	//this->event_pool.SetCreateFunc(&SSDataDomainSvcSyncStub::event_create_func);
	//this->event_pool.SetDestroyFunc(&SSDataDomainSvcSyncStub::event_destroy_func);
}

SSDataDomainSvcSyncStub::~SSDataDomainSvcSyncStub()
{
}

//HANDLE SSDataDomainSvcSyncStub::event_create_func()
//{
//	return CreateEvent(NULL, TRUE, FALSE, NULL);
//}
//void SSDataDomainSvcSyncStub::event_destroy_func(HANDLE evt)
//{
//	CloseHandle(evt);
//}

template <typename TRequest, typename TResponse>
TResponse* SSDataDomainSvcSyncStub::CallMethod(void (SSDataDomainSvc::Stub::*method)(RpcController*, const TRequest*, TResponse*, Closure*), RpcController* controller, TRequest* request)
{
	TResponse* response = new TResponse();
	HANDLE evt = CreateEvent(NULL, TRUE, FALSE, NULL);
	//HANDLE evt = this->event_pool.GetObject();
	Closure* callback = NewCallback<HANDLE>(SSDataDomainSvcSyncStub::CallCompletion, evt);
	((&stub)->*method)(controller, request, response, callback);
	if (WAIT_TIMEOUT == WaitForSingleObject(evt, timeout)) {
		controller->StartCancel();
		delete response;
		string failed_message("Timeout.");
		controller->SetFailed(failed_message);
		CloseHandle(evt);
		//this->event_pool.SetObject(evt);
		return NULL;
	}
	CloseHandle(evt);
	//this->event_pool.SetObject(evt);
	return response;
}

void SSDataDomainSvcSyncStub::CallCompletion(HANDLE evt)
{
	SetEvent(evt);
}

#define IMPLEMENT_METHOD(fn) \
fn##Response* SSDataDomainSvcSyncStub::##fn(RpcController* controller, fn##Request* request) \
{ \
	return CallMethod<fn##Request, fn##Response>(&SSDataDomainSvc::Stub::##fn, controller, request); \
}

IMPLEMENT_METHOD(CreateAccount)
IMPLEMENT_METHOD(GetAccount)
IMPLEMENT_METHOD(ChangePassword)
IMPLEMENT_METHOD(SetProfile)
IMPLEMENT_METHOD(GetProfile)
//IMPLEMENT_METHOD(GetIcon)
IMPLEMENT_METHOD(Follow)
IMPLEMENT_METHOD(CancelFollow)
IMPLEMENT_METHOD(GetFollowing)
IMPLEMENT_METHOD(GetFollowerCount)
IMPLEMENT_METHOD(GetFollower)
IMPLEMENT_METHOD(Invite)
IMPLEMENT_METHOD(GetInviting)
IMPLEMENT_METHOD(GetInviter)
IMPLEMENT_METHOD(SetSessionInfo)
IMPLEMENT_METHOD(GetSessionInfo)
//IMPLEMENT_METHOD(AddMessage)
//IMPLEMENT_METHOD(RemoveMessage)
IMPLEMENT_METHOD(GetMessage)
IMPLEMENT_METHOD(GetMessageByTimestamp)
IMPLEMENT_METHOD(SendPublicMessage)
IMPLEMENT_METHOD(SendMessage)
IMPLEMENT_METHOD(AddFavoriteMessage)
IMPLEMENT_METHOD(AddContact)
IMPLEMENT_METHOD(RemoveContact)
IMPLEMENT_METHOD(GetContact)
IMPLEMENT_METHOD(AddGroup)
IMPLEMENT_METHOD(RemoveGroup)
IMPLEMENT_METHOD(GetGroup)
IMPLEMENT_METHOD(AddMember)
IMPLEMENT_METHOD(RemoveMember)
IMPLEMENT_METHOD(GetMember)
IMPLEMENT_METHOD(QueryAndUpdate)
IMPLEMENT_METHOD(QueryAndDelete)
IMPLEMENT_METHOD(AddLogicNodeSubscribeInfo)
IMPLEMENT_METHOD(RemoveLogicNodeSubscribeInfo)
IMPLEMENT_METHOD(GetLogicNodeSubscribeInfo)
IMPLEMENT_METHOD(GetFollowingWithProfile)
IMPLEMENT_METHOD(GetFollowerWithProfile)
IMPLEMENT_METHOD(GetContactEx)
