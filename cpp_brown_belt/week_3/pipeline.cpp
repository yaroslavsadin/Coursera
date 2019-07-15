#include "test_runner.h"
#include <functional>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

using namespace std;


struct Email {
  string from;
  string to;
  string body;
  friend ostream& operator<<(ostream& os, const Email& e) {
    return os << e.from << '\n' << e.to << '\n' << e.body;
  }
  friend istream& operator>>(istream& is, Email& e) {
    getline(is,e.from);
    getline(is,e.to);
    getline(is,e.body);
    return is;
  }
};


class Worker {
public:
  virtual ~Worker() = default;
  virtual void Process(unique_ptr<Email> email) = 0;
  virtual void Run() {
    // только первому worker-у в пайплайне нужно это имплементировать
    throw logic_error("Unimplemented");
  }

protected:
  // реализации должны вызывать PassOn, чтобы передать объект дальше
  // по цепочке обработчиков
  void PassOn(unique_ptr<Email> email) const {
    if(next_.get() != nullptr) {
      next_->Process(move(email));
    }
  }

public:
  void SetNext(unique_ptr<Worker> next) {
    next_ = move(next);
  }
private:
  unique_ptr<Worker> next_;
};


class Reader : public Worker {
public:
  // реализуйте класс
  explicit Reader(istream& in) :
  in_(in) 
  {}

  void Run() override {
    Email current;
    while(in_ >> current) {
      Process(make_unique<Email>(current));
    }
  }

  void Process(unique_ptr<Email> email) override {
    PassOn(move(email));
  }
  
  
private:
  istream& in_;
};


class Filter : public Worker {
public:
  using Function = function<bool(const Email&)>;
  Filter(Function f) :
  f_(move(f))
  {}

  void Process(unique_ptr<Email> email) override {
    if(f_(*email)) {
      PassOn(move(email));
    }
  }

private:
  // реализуйте класс
  const Function f_;
};


class Copier : public Worker {
public:
  // реализуйте класс
  Copier(const string cc) :
  cc_(cc)
  {}

  void Process(unique_ptr<Email> email) override {
    Email copy_;
    if(email->to != cc_) {
      copy_ = {email->from,cc_,email->body};
    }
    PassOn(move(email));
    if(copy_.to == cc_) {
      PassOn(make_unique<Email>(copy_));
    }
  }

private:
  const string cc_;
};


class Sender : public Worker {
public:
  // реализуйте класс
  Sender(ostream& os) :
  os_(os)
  {}

  void Process(unique_ptr<Email> email) override {
    os_ << *email << '\n';
  }

private:
  ostream& os_;
};


// реализуйте класс
class PipelineBuilder {
public:
  // добавляет в качестве первого обработчика Reader
  explicit PipelineBuilder(istream& in)
  {
    workers.emplace_back(make_unique<Reader>(in));
  }

  // добавляет новый обработчик Filter
  PipelineBuilder& FilterBy(Filter::Function filter) {
    workers.push_back(make_unique<Filter>(filter));
    return *this;
  }

  // добавляет новый обработчик Copier
  PipelineBuilder& CopyTo(string recipient){
    workers.push_back(make_unique<Copier>(recipient));
    return *this;
  }

  // добавляет новый обработчик Sender
  PipelineBuilder& Send(ostream& out) {
    workers.push_back(make_unique<Sender>(out));
    return *this;
  }

  // возвращает готовую цепочку обработчиков
  unique_ptr<Worker> Build() {
    for(auto it = workers.end() - 1; it != workers.begin(); it--) {
      (*(it - 1))->SetNext(move(*it));
    }
    return move(workers.front());
  }
private:
  vector<unique_ptr<Worker>> workers;
};

void TestEmails() {
  string input = (
    "lol@example.com\n"
    "kek@example.com\n"
    "Hello there\n"

    "kek@example.com\n"
    "lol@example.com\n"
    "I got a trophy from your mom\n"

    "rolf@example.com\n"
    "lol@example.com\n"
    "Troll face\n"

    "lol@example.com\n"
    "rolf@example.com\n"
    "LOOOOOOOL\n"
  );
  istringstream inStream(input);
  ostringstream outStream;

  PipelineBuilder builder(inStream);
  builder.FilterBy([](const Email& email) {
    return (email.to != "lol@example.com");
  });
  builder.CopyTo("kek@example.com");
  builder.Send(outStream);
  auto workers = builder.Build();
  workers->Run();
  
  string expectedOutput = (
    "lol@example.com\n"
    "kek@example.com\n"
    "Hello there\n"

    "lol@example.com\n"
    "rolf@example.com\n"
    "LOOOOOOOL\n"

    "lol@example.com\n"
    "kek@example.com\n"
    "LOOOOOOOL\n"
  );

  ASSERT_EQUAL(expectedOutput, outStream.str());
}

void TestSanity() {
  string input = (
    "erich@example.com\n"
    "richard@example.com\n"
    "Hello there\n"

    "erich@example.com\n"
    "ralph@example.com\n"
    "Are you sure you pressed the right button?\n"

    "ralph@example.com\n"
    "erich@example.com\n"
    "I do not make mistakes of that kind\n"
  );
  istringstream inStream(input);
  ostringstream outStream;

  PipelineBuilder builder(inStream);
  builder.FilterBy([](const Email& email) {
    return email.from == "erich@example.com";
  });
  builder.CopyTo("richard@example.com");
  builder.Send(outStream);
  auto pipeline = builder.Build();

  pipeline->Run();

  string expectedOutput = (
    "erich@example.com\n"
    "richard@example.com\n"
    "Hello there\n"

    "erich@example.com\n"
    "ralph@example.com\n"
    "Are you sure you pressed the right button?\n"

    "erich@example.com\n"
    "richard@example.com\n"
    "Are you sure you pressed the right button?\n"
  );

  ASSERT_EQUAL(expectedOutput, outStream.str());
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestSanity);
  RUN_TEST(tr, TestEmails);
  return 0;
}
