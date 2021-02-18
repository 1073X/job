#include <gtest/gtest.h>

#include <iostream>
#include <meta/info.hpp>

#include "job/version.hpp"

TEST(ut_version, version) {
    std::cout << miu::job::version() << std::endl;
    std::cout << miu::job::build_info() << std::endl;

    std::cout << miu::meta::info() << std::endl;
}
