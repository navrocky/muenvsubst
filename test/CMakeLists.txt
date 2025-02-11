set(TARGET muenvsubst-test)

find_package(Catch2 REQUIRED)

include_directories(${CMAKE_SOURCE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/3rdparty)

set(SOURCES
    common/main.cpp
    functions_test.cpp
    process_executor_test.cpp
)

add_executable(${TARGET} ${SOURCES})

target_link_libraries(${TARGET}
  Catch2::Catch2
  muenvsubst-lib
)
