/**
 * 单元测试
 */
#include "SSCommon.h"
#include <process.h>
#include <tchar.h>

#include "SSRpcChannelProxy.h"
#include "SSRpcController.h"

#include "ssdatadomain.pb.h"
#include "SSDataDomainSvcSyncStub.h"

#include "cppunit/TestCase.h"
#include "CppUnit/extensions/HelperMacros.h"
#include "cppunit/extensions/TestFactoryRegistry.h"

#ifdef USE_MFC_MODULE
#include "cppunit/ui/MFC/TestRunner.h"
#else
#include "cppunit/ui/text/TestRunner.h"
#endif


#ifdef _DEBUG
	#pragma comment(lib, "cppunitd")
	#pragma comment(lib, "TestRunnerd")
#else
	#pragma comment(lib, "cppunit")
	#pragma comment(lib, "TestRunner")
#endif

using namespace std;
using namespace SSDataDomain;
using namespace CppUnit;
using namespace google::protobuf;

#define TESTINTERFACE(name, a, b) do{ \
	SSRpcController controller; \
	##name##Request request; \
	##name##Response* response; \
	{ \
		##a; \
	} \
	response = sync_stub->##name(&controller, &request); \
	if (response == NULL) { \
		SSTRACE("no response result."); \
		break; \
	} \
	SSTRACE("response result: " << response->result().code()); \
	{ \
		##b; \
	} \
	delete response; \
}while(0)

class SSDataDomainTester : public TestCase
{
public:
	// 宏定义
	CPPUNIT_TEST_SUITE(SSDataDomainTester);
		CPPUNIT_TEST(TestCreateAccount);
		CPPUNIT_TEST(TestGetAccount);
		CPPUNIT_TEST(TestChangePassword);
		CPPUNIT_TEST(TestSetProfile);
		CPPUNIT_TEST(TestGetProfile);
		CPPUNIT_TEST(TestFollow);
		CPPUNIT_TEST(TestCancelFollow);
		CPPUNIT_TEST(TestGetFollowing);
		CPPUNIT_TEST(TestGetFollowerCount);
		CPPUNIT_TEST(TestGetFollower);
		CPPUNIT_TEST(TestGetFollower1);
		CPPUNIT_TEST(TestGetFollower2);
		CPPUNIT_TEST(TestInvite);
		CPPUNIT_TEST(TestGetInviting);
		CPPUNIT_TEST(TestGetInviter);
		CPPUNIT_TEST(TestSetSessionInfo);
		CPPUNIT_TEST(TestGetSessionInfo);
		CPPUNIT_TEST(TestAddMessage);
		CPPUNIT_TEST(TestRemoveMessage);
		CPPUNIT_TEST(TestGetMessage);
		CPPUNIT_TEST(TestGetMessageByTimestamp);
		CPPUNIT_TEST(TestSendPublicMessage);
		CPPUNIT_TEST(TestSendMessage);
		CPPUNIT_TEST(TestAddFavoriteMessage);
		CPPUNIT_TEST(TestAddContact);
		CPPUNIT_TEST(TestRemoveContact);
		CPPUNIT_TEST(TestGetContact);
		CPPUNIT_TEST(TestAddGroup);
		CPPUNIT_TEST(TestRemoveGroup);
		CPPUNIT_TEST(TestGetGroup);
		CPPUNIT_TEST(TestAddMember);
		CPPUNIT_TEST(TestRemoveMember);
		CPPUNIT_TEST(TestGetMember);
		CPPUNIT_TEST(TestQueryAndUpdate);
		CPPUNIT_TEST(TestQueryAndDelete);

