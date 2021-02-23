#include <gtest/gtest.h>

#include <log/log.hpp>

#include "job/utility.hpp"

struct ut_utility : public testing::Test {
    void SetUp() override {
        using miu::log::severity;
        // miu::log::log::instance()->reset(severity::DEBUG, 1024);
    }

    void TearDown() override {
        // miu::log::log::instance()->dump();
    }
};

TEST_F(ut_utility, name) {
    auto name = miu::job::get_name();

    miu::job::set_name("ut_utility");
    EXPECT_EQ("ut_utility", miu::job::get_name());

    miu::job::set_name("loooooooong name");

    miu::job::set_name(name);
}

TEST_F(ut_utility, core) {
    auto core = miu::job::get_core();

    miu::job::set_core(0);
    EXPECT_EQ(0, miu::job::get_core());

    miu::job::set_core(99);

    miu::job::set_core(core);
}
