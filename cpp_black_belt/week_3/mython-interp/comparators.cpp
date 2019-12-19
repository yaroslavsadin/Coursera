#include "comparators.h"
#include "object.h"
#include "object_holder.h"

#include <functional>
#include <optional>
#include <sstream>

using namespace std;

namespace Runtime {

bool Equal(ObjectHolder lhs, ObjectHolder rhs) {
    return *lhs == *rhs;
}

bool Less(ObjectHolder lhs, ObjectHolder rhs) {
    return *lhs < *rhs;
}

} /* namespace Runtime */
