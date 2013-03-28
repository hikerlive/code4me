#ifndef __INTERFACE_H
#define __INTERFACE_H

class SimpleDll {
public:
	virtual HRESULT SimpleMethod() = 0;
	virtual HRESULT Swap(int& a, int& b) =0;
};

extern "C" void CreateInstance(SimpleDll* &pInstance);
extern "C" void DestoryInstance(SimpleDll* &pInstance);

#endif // SIMPLEDLL_INTERFACE_H