/*
 * main.cxx:
 *
 * Author(s): objectx
 *
 */

#include "common.h"
#include <whirlpool.h>

namespace {

    std::ostream &      operator << (std::ostream &output, const Whirlpool::Digest &digest) {
        const std::ios::fmtflags        mask = std::ios::basefield | std::ios::uppercase ;
        std::ios::fmtflags      f = output.setf (std::ios::hex | std::ios::uppercase, mask) ;
        char    fill = output.fill ('0') ;
        for (size_t i = 0 ; i < 2 ; ++i) {
            output <<
                std::setw (2) << static_cast<unsigned int> (digest [32 * i +  0]) <<
                std::setw (2) << static_cast<unsigned int> (digest [32 * i +  1]) <<
                std::setw (2) << static_cast<unsigned int> (digest [32 * i +  2]) <<
                std::setw (2) << static_cast<unsigned int> (digest [32 * i +  3]) <<
                std::setw (2) << static_cast<unsigned int> (digest [32 * i +  4]) <<
                std::setw (2) << static_cast<unsigned int> (digest [32 * i +  5]) <<
                std::setw (2) << static_cast<unsigned int> (digest [32 * i +  6]) <<
                std::setw (2) << static_cast<unsigned int> (digest [32 * i +  7]) << ' ' <<
                std::setw (2) << static_cast<unsigned int> (digest [32 * i +  8]) <<
                std::setw (2) << static_cast<unsigned int> (digest [32 * i +  9]) <<
                std::setw (2) << static_cast<unsigned int> (digest [32 * i + 10]) <<
                std::setw (2) << static_cast<unsigned int> (digest [32 * i + 11]) <<
                std::setw (2) << static_cast<unsigned int> (digest [32 * i + 12]) <<
                std::setw (2) << static_cast<unsigned int> (digest [32 * i + 13]) <<
                std::setw (2) << static_cast<unsigned int> (digest [32 * i + 14]) <<
                std::setw (2) << static_cast<unsigned int> (digest [32 * i + 15]) << ' ' <<
                std::setw (2) << static_cast<unsigned int> (digest [32 * i + 16]) <<
                std::setw (2) << static_cast<unsigned int> (digest [32 * i + 17]) <<
                std::setw (2) << static_cast<unsigned int> (digest [32 * i + 18]) <<
                std::setw (2) << static_cast<unsigned int> (digest [32 * i + 19]) <<
                std::setw (2) << static_cast<unsigned int> (digest [32 * i + 20]) <<
                std::setw (2) << static_cast<unsigned int> (digest [32 * i + 21]) <<
                std::setw (2) << static_cast<unsigned int> (digest [32 * i + 22]) <<
                std::setw (2) << static_cast<unsigned int> (digest [32 * i + 23]) << ' ' <<
                std::setw (2) << static_cast<unsigned int> (digest [32 * i + 24]) <<
                std::setw (2) << static_cast<unsigned int> (digest [32 * i + 25]) <<
                std::setw (2) << static_cast<unsigned int> (digest [32 * i + 26]) <<
                std::setw (2) << static_cast<unsigned int> (digest [32 * i + 27]) <<
                std::setw (2) << static_cast<unsigned int> (digest [32 * i + 28]) <<
                std::setw (2) << static_cast<unsigned int> (digest [32 * i + 29]) <<
                std::setw (2) << static_cast<unsigned int> (digest [32 * i + 30]) <<
                std::setw (2) << static_cast<unsigned int> (digest [32 * i + 31]) << std::endl ;
        }
        output.fill (fill) ;
        output.setf (f, mask) ;
        return output ;
    }
}


static void     tester (const std::string &teststr, const std::string &message) {

    std::cout << message << std::endl ;
    std::cout << Whirlpool::ComputeDigest (teststr.c_str (), teststr.size ()) << std::endl ;
}

int     main (int argc, char **argv) {
    tester (std::string (),
	    "In this example the data string is the empty string, i.e. the string of length zero.") ;
    tester ("a",
	    "In this example the data string consists of a single byte, namely the ASCII-coded version of the letter 'a'.") ;
    tester ("abc",
	    "In this example the data string is the 3-byte string consisting of the ASCII-coded version of 'abc'.") ;
    tester ("message digest",
            "In this example the data-string is the 14-byte string consisting of the ASCII-coded version of 'message digest'.") ;
    tester ("abcdefghijklmnopqrstuvwxyz",
            "In this example the data-string is the 26-byte string consisting of the ASCII-coded version of 'abcdefghijklmnopqrstuvwxyz'.") ;
    tester ("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789",
            "In this example the data-string is the 62-byte string consisting of the ASCII-coded version of 'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789'.") ;
    tester ("12345678901234567890123456789012345678901234567890123456789012345678901234567890",
            "In this example the data-string is the 80-byte string consisting of the ASCII-coded version of eight repetitions of '1234567890'.") ;
    tester ("abcdbcdecdefdefgefghfghighijhijk",
            "In this example the data-string is the 32-byte string consisting of the ASCII-coded version of 'abcdbcdecdefdefgefghfghighijhijk'.") ;
    tester (std::string (1000000, 'a'), "In this example the data-string is the 1000000-byte string consisting of the ASCII-coded version of 'a' repeated 10^6 times.") ;
    return 0 ;
}
