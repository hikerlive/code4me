/*--------------------------------------------------------------------------------*\

     文件：  DataConverter.cpp
     作者：  姚松文
     日期：  2010年10月20日
     说明：  无

\*--------------------------------------------------------------------------------*/
#include "stdafx.h"

#define __SD_CLIENT_DLL_INTERNAL_USE__
#include <ISessionDomainClient.h>

#include "SSDataDomain.api.h"
using namespace SSDataDomainClient;

#include "DataConverter.h"

void convert(SSProfileT* profile_t, SSProfile* profile) {
#pragma push_macro("new")
#undef new
    new(profile_t)SSProfileT();
#pragma pop_macro("new")
    if (profile->has_userid())
        strncpy(profile_t->userid, profile->get_userid(), USER_ID_LENGTH);
    if (profile->has_age())
        profile_t->age = profile->get_age();
    if (profile->has_nickname())
        strncpy(profile_t->nickname, profile->get_nickname(), MAX_STR_LENGTH-1);
    if (profile->has_gender())
        profile_t->sex = profile->get_gender();
    if (profile->has_icon()) {
        size_t size;
        const void* icon = profile->get_icon(&size);
        size = min(size, MAX_ICON_LENGTH);
        memcpy(profile_t->icon, icon, size);
        profile_t->LengthIcon = size;
    }
    if (profile->has_icon_name())
        strncpy(profile_t->iconname, profile->get_icon_name(), MAX_STR_LENGTH-1);
    if (profile->has_binary()) {
        size_t size;
        const void* binary = profile->get_binary(&size);
        profile_t->extend = new char[size];
        memcpy(profile_t->extend, binary, size);
        profile_t->LengthEx = size;
    }
    if (profile->has_introduction()) {
        strncpy(profile_t->remark, profile->get_introduction(), MAX_STR_LENGTH-1);
    }
}

void convert(SSProfile* profile, SSProfileT* profile_t) {
    if (strlen(profile_t->userid) > 0)
        profile->set_userid(profile_t->userid);
    if (profile_t->age > 0)
        profile->set_age(profile_t->age);
    if (SSGender_IsValid(profile_t->sex))
        profile->set_gender((SSGender)profile_t->sex);
    if (profile_t->LengthIcon > 0)
        profile->set_icon(profile_t->icon, profile_t->LengthIcon);
    if (strlen(profile_t->iconname) > 0)
        profile->set_icon_name(profile_t->iconname);
    if (strlen(profile_t->nickname) > 0)
        profile->set_nickname(profile_t->nickname);
    if (profile_t->LengthEx > 0)
        profile->set_binary(profile_t->extend, profile_t->LengthEx);
    if (strlen(profile_t->remark) > 0)
        profile->set_introduction(profile_t->remark);
}

void convert(ShareMsg* message_t, SSMessage* message, ShareType share_type/*, SSMessageType msg_type*/) {
#pragma push_macro("new")
#undef new
    new(message_t)ShareMsg();
#pragma pop_macro("new")
    if (message->has_msgid())
        strncpy(message_t->msgid, message->get_msgid(), MSG_ID_LENGTH);
    if (message->has_parent_msgid())
        strncpy(message_t->parentMsgid, message->get_parent_msgid(), MSG_ID_LENGTH);
    if (message->has_root_msgid())
        strncpy(message_t->rootMsgid, message->get_root_msgid(), MSG_ID_LENGTH);
    if (message->has_seq_number())
        message_t->seqNum = message->get_seq_number();
    if (message->has_timestamp())
        message_t->timestamp = message->get_timestamp();
    if (message->has_sender())
        strncpy(message_t->sender, message->get_sender(), USER_ID_LENGTH);
    if (message->get_recievers_size() > 0) {
        message_t->LengthRecv = message->get_recievers_size()*USER_ID_LENGTH;
        message_t->recievers = new char[message_t->LengthRecv+1];
        memset(message_t->recievers, 0, message_t->LengthRecv+1);
        for (int i = 0; i < message->get_recievers_size(); i++) {
            strncpy(message_t->recievers+USER_ID_LENGTH*i, message->get_recievers(i), USER_ID_LENGTH);
        }
    }
    if (message->has_subject())
        strncpy(message_t->subject, message->get_subject(), MAX_STR_LENGTH-1);
    if (message->has_content()) {
        size_t size;
        const void* content = message->get_content(&size);
        message_t->body = new char[size];
        memcpy(message_t->body, content, size);
        message_t->LengthBody = size;
    }
    message_t->type = share_type;
    SSMessageType msg_type = (SSMessageType)(int)message->get_content_type();
    message_t->Msgtype = msg_type;
}

