#pragma once

#include "descriptions.h"
#include "svg.h"

struct RenderSettings {
    double width;
    double height;
    double padding;
    double stop_radius;
    double line_width;
    int stop_label_font_size;
    Svg::Point stop_label_offset;
    Svg::Color underlayer_color;
    double underlayer_width;
    std::vector<Svg::Color> color_palette;
};

class SvgRender {
public:
    SvgRender() =  default;

    SvgRender& SetWidth(double);
    SvgRender& SetHeight(double);
    SvgRender& SetPadding(double);
    SvgRender& SetStopRadius(double);
    SvgRender& SetLineWidth(double);
    SvgRender& SetFontSize(int);
    SvgRender& SetStopLabelOffset(Svg::Point);
    SvgRender& SetUnderlayerColor(Svg::Color);
    SvgRender& SetUnderlayerWidth(double);
    SvgRender& SetColorPalette(std::vector<Svg::Color>);

    Svg::Document GetMap(const Buses& buses, const Stops& stops) const;
private:
    RenderSettings settings;
    mutable std::optional<Svg::Document> cache;
    Svg::Point PointFromLocation(double lat, double lon) const;
};