		CPPUNIT_TEST(TestAddLogicNodeSubscribeInfo);
		CPPUNIT_TEST(TestRemoveLogicNodeSubscribeInfo);
		CPPUNIT_TEST(TestGetLogicNodeSubscribeInfo);
		CPPUNIT_TEST(TestGetFollowingWithProfile);
		CPPUNIT_TEST(TestGetFollowerWithProfile);
		CPPUNIT_TEST(TestGetContactEx);
	CPPUNIT_TEST_SUITE_END();

private:
	SSRpcChannelProxy* rpc_channel;
	SSDataDomain::SSDataDomainSvcSyncStub* sync_stub;

public:
	virtual void setUp()
	{
		rpc_channel = new SSRpcChannelProxy();
		rpc_channel->AddChannel(SSEndpoint("192.168.1.101", 29161));
		rpc_channel->AddChannel(SSEndpoint("192.168.1.102", 29161));
		rpc_channel->AddChannel(SSEndpoint("192.168.1.100", 29161));
		//rpc_channel->AddChannel(SSEndpoint("127.0.0.101", 9161));
		//rpc_channel->AddChannel(SSEndpoint("127.0.0.102", 9161));
		//rpc_channel->AddChannel(SSEndpoint("127.0.0.103", 9161));
		sync_stub = new SSDataDomain::SSDataDomainSvcSyncStub(rpc_channel);
		sync_stub->SetTimeout(3000);
	};

	virtual void tearDown()
	{
#ifdef _DEBUG
		SSTRACE("Waiting for channel...");
		while (!rpc_channel->CanExit())
			::Sleep(1000);
#endif
		SSTRACE("Release channel...");
		delete rpc_channel;
		delete sync_stub;
	};

private:
	template<typename T>
	void Print(const T& t)
	{
		SSTRACE(t);
	}

	void Print(const SSAccount& account)
	{
		SSTRACE(account.userid());
		if (account.has_email())
			SSTRACE(account.email());
		if (account.has_password())
			SSTRACE(account.password());
	}

	void Print(const SSProfile& profile)
	{
		SSTRACE(profile.userid());
		if (profile.has_nickname())
			SSTRACE(profile.nickname());
		if (profile.has_age())
			SSTRACE(profile.age());
		if (profile.has_gender())
			SSTRACE(profile.gender());
		if (profile.has_icon())
			profile.icon();
	}

	void Print(const SSSessionInfo& session_info)
	{
		if (session_info.client_state().has_last_send_time())
			SSTRACE(session_info.client_state().last_send_time());
		if (session_info.client_state().has_last_fetch_time())
			SSTRACE(session_info.client_state().last_fetch_time());
		if (session_info.client_state().has_last_seq_number())
			SSTRACE(session_info.client_state().last_seq_number());
		if (session_info.client_state().has_last_logout_time())
			SSTRACE(session_info.client_state().last_logout_time());
		if (session_info.client_state().has_last_client_identifier())
			SSTRACE(session_info.client_state().last_client_identifier());

		Print(session_info.message_state());
	}

	void Print(const SSMessageState& message_state)
	{
		SSTRACE(message_state.userid());
		SSTRACE(message_state.seq_number());
	}

	void Print(const SSMessage& message)
	{
		SSTRACE(message.msgid());
		if (message.has_timestamp())
			SSTRACE(message.timestamp());
		if (message.has_seq_number())
			SSTRACE(message.seq_number());
		SSTRACE(message.sender());
		Print(message.recievers());
		SSTRACE(message.subject());
		SSTRACE(message.content_type());
	}

	void Print(const SSContact& contact)
	{
		SSTRACE(contact.contactid());
		if (contact.has_userid())
			SSTRACE(contact.userid());
		if (contact.has_email())
			SSTRACE(contact.email());
		if (contact.has_alias_nickname())
			SSTRACE(contact.alias_nickname());
	}

	void Print(const SSGroup& group)
	{
		SSTRACE(group.groupid());
		if (group.has_name())
			SSTRACE(group.name());
		if (group.has_system())
			SSTRACE(group.system());
	}

	void Print(const SSMember& member)
	{
		SSTRACE(member.groupid());
		Print(member.members());
	}

