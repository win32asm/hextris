cmake_minimum_required(VERSION 3.2)
project(icfp2015)

include_directories(
/usr/include/
/usr/include/jsoncpp
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

find_library(libjsoncpp-dev jsoncpp)

set(SOURCE_FILES main.cpp Field.cpp Finder.cpp Solver.cpp units.cpp
Finder.h Field.h Solver.h units.h

/usr/include/jsoncpp/json/autolink.h
/usr/include/jsoncpp/json/config.h
/usr/include/jsoncpp/json/features.h
/usr/include/jsoncpp/json/forwards.h
/usr/include/jsoncpp/json/json.h
/usr/include/jsoncpp/json/reader.h
/usr/include/jsoncpp/json/value.h
/usr/include/jsoncpp/json/writer.h

)
add_executable(icfp2015 ${SOURCE_FILES})
target_link_libraries(icfp2015 jsoncpp)