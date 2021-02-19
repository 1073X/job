#pragma once

#include <com/microseconds.hpp>
#include <string_view>

namespace miu::job {

class status {
  public:
    status(std::string_view name, int32_t core);

    std::string_view name() const { return _name; }
    auto core() const { return _core; }
    auto heartbeat() const { return _heartbeat; }

    auto lag() const { return _lag; }
    auto set_lag(com::microseconds v) { _lag = v; }

    void kill();
    bool beat();

  private:
    uint32_t _step;
    uint32_t _heartbeat;
    com::microseconds _lag;
    int32_t _core;
    uint32_t _padding[7];
    char _name[16];
};
static_assert(sizeof(status) == CACHE_LINE);

}    // namespace miu::job
