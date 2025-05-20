
#include "helper.hpp"

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>

using Catch::Matchers::Equals;

std::string normalize (const std::string &s) {
    std::string result;
    result.reserve (s.size ());
    for (auto const ch : s) {
        if (std::isxdigit (ch)) {
            result.push_back (static_cast<std::string::value_type> (std::toupper (ch)));
        }
    }
    return result;
}

bool check (const std::string &actual, const std::string &expected) {
    return normalize (actual) == normalize (expected);
}

TEST_CASE ("Normalize string (for comparison)", "[normalize]") {
    REQUIRE_THAT (normalize (""), Equals (""));
    REQUIRE_THAT (normalize (" "), Equals (""));
    REQUIRE_THAT (normalize ("   "), Equals (""));
    REQUIRE_THAT (normalize ("   Z"), Equals (""));
    REQUIRE_THAT (normalize ("0 1 2 3 4 5 6 7 8 9 A B C D E F G"), Equals ("0123456789ABCDEF"));
    REQUIRE_THAT (normalize ("01234567\n89ABcDeFG\n"), Equals ("0123456789ABCDEF"));
}

TEST_CASE ("Compare with normalization", "[check]") {
    REQUIRE (check ("", ""));
    REQUIRE (check ("", " "));
    REQUIRE (check ("A", "a"));
    REQUIRE_FALSE (check ("a", "B"));
}
