	.option	%reg
	.file	"c_arr_ref.c"
	.file	1 "c_arr_ref.c"
	.size	.Lmain.person, 24
	.type	.Lmain.person,@object
	.globl	name
	.size	name, 9
	.type	name,@object
	.globl	print_name
	.type	print_name,@function
	.size	print_name, .Lfunc_end0-print_name
	.globl	main
	.type	main,@function
	.size	main, .Lfunc_end1-main
	.equ	.CC_I,0
	.global	.CC_I
	.ident	"LLVM 6.0.1/O-2018.09. (build 003) (LLVM 6.0.1) -O2 -av2hs -core4 -Mm -Xbs -Xcd -Xmpy_option=2 -Xnorm -Xsa -Xswap -Xunaligned"
	.section	.ARC.attributes,"",@attributes
	.align	4
	.byte	65
.LabiStart0:                            ; @0x1
	.word	.LabiEnd0-.LabiStart0
	.asciz	"ARC"
.LabiStartList0:                        ; @0x9
	.byte	1
	.word	.LabiEnd0-.LabiStartList0
	.byte	20
	.byte	1                       ; version=1
	.byte	10
	.byte	1                       ; sda=1
	.byte	13
	.byte	1                       ; fshort-enums
	.byte	5
	.byte	4                       ; processor
	.byte	6
	.byte	4                       ; core
	.byte	16
	.asciz	"BS"
	.byte	16
	.asciz	"SWAP"
	.byte	16
	.asciz	"NORM"
	.byte	16
	.asciz	"CD"
	.byte	16
	.asciz	"SA"
	.byte	18
	.byte	2                       ; MPY_OPTION
.LabiEnd0:                              ; @0x32
	.section	.rodata,"a",@progbits
	.align	4
.Lmain.person:                          ; @0x0
	.asciz	"Yaroslav\000"
	.asciz	"Sadin\000\000\000\000"
	.word	28                      ; 0x1c
	.align	4
name:                                   ; @0x18
	.asciz	"Yaroslav"
.Lsec_end0:                             ; @0x21
	.section	.debug_str,"MS",@progbits,1
.Linfo_string0:                         ; @0x0
	.asciz	"clang version 6.0.1 (branches/release_60) O-2018.09. (build 003) (LLVM 6.0.1)" ; string offset=0
.Linfo_string1:                         ; @0x4e
	.asciz	"c_arr_ref.c"           ; string offset=78
.Linfo_string2:                         ; @0x5a
	.asciz	"c:\\Work\\Coursera\\cpp_white_belt\\week_2" ; string offset=90
.Linfo_string3:                         ; @0x81
	.asciz	"name"                  ; string offset=129
.Linfo_string4:                         ; @0x86
	.asciz	"char"                  ; string offset=134
.Linfo_string5:                         ; @0x8b
	.asciz	"sizetype"              ; string offset=139
.Linfo_string6:                         ; @0x94
	.asciz	"print_name"            ; string offset=148
.Linfo_string7:                         ; @0x9f
	.asciz	"main"                  ; string offset=159
.Linfo_string8:                         ; @0xa4
	.asciz	"int"                   ; string offset=164
.Linfo_string9:                         ; @0xa8
	.asciz	"person"                ; string offset=168
.Linfo_string10:                        ; @0xaf
	.asciz	"surname"               ; string offset=175
.Linfo_string11:                        ; @0xb7
	.asciz	"age"                   ; string offset=183
.Linfo_string12:                        ; @0xbb
	.asciz	"Person"                ; string offset=187
	.section	.debug_abbrev,"",@progbits
