#include <algorithm>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>
#include <sstream>
#include "test_runner.h"

using namespace std;


bool IsSubdomain(string_view subdomain, string_view domain) {
  if(!domain.size() && !subdomain.size()) {
    return true;
  }
  if(domain.size() > subdomain.size()) {
    return false;
  }
  size_t i = 0;
  size_t j = 0;
  while (i < subdomain.size() - 1 && j < domain.size() - 1) {
    if (subdomain[i++] != domain[j++]) {
      return false;
    }
  }
  return true;
}

string EraseLeadingZeroes(string_view s) {
  s.remove_prefix(s.find_first_not_of(' '));
  return string(s);
}

vector<string> ReadDomains(istream& is = cin) {
  size_t count;
  is >> count;

  string domain;
  getline(is, domain);

  vector<string> domains;
  for (size_t i = 0; i < count; ++i) {
    getline(is, domain);
    domains.push_back(EraseLeadingZeroes(domain));
  }
  return domains;
}

void RemoveSubdomains(vector<string>& domains) {
  sort(begin(domains), end(domains));
  vector<string> domains_(domains.size());
  size_t insert_pos = 0;
  for (const string& domain : domains) {
    if (insert_pos == 0 || !IsSubdomain(domain, domains_[insert_pos - 1])) {
      domains_[insert_pos++] = move(domain);
    }
  }
  domains_.resize(insert_pos);
  domains = move(domains_);
}

void CheckDomains(const vector<string>& domains_to_check, const vector<string>& banned_domains, ostream& os = cout) {
  for (string_view domain : domains_to_check) {
    if (const auto it = upper_bound(begin(banned_domains), end(banned_domains), domain);
        (it != begin(banned_domains) && IsSubdomain(domain, *prev(it))) ) {
      os << "Bad" << endl;
    } else {
      os << "Good" << endl;
    }
  }
}

////////////////// TESTS /////////////////////

void TestRemoveSubdomains() {
  vector<string> banned_domains = {
    {"ya.ru"},
    {"m.ya.ru"},
    {"aaa.ya.ru"},
    {"ya.ru.com"},
    {"com"},
    {"mmm.mmm.net"},
    {"r.a.ya.ru"},
    {"a.ya.ru.com"},
    {"fff.d.ya.ru"},
    {"ru"},
    {"mmm.net"}
  };
  for (string& domain : banned_domains) {
    reverse(begin(domain), end(domain));
  }
  sort(begin(banned_domains), end(banned_domains));
  vector<string> expected = {
    {"moc"},
    {"ten.mmm"},
    {"ur"}
  };
  RemoveSubdomains(banned_domains);
  ASSERT_EQUAL(expected,banned_domains);
}

void TestReadDomains() {
  {
    istringstream is {R"(
      3
      ebu.ya.ru
      lol.kek.rofl.com
      no.no.no.yes)"
    };

    vector<string> expected = {
      {"ebu.ya.ru"},
      {"lol.kek.rofl.com"},
      {"no.no.no.yes"}
    };

    vector<string> res = ReadDomains(is);

    ASSERT_EQUAL(expected,res);
  }
}

void TestIsSubdomain() {
  {
    string domain = {};
    string subdomain = {};
    ASSERT(IsSubdomain(subdomain,domain));
  }
  {
    string domain = {};
    string subdomain{"ur.ay"};
    ASSERT(!IsSubdomain(subdomain,domain));
  }
  {
    string domain{"ur.ay"};
    string subdomain = {"ube.ur.ay"};
    ASSERT(!IsSubdomain(subdomain,domain));
  }
  {
    string domain{"ur.ay"};
    string subdomain{"ur.ay.ube"};
    ASSERT(IsSubdomain(subdomain,domain));
  }
  {
    string domain{"synopsys.com"};
    string subdomain{"crm.synopsys.com"};
    reverse(domain.begin(),domain.end());
    reverse(subdomain.begin(),subdomain.end());
    ASSERT(IsSubdomain(subdomain,domain));
  }
  {
    string domain{"synopsys.com"};
    string subdomain{"aaaaa.com"};
    reverse(domain.begin(),domain.end());
    reverse(subdomain.begin(),subdomain.end());
    ASSERT(!IsSubdomain(subdomain,domain));
  }
  {
    string domain{"aaaaaa.com"};
    string subdomain{"aaaaa.com"};
    reverse(domain.begin(),domain.end());
    reverse(subdomain.begin(),subdomain.end());
    ASSERT(!IsSubdomain(subdomain,domain));
  }
}

void TestCheckDomains() {
  vector<string> banned_domains = {
    {"ya.com"},
    {"ru.ru"},
    {"net"},
    {"battle.su"},
    {"something"}
  };
  vector<string> domains_to_check = {
    {"m.ya.com"},
    {"com.something"},
    {"ya.ru"},
    {"com.su"},
    {"battle.net"},
    {"steam.ru"}
  };
  for (string& domain : banned_domains) {
    reverse(begin(domain), end(domain));
  }
  for (string& domain : domains_to_check) {
    reverse(begin(domain), end(domain));
  }
  RemoveSubdomains(banned_domains);
  ostringstream os;
  CheckDomains(domains_to_check, banned_domains,os);
  string expected{"Bad\nBad\nGood\nGood\nBad\nGood\n"};
  ASSERT_EQUAL(os.str(),expected);
}

int main() {
  TestRunner tr;
  RUN_TEST(tr,TestIsSubdomain);
  RUN_TEST(tr,TestReadDomains);
  RUN_TEST(tr,TestRemoveSubdomains);
  RUN_TEST(tr,TestCheckDomains);
  
  vector<string> banned_domains = ReadDomains();
  vector<string> domains_to_check = ReadDomains();

  for (string& domain : banned_domains) {
    reverse(begin(domain), end(domain));
  }
  for (string& domain : domains_to_check) {
    reverse(begin(domain), end(domain));
  }
  
  RemoveSubdomains(banned_domains);
  CheckDomains(domains_to_check, banned_domains);

  return 0;
}
