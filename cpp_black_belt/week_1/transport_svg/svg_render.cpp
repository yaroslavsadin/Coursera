#include "svg_render.h"
#include <vector>
#include "misc.h"

SvgRender& SvgRender::SetWidth(double x){
    settings.width = x;
    return *this;
}
SvgRender& SvgRender::SetHeight(double x){
    settings.height = x;
    return *this;
}
SvgRender& SvgRender::SetPadding(double x){
    settings.padding = x;
    return *this;
}
SvgRender& SvgRender::SetStopRadius(double x){
    settings.stop_radius = x;
    return *this;
}
SvgRender& SvgRender::SetLineWidth(double x){
    settings.line_width = x;
    return *this;
}
SvgRender& SvgRender::SetFontSize(int x){
    settings.stop_label_font_size = x;
    return *this;
}
SvgRender& SvgRender::SetStopLabelOffset(Svg::Point x){
    settings.stop_label_offset = x;
    return *this;
}
SvgRender& SvgRender::SetUnderlayerColor(Svg::Color x){
    settings.underlayer_color = x;
    return *this;
}
SvgRender& SvgRender::SetUnderlayerWidth(double x){
    settings.underlayer_width = x;
    return *this;
}
SvgRender& SvgRender::SetColorPalette(std::vector<Svg::Color> x){
    settings.color_palette = move(x);
    return *this;
}

static double min_lat {0}, max_lat {0}, min_lon {0}, max_lon {0};
static double zoom_coef = 0;

Svg::Point SvgRender::PointFromLocation(double lat, double lon) const {
    double x = (lon - min_lon) * zoom_coef + settings.padding;
    double y = (max_lat - lat) * zoom_coef + settings.padding;
    return Svg::Point(x,y);
}

struct ColorsCircular {
    ColorsCircular(const std::vector<Svg::Color>& palette)
    : palette(palette),cur_index(0) {}
    Svg::Color operator()() {
        auto res = palette[cur_index++];
        if(cur_index == palette.size()) {
            cur_index = 0;
        }
        return res;
    }
    const std::vector<Svg::Color>& palette;
    size_t cur_index;
};

Svg::Document SvgRender::GetMap(const Buses& buses, const Stops& stops) const {
    if(!cache) {
        min_lat = stops.begin()->second.latitude;
        min_lon = stops.begin()->second.longtitude;
        for(const auto& [_,stop] : stops) {
            if(stop.latitude < min_lat) min_lat = stop.latitude;
            if(stop.latitude > max_lat) max_lat = stop.latitude;
            if(stop.longtitude < min_lon) min_lon = stop.longtitude;
            if(stop.longtitude > max_lon) max_lon = stop.longtitude;
        }

        double width_zoom_coef = (max_lon - min_lon) ? 
                                    (settings.width - 2 * settings.padding) / (max_lon - min_lon) : 0;
        double height_zoom_coef = (max_lat - min_lat) ? 
                                    (settings.height - 2 * settings.padding) / (max_lat - min_lat) : 0;
        zoom_coef = std::min(width_zoom_coef,height_zoom_coef);
        
        Svg::Document doc;
        ColorsCircular get_color(settings.color_palette);
        for(const auto& [_,bus] : buses) {
            Svg::Polyline bus_line;
            bus_line.SetStrokeColor(get_color())
                    .SetStrokeWidth(settings.line_width)
                    .SetStrokeLineCap("round")
                    .SetStrokeLineJoin("round");
            for(const auto& stop : bus.route) {
                bus_line.AddPoint(PointFromLocation(stops.at(stop).latitude, stops.at(stop).longtitude));
            }
            if(bus.route_type == Bus::RouteType::LINEAR) {
                for(const auto& stop : Range(bus.route.rbegin() + 1,bus.route.rend())) {
                    bus_line.AddPoint(PointFromLocation(stops.at(stop).latitude, stops.at(stop).longtitude));
                }
            }
            doc.Add(bus_line);
        }
        for(const auto& [name,stop] : stops) {
            doc.Add(
                Svg::Circle{}
                .SetFillColor("white")
                .SetRadius(settings.stop_radius)
                .SetCenter(PointFromLocation(stop.latitude, stop.longtitude))
            );
        }
        for(const auto& [name,stop] : stops) {
            Svg::Text common = Svg::Text{}
                .SetPoint(PointFromLocation(stop.latitude, stop.longtitude))
                .SetOffset(settings.stop_label_offset)
                .SetFontSize(settings.stop_label_font_size)
                .SetFontFamily("Verdana")
                .SetData(name);
            Svg::Text underlayer = common;
            doc.Add(
                underlayer
                .SetFillColor(settings.underlayer_color)
                .SetStrokeColor(settings.underlayer_color)
                .SetStrokeWidth(settings.underlayer_width)
                .SetStrokeLineCap("round")
                .SetStrokeLineJoin("round")
            );
            doc.Add(
                common.SetFillColor("black")
            );
        }
        cache = std::move(doc);
    }
    return *cache;
}