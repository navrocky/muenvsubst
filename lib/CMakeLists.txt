set(TARGET muenvsubst-lib)

# find_package(inja REQUIRED)
find_package(nlohmann_json REQUIRED)

include_directories(${CMAKE_SOURCE_DIR}/3rdparty)

set(SOURCES
  funcs/base64.cpp
  funcs/base64.h
  funcs/json.cpp
  funcs/json.h
  funcs/other.cpp
  funcs/other.h
  funcs/shell.cpp
  funcs/shell.h
  funcs/string.cpp
  funcs/string.h
  inja_renderer.cpp
  inja_renderer.h
  process_executor.cpp
  process_executor.h
  tools.cpp
  tools.h
)

add_library(${TARGET} ${SOURCES})

target_link_libraries(${TARGET}
  nlohmann_json::nlohmann_json
  # pantor::inja
)
