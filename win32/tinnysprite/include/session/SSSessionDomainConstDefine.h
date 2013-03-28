#pragma once
#include "ISessionDomainClient.h"
#include <list>

enum SSSD_Result
{
	RESULT_SUCCESSS_SSSD_OK								= 1,
	RESULT_ERROR_SSSD_ASYN_MODEL_INIT_FAIL				= 2,
	RESULT_ERROR_SSSD_ASYN_MODEL_START_FAIL				= 3,
	RESULT_ERROR_SSSD_ASYN_MODEL_SET_UDPADDR_FAIL		= 4,
	RESULT_ERROR_SSSD_ASYN_MODEL_SET_TCPADDR_FAIL		= 5,
	RESULT_ERROR_SSSD_MEMORY_ALLOC_FAIL					= 6,
	RESULT_ERROR_SSSD_ASYN_MODEL_REG_PREPROSS_FAIL		= 7,
	RESULT_ERROR_SSSD_NEW_DealClientCMDSControl_FAIL	= 8,
	RESULT_ERROR_SSSD_NEW_FollowTargetSControl_FAIL		= 9,
	RESULT_ERROR_SSSD_NEW_LogicNearTargetSControl_FAIL	= 10,
	RESULT_ERROR_SSSD_NEW_RegistSession_FAIL			= 11,

	RESULT_ERROR_CONFIG_ERROR,
	RESULT_ERROR_CONFIG_IPPORT_ERROR,


};

#define MAX_CHECK_CODE							20
#define SS_SEND_MAX_LEN							2000
#define FETCH_PRE_TIME							5*60*1000

#define FOLLOW_TARGET_NO_SUBSCRIBE_CHECK_TIME	5*60*1000
#define FOLLOW_TARGET_LOGIC_SUBSCRIBE_TIME_OUT	10*1000

#define DHT_FINDNODE_RESULT_MAXNUM				20
#define DHT_LOGIC_NODE_NUM						10		//这里需要考虑改为配置文件配置

#define GET_INBOX_MAX_COUNT_PER_TIMES			1000

typedef std::map< std::string, uint32> FollowTargetMap;

struct SSSessionDomainMsg
{
	std::string				 m_msgid;			//
	std::string				 m_parentMsgid;
	std::string				 m_rootMsgid;


	ShareType				 m_type;
	int						 m_seq_number; // 此字段可选，也就是msgdi前面的8字节
	__int64					 m_timestamp; // 此字段可选，也就是msgid前面的8字节
	std::string				 m_sender;
	std::vector<std::string> m_recievers;
	std::string				 m_subject; // 消息主题，网页为标题，一句话则全部放在这里
	SSMessageType			 m_content_type;
	std::string				 m_content; // 消息内容

	int serialize(char *pBuff, unsigned int len)
	{ 
		if( pBuff == NULL || len < getRealSize())
			return -1;

		int length = m_msgid.length();

		int pointer = 0;
		memcpy( pBuff + pointer, &length, 4);
		pointer += 4;

		if( length != 0)
		{
			memcpy( pBuff + pointer, m_msgid.c_str(), length);
			pointer += length;
		}

		int parentLen = m_parentMsgid.length();
		memcpy( pBuff + pointer, &parentLen, 4);
		pointer += 4;

		if( parentLen != 0)
		{
			memcpy( pBuff + pointer, m_parentMsgid.c_str(), parentLen);

			pointer += parentLen;
		}

		int rootLen = m_rootMsgid.length();
		memcpy( pBuff + pointer, & rootLen, 4);
		pointer += 4;

		if( rootLen != 0)
		{
			memcpy( pBuff + pointer, m_rootMsgid.c_str(), rootLen);

			pointer += rootLen;
		}
		
		memcpy( pBuff + pointer, &m_type, 4);
		pointer += 4;

		memcpy( pBuff + pointer, &m_seq_number, 4);
		pointer += 4;

		memcpy( pBuff + pointer, &m_timestamp, 8);
		pointer += 8;

		int senderLen = m_sender.length();
		memcpy( pBuff + pointer, &senderLen, 4);
		pointer += 4;

		if( senderLen != 0)
		{
			memcpy( pBuff + pointer, m_sender.c_str(), senderLen);

			pointer += senderLen;
		}

		int receiverNum = m_recievers.size();
		memcpy( pBuff + pointer, & receiverNum, 4);
		pointer += 4;

		if( receiverNum != 0)
		{
			std::vector<std::string>::iterator it = m_recievers.begin();
			for( ; it != m_recievers.end(); it++)
			{
				int reiverLen = it->length();
				memcpy( pBuff + pointer, &reiverLen, 4);
				pointer += 4;

				if( reiverLen != 0)
				{
					memcpy( pBuff + pointer, it->c_str(), reiverLen);

					pointer += reiverLen;
				}
			}
		}
		
		int subjectLen = m_subject.length();
		memcpy( pBuff + pointer, &subjectLen, 4);
		pointer += 4;

		if( subjectLen != 0)
		{
			memcpy( pBuff + pointer, m_subject.c_str(), subjectLen);

			pointer += subjectLen;
		}

		memcpy( pBuff + pointer, &m_content_type, sizeof( m_content_type));
		pointer += sizeof( m_content_type);

		int contentLen = m_content.length();
		memcpy( pBuff + pointer, &contentLen, 4);
		pointer += 4;

		if( contentLen != 0)
		{
			memcpy( pBuff + pointer, m_content.c_str(), contentLen);
			pointer += contentLen;
		}


		return pointer;
	}

