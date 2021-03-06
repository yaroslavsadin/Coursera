#include "svg.h"
namespace Svg {
    const Color NoneColor {"none"};

    void Document::Render(std::ostream& stream) {
        stream << R"(<?xml version=\"1.0\" encoding=\"UTF-8\" ?>)"
                << R"(<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\")"
#ifdef DEBUG                
                << R"( width=\"2000\" height=\"1000\")"
#endif
                << R"(>)";
        for(const auto& item : data) {
            std::visit( [&stream](const auto& item) { item.Print(stream); }, item );
        }

        stream << R"(</svg>)";
    }

    void Rect::Print(std::ostream& os) const {
        os << "<rect ";
        PrintCommon(os);
        os << "x=\\\"" << x << "\\\" " <<
        "y=\\\"" << y << "\\\" " <<
        "height=\\\"" << h << "\\\" " <<
        "width=\\\"" << w << "\\\" ";
        os << "/>";
    }

    void Circle::Print(std::ostream& os) const {
            os << "<circle ";
            PrintCommon(os);
            os << "cx=\\\"" << cx << "\\\" " <<
            "cy=\\\"" << cy << "\\\" " <<
            "r=\\\"" << r << "\\\" ";
            os << "/>";
        }

    void Polyline::Print(std::ostream& os) const {
        os << "<polyline ";
        PrintCommon(os);
        os << "points=\\\"";
        for(const auto p : points) {
            os << p.x << "," << p.y << ' ';
        }
        os << "\\\"";
        os << "/>";
    }

    void Text::Print(std::ostream& os) const {
        os << "<text ";
        PrintCommon(os);
        os << "x=\\\"" << x << "\\\" " << "y=\\\"" << y << "\\\" " <<
        "dx=\\\"" << dx << "\\\" " << "dy=\\\"" << dy << "\\\" " <<
        "font-size=\\\"" << font_size << "\\\"";
        if(font_family) {
            os << " font-family=\\\"" << *font_family << "\\\" ";
        }
        if(font_weight) {
            os << " font-weight=\\\"" << *font_weight << "\\\" ";
        }
        os << ">" << text << "</text>";
    }

}