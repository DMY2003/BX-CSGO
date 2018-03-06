//===== Copyright © 1996-2005, Valve Corporation, All rights reserved. ======//
//
// Purpose: 
//
//===========================================================================//
#pragma once
#include <cstring>

typedef struct
{
	unsigned int buf[4];
	unsigned int bits[2];
	unsigned char in[64];
} MD5Context_t;

#define MD5_DIGEST_LENGTH 16

// The four core functions - F1 is optimized somewhat
// #define F1(x, y, z) (x & y | ~x & z)
#define F1(x, y, z) (z ^ (x & (y ^ z)))
#define F2(x, y, z) F1(z, x, y)
#define F3(x, y, z) (x ^ y ^ z)
#define F4(x, y, z) (y ^ (x | ~z))

// This is the central step in the MD5 algorithm.
#define MD5STEP(f, w, x, y, z, data, s) \
        ( w += f(x, y, z) + data,  w = w<<s | w>>(32-s),  w += x )

//-----------------------------------------------------------------------------
// Purpose: The core of the MD5 algorithm, this alters an existing MD5 hash to
//  reflect the addition of 16 longwords of new data.  MD5Update blocks
//  the data and converts bytes into longwords for this routine.
// Input  : buf[4] - 
//			in[16] - 
// Output : static void
//-----------------------------------------------------------------------------
void MD5Transform(unsigned int buf[4], unsigned int const in[16]);

//-----------------------------------------------------------------------------
// Purpose: Start MD5 accumulation.  Set bit count to 0 and buffer to mysterious initialization constants.

// Input  : *ctx - 
//-----------------------------------------------------------------------------
inline void MD5Init(MD5Context_t* ctx);

//-----------------------------------------------------------------------------
// Purpose: Update context to reflect the concatenation of another buffer full of bytes.
// Input  : *ctx - 
//			*buf - 
//			len - 
//-----------------------------------------------------------------------------
inline void MD5Update(MD5Context_t* ctx, unsigned char const* buf, unsigned int len);

//-----------------------------------------------------------------------------
// Purpose: Final wrapup - pad to 64-byte boundary with the bit pattern 
// 1 0* (64-bit count of bits processed, MSB-first)
// Input  : digest[MD5_DIGEST_LENGTH] - 
//			*ctx - 
//-----------------------------------------------------------------------------
inline void MD5Final(unsigned char digest[MD5_DIGEST_LENGTH], MD5Context_t* ctx);

//-----------------------------------------------------------------------------
// Purpose: generate pseudo random number from a seed number
// Input  : seed number
// Output : pseudo random number
//-----------------------------------------------------------------------------
unsigned int MD5_PseudoRandom(unsigned int nSeed);