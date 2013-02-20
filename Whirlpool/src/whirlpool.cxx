/*
 * whirlpool.cxx:
 *
 * Author(s): objectx
 */
#include <assert.h>
#include <stdexcept>
#include "whirlpool.h"

#include "whirlpool.inc"

#define ARRAY_SIZE(X_)  (sizeof (X_) / sizeof (*(X_)))

/** Non-zero means using full-size (x8 in size) table for computation.  */
#define WHIRLPOOL_USE_FULL_TABLE        0

/** Non-zero enables runtime exception to notifiy errors.  */
#define	USE_EXCEPTION	0

#if defined (USE_EXCEPTION) && (USE_EXCEPTION != 0)
#   define NOTIFY_ERROR(MSG_)	do { std::runtime_error (MSG_) ; } while (0)
#else
#   define NOTIFY_ERROR(MSG_)	do { assert (false) ; } while (0)
#endif

static const size_t     MAX_ROUND = 10 ;

namespace Whirlpool {

    Digest::Digest (const Digest &src) {
        ::memcpy (digest_, src.digest_, sizeof (digest_)) ;
    }

    Digest::Digest (const uint64_t *digest) {
        for (size_t i = 0 ; i < (sizeof (digest_) / 8) ; ++i) {
            uint64_t    tmp = digest [i] ;
            digest_ [8 * i + 0] = static_cast<unsigned char> (tmp >> 56) ;
            digest_ [8 * i + 1] = static_cast<unsigned char> (tmp >> 48) ;
            digest_ [8 * i + 2] = static_cast<unsigned char> (tmp >> 40) ;
            digest_ [8 * i + 3] = static_cast<unsigned char> (tmp >> 32) ;
            digest_ [8 * i + 4] = static_cast<unsigned char> (tmp >> 24) ;
            digest_ [8 * i + 5] = static_cast<unsigned char> (tmp >> 16) ;
            digest_ [8 * i + 6] = static_cast<unsigned char> (tmp >>  8) ;
            digest_ [8 * i + 7] = static_cast<unsigned char> (tmp >>  0) ;
        }
    }

    uint_fast64_t	Digest::Hash () const {
	uint_fast64_t	result = 0 ;

        for (size_t i = 0 ; i < sizeof (digest_) ; i += 8) {
	    uint_fast64_t	tmp = ((static_cast<uint64_t> (digest_ [i + 0]) << 56) |
				       (static_cast<uint64_t> (digest_ [i + 1]) << 48) |
				       (static_cast<uint64_t> (digest_ [i + 2]) << 40) |
				       (static_cast<uint64_t> (digest_ [i + 3]) << 32) |
				       (static_cast<uint64_t> (digest_ [i + 4]) << 24) |
				       (static_cast<uint64_t> (digest_ [i + 5]) << 16) |
				       (static_cast<uint64_t> (digest_ [i + 6]) <<  8) |
				       (static_cast<uint64_t> (digest_ [i + 7]) <<  0)) ;
            result ^= tmp ;
        }
        return result ;
    }

    Digest &    Digest::Assign (const Digest &src) {
        ::memcpy (digest_, src.digest_, sizeof (digest_)) ;
        return *this ;
    }

    bool        Digest::IsEqual (const Digest &a, const Digest &b) {
        for (size_t i = 0 ; i < sizeof (a.digest_) ; ++i) {
            if (a.digest_ [i] != b.digest_ [i]) {
                return false ;
            }
        }
        return true ;
    }

    bool        Digest::IsNotEqual (const Digest &a, const Digest &b) {
        for (size_t i = 0 ; i < sizeof (a.digest_) ; ++i) {
            if (a.digest_ [i] != b.digest_ [i]) {
                return true ;
            }
        }
        return false ;
    }

    int_fast32_t	Digest::Compare (const Digest &a, const Digest &b) {
        return ::memcmp (a.digest_, b.digest_, sizeof (a.digest_)) ;
    }

    Generator::Generator () {
        Clear () ;
    }

    Generator::Generator (const Generator &src) :
        finalized_ (src.finalized_), remain_ (src.remain_) {
        ::memcpy (digest_, src.digest_, sizeof (digest_)) ;
        ::memcpy (buffer_, src.buffer_, sizeof (buffer_)) ;
        ::memcpy (bitCount_, src.bitCount_, sizeof (bitCount_)) ;
    }

    Generator & Generator::Assign (const Generator &src) {
        finalized_ = src.finalized_ ;
        remain_ = src.remain_ ;
        ::memcpy (digest_, src.digest_, sizeof (digest_)) ;
        ::memcpy (buffer_, src.buffer_, sizeof (buffer_)) ;
        ::memcpy (bitCount_, src.bitCount_, sizeof (bitCount_)) ;
        return *this ;
    }

