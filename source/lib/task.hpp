#pragma once

#include <asp/asp.hpp>
#include <functional>
#include <log/log.hpp>
#include <thread>

#include "job/func_type.hpp"
#include "job/status.hpp"

#include "utility.hpp"

namespace miu::job {

class task {
  public:
    task(std::string_view name, int32_t core, com::microseconds lag, func_type const& func)
        : _status { name, core }
        , _func(func) {
        _status.set_lag(lag);

        asp::read({ +"job", name, +"heartbeat" }, &status::heartbeat, &_status);
        asp::read({ +"job", name, +"core" }, &status::core, &_status);
        asp::read({ +"job", name, +"lag" }, &status::lag, &_status);
        asp::read({ +"job", name, +"alive" }, &status::is_alive, &_status);
    }

    task(task&& another)
        : _status(std::move(another._status))
        , _thrd(std::move(another._thrd))
        , _func(std::move(another._func)) {}

    task& operator=(task&& another) {
        std::swap(_status, another._status);
        std::swap(_thrd, another._thrd);
        std::swap(_func, another._func);
        return *this;
    }

    ~task() { join(); }

    auto name() const { return _status.name(); }

    auto start() {
        if (_thrd.joinable()) {
            FATAL_ERROR(_status.name(), +"has is running.");
        } else if (!_status.is_alive()) {
            FATAL_ERROR(_status.name(), +"has been killed.");
        } else {
            _thrd = std::thread(std::bind(&task::proc, this));
        }
    }

    auto stop() { _status.kill(); }

    void join() {
        stop();
        if (_thrd.joinable()) {
            _thrd.join();
        }
    }

  private:
    void proc() try {
        set_name(_status.name());
        set_core(_status.core());

        log::debug(+"job", get_name(), +"STARTED on core", get_core(), +"LAG =", _status.lag());
        _func(&_status);
        log::debug(+"job", _status.name(), +"STOPPED");

    } catch (std::exception const& err) {
        log::error(+"job", _status.name(), std::string(err.what()));
    } catch (...) {
        log::error(+"job", _status.name(), +"terminated with unknown exception");
    }

  private:
    status _status;
    std::thread _thrd;
    func_type _func;
};

}    // namespace miu::job