.Lsection_abbrev:                       ; @0x0
	.byte	1                       ; Abbreviation Code
	.byte	17                      ; DW_TAG_compile_unit
	.byte	1                       ; DW_CHILDREN_yes
	.byte	37                      ; DW_AT_producer
	.byte	14                      ; DW_FORM_strp
	.byte	19                      ; DW_AT_language
	.byte	5                       ; DW_FORM_data2
	.byte	3                       ; DW_AT_name
	.byte	14                      ; DW_FORM_strp
	.byte	16                      ; DW_AT_stmt_list
	.byte	6                       ; DW_FORM_data4
	.byte	27                      ; DW_AT_comp_dir
	.byte	14                      ; DW_FORM_strp
	.ascii	"\264B"                 ; DW_AT_GNU_pubnames
	.byte	12                      ; DW_FORM_flag
	.byte	17                      ; DW_AT_low_pc
	.byte	1                       ; DW_FORM_addr
	.byte	18                      ; DW_AT_high_pc
	.byte	1                       ; DW_FORM_addr
	.byte	0                       ; EOM(1)
	.byte	0                       ; EOM(2)
	.byte	2                       ; Abbreviation Code
	.byte	52                      ; DW_TAG_variable
	.byte	0                       ; DW_CHILDREN_no
	.byte	3                       ; DW_AT_name
	.byte	14                      ; DW_FORM_strp
	.byte	73                      ; DW_AT_type
	.byte	19                      ; DW_FORM_ref4
	.byte	63                      ; DW_AT_external
	.byte	12                      ; DW_FORM_flag
	.byte	58                      ; DW_AT_decl_file
	.byte	11                      ; DW_FORM_data1
	.byte	59                      ; DW_AT_decl_line
	.byte	11                      ; DW_FORM_data1
	.byte	2                       ; DW_AT_location
	.byte	10                      ; DW_FORM_block1
	.byte	0                       ; EOM(1)
	.byte	0                       ; EOM(2)
	.byte	3                       ; Abbreviation Code
	.byte	1                       ; DW_TAG_array_type
	.byte	1                       ; DW_CHILDREN_yes
	.byte	73                      ; DW_AT_type
	.byte	19                      ; DW_FORM_ref4
	.byte	0                       ; EOM(1)
	.byte	0                       ; EOM(2)
	.byte	4                       ; Abbreviation Code
	.byte	33                      ; DW_TAG_subrange_type
	.byte	0                       ; DW_CHILDREN_no
	.byte	73                      ; DW_AT_type
	.byte	19                      ; DW_FORM_ref4
	.byte	55                      ; DW_AT_count
	.byte	11                      ; DW_FORM_data1
	.byte	0                       ; EOM(1)
	.byte	0                       ; EOM(2)
	.byte	5                       ; Abbreviation Code
	.byte	38                      ; DW_TAG_const_type
	.byte	0                       ; DW_CHILDREN_no
	.byte	73                      ; DW_AT_type
	.byte	19                      ; DW_FORM_ref4
	.byte	0                       ; EOM(1)
	.byte	0                       ; EOM(2)
	.byte	6                       ; Abbreviation Code
	.byte	36                      ; DW_TAG_base_type
	.byte	0                       ; DW_CHILDREN_no
	.byte	3                       ; DW_AT_name
	.byte	14                      ; DW_FORM_strp
	.byte	62                      ; DW_AT_encoding
	.byte	11                      ; DW_FORM_data1
	.byte	11                      ; DW_AT_byte_size
	.byte	11                      ; DW_FORM_data1
	.byte	0                       ; EOM(1)
	.byte	0                       ; EOM(2)
	.byte	7                       ; Abbreviation Code
	.byte	36                      ; DW_TAG_base_type
	.byte	0                       ; DW_CHILDREN_no
	.byte	3                       ; DW_AT_name
	.byte	14                      ; DW_FORM_strp
	.byte	11                      ; DW_AT_byte_size
	.byte	11                      ; DW_FORM_data1
	.byte	62                      ; DW_AT_encoding
	.byte	11                      ; DW_FORM_data1
	.byte	0                       ; EOM(1)
	.byte	0                       ; EOM(2)
	.byte	8                       ; Abbreviation Code
	.byte	46                      ; DW_TAG_subprogram
	.byte	1                       ; DW_CHILDREN_yes
	.byte	17                      ; DW_AT_low_pc
	.byte	1                       ; DW_FORM_addr
	.byte	18                      ; DW_AT_high_pc
	.byte	1                       ; DW_FORM_addr
	.byte	64                      ; DW_AT_frame_base
	.byte	10                      ; DW_FORM_block1
	.byte	3                       ; DW_AT_name
	.byte	14                      ; DW_FORM_strp
	.byte	58                      ; DW_AT_decl_file
	.byte	11                      ; DW_FORM_data1
	.byte	59                      ; DW_AT_decl_line
	.byte	11                      ; DW_FORM_data1
	.byte	39                      ; DW_AT_prototyped
	.byte	12                      ; DW_FORM_flag
	.byte	63                      ; DW_AT_external
	.byte	12                      ; DW_FORM_flag
	.byte	0                       ; EOM(1)
	.byte	0                       ; EOM(2)
	.byte	9                       ; Abbreviation Code
	.byte	5                       ; DW_TAG_formal_parameter
	.byte	0                       ; DW_CHILDREN_no
	.byte	3                       ; DW_AT_name
	.byte	14                      ; DW_FORM_strp
	.byte	58                      ; DW_AT_decl_file
	.byte	11                      ; DW_FORM_data1
	.byte	59                      ; DW_AT_decl_line
	.byte	11                      ; DW_FORM_data1
	.byte	73                      ; DW_AT_type
	.byte	19                      ; DW_FORM_ref4
	.byte	0                       ; EOM(1)
	.byte	0                       ; EOM(2)
	.byte	10                      ; Abbreviation Code
	.byte	46                      ; DW_TAG_subprogram
	.byte	1                       ; DW_CHILDREN_yes
	.byte	17                      ; DW_AT_low_pc
	.byte	1                       ; DW_FORM_addr
	.byte	18                      ; DW_AT_high_pc
	.byte	1                       ; DW_FORM_addr
	.byte	64                      ; DW_AT_frame_base
	.byte	10                      ; DW_FORM_block1
	.byte	3                       ; DW_AT_name
	.byte	14                      ; DW_FORM_strp
	.byte	58                      ; DW_AT_decl_file
	.byte	11                      ; DW_FORM_data1
	.byte	59                      ; DW_AT_decl_line
	.byte	11                      ; DW_FORM_data1
	.byte	39                      ; DW_AT_prototyped
	.byte	12                      ; DW_FORM_flag
	.byte	73                      ; DW_AT_type
	.byte	19                      ; DW_FORM_ref4
	.byte	63                      ; DW_AT_external
	.byte	12                      ; DW_FORM_flag
	.byte	0                       ; EOM(1)
	.byte	0                       ; EOM(2)
	.byte	11                      ; Abbreviation Code
	.byte	52                      ; DW_TAG_variable
	.byte	0                       ; DW_CHILDREN_no
	.byte	3                       ; DW_AT_name
	.byte	14                      ; DW_FORM_strp
	.byte	58                      ; DW_AT_decl_file
	.byte	11                      ; DW_FORM_data1
	.byte	59                      ; DW_AT_decl_line
	.byte	11                      ; DW_FORM_data1
	.byte	73                      ; DW_AT_type
	.byte	19                      ; DW_FORM_ref4
	.byte	0                       ; EOM(1)
	.byte	0                       ; EOM(2)
	.byte	12                      ; Abbreviation Code
	.byte	19                      ; DW_TAG_structure_type
	.byte	1                       ; DW_CHILDREN_yes
	.byte	3                       ; DW_AT_name
	.byte	14                      ; DW_FORM_strp
	.byte	11                      ; DW_AT_byte_size
	.byte	11                      ; DW_FORM_data1
	.byte	58                      ; DW_AT_decl_file
	.byte	11                      ; DW_FORM_data1
	.byte	59                      ; DW_AT_decl_line
	.byte	11                      ; DW_FORM_data1
	.byte	0                       ; EOM(1)
	.byte	0                       ; EOM(2)
	.byte	13                      ; Abbreviation Code
	.byte	13                      ; DW_TAG_member
	.byte	0                       ; DW_CHILDREN_no
	.byte	3                       ; DW_AT_name
	.byte	14                      ; DW_FORM_strp
	.byte	73                      ; DW_AT_type
	.byte	19                      ; DW_FORM_ref4
	.byte	58                      ; DW_AT_decl_file
	.byte	11                      ; DW_FORM_data1
	.byte	59                      ; DW_AT_decl_line
	.byte	11                      ; DW_FORM_data1
	.byte	56                      ; DW_AT_data_member_location
	.byte	11                      ; DW_FORM_data1
	.byte	0                       ; EOM(1)
	.byte	0                       ; EOM(2)
	.byte	0                       ; EOM(3)
	.section	.debug_info,"",@progbits
