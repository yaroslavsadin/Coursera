#include <algorithm>
#include <iostream>
#include <vector>
#include <string>
#include <numeric>
#include <optional>
#include <sstream>
#include <chrono>

using namespace std;

template <typename Iterator>
class IteratorRange {
public:
  IteratorRange(Iterator begin, Iterator end)
    : first(begin)
    , last(end)
  {
  }

  Iterator begin() const {
    return first;
  }

  Iterator end() const {
    return last;
  }

private:
  Iterator first, last;
};

template <typename Collection>
auto Head(Collection& v, size_t top) {
  return IteratorRange{v.begin(), next(v.begin(), min(top, v.size()))};
}

struct Person {
  string name;
  int age, income;
  bool is_male;
};

vector<Person> ReadPeople(istream& input) {
  int count;
  input >> count;

  vector<Person> result(count);
  for (Person& p : result) {
    char gender;
    input >> p.name >> p.age >> p.income >> gender;
    p.is_male = gender == 'M';
  }

  return result;
}

using namespace chrono;

static steady_clock::duration TGetMatureCount(0);

// std::ptrdiff_t
auto GetMatureCount(const vector<Person>& people, int adult_age) {
  steady_clock::time_point start = steady_clock::now();
  auto adult_begin = lower_bound(
      begin(people), end(people), adult_age, [](const Person& lhs, int age) {
        return lhs.age < age;
      }
  );
  TGetMatureCount += steady_clock::now() - start;
  return std::distance(adult_begin, end(people));
}

static steady_clock::duration TGetWealthyCount(0);

int GetWealthyCount(const vector<Person>& people, int count) {
  steady_clock::time_point start = steady_clock::now();
  auto head = Head(people, count);

  int total_income = accumulate(
    head.begin(), head.end(), 0, [](int cur, const Person& p) {
      return p.income + cur;
    }
  );
  TGetWealthyCount += steady_clock::now() - start;
  return total_income;
}

static steady_clock::duration TGetPopularNamePartition(0);
static steady_clock::duration TGetPopularNameSearch(0);

std::optional<string> GetPopularName(const vector<Person>& people, char gender) {
  static vector<optional<string>> cache(2);
  if(cache[gender].has_value()) {
     return cache[gender].value();
  }
  steady_clock::time_point start = steady_clock::now();
  auto it_divisor = lower_bound(people.begin(), people.end(), gender, [](const Person& p, char g) {
    return p.is_male;
  });
  IteratorRange range{
    (gender == 'M') ? people.begin() : it_divisor,
    (gender == 'M') ? it_divisor : people.end()
  };
  TGetPopularNamePartition += steady_clock::now() - start;
  if (range.begin() == range.end()) {
    return {};
  } else {
    const string* most_popular_name = &range.begin()->name;
    int count = 1;
    start = steady_clock::now();
    for (auto i = range.begin(); i != range.end(); ) {
      auto same_name_end = find_if_not(i, range.end(), [i](const Person& p) {
        return p.name == i->name;
      });
      auto cur_name_count = std::distance(i, same_name_end);
      if (cur_name_count > count) {
        count = cur_name_count;
        most_popular_name = &i->name;
      }
      i = same_name_end;
    }
    TGetPopularNameSearch += steady_clock::now() - start;
    return cache[gender] = *most_popular_name;
  }
}

int main() {
#ifdef TEST_OVERRIDE_CIN
  istringstream cin(R"(
    11
    Ivan 25 1000 M
    Olga 30 623 W
    Sergey 24 825 M
    Maria 42 1254 W
    Mikhail 15 215 M
    Oleg 18 230 M
    Denis 53 8965 M
    Maxim 37 9050 M
    Ivan 47 19050 M
    Ivan 17 50 M
    Olga 23 550 W
    AGE 18
    AGE 25
    WEALTHY 5
    POPULAR_NAME M
    POPULAR_NAME W
    )");
#endif

  vector<Person> people = ReadPeople(cin);
  vector<Person> people_sorted_by_income = [&people](){
    vector<Person> res(people);
    sort(begin(res), end(res), [](const Person& lhs, const Person& rhs) {
      return lhs.income > rhs.income;
    });
    return move(res);
  }();
  vector<Person>::iterator it_divisor;
  vector<Person> people_partitioned_by_gender = [&people,&it_divisor](){
    vector<Person> res(people);
    it_divisor = partition(begin(res), end(res), [](const Person& p) {
      return p.is_male == true;
    });
    sort(res.begin(), it_divisor, [](const Person& lhs, const Person& rhs) {
      return lhs.name < rhs.name;
    });
    sort(it_divisor, res.end(), [](const Person& lhs, const Person& rhs) {
      return lhs.name < rhs.name;
    });
    return move(res);
  }();

  sort(begin(people), end(people), [](const Person& lhs, const Person& rhs) {
    return lhs.age < rhs.age;
  });

  for (string command; cin >> command; ) {
    if (command == "AGE") {
      int adult_age;
      cin >> adult_age;
      cout << "There are " << GetMatureCount(people, adult_age)
           << " adult people for maturity age " << adult_age << '\n';
    } else if (command == "WEALTHY") {
      int count;
      cin >> count;
      cout << "Top-" << count << " people have total income " 
      << GetWealthyCount(people_sorted_by_income, count) << '\n';
    } else if (command == "POPULAR_NAME") {
      char gender;
      cin >> gender;
      auto res = GetPopularName(people_partitioned_by_gender,gender);
      if(!res.has_value()) {
        cout << "No people of gender " << gender << '\n';
      } else {
        cout << "Most popular name among people of gender " << gender << " is "
        << res.value() << '\n';
      }
    }
  }
  cerr << "TGetMatureCount: " << duration_cast<milliseconds>(TGetMatureCount).count() << endl;
  cerr << "TGetWealthyCount: " << duration_cast<milliseconds>(TGetWealthyCount).count() << endl;
  cerr << "TGetPopularNamePartition: " << duration_cast<milliseconds>(TGetPopularNamePartition).count() << endl;
  cerr << "TGetPopularNameSearch: " << duration_cast<milliseconds>(TGetPopularNameSearch).count() << endl;
}
