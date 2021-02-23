#pragma once

#include <string>

namespace miu::job {

std::string get_name();
void set_name(std::string_view);

int32_t get_core();
void set_core(int32_t);

}    // namespace miu::job
