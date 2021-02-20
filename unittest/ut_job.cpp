#include <gmock/gmock.h>

#include <log/log.hpp>
#include <thread>

#include "job/job.hpp"

using namespace std::chrono_literals;

struct ut_job : public testing::Test {
    MOCK_METHOD(void, func, (miu::job::status*));

    void SetUp() override {
        using miu::log::severity;
        miu::log::log::instance()->reset(severity::DEBUG, 1024);
    }

    void TearDown() override { miu::log::log::instance()->dump(); }
};

TEST_F(ut_job, test) {
    using std::placeholders::_1;
    miu::job::add("task0", 1, &ut_job::func, static_cast<ut_job*>(this));
    miu::job::add("task1", 2, 100us, &ut_job::func, static_cast<ut_job*>(this));

    EXPECT_CALL(*this, func(testing::_)).Times(2);

    miu::job::start();
    std::this_thread::sleep_for(100us);
    miu::job::stop();
}
