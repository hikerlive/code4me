#ifndef __XSTRING_H
#define __XSTRING_H

#include <stdio.h>

namespace gutils
{

class XString
{
public:
    XString(const char* pdata = NULL);
    ~XString();
    XString(const XString& rhs);
    XString& operator=(const XString& rhs);
    
    char* GetData() const;
        
private:
    char* m_pdata;    
};  
  
};

#endif