    Generator & Generator::Clear () {
        finalized_ = false ;
        remain_ = sizeof (buffer_) ;
        ::memset (digest_, 0, sizeof (digest_)) ;
        ::memset (bitCount_, 0, sizeof (bitCount_)) ;
        return *this ;
    }

    Generator & Generator::Update (unsigned char value) {
        if (finalized_) {
	    NOTIFY_ERROR ("Already finalized.") ;
        }
        if (remain_ <= 0) {
            Flush () ;
        }
	uint8_t *	q = &buffer_ [sizeof (buffer_) - remain_] ;
        *q++ = value ;
        --remain_ ;
        AddBitCount (8) ;
        return *this ;
    }
    Generator & Generator::Update (const void *data, size_t size) {
        if (finalized_) {
	    NOTIFY_ERROR ("Already finalized.") ;
        }
        const uint8_t *	p = static_cast<const uint8_t *> (data) ;
        const uint8_t *	p_end = p + size ;

        uint8_t *	q = &buffer_ [sizeof (buffer_) - remain_] ;

        while (p < p_end) {
            if (remain_ <= 0) {
                Flush () ;
                q = &buffer_ [0] ;
            }
            *q++ = *p++ ;
            --remain_ ;
        }
        AddBitCount (8 * size) ;
        return *this ;
    }

#if defined (WHIRLPOOL_USE_FULL_TABLE) && (WHIRLPOOL_USE_FULL_TABLE != 0)
    static inline uint64_t      CIR (size_t n, uint64_t value) {
        return CIR_ [256 * n + (static_cast<int> (value) & 0xFF)] ;
    }
#else

    static inline uint64_t      RotateRight (uint64_t value, size_t count) {
#if defined (_MSC_VER) && (1300 <= _MSC_VER)
        return _rotr64 (value, static_cast<int> (count)) ;
#else
        return ((value >> count) | (value << (64 - count))) ;
#endif
    }

    static inline uint64_t      CIR (size_t n, uint64_t value) {
        uint64_t        result = CIR_ [static_cast<int> (value) & 0xFF] ;
        return RotateRight (result, 8 * n) ;
    }
#endif

    static inline uint64_t      ToUInt64 (const void *data) {
        const unsigned char *   p = static_cast<const unsigned char *> (data) ;
        return ((static_cast<uint64_t> (p [0]) << 56) |
                (static_cast<uint64_t> (p [1]) << 48) |
                (static_cast<uint64_t> (p [2]) << 40) |
                (static_cast<uint64_t> (p [3]) << 32) |
                (static_cast<uint64_t> (p [4]) << 24) |
                (static_cast<uint64_t> (p [5]) << 16) |
                (static_cast<uint64_t> (p [6]) <<  8) |
                (static_cast<uint64_t> (p [7]) <<  0)) ;
    }

