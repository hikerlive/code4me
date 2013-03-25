#ifndef __STDUTILS_H
#define __STDUTILS_H

template<class T> inline void Delete(T& x)
{
    if (x) delete x;
    x = NULL;
}

template<class T> inline void DeleteArray(T& x)
{
    if (x) delete[] x;
    x = NULL;   
}

#endif 
