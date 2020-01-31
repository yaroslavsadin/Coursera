#pragma once
#include <utility>

template<typename F, typename ... Args>
void ApplyToMany(const F& f, Args&&... args) {
    (f(std::forward<Args>(args)) , ...);
}