
#include "job/job.hpp"

#include "pool.hpp"

namespace miu::job {

pool g_pool;

void add(std::string_view name, int32_t core, time::delta lag, func_type const& func) {
    g_pool.add(name, core, lag, func);
}

void start() {
    g_pool.start();
}

void stop() {
    g_pool.stop();
}

}    // namespace miu::job