void convert(SSMessage* message, ShareMsg* message_t) {
    if (strlen(message_t->msgid) > 0)
        message->set_msgid(message_t->msgid);
    if (strlen(message_t->parentMsgid) > 0)
        message->set_parent_msgid(message_t->parentMsgid);
    if (strlen(message_t->rootMsgid) > 0)
        message->set_root_msgid(message_t->rootMsgid);
    if (message_t->seqNum > 0)
        message->set_seq_number(message_t->seqNum);
    if (message_t->timestamp > 0)
        message->set_timestamp(message_t->timestamp);
    if (strlen(message_t->sender) > 0)
        message->set_sender(message_t->sender);
    if (message_t->LengthRecv > 0) {
        int count = message_t->LengthRecv / USER_ID_LENGTH;
        for (int i = 0; i < count; i++) {
            char reciever[USER_ID_SIZE];
            strncpy(reciever, message_t->recievers + i*USER_ID_LENGTH, USER_ID_LENGTH);
            reciever[USER_ID_LENGTH] = '\0';
            message->add_recievers(reciever);
        }
    }
    if (strlen(message_t->subject) > 0)
        message->set_subject(message_t->subject);
    if (message_t->LengthBody > 0)
        message->set_content(message_t->body, message_t->LengthBody);

    message->set_content_type((SSContentType)(int)message_t->Msgtype);
}
void convert(SSContactsT* contact_t, SSDataDomainClient::SSContactEx* contact_ex) {
#pragma push_macro("new")
#undef new
    new(contact_t)SSContactsT();
#pragma pop_macro("new")
    if (contact_ex->has_contactid())
        strncpy(contact_t->contactid, contact_ex->get_contactid(), USER_ID_LENGTH);
    if (contact_ex->has_userid())
        strncpy(contact_t->userid, contact_ex->get_userid(), USER_ID_LENGTH);
    if (contact_ex->has_email())
        strncpy(contact_t->user, contact_ex->get_email(), MAX_STR_LENGTH-1);
    if (contact_ex->has_nickname())
        strncpy(contact_t->nickname, contact_ex->get_nickname(), MAX_STR_LENGTH-1);
    if (contact_ex->has_alias_nickname())
        strncpy(contact_t->nickalias, contact_ex->get_alias_nickname(), MAX_STR_LENGTH-1);
    if (contact_ex->has_icon_name())
        strncpy(contact_t->iconname, contact_ex->get_icon_name(), MAX_STR_LENGTH-1);
    if (contact_ex->has_introduction())
        strncpy(contact_t->remark, contact_ex->get_introduction(), MAX_STR_LENGTH-1);
    if (contact_ex->has_age())
        contact_t->age = contact_ex->get_age();
    if (contact_ex->has_gender())
        contact_t->sex = contact_ex->get_gender();
    if (contact_ex->has_is_following())
        contact_t->IsFollowing = contact_ex->get_is_following();
    if (contact_ex->has_is_follower())
        contact_t->IsFollower = contact_ex->get_is_follower();
    if (contact_ex->has_is_member())
        contact_t->Issystem = contact_ex->get_is_member() == 0 ? false : true;
}

//void convert(SSDataDomainClient::SSContactEx* contact_ex, SSContactsT* contact_t) {
//    if (strlen(contact_t->contactid) > 0)
//        contact_ex->set_contactid(contact_t->contactid);
//    if (strlen(contact_t->userid) > 0)
//        contact_ex->set_userid(contact_t->userid);
//    if (strlen(contact_t->user) > 0)
//        contact_ex->set_email(contact_t->user);
//    if (strlen(contact_t->nickname) > 0)
//        contact_ex->set_nickname(contact_t->nickname);
//    if (strlen(contact_t->nickalias) > 0)
//        contact_ex->set_alias_nickname(contact_t->nickalias);
//    if (strlen(contact_t->iconname) > 0)
//        contact_ex->set_icon_name(contact_t->iconname);
//    if (strlen(contact_t->remark) > 0)
//        contact_ex->set_introduction(contact_t->remark);
//    if (contact_t->age > 0)
//        contact_ex->set_age(contact_t->age);
//    //if (contact_t->sex != -1)
//    //    contact_ex->set_gender(contact_t->sex);
//}

