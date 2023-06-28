cmake_minimum_required (VERSION 3.20)
 
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

project(dwmbar-k
  LANGUAGES CXX
  VERSION 1.0.0
)

add_executable(dwmbar-k src/dwmbar-k.cpp)
add_executable(dwmbar src/wrapper.cpp)
add_executable(battery-k modules/battery-k.cpp)
add_executable(bluetooth-k modules/bluetooth-k.cpp)
add_executable(media-k modules/media-k.cpp)
add_executable(network-k modules/network-k.cpp)
add_executable(time-k modules/time-k.cpp)
add_executable(volume-k modules/volume-k.cpp)
add_executable(weather-k modules/weather-k.cpp)

install(
    TARGETS dwmbar-k
    DESTINATION /usr/local/bin/dwmbar-k/
)

install(
    TARGETS dwmbar
    DESTINATION /usr/local/bin/
)

install(
    TARGETS battery-k
    DESTINATION /usr/local/bin/dwmbar-k/modules/
)

install(
    TARGETS bluetooth-k
    DESTINATION /usr/local/bin/dwmbar-k/modules/
)

install(
    TARGETS media-k
    DESTINATION /usr/local/bin/dwmbar-k/modules/
)

install(
    TARGETS network-k
    DESTINATION /usr/local/bin/dwmbar-k/modules/
)

install(
    TARGETS time-k
    DESTINATION /usr/local/bin/dwmbar-k/modules/
)

install(
    TARGETS volume-k
    DESTINATION /usr/local/bin/dwmbar-k/modules/
)

install(
    TARGETS weather-k
    DESTINATION /usr/local/bin/dwmbar-k/modules/
)

# target_sources(battery-k
#     PRIVATE modules/battery-k.cpp
# )

# target_sources(bluetooth-k
#     PRIVATE modules/bluetooth-k.cpp
# )

# target_sources(media-k
#     PRIVATE modules/media-k.cpp
# )

# target_sources(network-k
#     PRIVATE modules/network-k.cpp
# )

# target_sources(time-k
#     PRIVATE modules/time-k.cpp
# )

# target_sources(volume-k
#     PRIVATE modules/volume-k.cpp
# )

# target_sources(weather-k
#     PRIVATE modules/weather-k.cpp
# )

target_include_directories(dwmbar-k
    PRIVATE src
    SHARED ${std-k_SOURCE_DIR}/inc
)
target_include_directories(dwmbar
    PRIVATE src
    SHARED ${std-k_SOURCE_DIR}/inc
)
target_include_directories(battery-k
    PRIVATE modules
    SHARED ${std-k_SOURCE_DIR}/inc
)
target_include_directories(bluetooth-k
    PRIVATE modules
    SHARED ${std-k_SOURCE_DIR}/inc
)
target_include_directories(network-k
    PRIVATE modules
    SHARED ${std-k_SOURCE_DIR}/inc
)
target_include_directories(time-k
    PRIVATE modules
    SHARED ${std-k_SOURCE_DIR}/inc
)
target_include_directories(volume-k
    PRIVATE modules
    SHARED ${std-k_SOURCE_DIR}/inc
)
target_include_directories(weather-k
    PRIVATE modules
    SHARED ${std-k_SOURCE_DIR}/inc
)

find_package(ALSA REQUIRED)
if(ALSA_FOUND)                 
    include_directories(${ALSA_INCLUDE_DIRS})
    target_link_libraries(volume-k 
        PRIVATE ${ALSA_LIBRARIES}
        PRIVATE std-k
    )
endif(ALSA_FOUND)  

link_directories( ${std-k_BINARY_DIR} )
message(${CMAKE_CURRENT_BINARY_DIR})
message(${std-k_BINARY_DIR})
target_link_libraries(dwmbar-k
  PRIVATE std-k
)
target_link_libraries(dwmbar
  PRIVATE std-k
)
target_link_libraries(battery-k
  PRIVATE std-k
)
target_link_libraries(bluetooth-k
  PRIVATE std-k
)
target_link_libraries(media-k
  PRIVATE std-k
)
target_link_libraries(network-k
  PRIVATE std-k
)
target_link_libraries(time-k
  PRIVATE std-k
)
target_link_libraries(weather-k
  PRIVATE std-k
)