	template<class T>
	void Print(const RepeatedPtrField<T>& field)
	{
		for (RepeatedPtrField<T>::const_iterator it = field.begin(); it != field.end(); it++)
		{
			Print(*it);
		}
	}
public:
	void TestCreateAccount()
	{
		//SSRpcController controller;
		//CreateAccountRequest request;
		//CreateAccountResponse* response;

		//request.set_userid("userid_1234567890");
		//request.set_user("yaosw@channelsoft.com");
		//request.set_nickname("yaosw");
		//request.set_password("123456");

		//SSDataDomain::SSDataDomainSvcSyncStub sync_stub(rpc_channel);
		//response = sync_stub.CreateAccount(&controller, &request);
		//SSTRACE("response result: " << response->result().code());
		//delete response;

		char userid[16] = "1234567890";

		TESTINTERFACE(
			CreateAccount,
			request.set_userid(userid);
			request.set_email("yaosw@channelsoft.com");
			request.set_nickname("yaosw");
			request.set_password("123456");
			,
			0
			);
	}

	void TestGetAccount()
	{
		char userid[16] = "1234567890";
		TESTINTERFACE(
			GetAccount,
			//request.set_userid("userid_1234567890");
			request.set_userid(userid);
			,
			Print(response->account());
			);
	}

	void TestChangePassword()
	{
		TESTINTERFACE(
			ChangePassword,
			request.set_userid("userid_1234567890");
			request.set_new_password("654321");
			,
			0
			);
	}

	void TestSetProfile()
	{
		TESTINTERFACE(
			SetProfile,
			request.set_userid("userid_1234567890");
			request.mutable_profile()->set_age(10);
			request.mutable_profile()->set_gender(UNKNOWN);
			request.mutable_profile()->set_userid("userid_1234567890");
			request.mutable_profile()->set_nickname("haha");
			,
			0
			);
	}

	void TestGetProfile()
	{
		TESTINTERFACE(
			GetProfile,
			request.add_userid_list("userid_1234567890");
			,
			Print(response->profile_list());
			);
	}

	void TestFollow()
	{
		TESTINTERFACE(
			Follow,
			request.set_follower_userid("userid_1234567890");
			request.add_following_userid_list("following_1");
			request.add_following_userid_list("following_2");
			request.add_following_userid_list("following_3");
			,
			0
			);
		TESTINTERFACE(
			Follow,
			request.set_follower_userid("follower_1");
			request.add_following_userid_list("userid_1234567890");
			,
			0
			);
		TESTINTERFACE(
			Follow,
			request.set_follower_userid("follower_2");
			request.add_following_userid_list("userid_1234567890");
			,
			0
			);
		TESTINTERFACE(
			Follow,
			request.set_follower_userid("follower_3");
			request.add_following_userid_list("userid_1234567890");
			,
			0
			);
		TESTINTERFACE(
			Follow,
			request.set_follower_userid("follower_4");
			request.add_following_userid_list("userid_1234567890");
			,
			0
			);
		TESTINTERFACE(
			Follow,
			request.set_follower_userid("follower_5");
			request.add_following_userid_list("userid_1234567890");
			,
			0
			);
		TESTINTERFACE(
			Follow,
			request.set_follower_userid("follower_6");
			request.add_following_userid_list("userid_1234567890");
			,
			0
			);
		TESTINTERFACE(
			Follow,
			request.set_follower_userid("follower_7");
			request.add_following_userid_list("userid_1234567890");
			,
			0
			);
		TESTINTERFACE(
			Follow,
			request.set_follower_userid("follower_8");
			request.add_following_userid_list("userid_1234567890");
			,
			0
			);
		TESTINTERFACE(
			Follow,
			request.set_follower_userid("follower_9");
			request.add_following_userid_list("userid_1234567890");
			,
			0
			);
	}

