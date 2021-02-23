#include <gtest/gtest.h>

#include <asp/asp.hpp>
#include <log/log.hpp>

#include "source/lib/task.hpp"

using namespace std::chrono_literals;

struct ut_task : public testing::Test {
    void SetUp() override {
        using miu::log::severity;
        miu::log::reset(severity::DEBUG, 1024);
    }

    void TearDown() override {
        miu::asp::reset();
        miu::log::dump();
    }
};

TEST_F(ut_task, start) {
    bool called = false;
    miu::job::task task("ut_task", 2, 100us, [&called](auto st) {
        EXPECT_EQ("ut_task", miu::job::get_name());
        EXPECT_EQ(2, miu::job::get_core());
        called = true;
        while (st->beat()) {
        }
    });

    task.start();
    task.join();
    EXPECT_TRUE(called);
}

TEST_F(ut_task, join) {
    miu::job::task task("ut_task", 2, 0, [](auto) {});
    EXPECT_NO_THROW(task.join());
}

TEST_F(ut_task, repetition) {
    miu::job::task task("ut_task", 2, 100us, [](auto) {});

    task.start();
    EXPECT_ANY_THROW(task.start());
}

TEST_F(ut_task, restart) {
    miu::job::task task("ut_task", 2, 100us, [](auto st) {
        while (st->beat()) {
        }
    });

    task.start();
    task.join();
    EXPECT_ANY_THROW(task.start());
}

TEST_F(ut_task, exception) {
    miu::job::task task(
        "ut_task", 2, 100us, [](auto) { FATAL_ERROR<std::runtime_error>("exception"); });

    task.start();
    task.join();
}

TEST_F(ut_task, unknown_exp) {
    miu::job::task task("ut_task", 2, 100us, [](auto) { throw "exception"; });

    task.start();
    task.join();
}

TEST_F(ut_task, asps) {
    miu::job::task task("ut_task", 2, 100us, [](auto) {});
    miu::asp::reset("ut_task");
}
