#pragma once

#include "descriptions.h"
#include "svg.h"
#include <deque>
#include <functional>
#include <unordered_set>

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
    double outer_margin; 
};

class SvgRender {
public:
    using RouteMap = std::vector<const EdgeInfo*>;
    using BaseRenderFP = void(SvgRender::*)(Svg::Document&) const;
    using RouteRenderFP = void(SvgRender::*)(Svg::Document&,const RouteMap&) const;
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
    SvgRender& SetOuterMargin (double);

    Svg::Document Render() const;
    Svg::Document RenderRoute(RouteMap route) const;
private:
    RenderSettings settings;
    const Buses& buses;
    const Stops& stops;
    mutable std::map<std::string_view,StopsPos> stops_compressed;
    mutable std::unordered_map<std::string_view,Svg::Color> bus_to_color;

    mutable std::optional<Svg::Document> base_map_cache;

    Svg::Point PointFromLocation(double lat, double lon) const;

    void FillBusColors() const;

    void RenderBuses(Svg::Document& doc) const;
    void RenderStops(Svg::Document& doc) const;
    void RenderBusLabels(Svg::Document& doc) const;
    void RenderStopLabels(Svg::Document& doc) const;

    void RenderBuses(Svg::Document& doc, const RouteMap& route_map) const;
    void RenderStops(Svg::Document& doc, const RouteMap& route_map) const;
    void RenderBusLabels(Svg::Document& doc, const RouteMap& route_map) const;
    void RenderStopLabels(Svg::Document& doc, const RouteMap& route_map) const;

    static const std::unordered_map<
        std::string,
        std::function<void(const SvgRender*,Svg::Document&)>
    > render_table;
    static const std::unordered_map<
        std::string,
        std::function<void(const SvgRender*,Svg::Document&,const RouteMap&)>
    > render_table_route;

    void AddBusLabel(Svg::Document& doc,const std::string& bus_name, const std::string& stop, Svg::Color color) const;
    bool StopsAreAdjacent(const std::string& one, const std::string& another) const;
    size_t BundleCoordinates(const std::map<double,std::string_view>& sorted_map, double StopsPos::*field) const;
    bool StopIsBase(const std::string& stop) const;
    std::vector<std::string_view> GetAdjacentStops(const std::string_view stop_name, const std::unordered_set<std::string_view>& considered) const;
};
