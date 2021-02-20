#pragma once

#include <functional>

#include "status.hpp"

namespace miu::job {

using func_type = std::function<void(status*)>;

}
