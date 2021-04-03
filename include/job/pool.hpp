#pragma once

#include <string_view>
#include <time/delta.hpp>
#include <vector>

#include "func_type.hpp"

namespace miu::job {

class task;

class pool {
  public:
    pool();
    ~pool();

    void add(std::string_view name, int32_t core, time::delta, func_type);

    void start();
    void stop();
    void clear();

  private:
    std::vector<task*> _tasks;
};

}    // namespace miu::job