.Ldebug_info:                           ; @0x0
.Lcu_begin0:                            ; @0x0
	.word	215                     ; Length of Unit
	.half	3                       ; DWARF version number
	.word	.Lsection_abbrev        ; Offset Into Abbrev. Section
	.byte	4                       ; Address Size (in bytes)
	.byte	1                       ; Abbrev [1] 0xb:0xd0 DW_TAG_compile_unit
	.word	.Linfo_string0          ; DW_AT_producer
	.half	12                      ; DW_AT_language
	.word	.Linfo_string1          ; DW_AT_name
	.word	.Lline_table_start0     ; DW_AT_stmt_list
	.word	.Linfo_string2          ; DW_AT_comp_dir
	.byte	1                       ; DW_AT_GNU_pubnames
	.word	.Lfunc_begin0           ; DW_AT_low_pc
	.word	.Lfunc_end1             ; DW_AT_high_pc
	.byte	2                       ; Abbrev [2] 0x27:0x12 DW_TAG_variable
	.word	.Linfo_string3          ; DW_AT_name
	.word	57                      ; DW_AT_type
	.byte	1                       ; DW_AT_external
	.byte	1                       ; DW_AT_decl_file
	.byte	3                       ; DW_AT_decl_line
	.byte	5                       ; DW_AT_location
	.byte	3
	.word	name
	.byte	3                       ; Abbrev [3] 0x39:0xc DW_TAG_array_type
	.word	69                      ; DW_AT_type
	.byte	4                       ; Abbrev [4] 0x3e:0x6 DW_TAG_subrange_type
	.word	81                      ; DW_AT_type
	.byte	9                       ; DW_AT_count
	.byte	0                       ; End Of Children Mark
	.byte	5                       ; Abbrev [5] 0x45:0x5 DW_TAG_const_type
	.word	74                      ; DW_AT_type
	.byte	6                       ; Abbrev [6] 0x4a:0x7 DW_TAG_base_type
	.word	.Linfo_string4          ; DW_AT_name
	.byte	8                       ; DW_AT_encoding
	.byte	1                       ; DW_AT_byte_size
	.byte	7                       ; Abbrev [7] 0x51:0x7 DW_TAG_base_type
	.word	.Linfo_string5          ; DW_AT_name
	.byte	8                       ; DW_AT_byte_size
	.byte	7                       ; DW_AT_encoding
	.byte	8                       ; Abbrev [8] 0x58:0x1f DW_TAG_subprogram
	.word	.Lfunc_begin0           ; DW_AT_low_pc
	.word	.Lfunc_end0             ; DW_AT_high_pc
	.byte	1                       ; DW_AT_frame_base
	.byte	108
	.word	.Linfo_string6          ; DW_AT_name
	.byte	1                       ; DW_AT_decl_file
	.byte	11                      ; DW_AT_decl_line
	.byte	1                       ; DW_AT_prototyped
	.byte	1                       ; DW_AT_external
	.byte	9                       ; Abbrev [9] 0x6b:0xb DW_TAG_formal_parameter
	.word	.Linfo_string9          ; DW_AT_name
	.byte	1                       ; DW_AT_decl_file
	.byte	11                      ; DW_AT_decl_line
	.word	161                     ; DW_AT_type
	.byte	0                       ; End Of Children Mark
	.byte	10                      ; Abbrev [10] 0x77:0x23 DW_TAG_subprogram
	.word	.Lfunc_begin1           ; DW_AT_low_pc
	.word	.Lfunc_end1             ; DW_AT_high_pc
	.byte	1                       ; DW_AT_frame_base
	.byte	108
	.word	.Linfo_string7          ; DW_AT_name
	.byte	1                       ; DW_AT_decl_file
	.byte	15                      ; DW_AT_decl_line
	.byte	1                       ; DW_AT_prototyped
	.word	154                     ; DW_AT_type
	.byte	1                       ; DW_AT_external
	.byte	11                      ; Abbrev [11] 0x8e:0xb DW_TAG_variable
	.word	.Linfo_string9          ; DW_AT_name
	.byte	1                       ; DW_AT_decl_file
	.byte	16                      ; DW_AT_decl_line
	.word	161                     ; DW_AT_type
	.byte	0                       ; End Of Children Mark
	.byte	6                       ; Abbrev [6] 0x9a:0x7 DW_TAG_base_type
	.word	.Linfo_string8          ; DW_AT_name
	.byte	5                       ; DW_AT_encoding
	.byte	4                       ; DW_AT_byte_size
	.byte	12                      ; Abbrev [12] 0xa1:0x2d DW_TAG_structure_type
	.word	.Linfo_string12         ; DW_AT_name
	.byte	24                      ; DW_AT_byte_size
	.byte	1                       ; DW_AT_decl_file
	.byte	5                       ; DW_AT_decl_line
	.byte	13                      ; Abbrev [13] 0xa9:0xc DW_TAG_member
	.word	.Linfo_string3          ; DW_AT_name
	.word	206                     ; DW_AT_type
	.byte	1                       ; DW_AT_decl_file
	.byte	6                       ; DW_AT_decl_line
	.byte	0                       ; DW_AT_data_member_location
	.byte	13                      ; Abbrev [13] 0xb5:0xc DW_TAG_member
	.word	.Linfo_string10         ; DW_AT_name
	.word	206                     ; DW_AT_type
	.byte	1                       ; DW_AT_decl_file
	.byte	7                       ; DW_AT_decl_line
	.byte	10                      ; DW_AT_data_member_location
	.byte	13                      ; Abbrev [13] 0xc1:0xc DW_TAG_member
	.word	.Linfo_string11         ; DW_AT_name
	.word	154                     ; DW_AT_type
	.byte	1                       ; DW_AT_decl_file
	.byte	8                       ; DW_AT_decl_line
	.byte	20                      ; DW_AT_data_member_location
	.byte	0                       ; End Of Children Mark
	.byte	3                       ; Abbrev [3] 0xce:0xc DW_TAG_array_type
	.word	74                      ; DW_AT_type
	.byte	4                       ; Abbrev [4] 0xd3:0x6 DW_TAG_subrange_type
	.word	81                      ; DW_AT_type
	.byte	10                      ; DW_AT_count
	.byte	0                       ; End Of Children Mark
	.byte	0                       ; End Of Children Mark
	.section	.debug_aranges,"",@progbits
	.word	36                      ; Length of ARange Set
	.half	2                       ; DWARF Arange version number
	.word	.Lcu_begin0             ; Offset Into Debug Info Section
	.byte	4                       ; Address Size (in bytes)
	.byte	0                       ; Segment Size (in bytes)
	.byte	0xff,0xff,0xff,0xff
	.word	name
	.word	.Lsec_end0-name
	.word	.Lfunc_begin0
	.word	.Lsec_end1-.Lfunc_begin0
	.word	0                       ; ARange terminator
	.word	0
	.section	.debug_macinfo,"",@progbits