	void TestCancelFollow()
	{
		TESTINTERFACE(
			CancelFollow,
			request.set_follower_userid("userid_1234567890");
			request.add_following_userid_list("following_1");
			request.add_following_userid_list("following_2");
			request.add_following_userid_list("following_3");
			,
			0
			);
		TESTINTERFACE(
			CancelFollow,
			request.set_follower_userid("follower_1");
			request.add_following_userid_list("userid_1234567890");
			,
			0
			);
		TESTINTERFACE(
			CancelFollow,
			request.set_follower_userid("follower_2");
			request.add_following_userid_list("userid_1234567890");
			,
			0
			);
		TESTINTERFACE(
			CancelFollow,
			request.set_follower_userid("follower_3");
			request.add_following_userid_list("userid_1234567890");
			,
			0
			);
	}

	void TestGetFollowing()
	{
		TESTINTERFACE(
			GetFollowing,
			request.set_userid("userid_1234567890");
			,
			Print(response->following_userid_list());
			);
	}

	void TestGetFollowerCount()
	{
		TESTINTERFACE(
			GetFollowerCount,
			request.set_userid("userid_1234567890");
			,
			Print(response->count());
			);
	}

	void TestGetFollower()
	{
		TESTINTERFACE(
			GetFollower,
			request.set_userid("userid_1234567890");
			,
			Print(response->follower_userid_list());
			);
	}
	void TestGetFollower1()
	{
		TESTINTERFACE(
			GetFollower,
			request.set_userid("userid_1234567890");
			request.set_cursor_id("follower_5");
			request.set_count(3);
			,
			Print(response->follower_userid_list());
			);
	}
	void TestGetFollower2()
	{
		TESTINTERFACE(
			GetFollower,
			request.set_userid("userid_1234567890");
			request.set_cursor_id("follower_5");
			request.set_count(-3);
			,
			Print(response->follower_userid_list());
			);
	}

	void TestInvite()
	{
		TESTINTERFACE(
			Invite,
			request.set_inviter_userid("userid_1234567890");
			request.add_inviting_userid_list("inviting_1");
			request.add_inviting_userid_list("inviting_2");
			request.add_inviting_userid_list("inviting_3");
			,
			0
			);
		TESTINTERFACE(
			Invite,
			request.set_inviter_userid("inviter_1");
			request.add_inviting_userid_list("userid_1234567890");
			,
			0
			);
		TESTINTERFACE(
			Invite,
			request.set_inviter_userid("inviter_2");
			request.add_inviting_userid_list("userid_1234567890");
			,
			0
			);
		TESTINTERFACE(
			Invite,
			request.set_inviter_userid("inviter_3");
			request.add_inviting_userid_list("userid_1234567890");
			,
			0
			);
	}

	void TestGetInviting()
	{
		TESTINTERFACE(
			GetInviting,
			request.set_userid("userid_1234567890");
			,
			Print(response->inviting_userid_list());
			);
	}

	void TestGetInviter()
	{
		TESTINTERFACE(
			GetInviter,
			request.set_userid("userid_1234567890");
			,
			Print(response->inviter_userid_list());
			);
	}

	void TestSetSessionInfo()
	{
		TESTINTERFACE(
			SetSessionInfo,
			request.set_userid("userid_1234567890");
			request.mutable_session_info()->set_userid("userid_1234567890");
			request.mutable_session_info()->mutable_client_state()->set_last_send_time(1);
			request.mutable_session_info()->mutable_client_state()->set_last_fetch_time(2);
			request.mutable_session_info()->mutable_client_state()->set_last_seq_number(3);
			request.mutable_session_info()->mutable_client_state()->set_last_logout_time(4);
			request.mutable_session_info()->mutable_client_state()->set_last_client_identifier("5");
			SSMessageState* message_state;
			message_state = request.mutable_session_info()->add_message_state();
			message_state->set_userid("userid_1");
			message_state->set_seq_number(10001);
			message_state = request.mutable_session_info()->add_message_state();
			message_state->set_userid("userid_2");
			message_state->set_seq_number(10002);
			,
			0
			);
	}

	void TestGetSessionInfo()
	{
		TESTINTERFACE(
			GetSessionInfo,
			request.set_userid("userid_1234567890");
			,
			Print(response->session_info());
			);
	}

	void TestAddMessage()
	{
		CPPUNIT_ASSERT(FALSE);
	}

