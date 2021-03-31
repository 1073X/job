#pragma once

#include <time/delta.hpp>

#include "func_type.hpp"
#include "utility.hpp"

namespace miu::job {

extern void add(std::string_view name, int32_t core, time::delta lag, func_type const&);

template<typename R, typename T>
auto add(std::string_view name, int32_t core, time::delta lag, R (T::*f)(status*), T* t) {
    auto func = std::bind(f, t, std::placeholders::_1);
    add(name, core, lag, func);
}

template<typename R, typename T>
auto add(std::string_view name, int32_t core, R (T::*f)(status*), T* t) {
    auto lag = time::delta { 0 };
    add(name, core, lag, f, t);
}

template<typename R, typename T>
auto add(std::string_view name, time::delta lag, R (T::*f)(status*), T* t) {
    auto core = get_core();
    add(name, core, lag, f, t);
}

extern void start();
extern void stop();

}    // namespace miu::job
