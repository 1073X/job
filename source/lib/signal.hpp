#pragma once

#include <string.h>    // strsignal

#include <com/system_warn.hpp>
#include <csignal>
#include <log/log.hpp>
#include <meta/info.hpp>

namespace miu::job {

class signal {
  public:
    static auto instance() {
        static signal instance;
        return &instance;
    }

    ~signal() {
        SYSTEM_WARN(::strsignal(_value));
        std::signal(SIGSEGV, _segv);    // restore default SIGSEGV handler
    }

    auto value() const { return _value; }

  private:
    signal() {
        std::signal(SIGTERM, &signal::kill);
        std::signal(SIGINT, &signal::kill);
        _segv = std::signal(SIGSEGV, &signal::segv);

        log::info(+"capture SIGTERM, SIGINT and SIGSEGV");
    }

    static void kill(int32_t sig) { signal::instance()->_value = sig; }

    static void dummy(int32_t) {}
    static void segv(int32_t sig) {
        kill(sig);
        std::signal(SIGSEGV, &signal::dummy);
    }

  private:
    int32_t _value { 0 };
    void (*_segv)(int32_t);
};

}    // namespace miu::job
