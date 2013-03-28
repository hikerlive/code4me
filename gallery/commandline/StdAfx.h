// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__68A3FAF6_82B1_4CC0_A22D_7142684A8B9D__INCLUDED_)
#define AFX_STDAFX_H__68A3FAF6_82B1_4CC0_A22D_7142684A8B9D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000



#pragma warning(disable: 4786)	// stl warning
#define _WIN32_WINNT 0x0400
#include <stdio.h>
#include <io.h>
#include <windows.h>
#include <wincrypt.h>
#include <conio.h>
#define MY_ENCODING_TYPE  (PKCS_7_ASN_ENCODING | X509_ASN_ENCODING)
#define MAX_NAME 256
#define RC4_KEYLEN 128


#include <iostream>
using namespace std;


// TODO: reference additional headers your program requires here

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__68A3FAF6_82B1_4CC0_A22D_7142684A8B9D__INCLUDED_)
