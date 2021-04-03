#include <gmock/gmock.h>

#include <com/thread_id.hpp>
#include <log/log.hpp>
#include <thread>

#include "job/pool.hpp"

using namespace std::chrono_literals;

struct ut_pool : public testing::Test {
    MOCK_METHOD(void, func, (miu::job::status*));

    void SetUp() override {
        using miu::log::severity;
        // miu::log::log::instance()->reset(severity::DEBUG, 1024);
    }

    void TearDown() override {
        // miu::log::log::instance()->dump();
    }

    miu::job::pool pool;
};

TEST_F(ut_pool, exec) {
    pool.add("task0", 1, 100us, std::bind(&ut_pool::func, this, std::placeholders::_1));
    pool.add("task1", 100us, &ut_pool::func, static_cast<ut_pool*>(this));
    pool.add("task2", 2, &ut_pool::func, static_cast<ut_pool*>(this));

    EXPECT_CALL(*this, func(testing::_)).Times(testing::AtLeast(3));

    pool.start();
    std::this_thread::sleep_for(100ms);
    pool.stop();
    pool.clear();
}

TEST_F(ut_pool, duplicated) {
    pool.add("task0", 1, 100us, [](auto) {});
    EXPECT_ANY_THROW(pool.add("task0", 1, 100us, [](auto) {}));
}

TEST_F(ut_pool, too_many) {
    for (auto i = 0; i < miu::com::thread_id::max(); i++) {
        pool.add(std::to_string(i), 1, 100us, [](auto) {});
    }
    EXPECT_ANY_THROW(pool.add("task", 1, 100us, [](auto) {}));
}
