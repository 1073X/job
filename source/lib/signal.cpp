
#include "job/signal.hpp"

#include <string.h>    // strsignal

#include <com/system_warn.hpp>
#include <csignal>
#include <log/log.hpp>
#include <meta/info.hpp>

namespace miu::job {

signal* signal::instance() {
    static signal instance;
    return &instance;
}

signal::~signal() {
    SYSTEM_WARN(::strsignal(_value));
    std::signal(SIGSEGV, _segv);    // restore default SIGSEGV handler
}

signal::signal() {
    std::signal(SIGTERM, &signal::kill);
    std::signal(SIGINT, &signal::kill);
    _segv = std::signal(SIGSEGV, &signal::segv);

    log::info(+"capture SIGTERM, SIGINT and SIGSEGV");
}

void signal::kill(int32_t sig) {
    signal::instance()->_value = sig;
}

void signal::dummy(int32_t) {
}

void signal::segv(int32_t sig) {
    kill(sig);
    std::signal(SIGSEGV, &signal::dummy);
}

}    // namespace miu::job
