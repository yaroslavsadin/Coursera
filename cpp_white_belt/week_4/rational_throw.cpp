#include <iostream>
#include <exception>
using namespace std;

int gcd(int a, int b) {
    while (a > 0 && b > 0) {
        if (a > b) {
            a %= b;
        } else {
            b %= a;
        }
    }
    return (a > 0) ? a : b;
    }

class Rational {
   // Вставьте сюда реализацию класса Rational из первой части
public:
    Rational() {
        // Реализуйте конструктор по умолчанию
        p = 0;
        q = 1;
    }

    Rational(int numerator, int denominator) {
        // Реализуйте конструктор
        if (denominator == 0) {
            throw(invalid_argument("invalid_argument"));
        }
        int gcd_value = gcd(abs(numerator),abs(denominator));
        p = numerator/gcd_value;
        q = denominator/gcd_value;
        if(q < 0) {
            if(p < 0) {
                q = abs(q);
                p = abs(p);
            } else {
                q = abs(q);
                p = -p;
            }
        }
        if(p == 0) {
            q = 1;
        }
    }

    int Numerator() const {
        // Реализуйте этот метод
        return p;
    }

    int Denominator() const {
        // Реализуйте этот метод
        return q;
    }

private:
    // Добавьте поля
    int p,q;
};

// Вставьте сюда реализацию operator == для класса Rational из второй части
bool operator ==(const Rational& rhs, const Rational& lhs) {
    return (rhs.Denominator() == lhs.Denominator() && rhs.Numerator() == lhs.Numerator());
}

Rational operator +(const Rational& rhs, const Rational& lhs) {
    return Rational{rhs.Numerator()*lhs.Denominator() + lhs.Numerator()*rhs.Denominator(),
                                lhs.Denominator()*rhs.Denominator()};
}

Rational operator -(const Rational& rhs, const Rational& lhs) {
    return Rational{rhs.Numerator()*lhs.Denominator() - lhs.Numerator()*rhs.Denominator(),
                                lhs.Denominator()*rhs.Denominator()};
}

// Реализуйте для класса Rational операторы * и /
Rational operator*(const Rational& rhs, const Rational& lhs) {
    return Rational{rhs.Numerator()*lhs.Numerator(),
                    rhs.Denominator()*lhs.Denominator()};
}

Rational operator/(const Rational& rhs, const Rational& lhs) {
    if (lhs.Numerator() == 0) {
            throw(domain_error("domain_error"));
        }
    return Rational{rhs.Numerator()*lhs.Denominator(),
                    rhs.Denominator()*lhs.Numerator()};
}

// Реализуйте для класса Rational операторы << и >>

istream& operator >>(istream& stream,Rational& rat) {
    if(!stream.eof()) {    
        int p,q;
        stream >> p; stream.ignore(1); stream >> q;
        rat = {p,q};
    }
    return stream;
}

ostream& operator <<(ostream& stream,const Rational& rat) {
    stream << rat.Numerator() << "/" << rat.Denominator();
    return stream;
}

bool operator >(const Rational& rhs, const Rational& lhs) {
    return (rhs.Numerator()*lhs.Denominator() > lhs.Numerator()*rhs.Denominator());
}

bool operator <(const Rational& rhs, const Rational& lhs) {
    return (rhs.Numerator()*lhs.Denominator() < lhs.Numerator()*rhs.Denominator());
}

int main() {
    try {
        Rational r(1, 0);
        cout << "Doesn't throw in case of zero denominator" << endl;
        return 1;
    } catch (invalid_argument&) {
    }

    try {
        auto x = Rational(1, 2) / Rational(0, 1);
        cout << "Doesn't throw in case of division by zero" << endl;
        return 2;
    } catch (domain_error&) {
    }

    cout << "OK" << endl;
    return 0;
}
