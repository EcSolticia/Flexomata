#pragma once
#include <functional>

namespace Flexomata {
    namespace Types {
        typedef std::function<size_t(size_t, size_t)> RuleFunc;
    }
}