
#include "helper.hpp"

#include <iomanip>

std::ostream &      operator << (std::ostream &output, const Whirlpool::digest_t &digest) {
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

std::string normalize (const std::string &s) {
    std::string result ;
    result.reserve (s.size ()) ;
    for (auto ch : s) {
        if (std::isxdigit (ch)) {
            result.push_back (std::toupper (ch)) ;
        }
    }
    return result ;
}

bool check (const std::string &actual, const std::string &expected) {
    return normalize (actual) == normalize (expected) ;
}
