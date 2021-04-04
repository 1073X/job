#pragma once

#include <string_view>
#include <time/delta.hpp>

namespace miu::job {

class status {
  public:
    status(std::string_view name, int32_t core);

    std::string_view name() const { return _name; }
    auto core() const { return _core; }
    auto heartbeat() const { return _heartbeat; }

    bool is_alive() const { return _alive; }
    auto kill() { _alive = false; }

    auto lag() const { return _lag; }
    auto set_lag(time::delta v) { _lag = v; }

    bool beat();

  private:
    bool _alive;
    uint32_t _heartbeat;
    time::delta _lag;
    int32_t _core;
    uint32_t _padding[8];
    char _name[16];
};
static_assert(sizeof(status) == CACHE_LINE);

}    // namespace miu::job
