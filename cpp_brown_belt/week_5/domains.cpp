#include <algorithm>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>
#include <sstream>
#include"test_runner.h"

using namespace std;

using Domain = vector<string>;

bool IsSubdomain(const Domain& subdomain, const Domain& domain) {
  if(!domain.size() && !subdomain.size()) {
    return true;
  }
  if(domain.size() < subdomain.size()) {
    return false;
  }

  auto it_domain = domain.rbegin();
  auto it_subdomain = subdomain.rbegin();

  while(it_domain != domain.rend() && it_subdomain != subdomain.rend()) {
    if(*it_domain++ != *it_subdomain++) {
      return false;
    }
  }
  return true;
}

Domain ParseDomain(string_view raw) {
  raw.remove_prefix(raw.find_first_not_of(' '));
  Domain res;
  for(size_t dot = raw.find('.'); dot != raw.npos; dot = raw.find('.')) {
    res.push_back(string(raw.substr(0,dot)));
    raw.remove_prefix(dot + 1);
  }
  res.push_back(string(raw.substr(0)));
  return res;
}

vector<Domain> ReadDomains(istream& is = cin) {
  size_t count;
  is >> count;

  string domain;
  getline(is,domain);
  vector<Domain> domains;
  for (size_t i = 0; i < count; ++i) {
    getline(is, domain);
    domains.push_back(ParseDomain(domain));
  }
  return domains;
}

void TestReadDomains() {
  {
    istringstream is {\
      "3\n\
      ebu.ya.ru\n\
      lol.kek.rofl.com\n\
      no.no.no.yes"
    };

    vector<Domain> expected = {
      {"ebu","ya","ru"},
      {"lol","kek","rofl","com"},
      {"no","no","no","yes"}
    };

    vector<Domain> res = ReadDomains(is);

    ASSERT_EQUAL(expected,res);
  }
}

void TestIsSubdomain() {
  {
    Domain domain = {};
    Domain subdomain = {};
    ASSERT(IsSubdomain(subdomain,domain));
  }
  {
    Domain domain = {};
    Domain subdomain = {"ya","ru"};
    ASSERT(!IsSubdomain(subdomain,domain));
  }
  {
    Domain domain = {"ya","ebu","ya","ru"};
    Domain subdomain = {"ya","ru","ebu"};
    ASSERT(!IsSubdomain(subdomain,domain));
  }
  {
    Domain domain = {"ya","ebu","ya","ru"};
    Domain subdomain = {"ya","ru"};
    ASSERT(IsSubdomain(subdomain,domain));
  }
}

int main() {
  TestRunner tr;
  RUN_TEST(tr,TestReadDomains);
  RUN_TEST(tr,TestIsSubdomain);
#if 1
  vector<Domain> banned_domains = ReadDomains(cin);
  vector<Domain> domains_to_check = ReadDomains(cin);

  for(Domain& domain : banned_domains) {
    reverse(domain.begin(),domain.end());
  }
  
  sort(banned_domains.begin(),banned_domains.end());


#endif
  return 0;
}
