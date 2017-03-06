/**
 * file :	bs_macros.h
 * author :	bushaofeng
 * create :	2016-08-23 18:55
 * func : 
 * history:
 */

#ifndef	__BS_MACROS_H_
#define	__BS_MACROS_H_


#ifndef	__CODE_LINK__
	#include	<stdio.h>
	#include	<stdarg.h>
	#include	<string.h>
#endif



/**//*!	@defgroup _data_types
	*	Elementary data types.
	******************************************************/
/*!	@{
 */
	#ifndef NULL
	#ifdef	__cplusplus
	#define	NULL	0
	#else
	#define	NULL	((void *)0)
	#endif
									#endif
	#ifndef	UNSG8
	#define	UNSG8	UNSG8_
						#endif
	#ifndef	SIGN8
	#define	SIGN8	SIGN8_
						#endif
	#ifndef	UNSG16
	#define	UNSG16	UNSG16_
						#endif
	#ifndef	SIGN16
	#define	SIGN16	SIGN16_
						#endif
	#ifndef	UNSG32
	#define	UNSG32	UNSG32_
						#endif
	#ifndef	SIGN32
	#define	SIGN32	SIGN32_
						#endif
	#ifndef	UNSG64
	#define	UNSG64	UNSG64_
						#endif
	#ifndef	SIGN64
	#define	SIGN64	SIGN64_
						#endif
	#ifndef	REAL32
	#define	REAL32	REAL32_
						#endif
	#ifndef	REAL64
	#define	REAL64	REAL64_
						#endif

	//*	Microsoft Visual C++
	#if		defined(_MSC_VER)
    typedef	unsigned char		UNSG8_;
    typedef	signed char			SIGN8_;
    typedef	unsigned short		UNSG16_;
    typedef	signed short		SIGN16_;
    typedef	unsigned int		UNSG32_;
    typedef	signed int			SIGN32_;
    typedef	unsigned __int64	UNSG64_;
    typedef	signed __int64		SIGN64_;
    typedef	float				REAL32_;
    typedef	double				REAL64_;

	//*	GNU gcc/g++
	#elif	defined(__GNUC__)
    typedef	unsigned char		UNSG8_;
    typedef	signed char			SIGN8_;
    typedef	unsigned short		UNSG16_;
    typedef	signed short		SIGN16_;
    typedef	unsigned int		UNSG32_;
    typedef	signed int			SIGN32_;
    typedef	unsigned long long	UNSG64_;
    typedef	signed long long	SIGN64_;
    typedef	float				REAL32_;
    typedef	double				REAL64_;

	//*	ARMCC (RVCT)
	#elif	defined(__ARMCC_VERSION)
    typedef	unsigned char		UNSG8_;
    typedef	signed char			SIGN8_;
    typedef	unsigned short		UNSG16_;
    typedef	signed short		SIGN16_;
    typedef	unsigned int		UNSG32_;
    typedef	signed int			SIGN32_;
    typedef	unsigned long long	UNSG64_;
    typedef	signed long long	SIGN64_;
    typedef	float				REAL32_;
    typedef	double				REAL64_;
	#endif

/*!	@}
 */



/**//*!	@defgroup _static_types
	*	Unify types with 'static' prefix.
	******************************************************/
/*!	@{
 */
	//*	Microsoft Visual C++
	#if		defined(_MSC_VER)
	#ifndef	INLINE
	#define	INLINE	static __forceinline
									#endif
	//*	GNU gcc/g++
	#elif	defined(__GNUC__)
	#ifndef	INLINE
	#define	INLINE	static inline
									#endif
	//*	ARMCC (RVCT)
	#elif	defined(__ARMCC_VERSION)
	#ifndef	INLINE
	#define	INLINE	static __inline
									#endif
	#endif

	#ifdef	CONST
	#undef	CONST
	#endif
	#define	CONST	static const

/*!	@}
 */


/**//*!	@defgroup _critical_section
	*	Wrappers for critical section functions.
	******************************************************/
