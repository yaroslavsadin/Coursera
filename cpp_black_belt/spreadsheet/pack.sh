#!/bin/sh

path=/cygdrive/c/Work/Uploads
files="ast.cpp
        ast_delete_visitor.h
        ast_insert_visitor.h
        ast_node.h
        ast_print_visitor.cpp
        ast_print_visitor.h
        ast_ref_cells_visitor.h
        ast_visitor.cpp
        ast_visitor.h
        cell.cpp
        cell.h
        formula.cpp
        formula_impl.h
        position.cpp
        sheet.cpp
        sheet.h
        table.h"

rm -rf $path/spreadsheet.tar.gz
tar -czvf $path/spreadsheet.tar.gz $files