	int unserialize(const char * pBuff, unsigned int len)
	{ 
		
		if( pBuff == NULL || len < getRealSize())
			return -1;

		int pointer = 0;

		int msgidLen = 0;
		memcpy(&msgidLen, pBuff + pointer, 4);
		pointer += 4;

		if( msgidLen > 0)
		{
			m_msgid.append(pBuff + pointer, msgidLen);
			pointer += msgidLen;
		}

		int parentLen = 0;
		memcpy(&parentLen, pBuff + pointer, 4);
		pointer += 4;

		if( parentLen > 0)
		{
			m_parentMsgid.append(pBuff + pointer, parentLen);
			pointer += parentLen;
		}


		int rootLen = 0;
		memcpy(&rootLen, pBuff + pointer, 4);
		pointer += 4;

		if( rootLen > 0)
		{
			m_rootMsgid.append(pBuff + pointer, rootLen);
			pointer += rootLen;
		}

		memcpy( &m_type, pBuff + pointer, 4);
		pointer += 4;

		memcpy( &m_seq_number,pBuff + pointer,  4);
		pointer += 4;

		memcpy( &m_timestamp,pBuff + pointer,  8);
		pointer += 8;

		int senderLen = 0;
		memcpy(&senderLen, pBuff + pointer, 4);
		pointer += 4;

		if( senderLen > 0)
		{
			m_sender.append(pBuff + pointer, senderLen);
			pointer += senderLen;
		}

		int receiveNum = 0;
		memcpy( &receiveNum, pBuff + pointer, 4);
		pointer += 4;

		if( receiveNum > 0)
		{
			for( int i = 0; i< receiveNum; i++)
			{
				int receiveLen = 0;
				memcpy(&receiveLen, pBuff + pointer, 4);
				pointer += 4;

				if( receiveLen > 0)
				{
					std::string receivers;
					receivers.append( pBuff + pointer, receiveLen);

					m_recievers.push_back( receivers);
					pointer += receiveLen;
				}
			}
		}

		int subjectLen = 0;
		memcpy(&subjectLen, pBuff + pointer, 4);
		pointer += 4;

		if( subjectLen > 0)
		{
			m_subject.append(pBuff + pointer, subjectLen);
			pointer += subjectLen;
		}

		memcpy( &m_content_type, pBuff + pointer,  sizeof( m_content_type));
		pointer += sizeof( m_content_type);

		int contentLen = 0;
		memcpy(&contentLen, pBuff + pointer, 4);
		pointer += 4;

		if( contentLen > 0)
		{
			m_content.append(pBuff + pointer, contentLen);
			pointer += contentLen;
		}
		
		return pointer;
	}

	int getRealSize()
	{ 
		int realSize = 0;

		realSize += 4;
		realSize += m_msgid.length();
		realSize += 4;
		realSize += m_parentMsgid.length();
		realSize += 4;
		realSize += m_rootMsgid.length();

		realSize += sizeof( m_type);
		realSize += sizeof( m_seq_number);
		realSize += sizeof( m_timestamp);

		realSize += 4;
		realSize += m_sender.length();

		realSize += 4;
		std::vector<std::string>::iterator it = m_recievers.begin();
		for( ; it != m_recievers.end(); it++)
		{
			realSize += 4;
			realSize += it->length();
		}

		realSize += 4;
		realSize += m_subject.length();

		realSize += sizeof( m_content_type);

		realSize += 4;
		realSize += m_content.length();

		return realSize;
	}
};

class LoginInfo
{
public:
	T_IP			m_sessionDomainIP;
	unsigned short	m_sessionDomainPort;
	uint32			m_sid;

	LoginInfo()
	{
		memset(m_sessionDomainIP, 0, 16);
		m_sessionDomainPort = 0;
		m_sid = 0;
	}

	bool StringToValue(const  std::string &loginInString)
	{
		if( loginInString.length() != 22)
			return false;

		memcpy( m_sessionDomainIP, loginInString.c_str(), 16);
		memcpy(&m_sessionDomainPort, loginInString.c_str() + 16, 2);
		memcpy(&m_sid, loginInString.c_str() + 18, 4);

		return true;
	}

	std::string ToString()
	{
		std::string temp;
		temp.append(m_sessionDomainIP,16);

		temp.append( (char *)&m_sessionDomainPort,2);

		temp.append( ( char *)&m_sid, 4);

		return temp;
	}
};

//为了将msg做智能指针而扩展的一个结构
class SSSessionDomainMsgEx: public GMRefCounterBase<SSSessionDomainMsgEx>
{
public:
	SSSessionDomainMsg m_msg;
};

typedef GMEmbedSmartPtr<SSSessionDomainMsgEx> SptrMsg;