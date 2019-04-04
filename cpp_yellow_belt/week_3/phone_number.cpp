#include "phone_number.h"
#include "test_runner.h"
#include <sstream>

// Forgot about getline(is, country_code_, '-');.....
string ReadToDelimiter(istream& s, char delim = '-') {
    char t;
    string str;
    while(s && ((t = s.get()) != '-')) {
        str += t;
    }
    return str;
}

PhoneNumber::PhoneNumber(const string &international_number) {
    istringstream ss{international_number};
    string state_code, city_code, number;
    char t;
    ss >> t;
    if (t != '+') {
        throw invalid_argument("Doesn't start with +");
    }
    country_code_ = ReadToDelimiter(ss);
    if(!ss) throw invalid_argument("First dash");
    city_code_ = ReadToDelimiter(ss);
    if(!ss) throw invalid_argument("Second dash");
    ss >> local_number_;
    if(local_number_.empty()) 
        throw invalid_argument("Local number");
}

string PhoneNumber::GetCityCode() const {
    return city_code_;
}

string PhoneNumber::GetCountryCode() const {
    return country_code_;
}

string PhoneNumber::GetLocalNumber() const {
    return local_number_;
}

string PhoneNumber::GetInternationalNumber() const {
    return '+'+country_code_+'-'+city_code_+'-'+local_number_;
}

#if 1
void TestPhoneNumber(void) {
    {
        PhoneNumber p("+999-2221-sf32f");
        string expected = "+999-2221-sf32f";
        AssertEqual(expected,p.GetInternationalNumber(),"should be correct",__LINE__,__FILE__);
    }
    {
        PhoneNumber p("+sfsd-sdf-sfasd32sdaaf");
        string expected = "+sfsd-sdf-sfasd32sdaaf";
        AssertEqual(expected,p.GetInternationalNumber(),"should be correct",__LINE__,__FILE__);
    }
    {
        string expected = "Doesn't start with +";
        try {    
            PhoneNumber p("999-2221-sf32f");
        } catch(exception &e) {
            AssertEqual(expected,e.what(),expected,__LINE__,__FILE__);
        }
    }
    {
        string expected = "First dash";
        try {    
            PhoneNumber p("+-2221-sf32f");
        } catch(exception &e) {
            AssertEqual(expected,e.what(),expected,__LINE__,__FILE__);
        }
    }
    {
        string expected = "Second dash";
        try {    
            PhoneNumber p("+1--sf32f");
        } catch(exception &e) {
            AssertEqual(expected,e.what(),expected,__LINE__,__FILE__);
        }
    }
    {
        string expected = "Local number";
        try {    
            PhoneNumber p("+1-sad-");
        } catch(exception &e) {
            AssertEqual(expected,e.what(),expected,__LINE__,__FILE__);
        }
    }
    {
        string expected = "First dash";
        try {    
            PhoneNumber p("+");
        } catch(exception &e) {
            AssertEqual(expected,e.what(),expected,__LINE__,__FILE__);
        }
    }
}

int main(void) {
    TestRunner tr;
    tr.RunTest(TestPhoneNumber,"TestPhoneNumber");
    return 0;
}
#endif