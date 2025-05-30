/*
 * whirlpool.h: Whirlpool message digest.
 *
 * Copyright (c) 2016 Masashi Fujita
 */
#pragma once

#include <array>
#include <cctype>
#include <cstddef>
#include <cstdint>

namespace Whirlpool {

    using digest_t = std::array<uint8_t, 64>;

    /** The whirlpool message digest generator.  */
    class Generator {
        std::array<uint64_t, 8> digest_ {};
        bool                    finalized_ {false};
        int32_t                 remain_ {sizeof (buffer_)};
        std::array<uint8_t, 64> buffer_ {};
        std::array<uint64_t, 4> bitCount_ {};  // 256bits counter.
    public:
        ~Generator () noexcept = default;
        /** The default constructor.  */
        Generator () { clear (); }

        /**
         * The copy constructor.
         *
         * @param src    The source to copy
         */
        Generator (const Generator &src) noexcept = default;

        auto operator= (const Generator &src) noexcept -> Generator & = default;

        /**
         * Clears internal state.
         *
         * @return *this
         */
        auto clear () noexcept -> Generator & {
            finalized_ = false;
            remain_    = sizeof (buffer_);
            digest_.fill (0);
            bitCount_.fill (0);
            return *this;
        }

        /**
         * Updates state with VALUE (1 byte).
         *
         * @param value Value to examine
         *
         * @return *this
         */
        auto update (unsigned char value) -> Generator &;

        /**
         * Updates state with the sequence of DATA [0..SIZE - 1].
         *
         * @param data Start of the input
         * @param size # of bytes for input
         *
         * @return *this
         */
        auto update (const void *data, size_t size) -> Generator &;

        /**
         * Finalizes internal state and computes digest.
         *
         * @return Computed digest
         */
        auto finalize () noexcept -> digest_t;

    private:
        void flush () noexcept;

        /**
         * Increments bitCount_ by VALUE.
         *
         * @param value  Value for increment
         */
        void add_bit_count (uint64_t value) noexcept;
        void embed_bit_count () noexcept;
    };

    /**
     * Convenience function for computing digest in 1-pass.
     *
     * @param data Data to examine
     * @param size # of bytes for input
     *
     * @return Computed digest
     */
    [[nodiscard]]
    inline auto compute_digest (const void *data, size_t size) -> digest_t {
        return Generator ().update (data, size).finalize ();
    }
}  // namespace Whirlpool
