#ifndef __SSCONCURRENCY_H__
#define __SSCONCURRENCY_H__

#include <Windows.h>

namespace SSDataDomain
{

class SSLock
{
private:
	CRITICAL_SECTION cs;
	SSLock(const SSLock& o);

public:
	SSLock();
	~SSLock();
	void lock();
	void unlock();
	CRITICAL_SECTION* get_critical_section();
};

inline SSLock::SSLock()
{
	::InitializeCriticalSection(&cs);
}

inline SSLock::~SSLock()
{
	::DeleteCriticalSection(&cs);
}

inline void SSLock::lock()
{
	::EnterCriticalSection(&cs);
}

inline void SSLock::unlock()
{
	::LeaveCriticalSection(&cs);
}

inline CRITICAL_SECTION* SSLock::get_critical_section()
{
	return &cs;
}

class SSScopedLock
{
private:
	SSLock* lock;
	SSScopedLock(const SSScopedLock& o);

public:
	SSScopedLock(SSLock* lock);
	~SSScopedLock();
};

inline SSScopedLock::SSScopedLock(SSLock* lock)
{
	this->lock = lock;
	this->lock->lock();
}
inline SSScopedLock::~SSScopedLock()
{
	this->lock->unlock();
}

//#define SYNCHRONIZED_SCOPE static SSLock l;SSScopedLock sl(l);
#define SYNCHRONIZED_SCOPE_WITH(l) SSScopedLock sl((l));

} // namespace

#endif