void convert(SSDataDomainClient::SSContact* contact, SSContactsExT* contact_ex_t) {
    if (strlen(contact_ex_t->contactid) > 0)
        contact->set_contactid(contact_ex_t->contactid);
    if (strlen(contact_ex_t->userid) > 0)
        contact->set_userid(contact_ex_t->userid);
    if (strlen(contact_ex_t->user) > 0)
        contact->set_email(contact_ex_t->user);
    if (strlen(contact_ex_t->nickalias) > 0)
        contact->set_alias_nickname(contact_ex_t->nickalias);
}

void convert(SSGroupT* group_t, SSDataDomainClient::SSGroup* group) {
#pragma push_macro("new")
#undef new
    new(group_t)SSGroupT();
#pragma pop_macro("new")
    if (group->has_groupid())
        strncpy(group_t->groupid, group->get_groupid(), USER_ID_LENGTH);
    if (group->has_name())
        strncpy(group_t->name, group->get_name(), MAX_STR_LENGTH-1);
    if (group->has_system())
        group_t->proprety = group->get_system();
    if (group->has_binary()) {
        size_t size;
        const void* binary = group->get_binary(&size);
        group_t->extend = new char[size];
        memcpy(group_t->extend, binary, size);
        group_t->LengthEx = size;
    }
}

void convert(SSDataDomainClient::SSGroup* group, SSGroupT* group_t) {
    if (strlen(group_t->groupid) > 0)
        group->set_groupid(group_t->groupid);
    if (strlen(group_t->name) > 0)
        group->set_name(group_t->name);
    if (group_t->proprety != -1)
        group->set_system(group_t->proprety == 0 ? true : false);
    if (group_t->LengthEx > 0)
        group->set_binary(group_t->extend, group_t->LengthEx);
}

//void convert(SSGroupMembersT* member_t, SSDataDomainClient::SSMember* member) {
//}
//
//void convert(SSDataDomainClient::SSMember* member, SSGroupMembersT* member_t) {
//}

void convert(SSLastTimeT* client_state_t, SSDataDomainClient::SSClientState* client_state) {
#pragma push_macro("new")
#undef new
    new (client_state_t)SSLastTimeT();
#pragma pop_macro("new")
    if (client_state->has_sc_last_user_time())
        strncpy(client_state_t->last_user_time, client_state->get_sc_last_user_time(), MAX_STR_LENGTH-1);
    if (client_state->has_sc_last_group_time())
        strncpy(client_state_t->last_group_time, client_state->get_sc_last_group_time(), MAX_STR_LENGTH-1);
    if (client_state->has_sc_last_contact_time())
        strncpy(client_state_t->last_contact_time, client_state->get_sc_last_contact_time(), MAX_STR_LENGTH-1);
    if (client_state->has_sc_last_member_time())
        strncpy(client_state_t->last_member_time, client_state->get_sc_last_member_time(), MAX_STR_LENGTH-1);
    if (client_state->has_sc_last_follow_time())
        strncpy(client_state_t->last_follow_time, client_state->get_sc_last_follow_time(), MAX_STR_LENGTH-1);
    if (client_state->has_sc_last_send_time())
        strncpy(client_state_t->last_send_time, client_state->get_sc_last_send_time(), MAX_STR_LENGTH-1);
    if (client_state->has_sc_last_recv_time())
        strncpy(client_state_t->last_recv_time, client_state->get_sc_last_recv_time(), MAX_STR_LENGTH-1);
    if (client_state->has_sc_last_favo_time())
        strncpy(client_state_t->last_favo_time, client_state->get_sc_last_favo_time(), MAX_STR_LENGTH-1);
}

void convert(SSDataDomainClient::SSClientState* client_state, SSLastTimeT* client_state_t) {
    if (strlen(client_state_t->last_user_time) > 0)
        client_state->set_sc_last_user_time(client_state_t->last_user_time);
    if (strlen(client_state_t->last_group_time) > 0)
        client_state->set_sc_last_group_time(client_state_t->last_group_time);
    if (strlen(client_state_t->last_contact_time) > 0)
        client_state->set_sc_last_contact_time(client_state_t->last_contact_time);
    if (strlen(client_state_t->last_member_time) > 0)
        client_state->set_sc_last_member_time(client_state_t->last_member_time);
    if (strlen(client_state_t->last_follow_time) > 0)
        client_state->set_sc_last_follow_time(client_state_t->last_follow_time);
    if (strlen(client_state_t->last_send_time) > 0)
        client_state->set_sc_last_send_time(client_state_t->last_send_time);
    if (strlen(client_state_t->last_recv_time) > 0)
        client_state->set_sc_last_recv_time(client_state_t->last_recv_time);
    if (strlen(client_state_t->last_favo_time) > 0)
        client_state->set_sc_last_favo_time(client_state_t->last_favo_time);
}