/*!	@{
 */
	#pragma	pack(8)
	typedef struct {
	union {
	volatile SIGN64	r;									///	for read
	volatile char	w[8];								///	for write
						};
	volatile SIGN32	sn;									///	running number
	volatile SIGN32	i;									///	current thread index
	} CriticalSection;
	#pragma	pack()

	#ifndef	OSSLEEP
	#define	OSSLEEP(ms)	do{}while(0)
	#endif

	INLINE SIGN32 SPINLOCK(CriticalSection *cs, SIGN32 id, SIGN32 timeout) {
			SIGN32	i = cs->sn ++;
			SIGN64	locked = 1;
			if(i >= 0) i = id;
			i &= 7;
			#ifdef	__big_endian__
			i = 7-i;
			#endif
			locked <<= (i<<3);
			while(timeout -- != 0) {
				cs->w[i] ++;
				if(cs->r == locked) {
					cs->i = i; return 1;
				}
				OSSLEEP(1); cs->w[i] --;
			}
			return 0;
									}
	INLINE void SPINUNLOCK(CriticalSection *cs, SIGN32 id) {
			cs->w[cs->i] = 0;
									}
	#ifndef	_MSC_VER
	#define	CRITICAL_SECTION	CriticalSection
	#define	CriticalSectionQuery(lock,thread)			SPINLOCK(lock, thread, 1)
	#define	CriticalSectionEnter(lock,thread)			SPINLOCK(lock, thread, -1)
	#define	CriticalSectionLeave(lock,thread)			SPINUNLOCK(lock, thread)
	#define	CriticalSectionReset(lock)					do{	(lock)->r = 0; }while(0)
	#define	QueryCriticalSection(lock)					CriticalSectionQuery(lock, -1)
	#define	EnterCriticalSection(lock)					CriticalSectionEnter(lock, -1)
	#define	LeaveCriticalSection(lock)					CriticalSectionLeave(lock, -1)
	#else
	#define	CriticalSectionQuery(lock,thread)			(1)
	#define	CriticalSectionEnter(lock,thread)			EnterCriticalSection(lock)
	#define	CriticalSectionLeave(lock,thread)			LeaveCriticalSection(lock)
	#define	CriticalSectionReset(lock)					InitializeCriticalSection(lock)
	#define	QueryCriticalSection(lock)					CriticalSectionQuery(lock, -1)
	#endif

	#define	GETLOCK(LK,id)			(&LK[id])
	#define	ENTER(LK,id,thread)		do{	int i = (id);																\
										CRITICAL_SECTION *lock = GETLOCK(LK,i);										\
										if(!bTST(LK##INIT,i))														\
											CriticalSectionReset(lock);												\
									CriticalSectionEnter(lock,thread); }while(0)
	#define	LEAVE(LK,id,thread)		CriticalSectionLeave(GETLOCK(LK,id),thread)
	#define	QUERY(LK,id,thread)		CriticalSectionQuery(GETLOCK(LK,id),thread)
	#define	RESET(LK,id)			CriticalSectionReset(GETLOCK(LK,id))
	#define	RETURN(LK,id,thread)	do{ LEAVE(LK,id,thread); return; }while(0)
	#define	RETURNV(LK,id,thread,v)	do{ LEAVE(LK,id,thread); return (v); }while(0)

/*!	@}
 */



/**//*!	@defgroup _misc_arith
	*	Miscellaneous arithmetic operations.
	******************************************************/
/*!	@{
 */
	#ifndef	MIN
	/*!	@def	MIN
	*	@param[in]		a		any type data
	*	@param[in]		b		any type data
	*	@return			any type					:	=MIN(a,b)
	******************************************************************************************************************/
	#define	MIN(a, b)				((a) < (b) ? (a) : (b))
														#endif
	#ifndef	MAX
	/*!	@def	MAX
	*	@param[in]		a		any type data
	*	@param[in]		b		any type data
	*	@return			any type					:	=MAX(a,b)
	******************************************************************************************************************/
	#define	MAX(a, b)				((a) > (b) ? (a) : (b))
														#endif
	#ifndef	ABS
	/*!	@def	ABS
	*	@param[in]		x		any type data
	*	@return			any type					:	=ABS(x)
	******************************************************************************************************************/
	#define	ABS(x)					((x) <  0  ? -(x) : (x))

	/*!	@brief	ABS32
	*	Optimized absolute value calculation for signed 32b data.
	*	@param[in]		x		signed 32b input
	*	@return			SIGN32						:	=ABS(x)
	******************************************************************************************************************/
	INLINE SIGN32 ABS32(SIGN32 x) { SIGN32 s = x >> 31; return (x ^ s) - s; }
														#endif
	#ifndef	SIGN
	/*!	@def	SIGN
	*	@param[in]		x		signed data
	*	@return			integer						:	=SIGN(x)
	******************************************************************************************************************/
	#define	SIGN(x)					((x) < 0 ? - 1 : 1)
														#endif
	#ifndef	ROUND
	/*!	@def	ROUND
	*	@param[in]		x		any type data
	*	@return			integer						:	=ROUND(x)
	******************************************************************************************************************/
	#define	ROUND(x)				((SIGN32)((x) + ((x) < 0 ? - 0.5f : 0.5f)))
														#endif
	/*!	@def	Median
	*	Median of 3 inputs.
	*	@param[in]		a		1st input
	*	@param[in]		b		2nd input
	*	@param[in]		c		3rd input
	*	@return			any type					:	Median(a,b,c)
	******************************************************************************************************************/
	#define	Median(a, b, c)			((a) > (b) ? ((b) > (c) ? (b) : MIN(a, c)) : ((b) < (c) ? (b) : MAX(a, c)))

	/*!	@def	Median4
	*	Average of 2 median values of 4 inputs.
	*	@param[in]		a		1st input
	*	@param[in]		b		2nd input
	*	@param[in]		c		3rd input
	*	@param[in]		d		4th input
	*	@return			integer						:	average of median 2 of 4
	******************************************************************************************************************/
	#define	Median4(a, b, c, d)						\
			(((a) + (b) + (c) + (d) - MIN(MIN(a, b), MIN(c, d)) - MAX(MAX(a, b), MAX(c, d)) + 1) >> 1)

	/*!	@def	SATURATE
	*	Input saturation/clipping.
	*	@param[in]		x		input
	*	@param[in]		min		lower bound
	*	@param[in]		max		upper bound
	*	@return			any type					:	clipping result
	******************************************************************************************************************/
	#define	SATURATE(x, min, max)	((x) < (min) ? (min) : (x) > (max) ? (max) : (x))

	/*!	@def	UNIVCLP
	*	Universal clipping with following convention.
	*	@param[in]		x		input
	*	@param[in]		lmt		bounds:
	*							-	=0: no clipping
	*							-	<0: [lmt,-lmt-1]
	*							-	>0: [0,lmt]
	*	@return			any type					:	clipping result
	******************************************************************************************************************/
	#define	UNIVCLP(x, lmt)																							\
			(((lmt) < 0) ?	SATURATE(x, lmt, - (lmt) - 1)	:														\
			(((lmt) > 0) ?	SATURATE(x, 0, lmt)				:														\
							(x)))
	/*!	@def	norm2pow2
	*	Normalize a N-bit unsigned value from 0~2^N-1 to 0~2^N.
	*	@param[in]		b		normalization bit-depth
	*	@param[in]		x		unsigned input
	*	@return			integer						:	normalized value
	******************************************************************************************************************/
	#define	norm2pow2(b, x)			((x) + ((x) >> ((b) - 1)))

	/*!	@def	AverageNorm2
	*	Average 2 integer inputs with weight (norm-2^b).
	*	@param[in]		b		normalization bit-depth
	*	@param[in]		x1		1st integer input
	*	@param[in]		x2		2nd integer input
	*	@param[in]		a		weight of x1 (0~2^b-1)
	*	@return			integer						:	average result
	******************************************************************************************************************/
	#define	AverageNorm2(b, x1, x2, a)				\
			(((x1) * norm2pow2(b, a) + (x2) * ((1 << (b)) - norm2pow2(b, a)) + (1 << (b) >> 1)) >> (b))

	/*!	@def	Average
	*	Average 2 inputs with weight.
	*	@param[in]		x1		1st input
	*	@param[in]		x2		2nd input
	*	@param[in]		a		weight of x1
	*	@param[in]		s		integer sum of weights (norm)
	*	@return			any type					:	average result
	******************************************************************************************************************/
	#define	Average(x1, x2, a, s)	(((x1) * (a) + (x2) * ((s) - (a)) + ((s) >> 1)) / (s))

	/*!	@def	Average3
	*	Average 3 inputs with weights.
	*	@param[in]		r		rounding
	*	@param[in]		x1		1st input
	*	@param[in]		p1		weight of x1
	*	@param[in]		x2		2nd input
	*	@param[in]		p2		weight of x2
	*	@param[in]		x3		3rd input
	*	@param[in]		p3		weight of x3
	*	@return			any type					:	average result
	******************************************************************************************************************/
	#define	Average3(r, x1, p1, x2, p2, x3, p3)		\
			(((r) + (x1) * (p1) + (x2) * (p2) + (x3) * (p3)) / ((p1) + (p2) + (p3)))

	/*!	@def	ModInc
	*	Modulo incremental.
	*	@param[in,out]	x		variable to increase
	*	@param[in]		i		volume to increase
	*	@param[in]		mod		modulo
	******************************************************************************************************************/
	#define	ModInc(x, i, mod)						\
			do{	(x) += (i); while((x) >= (mod)) (x) -= (mod); }while(0)

	/*!	@def	Log2
	*	Get magnitude bits.
	*	@param[in]		x		input number
	*	@param[out]		b		magnitude bits
	******************************************************************************************************************/
	#define	Log2(x, b)								\
			do{	for((b) = 0; ; (b) ++) if((UNSG32)(x) < (UNSG32)(1 << (b))) break; }while(0)
/*!	@}
 */



/**//*!	@defgroup _float_arith
	*	Common used floating point operations.
	******************************************************/
/*!	@{
 */
	/******************************************************************************************************************
	*	Function: Round2Int
	*	Description: Convert a floating value to integer with rounding.
	*	Return:			SIGN32						-	Rounding result
	******************************************************************************************************************/
	INLINE	SIGN32	Round2Int(
						REAL64	x					/*!	Floating value to convert !*/
						)
	{
	return ROUND(x);
	/**	ENDOFFUNCTION: Round2Int **/
	}

	/******************************************************************************************************************
	*	Function: Ceil2Int
	*	Description: Convert a floating value to integer by using ceiling function.
	*	Return:			SIGN32						-	Ceiling result
	******************************************************************************************************************/
	INLINE	SIGN32	Ceil2Int(
						REAL64	x					/*!	Floating value to convert !*/
						)
	{
	SIGN32	d = (SIGN32)x;
	if((x <= 0) || (x == (REAL64)d))
		return d;
	return d + 1;
	/**	ENDOFFUNCTION: Ceil2Int **/
	}

	/******************************************************************************************************************
	*	Function: Floor2Int
	*	Description: Convert a floating value to integer by using flooring function.
	*	Return:			SIGN32						-	Flooring result
	******************************************************************************************************************/
	INLINE	SIGN32	Floor2Int(
						REAL64	x					/*!	Floating value to convert !*/
						)
	{
	SIGN32	d = (SIGN32)x;
	if((x >= 0) || (x == (REAL64)d))
		return d;
	return d - 1;
	/**	ENDOFFUNCTION: Floor2Int **/
	}

	/******************************************************************************************************************
	*	Function: FloatSqrt
	*	Description: Fast square-root funtion for floating value.
	*	Return:			REAL32						-	sqrt()
	******************************************************************************************************************/
#ifndef	__FASTSQRT__
	#define	FloatSqrt(x)		((REAL32)sqrt(x))
#else
	INLINE	REAL32	FloatSqrt(
						REAL32	x					/*!	Floating value input !*/
						)
	{
	union {
	SIGN32	i;
	REAL32	f;
	}	c1, c2;

	c1.f = x;
	c2.f = x;
	c1.i = 0x1FBCF800 + (c1.i >> 1);
	c2.i = 0x5F3759DF - (c2.i >> 1); 

	return 0.5f * (c1.f + (c2.f * x)); 
	/**	ENDOFFUNCTION: FloatSqrt **/
	}
#endif

	/******************************************************************************************************************
	*	Function: FloatInvSqrt
	*	Description: Fast inverted square-root funtion for floating value.
	*	Return:			REAL32						-	1/sqrt()
	******************************************************************************************************************/
#ifndef	__FASTSQRT__
	#define	FloatInvSqrt(x)		(1/(REAL32)sqrt(x))
#else
	INLINE	REAL32	FloatInvSqrt(
						REAL32	x					/*!	Floating value input !*/
						)
	{
	REAL32	h = 0.5f * x;
	SIGN32	i = *(SIGN32*)&x;

	i = 0x5F3759DF - (i >> 1);
	x = *(REAL32*)&i;
	x *= 1.5f - h * x * x;

	return x;
	/**	ENDOFFUNCTION: FloatInvSqrt **/
	}
#endif

/*!	@}
 */



/**//*!	@defgroup _bitfield
	*	Common used bit/bit-field operations.
	******************************************************/
/*!	@{
 */
	/*!	@def	SCLb
	*	Cut least 5 bits for scaling/shifting bit count to avoid compilation warning.
	*	@param[in]		k		scaling/shifting bits
	*	@return			integer						:	31&k
	******************************************************************************************************************/
	#define	SCLb(k)					(31 & (k))

	/*!	@def	bTST
	*	Get a bit to test.
	*	@param[in]		x		integer input
	*	@param[in]		b		bit index to test
	*	@return			bit							:	x[b]
	******************************************************************************************************************/
	#define	bTST(x, b)				(((x) >> (b)) & 1)

	/*!	@def	bSETMASK
	*	Mask for single-bit-set operation: |= bSETMASK(b).
	*	@param[in]		b		bit index
	*	@return			integer						:	32b mask
	******************************************************************************************************************/
	#define	bSETMASK(b)				((b) < 32 ? (1u << SCLb(b)) : 0)

	/*!	@def	bSET
	*	Set a bit.
	*	@param[in,out]	x		bit-vector to be set
	*	@param[in]		b		bit index to set
	******************************************************************************************************************/
	#define	bSET(x, b)				do{	(x) |= bSETMASK(b); }while(0)

	/*!	@def	bCLRMASK
	*	Mask for single-bit-clear operation: &= bCLRMASK(b).
	*	@param[in]		b		bit index
	*	@return			integer						:	32b mask
	******************************************************************************************************************/
	#define	bCLRMASK(b)				(~(bSETMASK(b)))

	/*!	@def	bCLR
	*	Clear a bit.
	*	@param[in,out]	x		bit-vector to be clear
	*	@param[in]		b		bit index to clear
	******************************************************************************************************************/
	#define	bCLR(x, b)				do{	(x) &= bCLRMASK(b); }while(0)

	/*!	@def	NSETMASK
	*	Mask for multiple-bit-set operation.
	*	@param[in]		msb		highest bit index
	*	@param[in]		lsb		lowest bit index
	*	@return			integer						:	32b mask
	******************************************************************************************************************/
	#define	NSETMASK(msb, lsb)		(bSETMASK((msb) + 1) - bSETMASK(lsb))

	/*!	@def	NCLRMASK
	*	Mask for multiple-bit-clear operation.
	*	@param[in]		msb		highest bit index
	*	@param[in]		lsb		lowest bit index
	*	@return			integer						:	32b mask
	******************************************************************************************************************/
	#define	NCLRMASK(msb, lsb)		(~(NSETMASK(msb, lsb)))

	/*!	@def	CutTo
	*	Cut a 32b integer to a bit-field.
	*	@param[in]		x		32b input
	*	@param[in]		b		bit depth of the bit-field
	*	@return			integer						:	bit-field of specified bits
	******************************************************************************************************************/
	#define	CutTo(x, b)				((x) & (bSETMASK(b) - 1))

	/*!	@def	SignedRestore
	*	Restore a signed bit-field to a 32b integer.
	*	@param[in]		x		signed bit-field
	*	@param[in]		b		bit depth of the bit-field
	*	@return			integer						:	32b signed value
	******************************************************************************************************************/
	#define	SignedRestore(x, b)		((SIGN32)(x) << (32 - (b)) >> (32 - (b)))

	/*!	@def	SignMagnitude
	*	Convert a 32b signed integer into a sign-magnitude bit-field (assuming no overflow).
	*	@param[in]		x		32b signed input
	*	@param[in]		b		bit depth of the bit-field
	*	@return			integer						:	sign-magnitude bit-field
	******************************************************************************************************************/
	#define	SignMagnitude(x, b)		CutTo((SIGN32)(x) < 0 ? bSETMASK((b) - 1) - (x) : (x), b)

	/*!	@def	InvSignMagnitude
	*	Restore a sign-magnitude bit-field to a 32b integer.
	*	@param[in]		x		sign-magnitude bit-field
	*	@param[in]		b		bit depth of the bit-field
	*	@return			integer						:	32b signed value
	******************************************************************************************************************/
	#define	InvSignMagnitude(x, b)	((SIGN32)(bTST(x, (b) - 1) ? bSETMASK((b) - 1) - (x) : (x)))

	/*!	@def	ClpSignMagnitude
	*	Convert a 32b signed integer into a sign-magnitude bit-field with clipping to avoid overflow.
	*	@param[out]		r		sign-magnitude bit-field
	*	@param[in]		x		32b signed input
	*	@param[in]		b		bit depth of the bit-field
	******************************************************************************************************************/
	#define	ClpSignMagnitude(r, x, b)				\
			do{	SIGN32 lmt_= (1 << ((b) - 1)) - 1, y = SATURATE(x, - lmt_, lmt_); r = SignMagnitude(y, b); }while(0)

	/*!	@def	DeltaField
	*	Calculate delta of 2 N-bit fields (free-run counter e.g.).
	*	@param[in]		fa		field.a (ending counter)
	*	@param[in]		fb		field.b (starting counter)
	*	@param[in]		b		valid count bits
	*	@return			integer						:	fa-fb
	******************************************************************************************************************/
	#define	DeltaField(fa, fb, b)	SignedRestore((fa) - (fb), b)

	/*!	@def	GetField
	*	Get a bit-field from a bit-vector by lsb & mask.
	*	@param[in]		r		bit-vector input
	*	@param[in]		lsb		lowest bit index of the bit-field
	*	@param[in]		mask	bit-field mask
	*	@return			integer						:	bit-field output
	******************************************************************************************************************/
	#define	GetField(r, lsb, mask)	(((r) & (mask)) >> (lsb))

	/*!	@def	BFGET
	*	Get a bit-field from a bit-vector by lsb & bit-depth.
	*	@param[in]		r		bit-vector input
	*	@param[in]		lsb		lowest bit index of the bit-field
	*	@param[in]		b		bit-field depth
	*	@return			integer						:	bit-field output
	******************************************************************************************************************/
	#define	BFGET(r, lsb, b)		GetField(r, lsb, NSETMASK((lsb) + (b) - 1, 0))

	/*!	@def	SetField
	*	Set a bit-field from a bit-vector by lsb & mask.
	*	@param[in,out]	r		bit-vector
	*	@param[in]		bf		bit-field
	*	@param[in]		lsb		lowest bit index of the bit-field
	*	@param[in]		mask	bit-field mask
	******************************************************************************************************************/
	#define	SetField(r, bf, lsb, mask)				\
			do{	(r) &= ~(mask); (r) |= ((bf) << (lsb)) & (mask); }while(0)

	/*!	@def	BFSET
	*	Set a bit-field from a bit-vector by lsb & bit-depth.
	*	@param[in,out]	r		bit-vector
	*	@param[in]		bf		bit-field
	*	@param[in]		lsb		lowest bit index of the bit-field
	*	@param[in]		b		bit-field depth
	******************************************************************************************************************/
	#define	BFSET(r, bf, lsb, b)	SetField(r, bf, lsb, NSETMASK((lsb) + (b) - 1, lsb))

	/*!	@def	GetUnsigned
	*	Restore an unsigned bit-field.
	*	@param[out]		v		output value
	*	@param[in]		bf		bit-field
	*	@param[in]		b		bit-field depth
	*	@return			integer						:	restored bit field value
	******************************************************************************************************************/
	#define	GetUnsigned(v, bf, b)	do{	(v) = (UNSG32)(bf); }while(0)

	/*!	@def	GetSigned
	*	Restore a signed bit-field.
	*	@param[out]		v		output value
	*	@param[in]		bf		bit-field
	*	@param[in]		b		bit-field depth
	*	@return			integer						:	restored bit field value
	******************************************************************************************************************/
	#define	GetSigned(v, bf, b)		do{	(v) = SignedRestore((UNSG32)bf, b); }while(0)

	/*!	@def	GetSignMagnitude
	*	Restore a sign-magnitude bit-field.
	*	@param[out]		v		output value
	*	@param[in]		bf		bit-field
	*	@param[in]		b		bit-field depth
	*	@return			integer						:	restored bit field value
	******************************************************************************************************************/
	#define	GetSignMagnitude(v, bf, b)	do{	(v) = InvSignMagnitude((UNSG32)bf, b); }while(0)

	/*!	@def	SetUnsigned
	*	Convert a value to an unsigned bit-field.
	*	@param[out]		bf		bit-field
	*	@param[in]		v		input value
	*	@param[in]		b		bit-field depth
	******************************************************************************************************************/
	#define	SetUnsigned(bf, v, b)	do{	(bf) = CutTo((UNSG32)v, b); }while(0)

	/*!	@def	SetSigned
	*	Convert a value to a signed bit-field.
	*	@param[out]		bf		bit-field
	*	@param[in]		v		input value
	*	@param[in]		b		bit-field depth
	******************************************************************************************************************/
	#define	SetSigned(bf, v, b)		SetUnsigned(bf, v, b)

	/*!	@def	SetSignMagnitude
	*	Convert a value to a sign-magnitude bit-field.
	*	@param[out]		bf		bit-field
	*	@param[in]		v		input value
	*	@param[in]		b		bit-field depth
	******************************************************************************************************************/
	#define	SetSignMagnitude(bf, v, b)				\
			do{	(bf) = SignMagnitude((SIGN32)v, b); }while(0)

	/*!	@def	UnsignedBF
	*	Convert a value to an unsigned bit-field & place to its bit position.
	*	@param[in]		v		input value
	*	@param[in]		b		bit-field depth
	*	@param[in]		lsb		lowest bit index of the bit-field location
	*	@return			integer						:	positioned bit field value
	******************************************************************************************************************/
	#define	UnsignedBF(v, b, lsb)	(CutTo((UNSG32)v, b) << (lsb))

	/*!	@def	SignedBF
	*	Convert a value to a signed bit-field & place to its bit position.
	*	@param[in]		v		input value
	*	@param[in]		b		bit-field depth
	*	@param[in]		lsb		lowest bit index of the bit-field location
	*	@return			integer						:	positioned bit field value
	******************************************************************************************************************/
	#define	SignedBF(v, b, lsb)		(CutTo((SIGN32)v, b) << (lsb))

	/*!	@def	SignMagnitudeBF
	*	Convert a value to a sign-magnitude bit-field & place to its bit position.
	*	@param[in]		v		input value
	*	@param[in]		b		bit-field depth
	*	@param[in]		lsb		lowest bit index of the bit-field location
	*	@return			integer						:	positioned bit field value
	******************************************************************************************************************/
	#define	SignMagnitudeBF(v, b, lsb)				\
			(SignMagnitude((SIGN32)v, b) << (lsb))
/*!	@}
 */



/**//*!	@defgroup _scaling_division
	*	Scaling/shifting/division operations.
	******************************************************/
/*!	@{
 */
	/*!	@def	UNIVSCL
	*	Universal scaling (positive to shift-left, negative to shift-right).
	*	@param[in]		a		pre-scaled integer input
	*	@param[in]		k		scaling bits
	*	@return			integer						:	post-scale result output
	******************************************************************************************************************/
	#define	UNIVSCL(a, k)			(((k) < 0) ? ((a) >> SCLb(- (k))) : ((a) << SCLb(k)))

	/*!	@def	UNSGDES
	*	Unsigned de-scaling (shift-right) with rounding.
	*	@param[in]		a		pre-scaled integer input
	*	@param[in]		k		de-scaling bits
	*	@return			integer						:	post-scale result output
	******************************************************************************************************************/
	#define	UNSGDES(a, k)			(((a) + (1 << SCLb(k) >> 1)) >> SCLb(k))

	/*!	@def	UNSGSCL
	*	Unsigned scaling (positive to shift-left, negative to shift-right) with rounding.
	*	@param[in]		a		pre-scaled integer input
	*	@param[in]		k		scaling bits
	*	@return			integer						:	post-scale result output
	******************************************************************************************************************/
	#define	UNSGSCL(a, k)			(((k) < 0) ? UNSGDES(a, - (k)) : ((a) << SCLb(k)))

	/*!	@def	SIGNDES
	*	Signed de-scaling (shift-right) with rounding.
	*	@param[in]		a		pre-scaled integer input
	*	@param[in]		k		de-scaling bits
	*	@return			integer						:	post-scale result output
	******************************************************************************************************************/
	#define	SIGNDES(a, k)			(((a) < 0) ? - UNSGDES(- (a), k) : UNSGDES(a, k))

	/*!	@def	SIGNSCL
	*	Signed scaling (positive to shift-left, negative to shift-right) with rounding.
	*	@param[in]		a		pre-scaled integer input
	*	@param[in]		k		scaling bits
	*	@return			integer						:	post-scale result output
	******************************************************************************************************************/
	#define	SIGNSCL(a, k)			(((k) < 0) ? SIGNDES(a, - (k)) : ((a) << SCLb(k)))

	/*!	@def	REALSCL
	*	Scaling (positive to shift-left, negative to shift-right) on floating point input.
	*	@param[in]		a		pre-scaled float/double input
	*	@param[in]		k		scaling bits
	*	@return			float/double				:	post-scale result output
	******************************************************************************************************************/
	#define	REALSCL(a, k)			(((k) < 0) ? (a) / (1 << SCLb(- (k))) : (a) * (1 << SCLb(k)))

	/*!	@def	UNSGEXP
	*	Unsigned bits (de-)expansion.
	*	@param[in]		a		unsigned integer input
	*	@param[in]		b		bit-depth of input
	*	@param[in]		k		bit-expanding (positive) or de-expanding (negative) for output
	*	@return			integer						:	output pixel value
	******************************************************************************************************************/
	#define	UNSGEXP(a, b, k)		(((k) < 0) ? ((a) >> SCLb(- (k))) : (((a) << (k)) + ((a) >> ((b) - (k)))))

	/*!	@def	SIGNEXP
	*	Signed bits (de-)expansion.
	*	@param[in]		a		signed integer input
	*	@param[in]		b		bit-depth of input magnitude
	*	@param[in]		k		bit-expanding (positive) or de-expanding (negative) for output
	*	@return			integer						:	output pixel value
	******************************************************************************************************************/
	#define	SIGNEXP(a, b, k)		(((a) < 0) ? - UNSGEXP(- (a), b, k) : UNSGEXP(a, b, k))

	/*!	@def	UNIVRND
	*	Scaling (positive to shift-left, negative to shift-right) rounding.
	*	@param[in]		k		scaling bits
	*	@return			integer						:	rounding equivalent to 1/2
	******************************************************************************************************************/
	#define	UNIVRND(k)				UNIVSCL(1, (k) - 1)

	/*!	@def	CEILDIV
	*	Ceiling of integer division.
	*	@param[in]		a		integer dividend
	*	@param[in]		b		integer diviser
	*	@return			integer						:	ceiling of quotiant
	******************************************************************************************************************/
	#define	CEILDIV(a, b)			(((a) + (b) - 1) / (b))

	/*!	@def	UNSGDIV
	*	Unsigned integer division with 1/2 rounding.
	*	@param[in]		a		integer dividend
	*	@param[in]		b		integer diviser
	*	@return			integer						:	quotiant
	******************************************************************************************************************/
	#define	UNSGDIV(a, b)			(((a) + ((b) >> 1)) / (b))

	/*!	@def	SIGNDIV
	*	Signed integer division with 1/2 rounding.
	*	@param[in]		a		integer dividend
	*	@param[in]		b		integer diviser
	*	@return			integer						:	quotiant
	******************************************************************************************************************/
	#define	SIGNDIV(a, b)			((((a) > 0) ? ((a) + ((b) >> 1)) : ((a) - ((b) >> 1))) / (b))

	/*!	@def	NSclDIV
	*	Integer division with pre/pos-scaling to get higher precision and avoid overflow.
	*	@param[in]		a		integer dividend
	*	@param[in]		k1		pre-upscaling
	*	@param[in]		b		integer diviser
	*	@param[in]		k2		pos-upscaling
	*	@return			integer						:	~= a*k1/b*k2
	******************************************************************************************************************/
	#define	NSclDIV(a, k1, b, k2)	((a) * (k1) / (b) * (k2) + UNSGDIV((((a) * (k1)) % (b)) * (k2), b))

/*!	@}
 */



/**//*!	@defgroup _matrix3x3
	*	3x3 matrix & 3x1 vector operations.
	******************************************************/
/*!	@{
 */
	/*!	@def	DET2x2
	*	Calculate 2x2 determinant.
	*	@param[in]		a		m[0][0]
	*	@param[in]		b		m[0][1]
	*	@param[in]		c		m[1][0]
	*	@param[in]		d		m[1][1]
	*	@return			any type					:	determinant
	******************************************************************************************************************/
	#define	DET2x2(a, b, c, d)		((a) * (d) - (b) * (c))

	/*!	@def	INV2x2
	*	Calculate inverse of 2x2 matrix (pre-normalization).
	*	@param[out]		t		inverse matrix
	*	@param[in]		m		original matrix
	******************************************************************************************************************/
	#define	INV2x2(t, m)																							\
			do{	t[0][0] =   m[1][1]; t[0][1] = - m[1][0];															\
				t[1][0] = - m[0][1]; t[1][1] =   m[0][0];															\
									}while(0)
	/*!	@def	DET3x3
	*	Calculate 3x3 determinant.
	*	@param[in]		m		input matrix
	*	@return			any type					:	determinant
	******************************************************************************************************************/
	#define	DET3x3(m)				(																				\
			+ m[0][0] * DET2x2(m[1][1], m[1][2], m[2][1], m[2][2])													\
				- m[1][0] * DET2x2(m[0][1], m[0][2], m[2][1], m[2][2])												\
					+ m[2][0] * DET2x2(m[0][1], m[0][2], m[1][1], m[1][2])											\
									)
	/*!	@def	INV3x3
	*	Calculate inverse of 3x3 matrix (pre-normalization).
	*	@param[out]		t		inverse matrix
	*	@param[in]		m		original matrix
	******************************************************************************************************************/
	#define	INV3x3(t, m)																							\
			do{	t[0][0] = + DET2x2(m[1][1], m[1][2], m[2][1], m[2][2]);												\
					t[0][1] = - DET2x2(m[0][1], m[0][2], m[2][1], m[2][2]);											\
						t[0][2] = + DET2x2(m[0][1], m[0][2], m[1][1], m[1][2]);										\
				t[1][0] = - DET2x2(m[1][0], m[1][2], m[2][0], m[2][2]);												\
					t[1][1] = + DET2x2(m[0][0], m[0][2], m[2][0], m[2][2]);											\
						t[1][2] = - DET2x2(m[0][0], m[0][2], m[1][0], m[1][2]);										\
				t[2][0] = + DET2x2(m[1][0], m[1][1], m[2][0], m[2][1]);												\
					t[2][1] = - DET2x2(m[0][0], m[0][1], m[2][0], m[2][1]);											\
						t[2][2] = + DET2x2(m[0][0], m[0][1], m[1][0], m[1][1]);										\
									}while(0)
	/*!	@def	ADD3x1
	*	3x3 matrix column (3x1 vector) add.
	*	@param[out]		t		output matrix
	*	@param[in]		m		1st input matrix
	*	@param[in]		n		2nd input matrix
	*	@param[in]		j		vector column index in matrix
	******************************************************************************************************************/
	#define	ADD3x1(t, m, n, j)																						\
			do{	t[0][j] = m[0][j] + n[0][j];																		\
				t[1][j] = m[1][j] + n[1][j];																		\
				t[2][j] = m[2][j] + n[2][j];																		\
									}while(0)
	/*!	@def	ADD3x3
	*	3x3 matrix add.
	*	@param[out]		t		output matrix
	*	@param[in]		m		1st input matrix
	*	@param[in]		n		2nd input matrix
	******************************************************************************************************************/
	#define	ADD3x3(t, m, n)			do{	ADD3x1(t, m, n, 0); ADD3x1(t, m, n, 1); ADD3x1(t, m, n, 2); }while(0)

	/*!	@def	SUB3x1
	*	3x3 matrix column (3x1 vector) subtraction.
	*	@param[out]		t		output matrix
	*	@param[in]		m		1st input matrix
	*	@param[in]		n		2nd input matrix
	*	@param[in]		j		vector column index in matrix
	******************************************************************************************************************/
	#define	SUB3x1(t, m, n, j)																						\
			do{	t[0][j] = m[0][j] - n[0][j];																		\
				t[1][j] = m[1][j] - n[1][j];																		\
				t[2][j] = m[2][j] - n[2][j];																		\
									}while(0)
	/*!	@def	SUB3x3
	*	3x3 matrix subtraction.
	*	@param[out]		t		output matrix
	*	@param[in]		m		1st input matrix
	*	@param[in]		n		2nd input matrix
	******************************************************************************************************************/
	#define	SUB3x3(t, m, n)			do{	SUB3x1(t, m, n, 0); SUB3x1(t, m, n, 1); SUB3x1(t, m, n, 2); }while(0)

	/*!	@def	CNV3ij
	*	Multiply a 3x3 matrix row (1x3 vector) with a 3x3 matrix column (3x1 vector).
	*	@param[in]		m		1st input matrix
	*	@param[in]		n		2nd input matrix
	*	@param[in]		i		row index in 1st matrix
	*	@param[in]		j		column index in 2nd matrix
	*	@return			any type					:	convolution result
	******************************************************************************************************************/
	#define	CNV3ij(m, n, i, j)		(m[i][0] * n[0][j] + m[i][1] * n[1][j] + m[i][2] * n[2][j])

	/*!	@def	CNV3x1
	*	Multiply a 3x3 matrix with a 3x3 matrix column (3x1 vector).
	*	@param[in]		m		1st input matrix
	*	@param[in]		n		2nd input matrix
	*	@param[in]		j		column index in 2nd matrix
	******************************************************************************************************************/
	#define	CNV3x1(t, m, n, j)																						\
			do{	t[0][j] = CNV3ij(m, n, 0, j);																		\
				t[1][j] = CNV3ij(m, n, 1, j);																		\
				t[2][j] = CNV3ij(m, n, 2, j);																		\
									}while(0)
	/*!	@def	CNV3x3
	*	Multiply a 3x3 matrix with a 3x3 matrix.
	*	@param[out]		t		output matrix
	*	@param[in]		m		1st input matrix
	*	@param[in]		n		2nd input matrix
	******************************************************************************************************************/
	#define	CNV3x3(t, m, n)			do{	CNV3x1(t, m, n, 0); CNV3x1(t, m, n, 1); CNV3x1(t, m, n, 2); }while(0)

	/*!	@def	INC3x1
	*	3x3 matrix column (3x1 vector) increase by a variable.
	*	@param[out]		t		output matrix
	*	@param[in]		m		input matrix
	*	@param[in]		x		incremental variable
	*	@param[in]		j		vector column index in matrix
	******************************************************************************************************************/
	#define	INC3x1(t, m, x, j)																						\
			do{	t[0][j] = m[0][j] + (x);																			\
				t[1][j] = m[1][j] + (x);																			\
				t[2][j] = m[2][j] + (x);																			\
									}while(0)
	/*!	@def	INC3x3
	*	3x3 matrix increase by a variable.
	*	@param[out]		t		output matrix
	*	@param[in]		m		input matrix
	*	@param[in]		x		incremental variable
	******************************************************************************************************************/
	#define	INC3x3(t, m, x)			do{	INC3x1(t, m, x, 0); INC3x1(t, m, x, 1); INC3x1(t, m, x, 2); }while(0)

	/*!	@def	MUL3x1
	*	3x3 matrix column (3x1 vector) multiply by a variable.
	*	@param[out]		t		output matrix
	*	@param[in]		m		input matrix
	*	@param[in]		x		multiplication factor
	*	@param[in]		j		vector column index in matrix
	******************************************************************************************************************/
	#define	MUL3x1(t, m, x, j)																						\
			do{	t[0][j] = m[0][j] * (x);																			\
				t[1][j] = m[1][j] * (x);																			\
				t[2][j] = m[2][j] * (x);																			\
									}while(0)
	/*!	@def	MUL3x3
	*	3x3 matrix multiply by a variable.
	*	@param[out]		t		output matrix
	*	@param[in]		m		input matrix
	*	@param[in]		x		multiplication factor
	******************************************************************************************************************/
	#define	MUL3x3(t, m, x)			do{	MUL3x1(t, m, x, 0); MUL3x1(t, m, x, 1); MUL3x1(t, m, x, 2); }while(0)

	/*!	@def	DIV3x1
	*	3x3 matrix column (3x1 vector) divide by a variable.
	*	@param[out]		t		output matrix
	*	@param[in]		m		input matrix
	*	@param[in]		x		diviser
	*	@param[in]		j		vector column index in matrix
	******************************************************************************************************************/
	#define	DIV3x1(t, m, x, j)																						\
			do{	t[0][j] = SIGNDIV(m[0][j],x);																		\
				t[1][j] = SIGNDIV(m[1][j],x);																		\
				t[2][j] = SIGNDIV(m[2][j],x);																		\
									}while(0)
	/*!	@def	DIV3x3
	*	3x3 matrix divide by a variable.
	*	@param[out]		t		output matrix
	*	@param[in]		m		input matrix
	*	@param[in]		x		diviser
	******************************************************************************************************************/
	#define	DIV3x3(t, m, x)			do{	DIV3x1(t, m, x, 0); DIV3x1(t, m, x, 1); DIV3x1(t, m, x, 2); }while(0)

	/*!	@def	SCL3x1
	*	3x3 matrix column (3x1 vector) scale by a variable.
	*	@param[out]		t		output matrix
	*	@param[in]		m		input matrix
	*	@param[in]		x		scale bits (positive to shift-left, negative to shift-right)
	*	@param[in]		j		vector column index in matrix
	******************************************************************************************************************/
	#define	SCL3x1(t, m, x, j)																						\
			do{	t[0][j] = UNSGSCL(m[0][j],x);																		\
				t[1][j] = UNSGSCL(m[1][j],x);																		\
				t[2][j] = UNSGSCL(m[2][j],x);																		\
									}while(0)
	/*!	@def	SCL3x3
	*	3x3 matrix scale by a variable.
	*	@param[out]		t		output matrix
	*	@param[in]		m		input matrix
	*	@param[in]		x		scale bits (positive to shift-left, negative to shift-right)
	******************************************************************************************************************/
	#define	SCL3x3(t, m, x)			do{	SCL3x1(t, m, x, 0); SCL3x1(t, m, x, 1); SCL3x1(t, m, x, 2); }while(0)

	/*!	@def	SET3x1
	*	Import a 3x3 matrix column (3x1 vector) from an optionally transposed linear array.
	*	@param[out]		m		output matrix
	*	@param[in]		v		input array
	*	@param[in]		d		if transpose required
	*	@param[in]		j		vector column index in matrix
	******************************************************************************************************************/
	#define	SET3x1(m, v, d, j)																						\
			do{	m[0][j] = v[d ? j : j * 3];																			\
				m[1][j] = v[d ? j + 3 : j * 3 + 1];																	\
				m[2][j] = v[d ? j + 6 : j * 3 + 2];																	\
									}while(0)
	/*!	@def	SET3x3
	*	Import 3x3 matrix from an optionally transposed linear array.
	*	@param[out]		m		output matrix
	*	@param[in]		v		input array
	*	@param[in]		d		if transpose required
	******************************************************************************************************************/
	#define	SET3x3(m, v, d)			do{	SET3x1(m, v, d, 0); SET3x1(m, v, d, 1); SET3x1(m, v, d, 2); }while(0)

	/*!	@def	GET3x1
	*	Export a 3x3 matrix column (3x1 vector) to an optionally transposed linear array.
	*	@param[out]		m		output matrix
	*	@param[in]		v		input array
	*	@param[in]		d		if transpose required
	*	@param[in]		j		vector column index in matrix
	******************************************************************************************************************/
	#define	GET3x1(m, v, d, j)																						\
			do{	v[d ? j : j * 3] = m[0][j];																			\
				v[d ? j + 3 : j * 3 + 1] = m[1][j];																	\
				v[d ? j + 6 : j * 3 + 2] = m[2][j];																	\
									}while(0)
	/*!	@def	GET3x3
	*	Export 3x3 matrix to an optionally transposed linear array.
	*	@param[out]		m		output matrix
	*	@param[in]		v		input array
	*	@param[in]		d		if transpose required
	******************************************************************************************************************/
	#define	GET3x3(m, v, d)			do{	GET3x1(m, v, d, 0); GET3x1(m, v, d, 1); GET3x1(m, v, d, 2); }while(0)

	/*!	@def	TSP3x3
	*	3x3 matrix transpose.
	*	@param[out]		t		transposed matrix
	*	@param[in]		m		original matrix
	******************************************************************************************************************/
	#define	TSP3x3(t, m)																							\
			do{	t[0][0] = m[0][0]; t[0][1] = m[1][0]; t[0][2] = m[2][0];											\
				t[1][0] = m[0][1]; t[1][1] = m[1][1]; t[1][2] = m[2][1];											\
				t[2][0] = m[0][2]; t[2][1] = m[1][2]; t[2][2] = m[2][2];											\
									}while(0)
	/*!	@def	INV3x3NORM
	*	Calculate inverse of 3x3 matrix (integer) and normalize to the given bits.
	*	@param[out]		t		inverse matrix
	*	@param[in]		tb		inverse matrix entry bit-depth
	*	@param[in]		m		original matrix
	*	@param[in]		mb		original matrix entry bit-depth
	******************************************************************************************************************/
	#define	INV3x3NORM(t, tb, m, mb)																				\
			do{	SIGN32 n_[3][3], det_;																				\
				SCL3x3(n_, m, (tb) - (mb)); det_= UNSGDES(DET3x3(n_), 4); INV3x3(t, n_);							\
				SCL3x3(n_, t, (tb) * 2 - 4); DIV3x3(t, n_, det_);													\
									}while(0)
