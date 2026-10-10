#pragma once

#if __has_include(<UnitTest++/UnitTest++.h>)
#  include <UnitTest++/UnitTest++.h>
#elif __has_include(<unittest++/UnitTest++.h>)
#  include <unittest++/UnitTest++.h>
#else
#  include <UnitTest++.h>
#endif
#include <string>

#define CHECK_STR(expected, actual) CHECK_EQUAL(std::string(expected), (actual))
