#ifndef __BITFLAG_H
#define __BITFLAG_H

#define BIT_LSHIFT(v, n)	((v) << (n))
#define BIT_RSHIFT(v, n)	((v) >> (n))
#define BIT_FLAG(n)		BIT_LSHIFT(1, n)
#define BIT_CHECK_FLAG(m, n)	((m & BIT_FLAG(n))?1:0)
#define BIT_SET_FLAG(m, n)	(m |= BIT_FLAG(n))
#define BIT_CLEAR_FLAG(m, n)	(m &= ~BIT_FLAG(n))

#endif //__BITFLAG_H
