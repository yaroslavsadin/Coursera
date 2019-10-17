#pragma once

#include <iostream>
#include <string>
#include <variant>
#include <vector>
#include <optional>

namespace Svg {
    struct Point {
        double x,y;
        Point() = default;
        Point(double x, double y) : x(x), y(y) {}
    };

    struct Rgb {
        uint8_t red, green, blue;
        Rgb() = default;
        Rgb(uint8_t r, uint8_t g, uint8_t b) : red(r), green(g), blue(b) {}
        friend std::ostream& operator<<(std::ostream& os, Rgb color) {
            return os << "rgb(" << static_cast<int>(color.red) << "," 
                                << static_cast<int>(color.green) << "," 
                                << static_cast<int>(color.blue) << ")";
        }
    };

    class Color {
    public:
        Color() = default;
        Color(const std::string& val) : data(val) {}
        Color(const char val[]) : data(val) {}
        Color(const Rgb& val) : data(val) {}
        friend std::ostream& operator<<(std::ostream& os, const Color color) {
            std::visit( [&os](const auto& color){ os << color; },  color.data );
            return os;
        }
    private:
        std::variant<std::string,Rgb> data;
    };

    // CRTP--------------\/
    template <typename Derived>
    class Shape {
    public:
        Shape() = default;
        virtual void Print(std::ostream& os) const = 0;

        Derived& SetFillColor(const Color& fill_)  { 
            fill = fill_;
            return *static_cast<Derived*>(this);
        }
        Derived& SetStrokeColor(const Color& stroke_)  {
            stroke = stroke_;
            return *static_cast<Derived*>(this);
        }
        Derived& SetStrokeWidth(double width)  {
            stroke_width = width;
            return *static_cast<Derived*>(this);
        }
        Derived& SetStrokeLineCap(const std::string& cap)  {
            stroke_linecap = cap;
            return *static_cast<Derived*>(this);
        }
        Derived& SetStrokeLineJoin(const std::string& join)  {
            stroke_linejoin = join;
            return *static_cast<Derived*>(this);
        }

        void PrintCommon(std::ostream& os) const {
            os << "fill=\"" << fill << "\" " << "stroke=\"" << stroke << "\" " <<
            "stroke-width=\"" << stroke_width << "\" ";
            if(stroke_linecap) {
                os << "stroke-linecap=\"" << *stroke_linecap << "\" ";
            }
            if(stroke_linejoin) {
                os << "stroke-linejoin=\"" << *stroke_linejoin << "\" ";
            }
        }

        virtual ~Shape() = default;
    private:
        Color fill{"none"};
        Color stroke{"none"};
        double stroke_width = 1.0;
        std::optional<std::string> stroke_linecap;
        std::optional<std::string> stroke_linejoin;
    };

    // CRTP-----------------------\/
    class Circle : public Shape<Circle> {
    public:
        Circle() = default;

        Circle& SetCenter(Point p) {
            cx = p.x; cy = p.y;
            return *this;
        }
        Circle& SetRadius(double r_) {
            r = r_;
            return *this;
        }
        void Print(std::ostream& os) const override;
    private:
        double cx = .0;
        double cy = .0;
        double r;
    };

    // CRTP-------------------------\/
    class Polyline : public Shape<Polyline> {
    public:
        Polyline& AddPoint(Point p) {
            points.push_back(std::move(p));
            return *this;
        }
        void Print(std::ostream& os) const override;
    private:
        std::vector<Point> points;
    };

    class Text : public Shape<Text> {
    public:
        Text& SetPoint(Point p) {
            x = p.x; y = p.y;
            return *this;
        }
        Text& SetOffset(Point p) {
            dx = p.x; dy = p.y;
            return *this;
        } 
        Text& SetFontSize(uint32_t s) {
            font_size = s;
            return *this;
        } 
        Text& SetFontFamily(const std::string& f) {
            font_family = f;
            return *this;
        }
        Text& SetData(const std::string& t) {
            text = t;
            return *this;
        }
        void Print(std::ostream& os) const override;
    private:
        double x , y;
        double dx , dy;
        uint32_t font_size;
        std::optional<std::string> font_family;
        std::string text; 
    };

    class Document {
    public:
        using ShapeHolder = std::variant<Circle,Polyline,Text>;
        Document() = default;
        void Add(const ShapeHolder& shape) {
            data.push_back(shape);
        }
        void Add(ShapeHolder&& shape) {
            data.push_back(move(shape));
        }
        void Render(std::ostream& stream);
    private:
        std::vector<ShapeHolder> data;
    };
}