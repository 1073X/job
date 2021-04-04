
#include "job/status.hpp"

#include <cassert>
#include <cstring>    // std::strncpy
#include <log/log.hpp>
#include <thread>

namespace miu::job {

status::status(std::string_view name, int32_t core)
    : _alive(true)
    , _heartbeat(1)
    , _lag { 0 }
    , _core(core) {
    assert(name.size() < sizeof(_name) && "status name overflow");
    std::strncpy(_name, name.data(), sizeof(_name));
}

bool status::beat() {
    _heartbeat++;
    std::this_thread::sleep_for(_lag);
    return is_alive();
}

}    // namespace miu::job
