# The following lines of boilerplate have to be in your project's CMakeLists
# in this exact order for cmake to work correctly
cmake_minimum_required(VERSION 3.16)

# Buid options
option(COPY_BINARIES "Włącz kopiowanie .bin po buildzie" OFF)

# Components
include_directories($ENV{IDF_PATH}/components)
set(EXTRA_COMPONENT_DIRS 
    "${CMAKE_SOURCE_DIR}/libs" 
    "$ENV{IDF_PATH}/components")

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(MQTT_PowerMeter)



# Copy .bin to ota_image directory after build
if(COPY_BINARIES)
    message(STATUS "📦 Kopiowanie .bin po kompilacji jest WŁĄCZONE")

    set(BUILD_BIN "${CMAKE_BINARY_DIR}/${PROJECT_NAME}.bin")
    set(COPY_TO_DIR "${CMAKE_SOURCE_DIR}/ota_image")

    add_custom_target(copy_bin ALL
        DEPENDS app
        COMMAND ${CMAKE_COMMAND} -E make_directory ${COPY_TO_DIR}
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${BUILD_BIN} ${COPY_TO_DIR}
        COMMENT "📥 Kopiuję ${PROJECT_NAME}.bin do ${COPY_TO_DIR}"
    )
else()
    message(STATUS "📦 Kopiowanie .bin po kompilacji jest WYŁĄCZONE")
endif()