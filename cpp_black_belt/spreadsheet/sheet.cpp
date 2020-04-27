#include "sheet.h"

void Sheet::SetCell(Position pos, std::string text){
}
const ICell* Sheet::GetCell(Position pos) const{
}
ICell* Sheet::GetCell(Position pos){
}
void Sheet::ClearCell(Position pos){
}
void Sheet::InsertRows(int before, int count){
}
void Sheet::InsertCols(int before, int count){
}
void Sheet::DeleteRows(int first, int count){
}
void Sheet::DeleteCols(int first, int count){
}
Size Sheet::GetPrintableSize() const{
}
void Sheet::PrintValues(std::ostream& output) const{
}
void Sheet::PrintTexts(std::ostream& output) const{
}

std::unique_ptr<ISheet> CreateSheet() {
    return std::make_unique<Sheet>();
}