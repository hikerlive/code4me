#include "xstring.h"
#include <string.h>

using namespace gutils;

XString::XString(const char* pdata)
{
    printf("pdata %s\n", pdata);
    if (pdata == NULL)
    {
        m_pdata = new char[1];
        m_pdata[0] = '\0';
    }
    else
    {
        int len = strlen(pdata);
        m_pdata = new char[len+1];
        strncpy(m_pdata, pdata, len);
        m_pdata[len] = '\0';
    }
}

XString::XString(const XString& rhs)
{
    if (m_pdata) delete[] m_pdata;
    m_pdata = NULL;

    int len = strlen(rhs.m_pdata);
    m_pdata = new char[len+1];
    strncpy(m_pdata, rhs.m_pdata, len);
    m_pdata[len] = '\0';        
}

XString& XString::operator=(const XString& rhs)
{
    if (this == &rhs)
        return *this;
    
    if (m_pdata) delete[] m_pdata;
    m_pdata = NULL;

    int len = strlen(rhs.m_pdata);
    m_pdata = new char[len+1];
    strncpy(m_pdata, rhs.m_pdata, len);
    m_pdata[len] = '\0'; 
            
    return *this;
}

XString::~XString()
{
    if (m_pdata) delete[] m_pdata;
    m_pdata = NULL;    
}

char* XString::GetData() const
{
    return m_pdata;    
}
