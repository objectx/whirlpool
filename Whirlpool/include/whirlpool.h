/*
 * whirlpool.h: Whirlpool message digest.
 *
 * Author(s): objectx
 */
#ifndef whirlpool_h__b4f60d2740a89d8c991cdeae0be6465d
#define	whirlpool_h__b4f60d2740a89d8c991cdeae0be6465d	1

#if defined (_MSC_VER) && (1300 <= _MSC_VER)
#pragma once
#endif

#include <sys/types.h>
#include <stdint.h>
#include <string.h>

namespace Whirlpool {

    /** Represents computed digest. */
    class Digest {
        friend class Generator ;
    private:
        uint8_t	digest_ [64] ;
    public:
        // The copy constructor.
        Digest (const Digest &src) ;
        size_t	Size () const {
            return sizeof (digest_) ;
        }
        uint_fast8_t	At (size_t index) const {
            return digest_ [index] ;
        }
        uint_fast8_t	operator [] (size_t index) const {
            return digest_ [index] ;
        }
        const uint8_t *	begin () const {
            return &digest_ [0] ;
        }
        const uint8_t *	end () const {
            return begin () + sizeof (digest_) ;
        }
        uint_fast64_t	Hash () const ;
        Digest &        Assign (const Digest &src) ;
        Digest &        operator = (const Digest &src) {
            return Assign (src) ;
        }
    public:
        static bool	IsEqual (const Digest &a, const Digest &b) ;
        static bool	IsNotEqual (const Digest &a, const Digest &b) ;
        static int_fast32_t	Compare (const Digest &a, const Digest &b) ;
    private:
        Digest (const uint64_t *digest) ;
    } ;

    /** The whirlpool message digest generator.  */
    class Generator {
    private:
        uint64_t	digest_ [8] ;
        bool		finalized_ ;
        int32_t		remain_ ;
	uint8_t		buffer_ [64] ;
        uint64_t	bitCount_ [4] ; // 256bits counter.
    public:
	/** The default constructor.  */
	Generator () ;
	/**
	 * The copy constructor.
	 *
	 * @param src    The source to copy
	 */
        Generator (const Generator &src) ;
	/**
	 * Assigns SRC's state to self.
	 *
	 * @param src    The source to assign
	 *
	 * @return *this
	 */
        Generator &     Assign (const Generator &src) ;
        Generator &     operator = (const Generator &src) {
            return Assign (src) ;
        }
	/**
	 * Clears internal state.
	 *
	 * @return *this
	 */
	Generator &     Clear () ;
	/**
	 * Updates state with VALUE (1 byte).
	 *
	 * @param value Value to examine
	 *
	 * @return *this
	 */
	Generator &     Update (unsigned char value) ;
	/**
	 * Updates state with the sequence of DATA [0..SIZE - 1].
	 *
	 * @param data Start of the input
	 * @param size # of bytes for input
	 *
	 * @return *this
	 */
	Generator &     Update (const void *data, size_t size) ;
	/**
	 * Finalizes internal state and computes digest.
	 *
	 * @return Computed digest
	 */
	Digest  Finalize () ;
    private:
        void    Flush () ;
	/**
	 * Increments bitCount_ by VALUE.
	 *
	 * @param value  Value for increment
	 */
	void    AddBitCount (uint64_t value) ;
        void    EmbedBitCount () ;
    } ;

    /**
     * Convenience function for computing digest in 1-pass.
     *
     * @param data Data to examine
     * @param size # of bytes for input
     *
     * @return Computed digest
     */
    inline Digest       ComputeDigest (const void *data, size_t size) {
        return Generator ().Update (data, size).Finalize () ;
    }

}       /* end of [namespace Whirlpool] */

// Operators.

inline bool	operator == (const Whirlpool::Digest &a, const Whirlpool::Digest &b) {
    return Whirlpool::Digest::IsEqual (a, b) ;
}

inline bool	operator != (const Whirlpool::Digest &a, const Whirlpool::Digest &b) {
    return Whirlpool::Digest::IsNotEqual (a, b) ;
}

inline int_fast32_t	operator < (const Whirlpool::Digest &a, const Whirlpool::Digest &b) {
    return Whirlpool::Digest::Compare (a, b) < 0 ;
}

inline int_fast32_t	operator <= (const Whirlpool::Digest &a, const Whirlpool::Digest &b) {
    return Whirlpool::Digest::Compare (a, b) <= 0 ;
}

inline int_fast32_t	operator > (const Whirlpool::Digest &a, const Whirlpool::Digest &b) {
    return Whirlpool::Digest::Compare (a, b) > 0 ;
}

inline int_fast32_t	operator >= (const Whirlpool::Digest &a, const Whirlpool::Digest &b) {
    return Whirlpool::Digest::Compare (a, b) >= 0 ;
}

#endif  /* whirlpool_h__b4f60d2740a89d8c991cdeae0be6465d */
/*
 * [END OF FILE]
 */
