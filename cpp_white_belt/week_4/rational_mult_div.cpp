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
    return Rational{rhs.Numerator()*lhs.Denominator(),
                    rhs.Denominator()*lhs.Numerator()};
}

int main() {
    {
        Rational a(2, 3);
        Rational b(4, 3);
        Rational c = a * b;
        bool equal = c == Rational(8, 9);
        if (!equal) {
            cout << "2/3 * 4/3 != 8/9" << endl;
            return 1;
        }
    }

    {
        Rational a(5, 4);
        Rational b(15, 8);
        Rational c = a / b;
        bool equal = c == Rational(2, 3);
        if (!equal) {
            cout << "5/4 / 15/8 != 2/3" << endl;
            return 2;
        }
    }

    cout << "OK" << endl;
    return 0;
}
