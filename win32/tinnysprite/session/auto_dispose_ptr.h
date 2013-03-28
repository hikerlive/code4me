/*--------------------------------------------------------------------------------*\

     文件：  auto_dispose_ptr.h
     作者：  姚松文
     日期：  2010年10月20日
     说明：  无

\*--------------------------------------------------------------------------------*/

#ifndef AUTO_DISPOSE_PTR_H
#define AUTO_DISPOSE_PTR_H

#include <memory>

template<class _Ty>
struct default_dispose
{
    default_dispose()
    {	// default construct
    }

    void operator()(_Ty *_Ptr) const
    {	// delete a pointer
        if (0 < sizeof (_Ty))	// won't compile for incomplete type
            _Ptr->Dispose();
    }
};
template<class _Ty>
class auto_dispose_ptr : public std::unique_ptr<_Ty, default_dispose<_Ty>>
{
public:
    explicit auto_dispose_ptr()
    {
    }

    explicit auto_dispose_ptr(const _Ty* _Ptr)
        : unique_ptr((_Ty*)_Ptr)
    {
    }

    _Ty** operator&() const _THROW0()
    {
        return (_Ty**)&_Myptr;
    }

    operator _Ty*() const _THROW0()
    {
        return (get());
    }
};

#endif //AUTO_DISPOSE_PTR_H