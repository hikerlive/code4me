#include "SSCommon.h"

#ifdef _WIN32
	#pragma comment(lib, "ws2_32.lib")
#endif

#ifdef _DEBUG
#pragma comment(lib, "libprotobufD")
#else
#pragma comment(lib, "libprotobuf")
#endif

#include <google/protobuf/descriptor.h>
#include <process.h>

#include "SSRpcChannel.h"
#include "nettyrpc.pb.h"
#include "ssdatadomain.pb.h"

using namespace SSDataDomain;
using namespace google::protobuf;

#ifdef _WIN32
namespace SSDataDomain
{
class WSAModule
{
public:
	WSAModule()
	{
		WSADATA wsaData = {0};
		if (WSAStartup(MAKEWORD(1, 1), &wsaData) == 0)
		{
			// check winsock version
			if (LOBYTE(wsaData.wVersion) != 1 || HIBYTE(wsaData.wVersion) != 1)
			{
				WSACleanup( );
			}
		}
	}
	~WSAModule()
	{
		WSACleanup();
	}
} __wsa__;
} // namespace
#endif

long SSRpcChannel::id_generator = 0;

SSRpcChannel::SSRpcChannel()
{
	SSTRACEMETHOD(("SSRpcChannel::SSRpcChannel()"));

	this->id = id_generator++;
	this->socket = INVALID_SOCKET;
	this->connected = false;
	this->sequence = 0;
	this->destroyed = false;
	this->busy = 0;
	
	recieve_thread = _beginthread((void (_cdecl*)(void*))static_recieve_thread_proc, 0, this);
}

SSRpcChannel::SSRpcChannel(SSEndpoint& endpoint)
{
	SSTRACEMETHOD(("SSRpcChannel::SSRpcChannel(SSEndpoint& endpoint)"));
	
	this->id = id_generator++;
	this->socket = INVALID_SOCKET;
	this->endpoint = endpoint;
	this->connected = false;
	this->sequence = 0;
	this->destroyed = false;
	this->busy = 0;
	
	recieve_thread = _beginthread((void (_cdecl*)(void*))static_recieve_thread_proc, 0, this);
}

SSRpcChannel::~SSRpcChannel()
{
	SSTRACEMETHOD(("SSRpcChannel::~SSRpcChannel()"));
	
	this->destroyed = true;
	::InterlockedIncrement(&busy);
	End();
	::WaitForSingleObject((HANDLE)recieve_thread, INFINITE);

#ifdef _DEBUG
	// 这里的都是未收到回复的包
	//for (hash_map<long, ResponseCallbackData>::const_iterator it = this->callback_map.cbegin();
	//	it != this->callback_map.cend();
	//	it++) {
	//		SSTRACE("channel [" << this->id << "] discard callback data: " << it->first);
	//}
	for (hash_map<long, ResponseCallbackData>::iterator it = this->callback_map.begin();
		it != this->callback_map.end();
		it++) {
			SSTRACE("channel [" << this->id << "] discard callback data: " << it->first);
	}
#endif
}

void SSRpcChannel::static_recieve_thread_proc(SSRpcChannel* channel)
{
	try {
		channel->recieve_thread_proc();
	} catch(...) {
		SSTRACE("channel [" << channel->id << "] something wrong happened in recieved thread.");
	}
	_endthread();
}

void SSRpcChannel::recieve_thread_proc()
{
	while(!this->destroyed)
	{
		// waiting for socket initialization.
		if (this->socket == INVALID_SOCKET)
		{
			::Sleep(100);
			continue;
		}

		// reconnect after 10s if disconnected.
		if (!this->connected)
		{
			this->Start();
			if (!this->connected)
			{
				SSTRACE("Failed to connect, retry 10s later...");
				::Sleep(10000);
				continue;
			}
		}

		int ret = 0;
		int size = 0;
		
		// get frame package size.
		ret = this->recv((char*)&size, sizeof(int), 0);
		if (ret == SOCKET_ERROR || ret == 0)
		{
			this->socket_lock.lock();
			this->connected = false;
			this->socket_lock.unlock();
			continue;
		}
		size = ntohl(size);

		// get frame package data.
		auto_ptr<char> buf(new char[size]);
		ret = this->recv(buf.get(), size, 0);
		if (ret == SOCKET_ERROR || ret == 0)
		{
			this->socket_lock.lock();
			this->connected = false;
			this->socket_lock.unlock();
			continue;
		}

		// decode package data.
		this->HandleRecievedData(buf.get(), size);
	}
}

void SSRpcChannel::HandleRecievedData(char* buf, int len)
{
	auto_busy ab(&busy);

	RpcResponse rpc_response;
	rpc_response.ParseFromArray(buf, len);
	int seq_id = rpc_response.id();
	SSTRACE("channel [" << this->id << "] recieved seq_id: " << seq_id);

	this->map_lock.lock();
	hash_map<long, ResponseCallbackData>::iterator it = this->callback_map.find(seq_id);
	if (it == this->callback_map.end())
	{
		SSTRACE("channel [" << this->id << "] the seq_id " << seq_id << " has been canceled");
		this->map_lock.unlock();
		return;
	}

	ResponseCallbackData rcd = it->second;
	delete it->second.cancel_callback;
	this->callback_map.erase(it);
	this->map_lock.unlock();

	if (rpc_response.has_error_code() && rcd.controller != NULL)
	{
		if (rpc_response.has_error_message()) {
			rcd.controller->SetFailed(rpc_response.error_message());
		} else {
			char error_code_buf[32] = {0};
			string error_msg;
			error_msg.append("rpc error code : ");
			#pragma warning(disable:4996)
			error_msg.append(ltoa(rpc_response.error_code(), error_code_buf, 10));
			rcd.controller->SetFailed(error_msg);
		}

		Done(rcd.done);
		return;
	}
	
	string response_message = rpc_response.response_message();
	rcd.response->ParseFromArray(response_message.c_str(), response_message.size());

	this->HandleRecievedMessage(rcd.response);

	Done(rcd.done);
}

void SSRpcChannel::Done(Closure* done)
{
	if (done != NULL) {
		try {
			done->Run();
		} catch(...){
			SSTRACE("channel [" << this->id << "] something wrong when done.");
		}
	}
}

void SSRpcChannel::HandleRecievedMessage(Message* response)
{
	// There is a chance to modify the response message before return to user, but nothing to do now.
}

bool SSRpcChannel::Start()
{
	SYNCHRONIZED_SCOPE_WITH(&this->socket_lock);
	SSTRACEMETHOD(("SSRpcChannel::Start()"));
	
	if (this->connected == true)
		return true;

	this->socket = ::socket(AF_INET, SOCK_STREAM, 0);
	if (this->socket == INVALID_SOCKET)
	{
		this->destroyed = true;
		return false;
	}

	sockaddr_in addr;
	addr.sin_addr.S_un.S_addr=inet_addr(this->endpoint.GetHost().c_str());
	addr.sin_family=AF_INET;
	addr.sin_port=htons(this->endpoint.GetPort());
	if (::connect(this->socket, (sockaddr*)&addr, sizeof(sockaddr)) != 0)
	{
		return false;
	}
	
	this->connected = true;
	this->sequence = 0;
	return true;
}

void SSRpcChannel::End()
{
	SYNCHRONIZED_SCOPE_WITH(&this->socket_lock);
	SSTRACEMETHOD(("SSRpcChannel::End()"));
	
	if (this->connected)
	{
		::shutdown(socket, SD_BOTH);
		::closesocket(this->socket);
		this->connected = false;
	}
}

