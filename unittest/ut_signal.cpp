#include <gtest/gtest.h>

#include "source/lib/signal.hpp"

TEST(ut_signal, init) {
    auto signal = miu::job::signal::instance();

    EXPECT_EQ(0, signal->value());
}

TEST(ut_signal, TERM) {
    auto signal = miu::job::signal::instance();

    std::raise(SIGTERM);
    EXPECT_EQ(SIGTERM, signal->value());
}

TEST(ut_signal, INT) {
    auto signal = miu::job::signal::instance();

    std::raise(SIGINT);
    EXPECT_EQ(SIGINT, signal->value());
}

TEST(ut_signal, SEGV) {
    auto signal = miu::job::signal::instance();

    std::raise(SIGSEGV);
    EXPECT_EQ(SIGSEGV, signal->value());
}
