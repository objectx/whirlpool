//
// Created by Masashi Fujita on 2019-03-19.
//
#pragma once

#include <whirlpool.hpp>

#include <iostream>
#include <string>

std::ostream& operator<< (std::ostream& output, const Whirlpool::digest_t& digest);

std::string normalize (const std::string& s);
bool        check (const std::string& actual, const std::string& expected);
