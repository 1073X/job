
#include "utility.hpp"

#include <pthread.h>
#include <string.h>    // strerror

#include <com/system_warn.hpp>
#include <log/log.hpp>

namespace miu::job {

std::string get_name() {
    char name[16] {};

    auto err = pthread_getname_np(pthread_self(), name, sizeof(name));
    if (err) {
        SYSTEM_WARN("get_name", err, strerror(err));
        log::error(+"get_name", err, (const char*)strerror(err));
    }
    return name;
}

void set_name(std::string_view name) {
    auto err = pthread_setname_np(pthread_self(), name.data());
    if (err) {
        SYSTEM_WARN("set_name(", name, ')', err, strerror(err));
        log::error(+"set_name(", name, ')', err, (const char*)strerror(err));
    }
}

int32_t get_core() {
    cpu_set_t cpu_set;
    CPU_ZERO(&cpu_set);
    if (!pthread_getaffinity_np(pthread_self(), sizeof(cpu_set), &cpu_set)) {
        for (auto i = 0; i < CPU_SETSIZE; ++i) {
            if (CPU_ISSET(i, &cpu_set)) {
                return i;
            }
        }
    }

    return -1;
}

void set_core(int32_t core) {
    cpu_set_t cpu_set;
    CPU_ZERO(&cpu_set);
    CPU_SET(core, &cpu_set);
    auto err = pthread_setaffinity_np(pthread_self(), sizeof(cpu_set), &cpu_set);
    if (err) {
        SYSTEM_WARN("set_core(", core, ')', err, strerror(err));
        log::error(+"set_core(", core, ')', err, (const char*)strerror(err));
    }
}

}    // namespace miu::job
