#ifndef __SSOBJECTPOOL_H__
#define __SSOBJECTPOOL_H__

#include <list>

namespace SSDataDomain
{

template <typename T>
class SSObjectPool
{
	typedef void (*DESTROYFUNC)(T);
	typedef T (*CREATEFUNC)(void);

private:
	static T default_create_func(void);
	static void default_destroy_func(T o);

private:
	SSLock lock;
	CONDITION_VARIABLE unused_not_empty;
	//list<T> used_list;
	list<T> unused_list;

	unsigned long used_count;

	unsigned long min_size;
	unsigned long max_size;

	CREATEFUNC create_func;
	DESTROYFUNC destroy_func;
public:
	T GetObject();
	void SetObject(T t);

public:
	SSObjectPool();
	virtual ~SSObjectPool();
	void SetMinSize(unsigned long min);
	void SetMaxSize(unsigned long max);
	unsigned long GetMinSize();
	unsigned long GetMaxSize();

	void SetDestroyFunc(DESTROYFUNC destroy_func);
	void SetCreateFunc(CREATEFUNC create_func);

#ifdef _DEBUG
	void print();
#endif
};

template<typename T>
SSObjectPool<T>::SSObjectPool()
{
	this->used_count = 0;
	this->max_size = 5;
	this->min_size = 5;
	this->create_func = &SSObjectPool<T>::default_create_func;
	this->destroy_func = &SSObjectPool<T>::default_destroy_func;
}

template<typename T>
SSObjectPool<T>::~SSObjectPool()
{
	this->lock.lock();
	//list<T>::const_iterator it = this->unused_list.cbegin();
	//while (it != this->unused_list.cend()) {
	//	T o = *it;
	//	this->destroy_func(o);
	//	it++;
	//}
	list<T>::iterator it = this->unused_list.begin();
	while (it != this->unused_list.end()) {
		T o = *it;
		this->destroy_func(o);
		it++;
	}
}

template<typename T>
void SSObjectPool<T>::SetMaxSize(unsigned long max)
{
	this->max_size = max;
}

template<typename T>
unsigned long SSObjectPool<T>::GetMaxSize()
{
	return this->max_size;
}

template<typename T>
void SSObjectPool<T>::SetMinSize(unsigned long min)
{
	this->min_size = min;
}

template<typename T>
unsigned long SSObjectPool<T>::GetMinSize()
{
	return this->min_size;
}

template<typename T>
void SSObjectPool<T>::SetCreateFunc(CREATEFUNC create_func)
{
	this->create_func = create_func;
}

template<typename T>
void SSObjectPool<T>::SetDestroyFunc(DESTROYFUNC destroy_func)
{
	this->destroy_func = destroy_func;
}

template<typename T>
T SSObjectPool<T>::default_create_func()
{
	return *new T();
}

template<typename T>
void SSObjectPool<T>::default_destroy_func(T o)
{
	delete &o;
}

template<typename T>
T SSObjectPool<T>::GetObject()
{
	this->lock.lock();

	//SSTRACE("<- total: " << (this->unused_list.size() + this->used_list.size()) << ", unused: " << this->unused_list.size() << ", used: " << this->used_list.size());
	SSTRACE("<- total: " << (this->unused_list.size() + this->used_count) << ", unused: " << this->unused_list.size() << ", used: " << this->used_count);

	if (!this->unused_list.empty()) {
		T o = this->unused_list.front();
		this->unused_list.pop_front();
		//this->used_list.push_back(o);
		this->used_count++;
		this->lock.unlock();
		return o;
	}

	//if (this->used_list.size() + this->unused_list.size() >= this->max_size) {
	if (this->used_count + this->unused_list.size() >= this->max_size) {
		SleepConditionVariableCS(&this->unused_not_empty, this->lock.get_critical_section(), INFINITE);
		return GetObject();
	}

	T o = this->create_func();
	//this->used_list.push_back(o);
	this->used_count++;
	this->lock.unlock();

	return o;
}

template<typename T>
void SSObjectPool<T>::SetObject(T o)
{
	this->lock.lock();
	
	//SSTRACE("-> total: " << (this->unused_list.size() + this->used_list.size()) << ", unused: " << this->unused_list.size() << ", used: " << this->used_list.size());
	SSTRACE("-> total: " << (this->unused_list.size() + this->used_count) << ", unused: " << this->unused_list.size() << ", used: " << this->used_count);

	//if (this->used_list.size() + this->unused_list.size() >= this->max_size) {
	if (this->used_count + this->unused_list.size() >= this->max_size) {
		//this->used_list.remove(o);
		this->used_count--;
		this->destroy_func(o);
		this->lock.unlock();
		return;
	}
	
	if (this->used_count + this->unused_list.size() > this->min_size) {
		//if (this->used_list.size() < this->unused_list.size()) {
		if (this->used_count <= this->unused_list.size()) {
			//this->used_list.remove(o);
			this->used_count--;
			this->destroy_func(o);
			this->destroy_func(this->unused_list.front());
			this->unused_list.pop_front();
			this->lock.unlock();
			return;
		}
	}

	//this->used_list.remove(o);
	this->used_count--;
	this->unused_list.push_back(o);
	this->lock.unlock();
}

#ifdef _DEBUG
template<typename T>
void SSObjectPool<T>::print()
{
	this->lock.lock();
	//SSTRACE("total: " << (this->unused_list.size() + this->used_list.size()) << ", unused: " << this->unused_list.size() << ", used: " << this->used_list.size());
	SSTRACE("total: " << (this->unused_list.size() + this->used_count) << ", unused: " << this->unused_list.size() << ", used: " << this->used_count);
	this->lock.unlock();
}
#endif

}

#endif