
#include "job/pool.hpp"

#include <com/thread_id.hpp>

#include "task.hpp"

namespace miu::job {

pool::pool() {
    _tasks.reserve(com::thread_id::max());
}

pool::~pool() {
    clear();
}

void pool::add(std::string_view name, int32_t core, time::delta lag, func_type func) {
    if (_tasks.size() >= _tasks.capacity()) {
        FATAL_ERROR<std::logic_error>("too many tasks", name);
    }

    for (auto& t : _tasks) {
        if (name == t->name()) {
            FATAL_ERROR<std::logic_error>("duplicated task", name);
        }
    }

    _tasks.emplace_back(new task { name, core, lag, std::move(func) });
}

void pool::start() {
    for (auto& t : _tasks) {
        t->start();
    }
}

void pool::stop() {
    for (auto& t : _tasks) {
        t->stop();
    }
    for (auto& t : _tasks) {
        t->join();
    }
}

void pool::clear() {
    for (auto& t : _tasks) {
        delete t;
    }
    _tasks.clear();
}

}    // namespace miu::job