	void TestRemoveMessage()
	{
		CPPUNIT_ASSERT(FALSE);
	}

	void TestGetMessage()
	{
		TESTINTERFACE(
			GetMessage,
			request.set_userid("userid_1234567890");
			request.set_source_box(PUBBOX);
			,
			Print(response->msg_list());
			);
		TESTINTERFACE(
			GetMessage,
			request.set_userid("userid_1234567890");
			request.set_source_box(INBOX);
			,
			Print(response->msg_list());
			);
		TESTINTERFACE(
			GetMessage,
			request.set_userid("userid_1234567890");
			request.set_source_box(OUTBOX);
			,
			Print(response->msg_list());
			);
		TESTINTERFACE(
			GetMessage,
			request.set_userid("userid_1234567890");
			request.set_source_box(FAVBOX);
			,
			Print(response->msg_list());
			);
	}

	void TestGetMessageByTimestamp()
	{
		std::string userid("userid_1234567890");

		TESTINTERFACE(
			GetMessageByTimestamp,
			request.set_userid(userid);
			request.set_source_box(PUBBOX);
			request.set_start_timestamp(102);
			request.set_end_timestamp(105);
			request.set_count(100);
			,
			Print(response->msg_list());
			);
		TESTINTERFACE(
			GetMessageByTimestamp,
			request.set_userid(userid);
			request.set_source_box(PUBBOX);
			request.set_start_timestamp(105);
			request.set_end_timestamp(102);
			request.set_count(100);
			,
			Print(response->msg_list());
			);
		//TESTINTERFACE(
		//	GetMessageByTimestamp,
		//	request.set_userid(userid);
		//	request.set_source_box(INBOX);
		//	request.set_start_timestamp(0);
		//	request.set_end_timestamp(100);
		//	request.set_count(-100);
		//	,
		//	Print(response->msg_list());
		//	);
		//TESTINTERFACE(
		//	GetMessageByTimestamp,
		//	request.set_userid(userid);
		//	request.set_source_box(OUTBOX);
		//	request.set_start_timestamp(0);
		//	request.set_end_timestamp(100);
		//	request.set_count(-100);
		//	,
		//	Print(response->msg_list());
		//	);
		//TESTINTERFACE(
		//	GetMessageByTimestamp,
		//	request.set_userid(userid);
		//	request.set_source_box(FAVBOX);
		//	request.set_start_timestamp(0);
		//	request.set_end_timestamp(100);
		//	request.set_count(-100);
		//	,
		//	Print(response->msg_list());
		//	);
	}

	void TestSendPublicMessage()
	{
		TESTINTERFACE(
			SendPublicMessage,
			request.mutable_msg()->set_seq_number(101);
			request.mutable_msg()->set_timestamp(1000000);
			request.mutable_msg()->set_sender("userid_1234567890");
			request.mutable_msg()->set_content_type(NORMAL);
			request.mutable_msg()->set_subject("public message1");
			,
			0
			);
		TESTINTERFACE(
			SendPublicMessage,
			request.mutable_msg()->set_seq_number(102);
			request.mutable_msg()->set_timestamp(1000000);
			request.mutable_msg()->set_sender("userid_1234567890");
			request.mutable_msg()->set_content_type(NORMAL);
			request.mutable_msg()->set_subject("public message2");
			,
			0
			);
		TESTINTERFACE(
			SendPublicMessage,
			request.mutable_msg()->set_seq_number(103);
			request.mutable_msg()->set_timestamp(1000000);
			request.mutable_msg()->set_sender("userid_1234567890");
			request.mutable_msg()->set_content_type(NORMAL);
			request.mutable_msg()->set_subject("public message3");
			,
			0
			);
		TESTINTERFACE(
			SendPublicMessage,
			request.mutable_msg()->set_seq_number(104);
			request.mutable_msg()->set_timestamp(1000000);
			request.mutable_msg()->set_sender("userid_1234567890");
			request.mutable_msg()->set_content_type(NORMAL);
			request.mutable_msg()->set_subject("public message4");
			,
			0
			);
		TESTINTERFACE(
			SendPublicMessage,
			request.mutable_msg()->set_seq_number(105);
			request.mutable_msg()->set_timestamp(1000000);
			request.mutable_msg()->set_sender("userid_1234567890");
			request.mutable_msg()->set_content_type(NORMAL);
			request.mutable_msg()->set_subject("public message5");
			,
			0
			);
	}

