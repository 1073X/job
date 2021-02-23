#pragma once

#include <cstdint>

namespace miu::job {

class signal {
  public:
    static signal* instance();

    ~signal();

    auto value() const { return _value; }

  private:
    signal();

    static void kill(int32_t);

    static void dummy(int32_t);
    static void segv(int32_t);

  private:
    int32_t _value { 0 };
    void (*_segv)(int32_t);
};

}    // namespace miu::job
