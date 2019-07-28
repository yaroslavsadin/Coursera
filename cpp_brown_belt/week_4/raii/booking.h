#pragma once
#include <iostream>
#include <functional>
#include <memory>

#define LOG_FUNCTION() std::cerr << __FUNCTION__ << std::endl

namespace RAII {

struct BookingDeleter {
    BookingDeleter(std::function<void()> cb) :
    callback_(cb)
    {}
    ~BookingDeleter() {
        callback_();
    }
private:
    std::function<void()> callback_;
};

template<typename Provider>
class Booking {
public:
    Booking(Provider* provider, int counter) :
    deleter( std::make_unique<BookingDeleter>(std::function<void()>([&]() {
        provider_->CancelOrComplete(*this);
    })) ),
    provider_(provider)
    {}
    Booking(Booking<Provider>&) = delete;
    Booking<Provider>& operator=(Booking<Provider>&) = delete;
    Booking(Booking<Provider>&& other) = default;
    Booking<Provider>& operator=(Booking<Provider>&&) = default;

private:
    std::unique_ptr<BookingDeleter> deleter;
    Provider* provider_;
};

}