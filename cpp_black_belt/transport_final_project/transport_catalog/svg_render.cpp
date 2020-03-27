#include "svg_render.h"
#include <vector>
#include <cassert>
#include "misc.h"

const std::unordered_map<
    std::string,
    std::function<void(const SvgRender*,Svg::Document&)>
> SvgRender::render_table 
    {
        { "bus_lines" , static_cast<BaseRenderFP>(&SvgRender::RenderBuses) },
        { "stop_points" , static_cast<BaseRenderFP>(&SvgRender::RenderStops) },
        { "bus_labels" , static_cast<BaseRenderFP>(&SvgRender::RenderBusLabels) },
        { "stop_labels" , static_cast<BaseRenderFP>(&SvgRender::RenderStopLabels) }
    };

const std::unordered_map<
    std::string,
    std::function<void(const SvgRender*,Svg::Document&,const SvgRender::RouteMap&)>
> SvgRender::render_table_route 
    {
        { "bus_lines" , static_cast<RouteRenderFP>(&SvgRender::RenderBuses) },
        { "stop_points" , static_cast<RouteRenderFP>(&SvgRender::RenderStops) },
        { "bus_labels" , static_cast<RouteRenderFP>(&SvgRender::RenderBusLabels) },
        { "stop_labels" , static_cast<RouteRenderFP>(&SvgRender::RenderStopLabels) },
        { "company_labels" , static_cast<RouteRenderFP>(&SvgRender::RenderCompanyLabels) },
        { "company_points" , static_cast<RouteRenderFP>(&SvgRender::RenderCompanyPoints) },
        { "company_lines" , static_cast<RouteRenderFP>(&SvgRender::RenderCompanyLines) }
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

SvgRender& SvgRender::SetOuterMargin (double x) {
    settings.outer_margin = x;
    return *this;
}

SvgRender& SvgRender::SetCompanyRadius (double x) {
    settings.company_radius = x;
    return *this;
}

SvgRender& SvgRender::SetCompanyLineWidth (double x) {
    settings.company_line_width = x;
    return *this;
}

Svg::Point SvgRender::PointFromLocation(double lat, double lon) const {
    return Svg::Point(lon,lat);
}

struct ColorGenerator {
    ColorGenerator(const std::vector<Svg::Color>& palette)
    : palette(palette),cur_index(0) {}
    Svg::Color operator()() {
        auto res = palette[cur_index++ % palette.size()];
        return res;
    }
    void Reset(void) { cur_index = 0; }
    const std::vector<Svg::Color>& palette;
    size_t cur_index;
};

void SvgRender::AddBusLabel(Svg::Document& doc,const std::string& bus_name, 
                            const std::string& stop, Svg::Color color) const {
    Svg::Text common = Svg::Text{}
        .SetPoint(PointFromLocation(stops_compressed.at(stop).latitude, stops_compressed.at(stop).longitude))
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
    for(const auto& [name,bus] : buses) {
        Svg::Polyline bus_line;
        bus_line.SetStrokeColor(bus_to_color.at(name))
                .SetStrokeWidth(settings.line_width)
                .SetStrokeLineCap("round")
                .SetStrokeLineJoin("round");
        for(const auto& stop : bus.route) {
            bus_line.AddPoint(PointFromLocation(stops_compressed.at(stop).latitude, stops_compressed.at(stop).longitude));
        }
        if(bus.route.size() && bus.route_type == Bus::RouteType::ONEWAY) {
            for(const auto& stop : Range(bus.route.rbegin() + 1,bus.route.rend())) {
                bus_line.AddPoint(PointFromLocation(stops_compressed.at(stop).latitude, stops_compressed.at(stop).longitude));
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
            .SetCenter(PointFromLocation(stop.latitude, stop.longitude))
        );
    }
}

void SvgRender::FillBusColors() const {
    ColorGenerator color_generator(settings.color_palette);
    for(const auto& [name,bus] : buses) {
        auto bus_color = color_generator();
        bus_to_color[name] = bus_color;
    }
}

void SvgRender::RenderBusLabels(Svg::Document& doc) const {
    for(const auto& [name,bus] : buses) {
        auto bus_color = bus_to_color.at(name);
        if(bus.route.size()) {
            const auto& start_stop = *bus.route.begin();
            const auto& finish_stop = *prev(bus.route.end());

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
            .SetPoint(PointFromLocation(stop.latitude, stop.longitude))
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

void SvgRender::RenderBuses(Svg::Document& doc, const RouteMap& route_map) const {
    for(auto edge: route_map) {
        if(edge->type_ == EdgeType::RIDE) {
            std::string_view bus_name = edge->item_name_;
            Svg::Polyline bus_line;
            bus_line.SetStrokeColor(bus_to_color.at(bus_name))
                    .SetStrokeWidth(settings.line_width)
                    .SetStrokeLineCap("round")
                    .SetStrokeLineJoin("round");
            for(auto it = edge->route->start; 
            (edge->route->start <= edge->route->end) ? it <= edge->route->end : it >= edge->route->end; 
            (edge->route->start <= edge->route->end) ? it++ : it--) {
                bus_line.AddPoint(
                    PointFromLocation(
                        stops_compressed.at(buses.at(edge->item_name_).route[it]).latitude, 
                        stops_compressed.at(buses.at(edge->item_name_).route[it]).longitude
                    )
                );
            }
            doc.Add(std::move(bus_line));
        }
    }
}
void SvgRender::RenderStopLabels(Svg::Document& doc, const RouteMap& route_map) const {
    std::unordered_set<std::string_view> drawn;
    /// TODO: f_add_label would be useful as a method to reuse in other RenderStopLabels overload
    auto f_add_label = [this,&doc](std::string_view stop_name){
        const auto& stop = stops_compressed.at(stop_name);
        Svg::Text common = Svg::Text{}
            .SetPoint(PointFromLocation(stop.latitude, stop.longitude))
            .SetOffset(settings.stop_label_offset)
            .SetFontSize(settings.stop_label_font_size)
            .SetFontFamily("Verdana")
            .SetData(std::string(stop_name));
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
    };
    if(route_map.size() == 1 && route_map.back()->type_ == EdgeType::WALK) {
        f_add_label(route_map.back()->item_name_);
    } else {
        for(auto edge : route_map) {
            if(edge->type_ == EdgeType::RIDE) {
                std::string_view stop_begin = buses.at(edge->item_name_).route[edge->route->start];
                std::string_view stop_end = buses.at(edge->item_name_).route[edge->route->end];
                if(!drawn.count(stop_begin)) {
                    f_add_label(stop_begin);
                    drawn.insert(stop_begin);
                }
                if(!drawn.count(stop_end)) {
                    f_add_label(stop_end);
                    drawn.insert(stop_end);
                }
            }
        }
    }
}
void SvgRender::RenderBusLabels(Svg::Document& doc, const RouteMap& route_map) const{
    for(auto edge : route_map) {
        if(edge->type_ == EdgeType::RIDE) {
            std::string_view bus_name = edge->item_name_;
            std::string_view stop_begin = buses.at(edge->item_name_).route[edge->route->start];
            std::string_view stop_end = buses.at(edge->item_name_).route[edge->route->end];
            auto f_is_terminal = 
            [this] (std::string_view bus_name, std::string_view stop_name) -> bool {
                const auto& bus = buses.at(std::string(bus_name));
                if(*bus.route.begin() == stop_name || *prev(bus.route.end()) == stop_name) {
                    return true;
                } else {
                    return false;
                }
            };
            if(f_is_terminal(bus_name,stop_begin)) {
                AddBusLabel(doc,std::string(bus_name),std::string(stop_begin),bus_to_color.at(bus_name));
            }
            if(f_is_terminal(bus_name,stop_end)) {
                AddBusLabel(doc,std::string(bus_name),std::string(stop_end),bus_to_color.at(bus_name));
            }
        }
    }
}
void SvgRender::RenderStops(Svg::Document& doc, const RouteMap& route_map) const{
    // std::unordered_set<std::string_view> drawn;
    for(auto edge : route_map) {
        if(edge->type_ == EdgeType::RIDE) {
            /// TODO: Take care of empty stop_names?
            for(auto it = edge->route->start; 
            (edge->route->start <= edge->route->end) ? it <= edge->route->end : it >= edge->route->end; 
            (edge->route->start <= edge->route->end) ? it++ : it--) {
                doc.Add(
                    Svg::Circle{}
                    .SetFillColor("white")
                    .SetRadius(settings.stop_radius)
                    .SetCenter(PointFromLocation(
                        stops_compressed.at(buses.at(edge->item_name_).route[it]).latitude, 
                        stops_compressed.at(buses.at(edge->item_name_).route[it]).longitude
                    ))
                );
            }
        }
    }
}

void SvgRender::RenderCompanyLabels(Svg::Document& doc, const RouteMap& route_map) const {
    if(!route_map.empty() && route_map.back()->type_ == EdgeType::WALK) {
        auto company_name = route_map.back()->company_name_;
        const auto& company = companies_compressed.at(company_name);
        std::string full_name = 
                (companies.at(std::string(company_name)).rubric.has_value()) ?
                std::string(*companies.at(std::string(company_name)).rubric)
                + " " +
                std::string(company_name)
                :
                std::string(company_name);
        Svg::Text common = Svg::Text{}
            .SetPoint(PointFromLocation(company.latitude, company.longitude))
            .SetOffset(settings.stop_label_offset)
            .SetFontSize(settings.stop_label_font_size)
            .SetFontFamily("Verdana")
            /// TODO: Full name somehow caused JSON error
            .SetData(full_name);
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
void SvgRender::RenderCompanyPoints(Svg::Document& doc, const RouteMap& route_map) const {
    if(!route_map.empty() && route_map.back()->type_ == EdgeType::WALK) {
        doc.Add(
            Svg::Circle{}
            .SetFillColor("black")
            .SetRadius(settings.company_radius)
            .SetCenter(PointFromLocation(
                companies_compressed.at(route_map.back()->company_name_).latitude, 
                companies_compressed.at(route_map.back()->company_name_).longitude
            ))
        );
    }
}
void SvgRender::RenderCompanyLines(Svg::Document& doc, const RouteMap& route_map) const {
    if(!route_map.empty() && route_map.back()->type_ == EdgeType::WALK) {
        Svg::Polyline line;
        line.SetStrokeColor("black")
            .SetStrokeWidth(settings.company_line_width)
            .SetStrokeLineCap("round")
            .SetStrokeLineJoin("round");
        line.AddPoint(
            PointFromLocation(
                stops_compressed.at(route_map.back()->item_name_).latitude, 
                stops_compressed.at(route_map.back()->item_name_).longitude
            )
        );
        line.AddPoint(
            PointFromLocation(
                companies_compressed.at(route_map.back()->company_name_).latitude, 
                companies_compressed.at(route_map.back()->company_name_).longitude
            )
        );
        doc.Add(std::move(line));
    }
}

std::vector<Coords> SvgRender::GetAdjacentStops(MapPoint point, double Coords::*field) const {
    static auto f_check_coord_processed = [](const auto& compressed, std::string_view name, double Coords::*field) {
        return compressed.count(name) && compressed.at(name).*field != -1.;
    };
    std::vector<Coords> res;
    if(point.type == MapPoint::Type::COMPANY) {
        for(auto stop : companies.at(point.name).nearby_stops) {
            if(f_check_coord_processed(stops_compressed,stop.name,field)) {    
                res.push_back(stops_compressed.at(stop.name));
            }
        }
    } else {
        const Stop& stop = stops.at(std::string(point.name));    
        for(const auto& bus_name : stop.buses) {
            const Bus& bus = buses.at(bus_name);
            for (auto it = std::find(bus.route.begin(),bus.route.end(),point.name); 
                    it != bus.route.end();
                    it = std::find(next(it),bus.route.end(),point.name)) {
                if(it != bus.route.begin() && f_check_coord_processed(stops_compressed,*prev(it),field)) {
                    res.push_back(stops_compressed.at(*prev(it)));
                }
                if(it != prev(bus.route.end()) && f_check_coord_processed(stops_compressed,*next(it),field)) {
                    res.push_back(stops_compressed.at(*next(it)));
                }
            }
        }
        for(const auto& [company_name,description] : companies) {
            auto it = std::find_if(description.nearby_stops.begin(),description.nearby_stops.end(),
            [&point](const auto& nearby_stop){
                return nearby_stop.name == point.name;
            });
            if(it != description.nearby_stops.end()) {
                if(f_check_coord_processed(companies_compressed,company_name,field)) {
                    res.push_back(companies_compressed.at(company_name));
                }
            }
        }
    }
    return res;
}

size_t SvgRender::BundleCoordinates(const std::map<double,MapPoint>& sorted_map, double Coords::*field) const {
    size_t max_idx = 0;
    for(const auto [_,point] : sorted_map) {
        auto& compressed = (point.type == MapPoint::Type::STOP) ? stops_compressed : companies_compressed;
        auto neighbours = GetAdjacentStops(point,field);
        size_t idx = 0;
        for(const auto& stop : neighbours) {
            idx = std::max(idx,static_cast<size_t>(stop.*field) + 1);
        }
        compressed[point.name].*field = idx;
        max_idx = std::max(idx,max_idx);
    }
    return max_idx;
}

bool SvgRender::StopIsBase(const std::string& stop_name) const {
    const Stop& stop = stops.at(stop_name);
    // все пересадочные остановки, то есть те, через которые проходит более одного автобуса
    if(stop.buses.empty() || stop.buses.size() > 1) {
        return true;
    }
    // все конечные (возле которых рисуется номер автобуса)
    for(const auto& bus_name : stop.buses) {
        const Bus& bus = buses.at(bus_name);
        if(bus.route.empty()) {
            continue;
        }
        if(stop_name == bus.route.front() || stop_name == bus.route.back()) {
            return true;
        }
    }
    //  через которые проходит один автобус, но более чем дважды за весь маршрут
    const Bus& bus = buses.at(*stop.buses.begin());
    size_t stop_cnt = std::count(bus.route.begin(),bus.route.end(),stop_name);
    if(bus.route_type == Bus::RouteType::ONEWAY) {
        stop_cnt *= 2;
    }
    if(stop_cnt > 2) {
        return true;
    }

    return false;
}

////// TODO: pass by reference? --------------\/\/\/
Svg::Document SvgRender::RenderRoute(RouteMap route) const {
    auto doc = Render();
    doc.Add(
        Svg::Rect{}
        .SetPoint({-settings.outer_margin,-settings.outer_margin})
        .SetDimensions
        (
            settings.width + settings.outer_margin * 2,
            settings.height + settings.outer_margin * 2
        )
        .SetFillColor(settings.underlayer_color)
    );
    for(const auto& layer : settings.layers) {
        render_table_route.at(layer)(this,doc,route);
    }
    return doc;
}

void SvgRender::CompressStopsCoordinates() const {
    std::unordered_map<std::string_view,double> lat;
    std::unordered_map<std::string_view,double> lon;
    std::map<double,MapPoint> lat_sorted;
    std::map<double,MapPoint> lon_sorted;

    for(const auto& [name,bus] : buses) {
        if(bus.route.empty()) continue;

        auto base_it = bus.route.begin(); 
        auto base_it_next = base_it;
        double lon_step, lat_step {.0};
    
        for(auto it = base_it; it != bus.route.end(); it++) {
            const std::string& stop_name = *it;
            if(it == base_it_next) {
                if(!lat.count(stop_name)) {
                    lat[stop_name] = stops.at(stop_name).latitude;
                    lon[stop_name] = stops.at(stop_name).longtitude;
                }
                base_it = base_it_next;
                base_it_next = std::find_if(next(it),bus.route.end(),
                                    [this](const std::string& stop){
                                        return StopIsBase(stop);
                                    });
                if(base_it_next != bus.route.end()) {
                    lon_step = (
                        stops.at(*base_it_next).longtitude - stops.at(*base_it).longtitude
                    ) / (base_it_next - base_it);
                    lat_step = (
                        stops.at(*base_it_next).latitude - stops.at(*base_it).latitude
                    ) / (base_it_next - base_it);
                }
                continue;
            }
            lat[stop_name] = stops.at(*base_it).latitude + lat_step * (it - base_it);
            lon[stop_name] = stops.at(*base_it).longtitude + lon_step * (it - base_it);
        }
    }

    for(const auto& [name,stop] : stops) {
        if(lat.count(name)) {
            lat_sorted[lat.at(name)] = {MapPoint::Type::STOP,name};
            lon_sorted[lon.at(name)] = {MapPoint::Type::STOP,name};
        } else {
            lat_sorted[stop.latitude] = {MapPoint::Type::STOP,name};
            lon_sorted[stop.longtitude] = {MapPoint::Type::STOP,name};
        }
    }

    for(const auto& [name,company] : companies) {
        lat_sorted[company.coords.latitude] = {MapPoint::Type::COMPANY,name};
        lon_sorted[company.coords.longitude] = {MapPoint::Type::COMPANY,name};
    }
    
    size_t x_idx = BundleCoordinates(lon_sorted, &Coords::longitude);
    size_t y_idx = BundleCoordinates(lat_sorted, &Coords::latitude);
    
    double x_step = (x_idx == 0) ? 0 : (settings.width - 2 * settings.padding) / (x_idx);
    double y_step = (y_idx == 0) ? 0 : (settings.height - 2 * settings.padding) / (y_idx);

    for(auto& [name,coordinates] : stops_compressed) {
        coordinates.longitude = coordinates.longitude * x_step + settings.padding;
        coordinates.latitude = settings.height - settings.padding - (coordinates.latitude * y_step);
    }

    for(auto& [name,coordinates] : companies_compressed) {
        coordinates.longitude = coordinates.longitude * x_step + settings.padding;
        coordinates.latitude = settings.height - settings.padding - (coordinates.latitude * y_step);
    }
}

Svg::Document SvgRender::Render() const {
    if(!base_map_cache) {
        CompressStopsCoordinates();

        Svg::Document doc;
        FillBusColors();

        for(const auto& layer : settings.layers) {
            if(render_table.count(layer))
                render_table.at(layer)(this,doc);
        }

        base_map_cache = std::move(doc);
    }

    return *base_map_cache;
}

void SvgRender::Serialize(ProtoTransport::Renderer& r) const {
    auto f_color_to_proto = [](const Svg::Color& color) {    
        ProtoTransport::Color c;
        if(color.IsRgb()) {
            const Svg::Rgb& rgb = color.Get<Svg::Rgb>();
            if(rgb.alpha.has_value()) {
                ProtoTransport::Rgba proto_rgba;
                proto_rgba.set_red(rgb.red);
                proto_rgba.set_blue(rgb.blue);
                proto_rgba.set_green(rgb.green);
                proto_rgba.set_alpha(*rgb.alpha);
                *c.mutable_as_rgba() = std::move(proto_rgba);
            } else {
                ProtoTransport::Rgb proto_rgb;
                proto_rgb.set_red(rgb.red);
                proto_rgb.set_blue(rgb.blue);
                proto_rgb.set_green(rgb.green);
                *c.mutable_as_rgb() = std::move(proto_rgb);
            }
        } else {
            const std::string& str = color.Get<std::string>();
            c.set_as_string(str);
        }
        return c;
    };

    auto f_point_to_proto = [](const Svg::Point& point) {
        ProtoTransport::Point p;
        p.set_x(point.x);
        p.set_y(point.y);
        return p;
    };

    auto* proto_settings = r.mutable_settings();
    proto_settings->set_width(settings.width);
    proto_settings->set_height(settings.height);
    proto_settings->set_padding(settings.padding);
    proto_settings->set_stop_radius(settings.stop_radius);
    proto_settings->set_company_radius(settings.company_radius);
    proto_settings->set_company_line_width(settings.company_line_width);
    proto_settings->set_line_width(settings.line_width);
    proto_settings->set_stop_label_font_size(settings.stop_label_font_size);
    *proto_settings->mutable_stop_label_offset() = f_point_to_proto(settings.stop_label_offset);
    *proto_settings->mutable_underlayer_color() = f_color_to_proto(settings.underlayer_color);
    proto_settings->set_underlayer_width(settings.underlayer_width);
    proto_settings->set_bus_label_font_size(settings.bus_label_font_size);
    *proto_settings->mutable_bus_label_offset() = f_point_to_proto(settings.bus_label_offset);
    auto* proto_color_palette = proto_settings->mutable_color_palette();
    for(const auto& color : settings.color_palette) {
        *proto_color_palette->Add() = f_color_to_proto(color);
    }
    auto* proto_layers = proto_settings->mutable_layers();
    for(const auto& layer : settings.layers) {
        *proto_layers->Add() = layer;
    }
    proto_settings->set_outer_margin(settings.outer_margin);

    CompressStopsCoordinates();
    auto* proto_stops = r.mutable_stops();
    for(auto& [name,pos] : stops_compressed) {
        auto* stop_info = proto_stops->Add();
        stop_info->set_name(std::string(name));
        stop_info->set_lat(pos.latitude);
        stop_info->set_lon(pos.longitude);
    }
    auto* proto_companies = r.mutable_companies();
    for(auto& [name,pos] : companies_compressed) {
        auto* company_info = proto_companies->Add();
        company_info->set_name(std::string(name));
        company_info->set_lat(pos.latitude);
        company_info->set_lon(pos.longitude);
    }
}
void SvgRender::Deserialize(const ProtoTransport::Renderer& r, const Stops& s, const Buses& b, const YP::Companies& companies) {
    auto f_proto_to_color = [](const ProtoTransport::Color& color) {
        if(color.has_as_rgb()) {
            return Svg::Color(
                Svg::Rgb (
                    color.as_rgb().red(),
                    color.as_rgb().green(),
                    color.as_rgb().blue()
                )
            );
        } else if(color.has_as_rgba()) {
            return Svg::Color(
                Svg::Rgb (
                    color.as_rgba().red(),
                    color.as_rgba().green(),
                    color.as_rgba().blue(),
                    color.as_rgba().alpha()
                )
            );
        } else {
            return Svg::Color(
                color.as_string()
            );
        }
    };

    auto f_proto_to_point = [](const ProtoTransport::Point& point) {
        return Svg::Point(point.x(),point.y());
    };

    const auto& proto_settings = r.settings();
    settings.width = proto_settings.width();
    settings.height = proto_settings.height();
    settings.padding= proto_settings.padding();
    settings.stop_radius= proto_settings.stop_radius();
    settings.company_radius= proto_settings.company_radius();
    settings.line_width= proto_settings.line_width();
    settings.company_line_width= proto_settings.company_line_width();
    settings.stop_label_font_size= proto_settings.stop_label_font_size();
    settings.stop_label_offset = f_proto_to_point(proto_settings.stop_label_offset());
    settings.underlayer_color = f_proto_to_color(proto_settings.underlayer_color());
    settings.underlayer_width= proto_settings.underlayer_width();
    settings.bus_label_font_size= proto_settings.bus_label_font_size();
    settings.bus_label_offset = f_proto_to_point(proto_settings.bus_label_offset());
    const auto& proto_color_palette = proto_settings.color_palette();
    for(const auto& color : proto_color_palette) {
        settings.color_palette.push_back(f_proto_to_color(color));
    }
    const auto& proto_layers = proto_settings.layers();
    for(const auto& layer : proto_layers) {
        settings.layers.push_back(layer);
    }
    settings.outer_margin= proto_settings.outer_margin();
    
    const auto& proto_stops = r.stops();
    for(const auto& stop_info : proto_stops) {
        std::string_view stop_name = stops.find(stop_info.name())->first;
        stops_compressed[stop_name] = {stop_info.lat(),stop_info.lon()};
    }
    const auto& proto_companies = r.companies();
    for(const auto& company_info : proto_companies) {
        std::string_view company_name = companies.find(company_info.name())->first;
        companies_compressed[company_name] = {company_info.lat(),company_info.lon()};
    }

    FillBusColors();

    Svg::Document doc;
    for(const auto& layer : settings.layers) {
        if(render_table.count(layer))    
            render_table.at(layer)(this,doc);
    }
    base_map_cache = std::move(doc);
}