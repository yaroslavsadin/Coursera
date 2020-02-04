#include <vector>
#include <iostream>
#include <random>
#include <stdexcept>
#include <cassert>

static std::random_device rd;
static std::mt19937 gen(rd());

struct Train {
    mutable size_t current;
    std::vector<bool> buf;

    Train(size_t num) : buf(num), current(0) {
        if(!num) throw std::runtime_error("num == 0");
        std::uniform_int_distribution<> dis(0, 1);
        for (int i=0; i<num; ++i)
            buf[i] = static_cast<bool>(dis(gen));
    }
    bool Next() const {
        return buf[current = (++current) % buf.size()];
    }
    bool Prev() const {
        current = (current != 0 ) ? (--current) : buf.size() - 1;
        return buf[current];
    }
    void Switch(bool b) {
        buf[current] = b;
    }
};

size_t CountCars(Train& t) {
    // Turn the light on in the current car
    t.Switch(true);

    for(size_t cnt = 0;;) {
        // Go to the next car where lights are on
        // and count the number of cars travelled
        do cnt++; while(!t.Next());
        // Turn the lights off in the car found above
        t.Switch(false);
        // Go back for the cnt number of steps
        bool cur;
        for(int i = 0; i < cnt; i++) {
            cur = t.Prev();
        }
        // If the lights are off, then we reached the first car
        if(!cur) {
            return cnt;
        // Otherwise go to next iteration
        } else {
            cnt = 0;
        } 
    }
    // Shouldn't end up here
    return 0;
}

int main(void) {
   std::uniform_int_distribution<> dis(1, 42);
   auto num = dis(gen);
   Train t(num);
   assert(num == CountCars(t));
   std::cerr << "OK";
   return 0;
}