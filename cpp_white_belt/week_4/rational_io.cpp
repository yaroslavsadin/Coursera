#include <iostream>
#include <sstream>
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

// Реализуйте для класса Rational операторы << и >>

istream& operator >>(istream& stream,Rational& rat) {
    if (stream.tellg() == -1) {
        return stream;
    }
    if(!stream.eof()) {    
        int p,q;
        stream >> p; stream.ignore(1); 
        if(stream >> q) {
            rat = {p,q};
        }
    }
    return stream;
}

ostream& operator <<(ostream& stream,const Rational& rat) {
    stream << rat.Numerator() << "/" << rat.Denominator();
    return stream;
}

int main() {
    {
        ostringstream output;
        output << Rational(-6, 8);
        if (output.str() != "-3/4") {
            cout << "Rational(-6, 8) should be written as \"-3/4\"" << endl;
            return 1;
        }
    }

    {
        istringstream input("5/7");
        Rational r;
        input >> r;
        bool equal = r == Rational(5, 7);
        if (!equal) {
            cout << "5/7 is incorrectly read as " << r << endl;
            return 2;
        }
    }

    {
        istringstream input("5/7 10/8");
        Rational r1, r2;
        input >> r1 >> r2;
        bool correct = r1 == Rational(5, 7) && r2 == Rational(5, 4);
        if (!correct) {
            cout << "Multiple values are read incorrectly: " << r1 << " " << r2 << endl;
            return 3;
        }

        input >> r1;
        input >> r2;
        correct = r1 == Rational(5, 7) && r2 == Rational(5, 4);
        if (!correct) {
            cout << "Read from empty stream shouldn't change arguments: " << r1 << " " << r2 << endl;
            return 4;
        }
    }

    cout << "OK" << endl;
    return 0;
}
