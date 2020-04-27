#include "cell.h"

Cell::Cell(std::string str) {
    text = std::move(str);

    if(text[0] == '=' && text.size() > 1) {
        /* Parse Formula */
    } else if (text[0] == '\'') {
        std::string_view view(text);
        view.remove_prefix(1);
        value = std::string(text);
    } else {
        value = text;
    }
}

Cell::Value Cell::GetValue() const {
    return 42;
}
std::string Cell::GetText() const {
    return "42";
}
std::vector<Position> Cell::GetReferencedCells() const {
    return {};
}