.Lcu_macro_begin0:                      ; @0x0
	.byte	0                       ; End Of Macro List Mark
	.section	.debug_pubnames,"",@progbits
	.word	.LpubNames_end0-.LpubNames_begin0 ; Length of Public Names Info
.LpubNames_begin0:                      ; @0x4
	.half	2                       ; DWARF Version
	.word	.Lcu_begin0             ; Offset of Compilation Unit Info
	.word	219                     ; Compilation Unit Length
	.word	39                      ; DIE offset
	.asciz	"name"                  ; External Name
	.word	119                     ; DIE offset
	.asciz	"main"                  ; External Name
	.word	88                      ; DIE offset
	.asciz	"print_name"            ; External Name
	.word	0                       ; End Mark
.LpubNames_end0:                        ; @0x33
	.section	.debug_pubtypes,"",@progbits
	.word	.LpubTypes_end0-.LpubTypes_begin0 ; Length of Public Types Info
.LpubTypes_begin0:                      ; @0x4
	.half	2                       ; DWARF Version
	.word	.Lcu_begin0             ; Offset of Compilation Unit Info
	.word	219                     ; Compilation Unit Length
	.word	161                     ; DIE offset
	.asciz	"Person"                ; External Name
	.word	154                     ; DIE offset
	.asciz	"int"                   ; External Name
	.word	74                      ; DIE offset
	.asciz	"char"                  ; External Name
	.word	0                       ; End Mark
