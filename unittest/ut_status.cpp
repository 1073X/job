#include <gtest/gtest.h>

#include <com/datetime.hpp>

#include "job/status.hpp"

using namespace std::chrono_literals;

TEST(ut_status, init) {
    miu::job::status status { "name", 1 };
    EXPECT_EQ("name", status.name());
    EXPECT_EQ(1, status.core());
}

TEST(ut_status, beat) {
    miu::job::status status { "name", 1 };
    EXPECT_EQ(1, status.heartbeat());

    EXPECT_TRUE(status.beat());
    EXPECT_EQ(3, status.heartbeat());
}

TEST(ut_status, kill) {
    miu::job::status status { "name", 1 };

    status.kill();
    EXPECT_FALSE(status.beat());
    EXPECT_TRUE(!status.is_alive());
}

TEST(ut_status, lag) {
    miu::job::status status { "name", 1 };

    EXPECT_EQ(0, status.lag().count());

    status.set_lag(200us);

    auto before = miu::com::datetime::now();
    status.beat();
    EXPECT_LE(200us, miu::com::datetime::now() - before);
}
