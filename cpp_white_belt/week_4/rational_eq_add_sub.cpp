#include <iostream>
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

// Реализуйте для класса Rational операторы ==, + и -
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

int main() {
    {
        Rational r1(4, 6);
        Rational r2(2, 3);
        bool equal = r1 == r2;
        if (!equal) {
            cout << "4/6 != 2/3" << endl;
            return 1;
        }
    }

    {
        Rational a(2, 3);
        Rational b(4, 3);
        Rational c = a + b;
        bool equal = c == Rational(2, 1);
        if (!equal) {
            cout << "2/3 + 4/3 != 2" << endl;
            return 2;
        }
    }

    {
        Rational a(5, 7);
        Rational b(2, 9);
        Rational c = a - b;
        bool equal = c == Rational(31, 63);
        if (!equal) {
            cout << "5/7 - 2/9 != 31/63" << endl;
            return 3;
        }
    }

    {
        Rational a(0, -1);
        Rational b(-1, 3);
        Rational c = a + b;
        bool equal = c == Rational(-1, 3);
        if (!equal) {
            cout << "0/1 + 6/45 != 2/15" << endl;
            return 4;
        }
    }

    cout << "OK" << endl;
    return 0;
}
