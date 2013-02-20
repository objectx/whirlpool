/*
 * whirlpool.h: Whirlpool message digest.
 *
 * Author(s): objectx
 *
 * $Id$
 */

#ifndef whirlpool_h__fbe744c0_426e_4cbb_ad94_6319cc686491
#define whirlpool_h__fbe744c0_426e_4cbb_ad94_6319cc686491       1

#if defined (_MSC_VER) && (1300 <= _MSC_VER)
#pragma once
#endif

#include <sys/types.h>
#include <cassert>
#include <string>
#include <functional>

namespace Whirlpool {
#if defined (_MSC_VER) && (1300 <= _MSC_VER)
    typedef unsigned __int64    uint64_t ;
#else
    typedef unsigned long       uint64_t ;
#endif
    /// <summary>
    /// Represents computed digest.
    /// </summary>
    class Digest {
        friend class Generator ;
    private:
        unsigned char   digest_ [64] ;
    public:
        // The copy constructor.
        Digest (const Digest &src) ;
        size_t  Size () const {
            return sizeof (digest_) ;
        }
        unsigned char   At (size_t index) const {
            return digest_ [index] ;
        }
        unsigned char   operator [] (size_t index) const {
            return digest_ [index] ;
        }
        const unsigned char *   begin () const {
            return &digest_ [0] ;
        }
        const unsigned char *   end () const {
            return begin () + sizeof (digest_) ;
        }
        size_t  Hash () const ;
        Digest &        Assign (const Digest &src) ;
        Digest &        operator = (const Digest &src) {
            return Assign (src) ;
        }
    public:
        static bool     IsEqual (const Digest &a, const Digest &b) ;
        static bool     IsNotEqual (const Digest &a, const Digest &b) ;
        static int      Compare (const Digest &a, const Digest &b) ;
    private:
        Digest (const uint64_t *digest) ;
    } ;


    /// <summary>
    /// The whirlpool message digest generator.
    /// </summary>
    class Generator {
    private:
        uint64_t        digest_ [8] ;
        bool            finalized_ ;
        int             remain_ ;
        unsigned char   buffer_ [64] ;
        uint64_t        bitCount_ [4] ; // 256bits counter.
    public:
        /// <summary>
        /// The default constructor.
        /// </summary>
        Generator () ;
        /// <summary>
        /// The copy constructor.
        /// </summary>
        /// <param name="src">The source to copy</param>
        Generator (const Generator &src) ;
        /// <summary>
        /// Assigns <paramref name="src"/>'s state to self.
        /// </summary>
        /// <param name="src"></param>
        /// <returns>Generator&</returns>
        Generator &     Assign (const Generator &src) ;
        Generator &     operator = (const Generator &src) {
            return Assign (src) ;
        }
        /// <summary>
        /// Clears internal state.
        /// </summary>
        /// <returns>Myself</returns>
        Generator &     Clear () ;
        /// <summary>
        /// Updates state with <paramref name="value"/> (1 byte).
        /// </summary>
        /// <param name="value">Value to examine</param>
        /// <returns>Myself</returns>
        Generator &     Update (unsigned char value) ;
        /// <summary>
        /// Updates state with the sequence of <paramref name="data"/> [0..<paramref name="size"/> - 1].
        /// </summary>
        /// <param name="data">Start of the input</param>
        /// <param name="size"># of bytes for input</param>
        /// <returns>Myself</returns>
        Generator &     Update (const void *data, size_t size) ;
        /// <summary>
        /// Finalizes internal state and computes digest.
        /// Once finalized, the call to <see cref="Finalize()"/> always return same digest value.
        /// </summary>
        /// <returns>Computed digest.</returns>
        Digest  Finalize () ;
    private:
        void    Flush () ;
        /// <summary>
        /// Increments bitCount_ by <paramref name="value"/>.
        /// </summary>
        void    AddBitCount (uint64_t value) ;
        void    EmbedBitCount () ;
    } ;

    /// <summary>
    /// Convenience function for computing digest in 1-pass.
    /// </summary>
    /// <param name="data">Data to examine</param>
    /// <param name="size"># of bytes for input</param>
    /// <returns>Computed digest</returns>
    inline Digest       ComputeDigest (const void *data, size_t size) {
        return Generator ().Update (data, size).Finalize () ;
    }

}       /* end of [namespace Whirlpool] */

// Operators.

inline bool     operator == (const Whirlpool::Digest &a, const Whirlpool::Digest &b) {
    return Whirlpool::Digest::IsEqual (a, b) ;
}

inline bool     operator != (const Whirlpool::Digest &a, const Whirlpool::Digest &b) {
    return Whirlpool::Digest::IsNotEqual (a, b) ;
}

inline int      operator < (const Whirlpool::Digest &a, const Whirlpool::Digest &b) {
    return Whirlpool::Digest::Compare (a, b) < 0 ;
}

inline int      operator <= (const Whirlpool::Digest &a, const Whirlpool::Digest &b) {
    return Whirlpool::Digest::Compare (a, b) <= 0 ;
}

inline int      operator > (const Whirlpool::Digest &a, const Whirlpool::Digest &b) {
    return Whirlpool::Digest::Compare (a, b) > 0 ;
}

inline int      operator >= (const Whirlpool::Digest &a, const Whirlpool::Digest &b) {
    return Whirlpool::Digest::Compare (a, b) >= 0 ;
}

#endif  /* whirlpool_h__fbe744c0_426e_4cbb_ad94_6319cc686491 */

/*
 * $LastChangedRevision$
 * $LastChangedBy$
 * $HeadURL$
 */
