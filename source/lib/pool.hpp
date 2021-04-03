#pragma once

#include <com/thread_id.hpp>
#include <vector>

#include "task.hpp"

namespace miu::job {

class pool {
  public:
    pool() { _tasks.reserve(com::thread_id::max()); }

    ~pool() { clear(); }

    void add(std::string_view name, int32_t core, time::delta lag, func_type const& func) {
        if (_tasks.size() >= _tasks.capacity()) {
            FATAL_ERROR<std::logic_error>("too many tasks", name);
        }

        for (auto& t : _tasks) {
            if (name == t.name()) {
                FATAL_ERROR<std::logic_error>("duplicated task", name);
            }
        }

        _tasks.emplace_back(name, core, lag, func);
    }

    void start() {
        for (auto& t : _tasks) {
            t.start();
        }
    }

    void clear() {
        for (auto& t : _tasks) {
            t.stop();
        }
        for (auto& t : _tasks) {
            t.join();
        }
        _tasks.clear();
    }

  private:
    std::vector<task> _tasks;
};

}    // namespace miu::job
