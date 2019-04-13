#include <vector>
#include "test_runner.h"
#include <stdexcept>
using namespace std;

template<typename T>
class Deque {
public:
    Deque() {}
    Deque(const T& t) {}
    bool Empty() const {
        return (front.empty() && back.empty());
    }
    size_t Size() const {
        return (front.size() + back.size());
    }
    const T& At(size_t index) const {
        if(index >= Size()) {
            throw out_of_range("Index out of range");
        } else if (front.size() && index <= (front.size()-1)) {
            return front[front.size()-1-index];
        } else {
            return back[index-front.size()];
        }
    }
    T& At(size_t index) {
        if(index >= Size()) {
            throw out_of_range("Index out of range");
        } else if (front.size() && index <= (front.size()-1)) {
            return front[front.size()-1-index];
        } else {
            return back[index-front.size()];
        }
    }
    const T& operator[](size_t index) const {
        return At(index);
    }
    T& operator[](size_t index) {
        return At(index);
    }
    void PushFront(const T& item) {
        front.push_back(item);
    }
    void PushBack(const T& item) {
        back.push_back(item);
    }
    const T& Front() const {
        return At(0);
    }
    T& Front() {
        return At(0);
    }
    const T& Back() const {
        return At(Size()-1);
    }
    T& Back() {
        return At(Size()-1);
    }
private:
    vector<T> front;
    vector<T> back;
};

void TestDeque() {
    {
        Deque d(42);
        d.PushFront(4);
        d.PushFront(3);
        d.PushFront(2);
        d.PushFront(1);
        d.PushBack(5);
        d.PushBack(6);
        d.PushFront(666);
        d.PushBack(7);
        d[6] = 667;
        ASSERT_EQUAL(8,d.Size());
        ASSERT_EQUAL(d.At(0),666);
        ASSERT_EQUAL(d.At(1),1);
        ASSERT_EQUAL(d.At(2),2);
        ASSERT_EQUAL(d.At(3),3);
        ASSERT_EQUAL(d[4],4);
        ASSERT_EQUAL(d[5],5);
        ASSERT_EQUAL(d[6],667);
        ASSERT_EQUAL(d[7],7);
        ASSERT_EQUAL(d.Front(),666);
        ASSERT_EQUAL(d.Back(),7);
    }
    {
        Deque <int> d;
        d.PushBack(1);
        ASSERT_EQUAL(d.Front(),1);
        ASSERT_EQUAL(d.Back(),1);
    }
}

int main(void) {
    TestRunner tr;
    RUN_TEST(tr,TestDeque);
    return 0;
}