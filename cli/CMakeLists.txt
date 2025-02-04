set(TARGET muenvsubst)

find_package(args-parser REQUIRED)
find_package(inja REQUIRED)

set(SOURCES
  inja_renderer.cpp
  inja_renderer.h
  main.cpp
  tools.cpp
  tools.h
)

add_executable(${TARGET} ${SOURCES})

target_link_libraries(${TARGET}
  args-parser::args-parser
  pantor::inja
)

include(GNUInstallDirs)
install(TARGETS ${TARGET}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
