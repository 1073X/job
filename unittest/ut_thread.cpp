#include <gtest/gtest.h>

#include <log/log.hpp>

#include "source/lib/thread.hpp"

using namespace std::chrono_literals;
using miu::job::thread;

struct ut_thread : public testing::Test {
    void SetUp() override {
        using miu::log::severity;
        miu::log::log::instance()->reset(severity::DEBUG, 1024);
    }

    void TearDown() override { miu::log::log::instance()->dump(); }
};

TEST_F(ut_thread, start) {
    bool called = false;
    thread thrd("ut_thread", 2, 100us, [&called](auto st) {
        EXPECT_EQ("ut_thread", miu::job::get_name());
        EXPECT_EQ(2, miu::job::get_core());
        called = true;
        while (st->beat()) {
        }
    });

    thrd.start();
    thrd.join();
    EXPECT_TRUE(called);
}

TEST_F(ut_thread, join) {
    thread thrd("ut_thread", 2, 0, [](auto) {});
    EXPECT_NO_THROW(thrd.join());
}

TEST_F(ut_thread, repetition) {
    thread thrd("ut_thread", 2, 100us, [](auto) {});

    thrd.start();
    EXPECT_ANY_THROW(thrd.start());
}

TEST_F(ut_thread, restart) {
    thread thrd("ut_thread", 2, 100us, [](auto st) {
        while (st->beat()) {
        }
    });

    thrd.start();
    thrd.join();
    EXPECT_ANY_THROW(thrd.start());
}

TEST_F(ut_thread, exception) {
    thread thrd("ut_thread", 2, 100us, [](auto) { FATAL_ERROR<std::runtime_error>("exception"); });

    thrd.start();
    thrd.join();
}

TEST_F(ut_thread, unknown_exp) {
    thread thrd("ut_thread", 2, 100us, [](auto) { throw "exception"; });

    thrd.start();
    thrd.join();
}

