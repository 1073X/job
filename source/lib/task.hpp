#pragma once

#include <asp/asp.hpp>
#include <functional>
#include <log/log.hpp>
#include <thread>
#include <utility>

#include "job/func_type.hpp"
#include "job/status.hpp"
#include "job/utility.hpp"

namespace miu::job {

class task {
  public:
    task(std::string_view name, int32_t core, time::delta lag, func_type func)
        : _status { name, core }
        , _func(std::move(func)) {
        _status.set_lag(lag);

        asp::read({ +"_COM_", +"job", name, +"hb" }, &status::heartbeat, &_status);
        asp::read({ +"_COM_", +"job", name, +"core" }, &status::core, &_status);
        asp::read({ +"_COM_", +"job", name, +"lag" }, &status::lag, &_status);
        asp::read({ +"_COM_", +"job", name, +"alive" }, &status::is_alive, &_status);
    }

    task(task const&) = delete;
    auto operator=(task const&) = delete;

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

    void stop() { _status.kill(); }

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

        log::debug(+"job BEG", get_name(), +"CORE", get_core(), +"LAG", _status.lag());
        _func(&_status);
        log::debug(+"job END", _status.name());

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
