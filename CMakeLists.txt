cmake_minimum_required(VERSION 3.5)

set(TARGET muenvsubst)

project(${TAGRET} LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(args-parser REQUIRED)
find_package(mbits-mstch REQUIRED)
find_package(inja REQUIRED)

if(CMAKE_BUILD_TYPE STREQUAL "Release")
  add_link_options(-static -static-libgcc -static-libstdc++ -s)
endif()

set(SOURCES
  src/main.cpp
)

add_executable(${TARGET} ${SOURCES})

target_link_libraries(${TARGET}
  args-parser::args-parser
  mbits::mstch
  pantor::inja
)

include(GNUInstallDirs)
install(TARGETS ${TARGET}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
