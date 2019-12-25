#pragma once

#include "descriptions.h"
#include "svg.h"
#include <functional>

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
    struct StopsPos {
        double latitude;
        double longtitude;
    };

    SvgRender(const Buses& buses, const Stops& stops)
    : buses(buses), stops(stops) {
    }

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

    Svg::Document Render() const;
private:
    RenderSettings settings;
    const Buses& buses;
    const Stops& stops;
    mutable std::map<std::string_view,StopsPos> stops_compressed;

    Svg::Point PointFromLocation(double lat, double lon) const;

    void RenderBuses(Svg::Document& doc) const;
    void RenderStops(Svg::Document& doc) const;
    void RenderBusLabels(Svg::Document& doc) const;
    void RenderStopLabels(Svg::Document& doc) const;

    static const std::unordered_map<std::string,std::function<void(const SvgRender*,Svg::Document&)>> render_table;

    void AddBusLabel(Svg::Document& doc,const std::string& bus_name, const std::string& stop, Svg::Color color) const;
    bool StopsAreAdjacent(const std::string& one, const std::string& another) const;
    size_t AdjustCoordinates(const std::map<double,std::string_view>& sorted_map, double StopsPos::*field) const;
};
