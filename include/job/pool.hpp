#pragma once

#include <string_view>
#include <time/delta.hpp>
#include <vector>

#include "func_type.hpp"
#include "utility.hpp"

namespace miu::job {

class task;

class pool {
  public:
    pool();
    ~pool();

    void add(std::string_view name, int32_t core, time::delta, func_type);

    auto add(std::string_view name, int32_t core, func_type func) {
        add(name, core, time::delta::min(), std::move(func));
    }

    template<typename R, typename T>
    auto add(std::string_view name, time::delta lag, R (T::*f)(status*), T* t) {
        add(name, get_core(), lag, std::bind(f, t, std::placeholders::_1));
    }

    void start();
    void stop();
    void clear();

  private:
    std::vector<task*> _tasks;
};

}    // namespace miu::job