/*!	@}
 */



/**//*!	@defgroup _mem_string
	*	Memory/string wrappers & operations.
	*	Freely use even for embedded porting:
	*	-	memcpy
	*	-	memset
	*	-	strlen
	*	-	strcpy
	*	-	strcat
	******************************************************/
/*!	@{
 */
	#ifdef	__big_endian__
	#define	ADDRi8					(0x3)
	#define	ADDRi16					(0x1)
	#define	MSB32(u32)				(((UNSG8*)&(u32))[0])
	#define	LSB32(u32)				(((UNSG8*)&(u32))[3])
	#else

	/*!	@def	ADDRi8
	*	Byte address mask to get 'linear' byte: addr ^= ADDRi8.
	******************************************************************************************************************/
	#define	ADDRi8					(0x0)

	/*!	@def	ADDRi16
	*	Double-byte address mask to get 'linear' d-byte: addr ^= ADDRi16.
	******************************************************************************************************************/
	#define	ADDRi16					(0x0)

	/*!	@def	MSB32
	*	Get highest byte of a 32b word.
	*	@param[in]		u32		input 32b word
	*	@return			UNSG8						:	MSB
	******************************************************************************************************************/
	#define	MSB32(u32)				(((UNSG8*)&(u32))[3])

	/*!	@def	LSB32
	*	Get lowest byte of a 32b word.
	*	@param[in]		u32		input 32b word
	*	@return			UNSG8						:	LSB
	******************************************************************************************************************/
	#define	LSB32(u32)				(((UNSG8*)&(u32))[0])
	#endif

	/*!	@def	offsetof
	*	Get byte-offsetr of structure member.
	*	@param[in]		T		structure type
	*	@param[in]		m		member name
	*	@return			SIGN32						:	byte-offset
	******************************************************************************************************************/
