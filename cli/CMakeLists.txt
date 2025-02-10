set(TARGET muenvsubst)

find_package(args-parser REQUIRED)

include_directories(${CMAKE_SOURCE_DIR})

set(SOURCES
  main.cpp
)

add_executable(${TARGET} ${SOURCES})

target_link_libraries(${TARGET}
  args-parser::args-parser
  muenvsubst-lib
)

include(GNUInstallDirs)
install(TARGETS ${TARGET}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
