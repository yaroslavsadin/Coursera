#include "common.h"
#include <tuple>
#include <algorithm>

static constexpr int ALPHA_NORM_VALUE {64}; 
static constexpr int ALPHA_SIZE {26};

bool Position::operator==(const Position& rhs) const {
    return std::tie(row,col) == std::tie(rhs.row,rhs.col); 
}
bool Position::operator<(const Position& rhs) const {
    return std::tie(row,col) < std::tie(rhs.row,rhs.col);
}
bool Position::IsValid() const {
    return row < kMaxRows && col < kMaxCols && row >= 0 && col >= 0;
}
std::string Position::ToString() const {
    if(!this->IsValid()) {
        return {};
    }
    std::string col_idx = [this]() {
        auto col_temp = this->col + 1;
        std::string res;
        while(col_temp) {
            // By decrementing col_temp we put coeffs
            // in the desired range of [0,16) instead of [1,16]
            auto modulo = (--col_temp) % ALPHA_SIZE; 
            res.push_back(static_cast<char>(modulo + ALPHA_NORM_VALUE + 1));
            col_temp = (col_temp) / ALPHA_SIZE;
        }
        std::reverse(res.begin(),res.end());
        return res;
    }();
    return col_idx + std::to_string(this->row + 1);
}
Position Position::FromString(std::string_view str) {
    Position res {-1,-1};
    size_t separator {0};
    for(; separator < str.size(); separator++) {
        if(std::isdigit(str[separator])) 
            break;
        if(!std::isalpha(str[separator]) || !std::isupper(str[separator])) 
            return res;
    }
    for(auto i = separator; i < str.size(); i++) {
        if(!std::isdigit(str[i])) 
            return res;
    }
    
    res.col = [str,separator]() {
        auto col_idx = str.substr(0,separator);
        int res {0};
        for(int weight = 1;col_idx.size(); weight *= ALPHA_SIZE) {
            char c = col_idx[col_idx.size()-1];
            res += (c - ALPHA_NORM_VALUE) * weight;
            col_idx.remove_suffix(1);
        }
        return res - 1;
    }();
    str.remove_prefix(separator);
    res.row = std::atoi(str.data()) - 1;
    
    return res;
}