#ifndef offsetof
	#define	offsetof(T, m)			((SIGN32)&(((T*)0)->m))
#endif

	/*!	@def	sizeofType
	*	Size of a type in count of another type.
	*	@param[in]		Ta		parent type
	*	@param[in]		Tb		sub-type as counting unit
	*	@return			integer						:	unit count
	******************************************************************************************************************/
	#define	sizeofType(Ta, Tb)		(sizeof(Ta)/sizeof(Tb))

	/*!	@def	sizeofArray
	*	Size of an array (total entries).
	*	@param[in]		arr		array variable
	*	@return			integer						:	array entries
	******************************************************************************************************************/
	#define	sizeofArray(arr)		(sizeofType(arr, arr[0]))

	/*!	@def	sizeofMember
	*	Size of structure member.
	*	@param[in]		T		structure type
	*	@param[in]		m		member name
	*	@return			integer						:	byte size of member
	******************************************************************************************************************/
	#define	sizeofMember(T, m)		(sizeof(((T*)0)->m))

	/*!	@def	sizeofArrMember
	*	Size of array type structure member.
	*	@param[in]		T		structure type
	*	@param[in]		m		member name
	*	@return			integer						:	array member entries
	******************************************************************************************************************/
	#define	sizeofArrMember(T, m)	(sizeofArray(((T*)0)->m))

	/*!	@def	loopOp
	*	Looping of an expression.
	*	@param[in]		n		number of times to loop
	*	@param[in]		exp		expression to loop
	******************************************************************************************************************/
	#define	loopOp(n, exp)			\
		do{	SIGN32 i_= -1, n_= (n); while(++ i_< n_) exp; }while(0)

	/*!	@def	arr2op
	*	Operating 2-dimensional arrays.
	*	@param[in]		N		matrix width
	*	@param[in]		M		matrix height
	*	@param[in]		pT		output data type (*)
	*	@param[out]		pd		output matrix
	*	@param[in]		incp	output matrix incremental step
	*	@param[in]		pitchp	output matrix pitch incremental
	*	@param[in]		pTa		1st input data type (*)
	*	@param[in]		pa		1st input matrix
	*	@param[in]		incpa	1st input matrix incremental step
	*	@param[in]		pitchpa	1st input matrix pitch incremental
	*	@param[in]		pTb		2nd input data type (*)
	*	@param[in]		pb		2nd input matrix
	*	@param[in]		incpb	2nd input matrix incremental step
	*	@param[in]		pitchpb	2nd input matrix pitch incremental
	*	@param[in]		exp		operation expression of (p_[i_], pa_[ia_], pb_[ib_])
	******************************************************************************************************************/
	#define	arr2op(N, M, pT, pd, incp, pitchp, pTa, pa, incpa, pitchpa, pTb, pb, incpb, pitchpb, exp)				\
			do{	pT p_= (pd); pTa pa_= (pa); pTb pb_= (pb);															\
				SIGN32 N_= (N), M_= (M), m_, n_, i_, ia_, ib_,														\
						incp_= (incp), pitchp_= (pitchp),															\
						incpa_= (incpa), pitchpa_= (pitchpa),														\
						incpb_= (incpb), pitchpb_= (pitchpb);														\
				for(m_= 0; m_< M_; m_++) {																			\
					for(i_= ia_= ib_= 0,																			\
						n_= 0; n_< N_; n_++,																		\
						i_+= incp_, ia_+= incpa_, ib_+= incpb_)														\
						{exp;}																						\
						p_+= pitchp_; pa_+= pitchpa_; pb_+= pitchpb_;												\
								}																					\
									}while(0)
	/*!	@def	arr2cpy
	*	2-dimensional array copy.
	*	@param[in]		N		matrix width
	*	@param[in]		M		matrix height
	*	@param[in]		T		output data type
	*	@param[out]		p		output matrix
	*	@param[in]		pitchp	output matrix pitch incremental
	*	@param[in]		Ti		input data type
	*	@param[in]		pi		input matrix
	*	@param[in]		pitchpi	input matrix pitch incremental
	******************************************************************************************************************/
	#define	arr2cpy(N, M, T, p, pitchp, Ti, pi, pitchpi)															\
			arr2op(N, M, T*, p, 1, pitchp, Ti*, pi, 1, pitchpi, char*, NULL, 0, 0, (p_[i_] = (T)(pa_[ia_])))

	/*!	@def	arr2tsp
	*	2-dimensional array transpose.
	*	@param[in]		N		matrix width (output) & height (input)
	*	@param[in]		M		matrix height (output) & width (input)
	*	@param[in]		T		output data type
	*	@param[out]		p		output matrix
	*	@param[in]		pitchp	output matrix pitch incremental
	*	@param[in]		Ti		input data type
	*	@param[in]		pi		input matrix
	*	@param[in]		pitchpi	input matrix pitch incremental
	******************************************************************************************************************/
	#define	arr2tsp(N, M, T, p, pitchp, Ti, pi, pitchpi)															\
			arr2op(N, M, T*, p, 1, pitchp, Ti*, pi, pitchpi, 1, char*, NULL, 0, 0, (p_[i_] = (T)(pa_[ia_])))

	/*!	@def	arrSet
	*	Set an array by an expression.
	*	@param[in]		T		type of the array
	*	@param[out]		p		array pointer
	*	@param[in]		v		expression to set
	*	@param[in]		n		number of entries to set
	******************************************************************************************************************/
	#define	arrSet(T, p, v, n)		\
			do{	T *p_= (T*)(p); loopOp(n, (p_[i_] = (T)(v))); }while(0)

	/*!	@def	clrStruct
	*	Clear a structure to 0.
	*	@param[in]		T		type of the structure
	*	@param[out]		p		structure pointer
	******************************************************************************************************************/
	#define	clrStruct(T, p)			arrSet(UNSG32, p, 0, sizeofType(T, UNSG32))

	/*!	@def	xCpy
	*	Copy a number of entries between 2 arrays.
	*	@param[in]		Ta		type of the destination array
	*	@param[out]		pa		destination array pointer
	*	@param[in]		Tb		type of the source array
	*	@param[in]		pb		source array pointer
	*	@param[in]		n		number of entries to copy
	******************************************************************************************************************/
	#define	xCpy(Ta, pa, Tb, pb, n)	do{	Tb *pb_= (Tb*)(pb); arrSet(Ta, pa, pb_[i_], n); }while(0)

	/*!	@def	xSet
	*	Set an array by a variable.
	*	@param[in]		T		type of the array
	*	@param[out]		p		array pointer
	*	@param[in]		v		variable to set
	*	@param[in]		n		number of entries to set
	******************************************************************************************************************/
	#define	xSet(T, p, v, n)		do{	T v_= (T)(v); arrSet(T, p, v_, n); }while(0)

	/*!	@def	r32Cpy
	*	Copy a number of 32b-entries between 2 arrays.
	*	@param[out]		r32		destination array pointer
	*	@param[in]		src		source array pointer
	*	@param[in]		n		number of 32b-entries to copy
	******************************************************************************************************************/
	#define	r32Cpy(r32, src, n)		xCpy(UNSG32, r32, UNSG32, src, n)

	/*!	@def	eleCpy
	*	Copy between 2 structure instance.
	*	@param[in]		T		structure type
	*	@param[out]		ele		destination pointer
	*	@param[in]		src		source pointer
	******************************************************************************************************************/
	#define	eleCpy(T, ele, src)		r32Cpy(ele, src, sizeof(T)>>2)

	/*!	@def	r32Set
	*	Set an 32b array by a variable.
	*	@param[out]		r32		32b array pointer
	*	@param[in]		v		variable to set
	*	@param[in]		n		number of 32b-entries to set
	******************************************************************************************************************/
	#define	r32Set(r32, v, n)		xSet(UNSG32, r32, v, n)

	#ifndef	memcpy
	#ifdef	__CODE_LINK__
	#define	memcpy(mem, src, n)		xCpy(char, mem, char, src, n)
													#endif
														#endif
	#ifndef	memset
	#ifdef	__CODE_LINK__
	#define	memset(mem, val, n)		xSet(char, mem, val, n)
													#endif
														#endif
	#ifndef	strlen
	#ifdef	__CODE_LINK__
	#define	strlen					0
													#endif
														#endif
	#ifndef	strcpy
	#ifdef	__CODE_LINK__
	#define	strcpy(str, src)		1 ? "":
													#endif
														#endif
	#ifndef	strcat
	#ifdef	__CODE_LINK__
	#define	strcat(str, src)		1 ? "":
													#endif
														#endif
	/*!	@def	endofString
	*	Get pointer of end of a string (with an offset).
	*	@param[in]		str		string
	*	@param[in]		i		offset from the end of the string
	*	@return			pointer						:	end of string with offset
	******************************************************************************************************************/
	#define	endofString(str, i)		((str) + (strlen(str) - (i)))

	/*!	@def	freadLn
	*	Get pointer of end of a string (with an offset).
	*	@param[in]		str		string
	*	@param[in]		fp		file pointer
	******************************************************************************************************************/
	#define	freadLn(str, fp)																						\
			do{	char *s_= (char*)(str); FILE *fp_= (FILE*)(fp);														\
				while(!feof(fp_))																					\
					if((*s_++ = getc(fp_)) == '\n')																	\
						break;																						\
				*s_= 0;																								\
									}while(0)
	/*!	@def	ToBinary
	*	Convert a integer to a string of its binary format.
	*	@param[in]		x		integer
	*	@param[in]		b		number of bits
	*	@param[out]		str		output string
	******************************************************************************************************************/
	#define	ToBinary(x, b, str)																						\
			do{	char *str_= (char*)(str);																			\
				arrSet(char, str_, bTST(x, (b) - i_- 1) ? '1' : '0', b);											\
				str_[b] = 0;																						\
									}while(0)
	/*!	@def	FromBinary
	*	Convert a binary format string to its integer value.
	*	@param[out]		x		integer
	*	@param[in]		b		number of bits
	*	@param[in]		str		input string
	******************************************************************************************************************/
	#define	FromBinary(x, b, str)																					\
			do{	char *str_= (char*)(str); SIGN32 i_= -1, n_= b, x_= 0;												\
				while(++ i_< n_) x_= (x_<< 1) | (str_[i_] == '1');													\
				(x) = x_;																							\
									}while(0)
