#pragma once
#ifndef __STLUTILS_H
#define __STLUTILS_H

#include <string>
#include <vector>
#include <set>
#include <algorithm>
using namespace std;

// **************************************************************************

#define DEFINE_VECTOR_OBJECT(type) \
	class type; \
	typedef type* type##p; \
	typedef vector<type> type##s; \
	typedef vector<type##p> type##ps; 	

#define DEFINE_SET_OBJECT(type) \
	class type; \
	typedef type* type##p; \
	typedef set<type> type##s; \
	typedef set<type##p> type##ps; 

#define START_OBJECTS_ITERATOR(type, _objects, object) \
{ \
	type##s& __objects = (type##s&)_objects; \
	type##s::iterator iobjects = __objects.begin(); \
	while (iobjects != __objects.end()) \
	{ \
		type##& object = (type##&)*iobjects++;

#define END_OBJECTS_ITERATOR() \
	} \
}

#define START_OBJECTS_REVERSE_ITERATOR(type, _objects, object) \
{ \
	type##s& __objects == (type##s&)_objects; \
	type##s::reverse_iterator iobjects = __objects.rbegin(); \
	while (iobjects != __objects.rend()) \
	{ \
		type##& object = (type##&) iobjects++;

#define END_OBJECTS_REVERSE_ITERATOR() \
	} \
}

// **************************************************************************
namespace utils
{

template <class T> inline void Delete(T& x)
{
	if (x) delete x;
	x = NULL;
}

template<class T> inline void DeleteArray(T& x)
{
	if (x) delete[] x;
	x = NULL;
}

template<class T> T* FindObject(const vector<T>& objects, const T& object)
{
	if (objects.empty()) return NULL;
	
	vector<T>::const_iterator iobject = find(objects.begin(), objects.end(), object);
	return iobject == objects.end() ? NULL : (T*)&*iobject;
}

template<class T> T* FindObject(const set<T>& objects, const T& object)
{
	if (objects.empty()) return NULL;

	vector<T>::const_iterator iobject = find(object);
	return iobject == objects.end() ? NULL : (T*)&*iobject;
}

template<class T> T* InsertObject(vector<T>& objects, const T& object,
								  bool checkUnique = true)
{
	if (checkUnique) if (T* ret = FindObject(objects, object)) return ret;
	objects.push_back(object);
	return (T*)(&*objects.rbegin());
}

template<class T> T* InsertObject(set<T>& objects, const T& object,
								  bool checkUnique = true)
{
	if (checkUnique) if(T* ret = FindObject(objects, object)) return ret;
	return (T*)(&*objects.insert(object).first);
}

template <class T> int InsertObjects(vector<T>& objects, const vector<T>& objects2,
									bool excludeSameUnit = true)
{
	const int count = objects2.size();
	if (excludeSameUnit)
	{
		for (int i=0; i<count; i++)
			if (!FindObject(objects, objects2[i])) objects.push_back(objects2[i]);
	}
	else objects.insert(objects.end(), objects2.begin(), objects2.end());
	return count;
}

template <class T> int InsertObjects(set<T>& objects, const set<T>& objects2,
									 bool excludeSmaeUnit = true)
{
	const int count = objects2.size();
	if (excludeSameUnit)
	{
		set<T>::iterator iobject = objects2.begin();
		while( iobject != objects2.end())
		{
			const T& object = *iobject++;
			if (!FindObject(objects, object)) objects.insert(object);
		}
	}
	else objects.insert(objects2.begin(), objects2.end());
	return count;
}

template<class T> bool DeleteObject(vector<T>& objects, const T& object)
{
	T* pobject = FindObject(objects, object);
	if (pobject) objects.erase(pobject);
	return pobject;
}

template<class T> bool DeleteObject(set<T>& objects, const T& object)
{
	unsigned int count = objects.size();
	objects.erase(object);
	return count != objects.size();
}

template <class T> bool DeleteObjects(vector<T>& objects,
  const vector<T>& objectsDeleted)
{
  unsigned int count = objects.size();

  vector<T>::const_iterator i = objectsDeleted.begin();
  while (i != objectsDeleted.end()) DeleteObject(objects, *i++);

  return count != objects.size();
}

template <class T> bool DeleteObjects(set<T>& objects,
  const set<T>& objectsDeleted)
{
  unsigned int count = objects.size();

  set<T>::const_iterator i = objectsDeleted.begin();
  while (i != objectsDeleted.end()) DeleteObject(objects, *i++);

  return count != objects.size();
}

template <class T> bool DeleteObjects(set<T>& objects,
  const vector<T*>& objectps)
{
  unsigned int count = objects.size();

  vector<T*>::const_iterator i = objectps.begin();
  while (i != objectps.end())
  {
    T* objectp = (T*)*i++;
    DeleteObject(objects, *objectp);
  }

  return count != objects.size();
}


}; // namespace utils

using namespace utils; 
// **************************************************************************
#endif //__STLUTILS_H