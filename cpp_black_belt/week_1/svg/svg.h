#pragma once
#include <string>
#include <variant>

namespace Svg {
    class Color;
    Color NoneColor;

    struct Point {
        double x,y;
        Point() = default;
        Point(double x, double y) : x(x), y(y) {}
    };

    struct Rgb {
        int red, green, blue;
        Rgb() = default;
        Rgb(int r, int g, int b) : red(r), green(g), blue(b) {}
    };

    class Color {
    public:
        Color() = default;
        Color(const std::string& val) : data(val) {}
        Color(const Rgb& val) : data(val) {}

    private:
        std::variant<std::string,Rgb> data;
    };

    class Shape {
    public:
        void SetFillColor(const Color& color);
        virtual ~Shape();
    private:
    };

    class Circle : public Shape {
    };

    class Polyline : public Shape {
    };

    class Text : public Shape {
    };

    class Document {
    };
}