/*!	@}
 */



/**//*!	@defgroup _fiber
	*	'multi-fiber' program control.
	******************************************************/
/*!	@{
 */
	/*!	@def	FiberEnter
	*	Entering a 'fiber', shall be placed at the beginning of a fiber function.
	*	@param[in,out]	st		'fiber' state
	*	@param[out]		id2st	look-up table from state-id to state:
	*							-	NULL to ignore (default use)
	*							-	non-zero to initialize look-up table
	******************************************************************************************************************/
	#define	FiberEnter(st, id2st)																					\
			switch(st){ static SIGN32 st_;																			\
				case 0: if(id2st) do{ st_= ((SIGN32*)(id2st))[0] = 0; return -1; }while(0)

	/*!	@def	FiberYield
	*	Fiber function yield (to other fibers).
	*	@param[in,out]	st		'fiber' state
	*	@param[out]		id2st	look-up table from state-id to state:
	*							-	NULL to ignore (default use)
	*							-	non-zero to initialize look-up table
	*	@param[in]		enb		if enable 'yield'
	*	@param[out]		id		current state-id (shall be non-zero) to return
	******************************************************************************************************************/
	#define	FiberYield(st, id2st, enb, id)																			\
			do{	SIGN32 id_= (id); (st) = __LINE__; if(enb) return id_;												\
				case __LINE__: if(id2st){ ((SIGN32*)(id2st))[ABS(id_)] = st_; return st_= __LINE__; }				\
									}while(0)
	/*!	@def	FiberSleep
	*	Similar to FiberYield, except return a negative id to notify scheduler this fiber can be put on hold.
	*	@param[in,out]	st		'fiber' state
	*	@param[out]		id2st	look-up table from state-id to state:
	*							-	NULL to ignore (default use)
	*							-	non-zero to initialize look-up table
	*	@param[in]		enb		if enable 'yield'
	*	@param[out]		id		current state-id to return (its negative value)
	******************************************************************************************************************/
	#define	FiberSleep(st, id2st, enb, id)																			\
			FiberYield(st, id2st, enb, -(id))

	/*!	@def	FiberExit
	*	Exiting a 'fiber', shall be placed at the end of a fiber function.
	*	@param[in,out]	st		'fiber' state, will be reset to 0 (idle)
	*	@return			integer						:	state-id=0 as idle
	******************************************************************************************************************/
	#define	FiberExit(st)																							\
				case __LINE__:	(st) = 0; return 0;																	\
									} return -1
	/*!	@def	FiberSetup
	*	Setup a 'fiber', optionally initialize a state-id to state look-up table.
	*	@param[in,out]	st		'fiber' state, will be reset to 0 (idle)
	*	@param[out]		id2st	look-up table from state-id to state:
	*							-	NULL to ignore (default use)
	*							-	non-zero to initialize look-up table
	*	@param[in]		fiber	fiber function call expression
	******************************************************************************************************************/
	#define	FiberSetup(st, id2st, fiber)																			\
			do{	(st) = 0; if(id2st) while(fiber != 0) (st) ++; (st) = 0; }while(0)
/*!	@}
 */

#endif
