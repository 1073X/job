#pragma once

#include <log/log.hpp>
#include <thread>

#include "job/status.hpp"

#include "utility.hpp"

namespace miu::job {

class thread {
  public:
    using func_type = std::function<void(status*)>;

  public:
    thread(std::string_view name, int32_t core, com::microseconds lag, func_type const& func)
        : _status { name, core }
        , _func(func) {
        _status.set_lag(lag);
    }

    ~thread() { join(); }

    auto start() {
        if (_thrd.joinable()) {
            FATAL_ERROR(_status.name(), +"has is running.");
        } else if (!_status.is_alive()) {
            FATAL_ERROR(_status.name(), +"has been killed.");
        } else {
            _thrd = std::thread(std::bind(&thread::proc, this));
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

        log::info(+"THRD", get_name(), +"STARTED on core", get_core(), +"LAG =", _status.lag());
        _func(&_status);
        log::info(+"THRD", _status.name(), +"STOPPED");

    } catch (std::exception const& err) {
        log::error(_status.name(), std::string(err.what()));
    } catch (...) {
        log::error(_status.name(), +"terminated with unknown exception");
    }

  private:
    status _status;
    std::thread _thrd;
    func_type _func;
};

}    // namespace miu::job