.LpubTypes_end0:                        ; @0x2e

	.text
	.align	8                       ; -- Begin function print_name
print_name:                             ; @print_name
                                        ; @0x0
	.cfa_bf	print_name
.Lfunc_begin0:                          ; @0x0
	.loc	1 11 0                  ; c_arr_ref.c:11:0
;  %bb.0:                               ; %entry
	st.aw	%blink,[%sp,-28]        ; @0x0
	.cfa_push	28              ; @0x4
	.cfa_reg_offset	{%blink}, 0     ; @0x4
	st_s	%r0,[%sp,4]             ; @0x4
	add_s	%r0,%sp,4               ; @0x6
	st_s	%r1,[%sp,8]             ; @0x8
	st_s	%r2,[%sp,12]            ; @0xa
	st_s	%r3,[%sp,16]            ; @0xc
	st	%r4,[%sp,20]            ; @0xe
	.loc	1 12 0                  ; c_arr_ref.c:12:0
	bl.d	puts                    ; @0x12
	st	%r5,[%sp,24]            ; @0x16
	.loc	1 13 0                  ; c_arr_ref.c:13:0
	ld.ab	%blink,[%sp,28]         ; @0x1a
	.cfa_restore	{%blink}        ; @0x1e
	.cfa_pop	28              ; @0x1e
	j_s	[%blink]                ; @0x1e
