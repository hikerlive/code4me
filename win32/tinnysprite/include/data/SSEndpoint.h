#ifndef __SSENDPOINT_H__
#define __SSENDPOINT_H__

#include <string>

using namespace std;
namespace SSDataDomain
{
///////////////////////////////////////////////////////////////////////////////
class SSEndpoint
{
private:
	string host;
	int port;

public:
	SSEndpoint();
	SSEndpoint(const string host, int port);
	SSEndpoint(const SSEndpoint& o);
	SSEndpoint& operator =(const SSEndpoint& o);
	bool operator==(const SSEndpoint& o);

public:
	const string& GetHost() const;
	void SetHost(const string& host);
	int GetPort() const;
	void SetPort(const int port);
};
inline SSEndpoint::SSEndpoint()
{
	this->host = "127.0.0.1";
	this->port = 8080;
};
inline SSEndpoint::SSEndpoint(const string host, int port)
{
	this->host = host;
	this->port = port;
};
inline SSEndpoint::SSEndpoint(const SSEndpoint& o)
{
	this->host = o.host;
	this->port = o.port;
};
inline SSEndpoint& SSEndpoint::operator =(const SSEndpoint& o)
{
	this->host = o.host;
	this->port = o.port;
	return *this;
}
inline bool SSEndpoint::operator==(const SSEndpoint& o)
{
	return (this->host == o.host) && (this->port == o.port);
}
inline const string& SSEndpoint::GetHost() const
{
	return this->host;
}
inline void SSEndpoint::SetHost(const string& host)
{
	this->host = host;
}
inline int SSEndpoint::GetPort() const
{
	return this->port;
}
inline void SSEndpoint::SetPort(const int port)
{
	this->port = port;
}

} // namespace

#endif