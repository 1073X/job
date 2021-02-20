#pragma once

#include <com/microseconds.hpp>

#include "func_type.hpp"

namespace miu::job {

extern void add(std::string_view name, int32_t core, com::microseconds lag, func_type const&);

template<typename R, typename T>
auto add(std::string_view name, int32_t core, com::microseconds lag, R (T::*f)(status*), T* t) {
    auto func = std::bind(f, t, std::placeholders::_1);
    add(name, core, lag, func);
}

template<typename R, typename T>
auto add(std::string_view name, int32_t core, R (T::*f)(status*), T* t) {
    auto lag = com::microseconds { 0 };
    add(name, core, lag, f, t);
}

extern void start();
extern void stop();

}    // namespace miu::job
