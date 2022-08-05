cmake_minimum_required(VERSION 3.5)

project(VMGB VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 REQUIRED COMPONENTS Widgets LinguistTools)
find_package(ALSA REQUIRED)

file(GLOB SOURCE_FILES
    main.cpp
    src/*.cpp
    src/*/*.cpp
)
file(GLOB HEADER_FILES
    include/*.h
    include/*/*.h
)
file(GLOB UI_FILES
    ui/*.ui
)
file(GLOB TS_FILES
    i18n/*
)

set(CMAKE_AUTOUIC_SEARCH_PATHS
    ${CMAKE_SOURCE_DIR}/ui
    ${CMAKE_SOURCE_DIR}/ui/setting
)
qt5_wrap_ui(UI_HEADERS ${UI_FILES})

qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
message(STATUS ${CMAKE_CURRENT_BINARY_DIR})

add_executable(${PROJECT_NAME}
    ${HEADER_FILES}
    ${SOURCE_FILES}
    ${UI_FILES}
    ${UI_HEADERS}
    ${TS_FILES}
    ${QM_FILES}
    README.md
)
include_directories(
    ${ALSA_INCLUDE_DIRS}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/ui
)


target_link_libraries(${PROJECT_NAME} PRIVATE Qt5::Widgets ${ALSA_LIBRARIES})
