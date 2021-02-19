
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

bool status::is_alive() const {
    return _heartbeat & 1;
}

void status::kill() {
    _step = 1;    // switch heartbeat to even number
}

bool status::beat() {
    _heartbeat += _step;
    std::this_thread::sleep_for(_lag);
    return is_alive();
}

}    // namespace miu::job
