cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)
project(mython_interpreter)

set(UTILSDIR ../..)
set(LEXER_DIR ../mython-lexer)

#Bring the headers into the project
include_directories(${UTILSDIR} ${LEXER_DIR})
 
#Can manually add the sources using the set command as follows:
#set(SOURCES src/mainapp.cpp src/Student.cpp)
 
#However, the file(GLOB...) allows for wildcard additions:
file(GLOB SOURCES *.cpp ${LEXER_DIR}/*.cpp)

#Set preprocessor definitions
add_compile_definitions()

#Set compile and link flags
set(COMMON_FLAGS -g -O0 -Wall)

if(SANITIZE)
set(COMMON_FLAGS ${COMMON_FLAGS} -fsanitize=address -fsanitize=undefined -fno-omit-frame-pointer -fno-optimize-sibling-calls)
endif()

if(DEBUG_MACROS)
add_compile_definitions(_GLIBCXX_DEBUG _GLIBCXX_DEBUG_PEDANTIC)
endif()

add_compile_options(${COMMON_FLAGS})
add_link_options(${COMMON_FLAGS})

add_executable(mython_interp ${SOURCES})
