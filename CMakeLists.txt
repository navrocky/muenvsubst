cmake_minimum_required(VERSION 3.5)

set(TARGET muenvsubst)

project(${TAGRET} LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(args-parser REQUIRED)
find_package(mbits-mstch REQUIRED)

add_link_options(-static -static-libgcc -static-libstdc++ -s)

set(SOURCES
    src/main.cpp
)

add_executable(${TARGET} ${SOURCES})

target_link_libraries(${TARGET}
    mbits::mstch
    args-parser::args-parser
)

include(GNUInstallDirs)
install(TARGETS ${TARGET}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