	void TestSendMessage()
	{
		TESTINTERFACE(
			SendMessage,
			request.mutable_msg()->set_timestamp(1000000);
			request.mutable_msg()->set_sender("userid_1234567890");
			request.mutable_msg()->add_recievers("reciever_1");
			request.mutable_msg()->add_recievers("reciever_2");
			request.mutable_msg()->add_recievers("reciever_3");
			request.mutable_msg()->set_subject("send to message");
			request.mutable_msg()->set_content_type(NORMAL);
			,
			0
			);
	}

	void TestAddFavoriteMessage()
	{
		TESTINTERFACE(
			AddFavoriteMessage,
			request.mutable_msg()->set_timestamp(1000000);
			request.mutable_msg()->set_sender("userid_1234567890");
			request.mutable_msg()->set_subject("favorite message");
			request.mutable_msg()->set_content_type(NORMAL);
			,
			0
			);
	}

	void TestAddContact()
	{
		TESTINTERFACE(
			AddContact,
			request.set_userid("userid_1234567890");
			SSContact* contact = NULL;
			contact = request.add_contact_list();
			contact->set_contactid("contactid_1");
			contact->set_email("a@email.com");
			contact = request.add_contact_list();
			contact->set_contactid("contactid_2");
			contact->set_email("b@email.com");
			contact = request.add_contact_list();
			contact->set_contactid("contactid_3");
			contact->set_email("c@email.com");
			,
			0
			);
	}

	void TestRemoveContact()
	{
		TESTINTERFACE(
			RemoveContact,
			request.set_userid("userid_1234567890");
			request.add_contactid_list("contactid_1");
			request.add_contactid_list("contactid_2");
			request.add_contactid_list("contactid_3");
			,
			0
			);
	}

	void TestGetContact()
	{
		TESTINTERFACE(
			GetContact,
			request.set_userid("userid_1234567890");
			,
			Print(response->contact_list());
			);
	}

	void TestAddGroup()
	{
		TESTINTERFACE(
			AddGroup,
			request.set_userid("userid_1234567890");
			SSGroup* group = NULL;
			group = request.add_group_list();
			group->set_groupid("groupid_1");
			group->set_name("group1");
			group = request.add_group_list();
			group->set_groupid("groupid_2");
			group->set_name("group2");
			group = request.add_group_list();
			group->set_groupid("groupid_3");
			group->set_name("group3");
			,
			0
			);
	}

	void TestRemoveGroup()
	{
		TESTINTERFACE(
			RemoveGroup,
			request.set_userid("userid_1234567890");
			request.add_groupid_list("groupid_1");
			request.add_groupid_list("groupid_2");
			request.add_groupid_list("groupid_3");
			,
			0
			);
	}

	void TestGetGroup()
	{
		TESTINTERFACE(
			GetGroup,
			request.set_userid("userid_1234567890");
			,
			Print(response->group_list());
			);
	}

	void TestAddMember()
	{
		TESTINTERFACE(
			AddMember,
			request.set_userid("userid_1234567890");
			SSMember* member = NULL;
			member = request.add_member_list();
			member->set_groupid("groupid_1");
			member->add_members("memberid_1_1");
			member->add_members("memberid_1_2");
			member->add_members("memberid_1_3");
			member = request.add_member_list();
			member->set_groupid("groupid_2");
			member->add_members("memberid_2_1");
			member->add_members("memberid_2_2");
			member->add_members("memberid_2_3");
			member = request.add_member_list();
			member->set_groupid("groupid_3");
			member->add_members("memberid_3_1");
			member->add_members("memberid_3_2");
			member->add_members("memberid_3_3");
			,
			0
			);
	}

