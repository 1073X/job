
#include "job/status.hpp"

#include <cassert>
#include <cstring>    // std::strncpy
#include <thread>

namespace miu::job {

status::status(std::string_view name, int32_t core)
    : _step(2)
    , _heartbeat(1)
    , _lag { 0 }
    , _core(core) {
    assert(name.size() < sizeof(_name) && "status name overflow");
    std::strncpy(_name, name.data(), sizeof(_name));
}

void status::kill() {
    _step = 1;
}

bool status::beat() {
    _heartbeat += _step;
    std::this_thread::sleep_for(_lag);
    return _heartbeat & 1;
}

}    // namespace miu::job