.Ltmp0:                                 ; @0x20
	.cfa_ef
.Lfunc_end0:                            ; @0x20

	.align	4                       ; -- End function
                                        ; -- Begin function main
main:                                   ; @main
                                        ; @0x20
	.cfa_bf	main
.Lfunc_begin1:                          ; @0x20
	.loc	1 15 0                  ; c_arr_ref.c:15:0
;  %bb.0:                               ; %entry
	st.aw	%blink,[%sp,-28]        ; @0x20
	.cfa_push	28              ; @0x24
	.cfa_reg_offset	{%blink}, 0     ; @0x24
	.loc	1 16 0                  ; c_arr_ref.c:16:0
	mov_s	%r1,.Lmain.person       ; @0x24
	ld_s	%r0,[%r1,20]            ; @0x2a
	st_s	%r0,[%sp,24]            ; @0x2c
	ld_s	%r0,[%r1,16]            ; @0x2e
	st_s	%r0,[%sp,20]            ; @0x30
	ld_s	%r0,[%r1,12]            ; @0x32
	st_s	%r0,[%sp,16]            ; @0x34
	ld_s	%r0,[%r1,8]             ; @0x36
	st_s	%r0,[%sp,12]            ; @0x38
	ld_s	%r0,[%r1,4]             ; @0x3a
	st_s	%r0,[%sp,8]             ; @0x3c
	ld_s	%r0,[%r1,0]             ; @0x3e
	st_s	%r0,[%sp,4]             ; @0x40
	.loc	1 17 0                  ; c_arr_ref.c:17:0
	mov_s	%r0,0x6f726159          ; @0x42
	mov_s	%r1,0x76616c73          ; @0x48
	mov_s	%r2,0x61530000          ; @0x4e
	mov_s	%r3,0x6e6964            ; @0x54
	mov_s	%r4,0                   ; @0x5a
	bl.d	print_name              ; @0x5c
	mov	%r5,28                  ; @0x60
	.loc	1 18 0                  ; c_arr_ref.c:18:0
	bl.d	puts                    ; @0x64
	add_s	%r0,%sp,4               ; @0x68
	.loc	1 19 0                  ; c_arr_ref.c:19:0
	ld.ab	%blink,[%sp,28]         ; @0x6a
	.cfa_restore	{%blink}        ; @0x6e
	.cfa_pop	28              ; @0x6e
	j_s.d	[%blink]                ; @0x6e
	mov_s	%r0,0                   ; @0x70
.Ltmp1:                                 ; @0x72
	.cfa_ef
.Lfunc_end1:                            ; @0x72

	.reloc	_init_ad,0;  startup code to enable %status AD bit ; -- End function
.Lsec_end1:                             ; @0x72
	.section	.debug_line,"",@progbits
.Lline_table_start0:
