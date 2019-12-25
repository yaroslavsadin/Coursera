#include "svg_render.h"
#include <vector>
#include <unordered_set>
#include "misc.h"

const std::unordered_map<std::string,std::function<void(const SvgRender*,Svg::Document&)>> SvgRender::render_table {
        { "bus_lines" , &SvgRender::RenderBuses },
        { "stop_points" , &SvgRender::RenderStops },
        { "bus_labels" , &SvgRender::RenderBusLabels },
        { "stop_labels" , &SvgRender::RenderStopLabels }
    };

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
SvgRender& SvgRender::SetStopLabelFontSize(int x){
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
SvgRender& SvgRender::SetBusLabelFontSize(int x){
    settings.bus_label_font_size = x;
    return *this;
}
SvgRender& SvgRender::SetBusLabelOffset(Svg::Point x){
    settings.bus_label_offset = x;
    return *this;
}
SvgRender& SvgRender::SetColorPalette(std::vector<Svg::Color> x){
    settings.color_palette = move(x);
    return *this;
}

SvgRender& SvgRender::SetLayers(std::vector<std::string> x){
    settings.layers = move(x);
    return *this;
}

Svg::Point SvgRender::PointFromLocation(double lat, double lon) const {
    return Svg::Point(lon,lat);
}

struct ColorGenerator {
    ColorGenerator(const std::vector<Svg::Color>& palette)
    : palette(palette),cur_index(0) {}
    Svg::Color operator()() {
        auto res = palette[cur_index++];
        if(cur_index == palette.size()) {
            cur_index = 0;
        }
        return res;
    }
    void Reset(void) { cur_index = 0; }
    const std::vector<Svg::Color>& palette;
    size_t cur_index;
};

void SvgRender::AddBusLabel(Svg::Document& doc,const std::string& bus_name, 
                            const std::string& stop, Svg::Color color) const {
    Svg::Text common = Svg::Text{}
        .SetPoint(PointFromLocation(stops_compressed.at(stop).latitude, stops_compressed.at(stop).longtitude))
        .SetOffset(settings.bus_label_offset)
        .SetFontSize(settings.bus_label_font_size)
        .SetFontFamily("Verdana")
        .SetFontWeight("bold")
        .SetData(bus_name);
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
        common.SetFillColor(color)
    );
}

void SvgRender::RenderBuses(Svg::Document& doc) const {
    ColorGenerator color_generator(settings.color_palette);
    for(const auto& [_,bus] : buses) {
        Svg::Polyline bus_line;
        bus_line.SetStrokeColor(color_generator())
                .SetStrokeWidth(settings.line_width)
                .SetStrokeLineCap("round")
                .SetStrokeLineJoin("round");
        for(const auto& stop : bus.route) {
            bus_line.AddPoint(PointFromLocation(stops_compressed.at(stop).latitude, stops_compressed.at(stop).longtitude));
        }
        if(bus.route.size() && bus.route_type == Bus::RouteType::LINEAR) {
            for(const auto& stop : Range(bus.route.rbegin() + 1,bus.route.rend())) {
                bus_line.AddPoint(PointFromLocation(stops_compressed.at(stop).latitude, stops_compressed.at(stop).longtitude));
            }
        }
        doc.Add(bus_line);
    }
}

void SvgRender::RenderStops(Svg::Document& doc) const {
    for(const auto& [_,stop] : stops_compressed) {
        doc.Add(
            Svg::Circle{}
            .SetFillColor("white")
            .SetRadius(settings.stop_radius)
            .SetCenter(PointFromLocation(stop.latitude, stop.longtitude))
        );
    }
}

void SvgRender::RenderBusLabels(Svg::Document& doc) const {
    ColorGenerator color_generator(settings.color_palette);
    for(const auto& [name,bus] : buses) {
        if(bus.route.size()) {
            const auto& start_stop = *bus.route.begin();
            const auto& finish_stop = *prev(bus.route.end());
            auto bus_color = color_generator();

            AddBusLabel(doc,name,start_stop,bus_color);
            if(start_stop != finish_stop) {
                AddBusLabel(doc,name,finish_stop,bus_color);
            }
        }
    }
}

void SvgRender::RenderStopLabels(Svg::Document& doc) const {
    for(const auto& [name,stop] : stops_compressed) {
        Svg::Text common = Svg::Text{}
            .SetPoint(PointFromLocation(stop.latitude, stop.longtitude))
            .SetOffset(settings.stop_label_offset)
            .SetFontSize(settings.stop_label_font_size)
            .SetFontFamily("Verdana")
            .SetData(std::string(name));
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
}

#include "test_runner.h"
bool SvgRender::StopsAreAdjacent(const std::string& one, const std::string& another) const {
    const Stop& lhs = stops.at(one);
    const Stop& rhs = stops.at(another);
    std::unordered_set<std::string> intersection;
    std::set_intersection(lhs.buses.begin(), lhs.buses.end(),
                          rhs.buses.begin(), rhs.buses.end(),
                          std::inserter(intersection,intersection.begin()));
    if(!intersection.size()) {
        return false;
    }
    for(const auto& bus_name : intersection) {
        const Bus& bus = buses.at(bus_name);
        // Using reverse iterators to deal with roundtrip routes
        auto it1 = std::find(bus.route.rbegin(),bus.route.rend(),one);
        auto it2 = std::find(bus.route.rbegin(),bus.route.rend(),another);
        if(std::distance(it1,it2) == 1 || std::distance(it2,it1) == 1) {
            return true;
        }
    }
    return false;
}

Svg::Document SvgRender::Render() const {
    std::map<double,std::string_view> lat_sorted;
    std::map<double,std::string_view> lon_sorted;
    for(const auto& [name,stop] : stops) {
        lat_sorted[stop.latitude] = name;
        lon_sorted[stop.longtitude] = name;
    }
    if(stops.size() == 1) {
        stops_compressed[lon_sorted.begin()->second].longtitude = settings.padding;
        stops_compressed[lat_sorted.begin()->second].latitude = settings.height - settings.padding;
    } else {
        double x_step = (settings.width - 2 * settings.padding) / (stops.size() - 1);
        size_t idx = 0;
        std::unordered_set<std::string> current_bundle;
        for(auto it = lon_sorted.begin(); it != lon_sorted.end(); it++) {
            auto name = it->second;
            stops_compressed[name].longtitude = idx * x_step + settings.padding;
            if(auto it_next = next(it); it_next != lon_sorted.end()) {
                if(current_bundle.size()) {
                    current_bundle.insert(std::string(name));
                    bool some_is_adjacent = false;
                    for(const auto& stop_name : current_bundle) {
                        some_is_adjacent = StopsAreAdjacent(stop_name,std::string(it_next->second));
                        if(some_is_adjacent) break;
                    }
                    if(some_is_adjacent) {
                        idx++;
                        current_bundle.clear();
                    }
                } else {
                    if(StopsAreAdjacent(std::string(name),std::string(it_next->second))) {
                        idx++;
                    } else {
                        current_bundle.insert(std::string(name));
                    }
                }
            }
        }

        double y_step = (settings.height - 2 * settings.padding) / (stops.size() - 1);
        idx = 0;
        current_bundle.clear();
        for(auto it = lat_sorted.begin(); it != lat_sorted.end(); it++) {
            auto name = it->second;
            stops_compressed[name].latitude = settings.height - settings.padding - (idx++ * y_step);
            // if(auto it_next = next(it); it_next != lat_sorted.end()) {
            //     if(current_bundle.size()) {
            //         current_bundle.insert(std::string(name));
            //         bool some_is_adjacent = false;
            //         for(const auto& stop_name : current_bundle) {
            //             some_is_adjacent = StopsAreAdjacent(stop_name,std::string(it_next->second));
            //             if(some_is_adjacent) break;
            //         }
            //         if(some_is_adjacent) {
            //             idx++;
            //             current_bundle.clear();
            //         }
            //     } else {
            //         if(StopsAreAdjacent(std::string(name),std::string(it_next->second))) {
            //             idx++;
            //         } else {
            //             current_bundle.insert(std::string(name));
            //         }
            //     }
            // }
        }
    }

    Svg::Document doc;

    for(const auto& layer : settings.layers) {
        render_table.at(layer)(this,doc);
    }

    return doc;
}