	void TestRemoveMember()
	{
		TESTINTERFACE(
			RemoveMember,
			request.set_userid("userid_1234567890");
			SSMember* member = NULL;
			member = request.add_member_list();
			member->set_groupid("groupid_1");
			member->add_members("memberid_1_2");
			member = request.add_member_list();
			member->set_groupid("groupid_2");
			,
			0
			);
	}

	void TestGetMember()
	{
		TESTINTERFACE(
			GetMember,
			request.set_userid("userid_1234567890");
			,
			Print(response->member_list());
			);
	}

	void TestQueryAndUpdate()
	{
		TESTINTERFACE(
			QueryAndUpdate,
			request.set_key("aaabbb");
			request.set_is_update(true);
			unsigned long o = 12;
			request.set_update_value(&o, sizeof(unsigned long));
			,
			string r = response->value();
			assert(r.size() == sizeof(unsigned long));
			unsigned long o = *(unsigned long*)r.data();
			Print(o);
			);
	}

	void TestQueryAndDelete()
	{
		CPPUNIT_ASSERT(FALSE);
	}

    
	void TestAddLogicNodeSubscribeInfo()
    {
        CPPUNIT_ASSERT(FALSE);
    }

	void TestRemoveLogicNodeSubscribeInfo()
    {
        CPPUNIT_ASSERT(FALSE);
    }
	
    void TestGetLogicNodeSubscribeInfo()
    {
        CPPUNIT_ASSERT(FALSE);
    }

	void TestGetFollowingWithProfile()
    {
        CPPUNIT_ASSERT(FALSE);
    }

    void TestGetFollowerWithProfile()
    {
        CPPUNIT_ASSERT(FALSE);
    }

	void TestGetContactEx()
    {
        CPPUNIT_ASSERT(FALSE);
    }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(SSDataDomainTester, "ssdatadomaintester"); 

int main(int argc, char** argv)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF|_CRTDBG_LEAK_CHECK_DF);
	_CrtSetReportMode(_CRT_ERROR, _CRTDBG_REPORT_MODE);

	GOOGLE_PROTOBUF_VERIFY_VERSION;

	SSTRACE(("int main(int argc, char** argv)"));
	int nRetCode = 0;
	
#ifdef USE_MFC_MODULE
	class CMyApp : public CWinApp {
	public:
		virtual BOOL InitInstance() {
			MfcUi::TestRunner runner;
			TestFactoryRegistry &registry = TestFactoryRegistry::getRegistry("ssdatadomaintester");
			runner.addTest(registry.makeTest());
			runner.run();
			return CWinApp::InitInstance();
		}
	} theApp;
	extern int AFXAPI AfxWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,LPTSTR lpCmdLine, int nCmdShow);
	nRetCode = AfxWinMain(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), SW_SHOW);

	//CWinApp theApp;
	//if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), SW_SHOW)) {
	//	nRetCode = -1;
	//} else {
	//	HINSTANCE exe = ::GetModuleHandle(NULL);
	//	HINSTANCE dll = ::GetModuleHandle(_T("TestRunnerd"));
	//	AfxSetResourceHandle(dll);
	//	MfcUi::TestRunner runner;
	//	TestFactoryRegistry &registry = TestFactoryRegistry::getRegistry("ssdatadomaintester");
	//	runner.addTest(registry.makeTest());
	//	runner.run();
	//	AfxSetResourceHandle(exe);
	//	AfxWinTerm();
	//}
#else
	TextUi::TestRunner runner;
	TestFactoryRegistry &registry = TestFactoryRegistry::getRegistry("ssdatadomaintester");
	runner.addTest(registry.makeTest());
	runner.run();
#endif
	
	SSTRACE(("main exit"));
	// dispose log module
#ifdef USE_LOG_MODULE
	SSLogModule::dispose();
#endif
	return nRetCode;
}