    void        Generator::Flush () {
        uint_fast64_t	K0 = digest_ [0] ;
        uint_fast64_t	K1 = digest_ [1] ;
        uint_fast64_t	K2 = digest_ [2] ;
        uint_fast64_t	K3 = digest_ [3] ;
        uint_fast64_t	K4 = digest_ [4] ;
        uint_fast64_t	K5 = digest_ [5] ;
        uint_fast64_t	K6 = digest_ [6] ;
        uint_fast64_t	K7 = digest_ [7] ;

        uint_fast64_t	B0 = ToUInt64 (&buffer_ [8 * 0]) ;
        uint_fast64_t	B1 = ToUInt64 (&buffer_ [8 * 1]) ;
        uint_fast64_t	B2 = ToUInt64 (&buffer_ [8 * 2]) ;
        uint_fast64_t	B3 = ToUInt64 (&buffer_ [8 * 3]) ;
        uint_fast64_t	B4 = ToUInt64 (&buffer_ [8 * 4]) ;
        uint_fast64_t	B5 = ToUInt64 (&buffer_ [8 * 5]) ;
        uint_fast64_t	B6 = ToUInt64 (&buffer_ [8 * 6]) ;
        uint_fast64_t	B7 = ToUInt64 (&buffer_ [8 * 7]) ;

        uint_fast64_t	S0 = B0 ^ K0 ;
        uint_fast64_t	S1 = B1 ^ K1 ;
        uint_fast64_t	S2 = B2 ^ K2 ;
        uint_fast64_t	S3 = B3 ^ K3 ;
        uint_fast64_t	S4 = B4 ^ K4 ;
        uint_fast64_t	S5 = B5 ^ K5 ;
        uint_fast64_t	S6 = B6 ^ K6 ;
        uint_fast64_t	S7 = B7 ^ K7 ;

        for (size_t r = 1 ; r <= MAX_ROUND ; ++r) {
            uint_fast64_t	L0 = K0 ;
            uint_fast64_t	L1 = K1 ;
            uint_fast64_t	L2 = K2 ;
            uint_fast64_t	L3 = K3 ;
            uint_fast64_t	L4 = K4 ;
            uint_fast64_t	L5 = K5 ;
            uint_fast64_t	L6 = K6 ;
            uint_fast64_t	L7 = K7 ;
            // Compute K^r from K^{r - 1}
            K0 = (CIR (0, L0 >> 56) ^ CIR (1, L7 >> 48) ^ CIR (2, L6 >> 40) ^ CIR (3, L5 >> 32) ^
                  CIR (4, L4 >> 24) ^ CIR (5, L3 >> 16) ^ CIR (6, L2 >>  8) ^ CIR (7, L1 >>  0) ^
                  RC_ [r]) ;
            K1 = (CIR (0, L1 >> 56) ^ CIR (1, L0 >> 48) ^ CIR (2, L7 >> 40) ^ CIR (3, L6 >> 32) ^
                  CIR (4, L5 >> 24) ^ CIR (5, L4 >> 16) ^ CIR (6, L3 >>  8) ^ CIR (7, L2 >>  0)) ;
            K2 = (CIR (0, L2 >> 56) ^ CIR (1, L1 >> 48) ^ CIR (2, L0 >> 40) ^ CIR (3, L7 >> 32) ^
                  CIR (4, L6 >> 24) ^ CIR (5, L5 >> 16) ^ CIR (6, L4 >>  8) ^ CIR (7, L3 >>  0)) ;
            K3 = (CIR (0, L3 >> 56) ^ CIR (1, L2 >> 48) ^ CIR (2, L1 >> 40) ^ CIR (3, L0 >> 32) ^
                  CIR (4, L7 >> 24) ^ CIR (5, L6 >> 16) ^ CIR (6, L5 >>  8) ^ CIR (7, L4 >>  0)) ;
            K4 = (CIR (0, L4 >> 56) ^ CIR (1, L3 >> 48) ^ CIR (2, L2 >> 40) ^ CIR (3, L1 >> 32) ^
                  CIR (4, L0 >> 24) ^ CIR (5, L7 >> 16) ^ CIR (6, L6 >>  8) ^ CIR (7, L5 >>  0)) ;
            K5 = (CIR (0, L5 >> 56) ^ CIR (1, L4 >> 48) ^ CIR (2, L3 >> 40) ^ CIR (3, L2 >> 32) ^
                  CIR (4, L1 >> 24) ^ CIR (5, L0 >> 16) ^ CIR (6, L7 >>  8) ^ CIR (7, L6 >>  0)) ;
            K6 = (CIR (0, L6 >> 56) ^ CIR (1, L5 >> 48) ^ CIR (2, L4 >> 40) ^ CIR (3, L3 >> 32) ^
                  CIR (4, L2 >> 24) ^ CIR (5, L1 >> 16) ^ CIR (6, L0 >>  8) ^ CIR (7, L7 >>  0)) ;
            K7 = (CIR (0, L7 >> 56) ^ CIR (1, L6 >> 48) ^ CIR (2, L5 >> 40) ^ CIR (3, L4 >> 32) ^
                  CIR (4, L3 >> 24) ^ CIR (5, L2 >> 16) ^ CIR (6, L1 >>  8) ^ CIR (7, L0 >>  0)) ;

            L0 = S0 ;
            L1 = S1 ;
            L2 = S2 ;
            L3 = S3 ;
            L4 = S4 ;
            L5 = S5 ;
            L6 = S6 ;
            L7 = S7 ;

            S0 = (CIR (0, L0 >> 56) ^ CIR (1, L7 >> 48) ^ CIR (2, L6 >> 40) ^ CIR (3, L5 >> 32) ^
                  CIR (4, L4 >> 24) ^ CIR (5, L3 >> 16) ^ CIR (6, L2 >>  8) ^ CIR (7, L1 >>  0) ^
                  K0) ;
            S1 = (CIR (0, L1 >> 56) ^ CIR (1, L0 >> 48) ^ CIR (2, L7 >> 40) ^ CIR (3, L6 >> 32) ^
                  CIR (4, L5 >> 24) ^ CIR (5, L4 >> 16) ^ CIR (6, L3 >>  8) ^ CIR (7, L2 >>  0) ^
                  K1) ;
            S2 = (CIR (0, L2 >> 56) ^ CIR (1, L1 >> 48) ^ CIR (2, L0 >> 40) ^ CIR (3, L7 >> 32) ^
                  CIR (4, L6 >> 24) ^ CIR (5, L5 >> 16) ^ CIR (6, L4 >>  8) ^ CIR (7, L3 >>  0) ^
                  K2) ;
            S3 = (CIR (0, L3 >> 56) ^ CIR (1, L2 >> 48) ^ CIR (2, L1 >> 40) ^ CIR (3, L0 >> 32) ^
                  CIR (4, L7 >> 24) ^ CIR (5, L6 >> 16) ^ CIR (6, L5 >>  8) ^ CIR (7, L4 >>  0) ^
                  K3) ;
            S4 = (CIR (0, L4 >> 56) ^ CIR (1, L3 >> 48) ^ CIR (2, L2 >> 40) ^ CIR (3, L1 >> 32) ^
                  CIR (4, L0 >> 24) ^ CIR (5, L7 >> 16) ^ CIR (6, L6 >>  8) ^ CIR (7, L5 >>  0) ^
                  K4) ;
            S5 = (CIR (0, L5 >> 56) ^ CIR (1, L4 >> 48) ^ CIR (2, L3 >> 40) ^ CIR (3, L2 >> 32) ^
                  CIR (4, L1 >> 24) ^ CIR (5, L0 >> 16) ^ CIR (6, L7 >>  8) ^ CIR (7, L6 >>  0) ^
                  K5) ;
            S6 = (CIR (0, L6 >> 56) ^ CIR (1, L5 >> 48) ^ CIR (2, L4 >> 40) ^ CIR (3, L3 >> 32) ^
                  CIR (4, L2 >> 24) ^ CIR (5, L1 >> 16) ^ CIR (6, L0 >>  8) ^ CIR (7, L7 >>  0) ^
                  K6) ;
            S7 = (CIR (0, L7 >> 56) ^ CIR (1, L6 >> 48) ^ CIR (2, L5 >> 40) ^ CIR (3, L4 >> 32) ^
                  CIR (4, L3 >> 24) ^ CIR (5, L2 >> 16) ^ CIR (6, L1 >>  8) ^ CIR (7, L0 >>  0) ^
                  K7) ;
        }
        digest_ [0] ^= S0 ^ B0 ;
        digest_ [1] ^= S1 ^ B1 ;
        digest_ [2] ^= S2 ^ B2 ;
        digest_ [3] ^= S3 ^ B3 ;
        digest_ [4] ^= S4 ^ B4 ;
        digest_ [5] ^= S5 ^ B5 ;
        digest_ [6] ^= S6 ^ B6 ;
        digest_ [7] ^= S7 ^ B7 ;
        remain_ = sizeof (buffer_) ;
    }

