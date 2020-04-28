#include "cell.h"

Cell::Cell(std::string str) {
    text = std::move(str);

    if(text[0] == '=' && text.size() > 1) {
        /* Parse Formula */
        throw std::runtime_error("Formula not implemented");
    } else if (text[0] == '\'') {
        std::string_view view(text);
        view.remove_prefix(1);
        value = std::string(view);
    } else {
        value = text;
    }
}

Cell::Value Cell::GetValue() const {
    return value;
}
std::string Cell::GetText() const {
    return text;
}
std::vector<Position> Cell::GetReferencedCells() const {
    return {};
}