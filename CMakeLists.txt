cmake_minimum_required (VERSION 3.11)
 
# --- Fetch libs --------------------------------------------------------------

include(FetchContent)

set(FETCHCONTENT_UPDATES_DISCONNECTED TRUE)
FetchContent_Declare(std-k
  GIT_REPOSITORY https://github.com/kiddersmxj/std-k
  GIT_TAG main
)
 
FetchContent_GetProperties(std-k)
if(NOT std-k_POPULATED)
  FetchContent_Populate(std-k)
  add_subdirectory(${std-k_SOURCE_DIR} ${std-k_BINARY_DIR})
endif()
 
# ------------------------------------------------------------------------------

project(dwmbarsrc
  LANGUAGES CXX
  VERSION 1.0.0
)

add_executable(dwmbarsrc src/dwmbar.cpp)
add_executable(dwmbar src/wrapper.cpp)

target_include_directories(dwmbarsrc
    PRIVATE src
    SHARED ${std-k_SOURCE_DIR}/inc
)
target_include_directories(dwmbar
    PRIVATE src
    SHARED ${std-k_SOURCE_DIR}/inc
)

target_sources(dwmbarsrc
    PRIVATE src/dwmbar.cpp
    PRIVATE src/bluetooth.cpp
    PRIVATE src/network.cpp
    PRIVATE src/media.cpp
    PRIVATE src/time.cpp
    PRIVATE src/volume.cpp
    PRIVATE src/weather.cpp
    PRIVATE src/battery.cpp
    PRIVATE src/signal.cpp
    PRIVATE src/cpu.cpp
    PRIVATE src/throttling.cpp
    PRIVATE src/brightness.cpp
    PRIVATE src/module.cpp
)

target_sources(dwmbar
    PRIVATE src/wrapper.cpp
)

install(
    TARGETS dwmbarsrc
    DESTINATION /usr/local/bin/dwmbarsrc/
)

install(
    TARGETS dwmbar
    DESTINATION /usr/local/bin/
)

find_package(X11 REQUIRED)
if (X11_FOUND)
    target_include_directories(dwmbarsrc PRIVATE ${X11_INCLUDE_DIR})
    target_link_libraries(dwmbarsrc PRIVATE ${X11_LIBRARIES})
else()
    message(FATAL_ERROR "X11 not found on your system")
endif()

find_package(ALSA REQUIRED)
if(ALSA_FOUND)                 
    include_directories(${ALSA_INCLUDE_DIRS})
    target_link_libraries(dwmbarsrc
        PRIVATE ${ALSA_LIBRARIES}
        PRIVATE std-k
    )
endif(ALSA_FOUND)  

link_directories( ${std-k_BINARY_DIR} )
message(${CMAKE_CURRENT_BINARY_DIR})
message(${std-k_SOURCE_DIR}/inc)
target_link_libraries(dwmbarsrc
    PUBLIC std-k
)
target_link_libraries(dwmbar
    PUBLIC std-k
)
