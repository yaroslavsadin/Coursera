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
    int bus_label_font_size;
    Svg::Point bus_label_offset;
    std::vector<Svg::Color> color_palette;
    std::vector<std::string> layers;
};

class SvgRender {
public:
    SvgRender() =  default;

    SvgRender& SetWidth(double);
    SvgRender& SetHeight(double);
    SvgRender& SetPadding(double);
    SvgRender& SetStopRadius(double);
    SvgRender& SetLineWidth(double);
    SvgRender& SetStopLabelFontSize(int);
    SvgRender& SetStopLabelOffset(Svg::Point);
    SvgRender& SetUnderlayerColor(Svg::Color);
    SvgRender& SetUnderlayerWidth(double);
    SvgRender& SetBusLabelFontSize(int);
    SvgRender& SetBusLabelOffset(Svg::Point);
    SvgRender& SetColorPalette(std::vector<Svg::Color>);
    SvgRender& SetLayers(std::vector<std::string>);

    Svg::Document GetMap(const Buses& buses, const Stops& stops) const;
private:
    RenderSettings settings;
    mutable std::optional<Svg::Document> cache;
    Svg::Point PointFromLocation(double lat, double lon) const;
    void RenderBuses(Svg::Document& doc, const Buses& buses, const Stops& stops) const;
    void RenderStops(Svg::Document& doc, const Buses& buses, const Stops& stops) const;
    void RenderBusLabels(Svg::Document& doc, const Buses& buses, const Stops& stops) const;
    void RenderStopLabels(Svg::Document& doc, const Buses& buses, const Stops& stops) const;
    void AddBusLabel(Svg::Document& doc,const std::string& bus_name, const std::string& stop, 
                                                        const Stops& stops, Svg::Color color) const;
};