    void        Generator::AddBitCount (uint64_t value) {
        uint_fast64_t	x = bitCount_ [0] ;
        bitCount_ [0] += value ;
        if (bitCount_ [0] < x) {
            for (size_t i = 1 ; i < ARRAY_SIZE (bitCount_) ; ++i) {
                x = bitCount_ [i] ;
                bitCount_ [i] += 1 ;
                if (x < bitCount_ [i]) {
                    break ;
                }
            }
        }
    }

    Digest      Generator::Finalize () {
        if (! finalized_) {
            if (remain_ <= 0) {
                Flush () ;
            }
            uint8_t *	q = &buffer_ [sizeof (buffer_) - remain_] ;
	    assert (static_cast<size_t> (&buffer_ [sizeof (buffer_)] - q) == remain_) ;
	    *q++ = 0x80 ;
	    --remain_ ;
            ::memset (q, 0, remain_) ;
            if (remain_ < sizeof (bitCount_)) {
                Flush () ;
                ::memset (buffer_, 0, sizeof (buffer_)) ;
            }
            EmbedBitCount () ;
            Flush () ;
            finalized_ = true ;
        }
        return Digest (digest_) ;
    }

    void        Generator::EmbedBitCount () {
        assert (sizeof (bitCount_) <= remain_) ;
        unsigned char * p = &buffer_ [sizeof (buffer_) - sizeof (bitCount_)] ;
        for (int_fast32_t i = ARRAY_SIZE (bitCount_) - 1 ; 0 <= i ; --i) {
            uint_fast64_t    v = bitCount_ [i] ;
            p [0] = static_cast<unsigned char> (v >> 56) ;
            p [1] = static_cast<unsigned char> (v >> 48) ;
            p [2] = static_cast<unsigned char> (v >> 40) ;
            p [3] = static_cast<unsigned char> (v >> 32) ;
            p [4] = static_cast<unsigned char> (v >> 24) ;
            p [5] = static_cast<unsigned char> (v >> 16) ;
            p [6] = static_cast<unsigned char> (v >>  8) ;
            p [7] = static_cast<unsigned char> (v >>  0) ;
            p += 8 ;
        }
    }
}       /* end of [namespace Whirlpool] */
/*
 * [END OF FILE]
 */