void SSRpcChannel::OnRpcCancel(long seq_id)
{
	SYNCHRONIZED_SCOPE_WITH(&this->map_lock);
	hash_map<long, ResponseCallbackData>::iterator it = this->callback_map.find(seq_id);
	if (it == this->callback_map.end())
	{
		SSTRACE("channel [" << this->id << "] can't find seq_id: " << seq_id);
		return;
	}
	delete it->second.done;
	//delete it->second.cancel_callback;
	this->callback_map.erase(it);
}

void SSRpcChannel::CallMethod(const MethodDescriptor * method, RpcController * controller, const Message * request, Message * response, Closure * done)
{
	if (!this->connected)
	{
		controller->SetFailed("Socket is not connected.");
		Done(done);
		return;
	}

	auto_busy ab(&busy);

	long seq_id = ::InterlockedIncrement(&this->sequence);
	SSTRACE("channel [" << this->id << "] sent seq_id: " << seq_id);

	// rpc_request
	RpcRequest rpc_request;
	rpc_request.set_id(seq_id);
	rpc_request.set_is_blocking_service(false);
	rpc_request.set_service_name(method->service()->full_name());
	rpc_request.set_method_name(method->name());

	int len = request->ByteSize();

	auto_ptr<char> buf(new char[len]);
	request->SerializeToArray(buf.get(), len);
	rpc_request.set_request_message(buf.get(), len);

	// rpc_frame
	int rpc_request_size = rpc_request.ByteSize();
	int rpc_frame_size = sizeof(int) + rpc_request_size;
	auto_ptr<RPCFRAME> p_rpc_frame((LPRPCFRAME)new char[rpc_frame_size]);
	p_rpc_frame->size = htonl(rpc_request_size);
	rpc_request.SerializePartialToArray(p_rpc_frame->buffer, rpc_request_size);
	
	// put to map
	ResponseCallbackData rcd;
	rcd.id = seq_id;
	rcd.controller = controller;
	rcd.response = response;
	rcd.done = done;
	this->map_lock.lock();
	if (controller->IsCanceled()) {
		return;
	}
	rcd.cancel_callback = NewCallback<SSRpcChannel, long>(this, &SSRpcChannel::OnRpcCancel, seq_id);
	controller->NotifyOnCancel(rcd.cancel_callback);
	this->callback_map[seq_id] = rcd;
	if (controller->IsCanceled()) {
		return;
	}
	this->map_lock.unlock();

	// send data
	int sr = this->send((const char*)p_rpc_frame.get(), rpc_frame_size, 0);
	if (sr == SOCKET_ERROR || sr == 0)
	{
		this->socket_lock.lock();
		this->connected = false;
		this->socket_lock.unlock();

		SSTRACE("channel [" << this->id << "] failed to sent seq_id: " << seq_id);
		string failed_msg("Failed to send data.");
		controller->SetFailed(failed_msg);
		Done(done);
		return;
	}
}

int SSRpcChannel::send(const char FAR * buf, int len, int flags)
{
	SSTRACEMETHOD(("int SSRpcChannel::send(const char FAR * buf, int len, int flags)"));

	SYNCHRONIZED_SCOPE_WITH(&this->send_lock);
	int sent_size = 0;
	while (sent_size < len)
	{
		int sr = ::send(this->socket, buf + sent_size, len - sent_size, flags);
		if (sr == SOCKET_ERROR || sr == 0)
		{
			return sr;
		}
		sent_size += sr;
	}
	return sent_size;
}

int SSRpcChannel::recv(char FAR * buf, int len, int flags)
{
	SSTRACEMETHOD(("int SSRpcChannel::recv(char FAR * buf, int len, int flags)"));
	
	SYNCHRONIZED_SCOPE_WITH(&this->recv_lock);
	int recieved_size = 0;
	while (recieved_size < len)
	{
		int sr = ::recv(this->socket, buf + recieved_size, len - recieved_size, flags);
		if (sr == SOCKET_ERROR || sr == 0)
		{
			return sr;
		}
		recieved_size += sr;
	}
	return recieved_size;
}