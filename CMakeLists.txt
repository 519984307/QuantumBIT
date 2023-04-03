cmake_minimum_required(VERSION 3.24)
project(ODMR)

set(CMAKE_CXX_STANDARD 20)
set(CppLibrary "D:/Document/CppLibrary")
set(CMAKE_AUTORCC ON)
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(spdlog_ROOT ${CppLibrary}/spdlog/spdlog-debug)
    set(json_ROOT ${CppLibrary}/json/nlohmann_json-debug)
elseif (CMAKE_BUILD_TYPE STREQUAL "Release")
    set(spdlog_ROOT ${CppLibrary}/spdlog/spdlog-release)
    set(json_ROOT ${CppLibrary}/json/nlohmann_json-release)
endif ()

set(BOOST_ROOT ${CppLibrary}/Boost)
set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
set(BOOST_COMPONENT )
set(qt_ROOT ${CppLibrary}/Qt6)
set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} ${spdlog_ROOT} ${qt_ROOT} ${json_ROOT})

find_package(spdlog REQUIRED)
get_target_property(SPDLOG_INC_DIR spdlog::spdlog INTERFACE_INCLUDE_DIRECTORIES)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets Gui Charts)
qt_standard_project_setup()

find_package(nlohmann_json REQUIRED)
get_target_property(JSON_INC_DIR nlohmann_json::nlohmann_json INTERFACE_INCLUDE_DIRECTORIES)

find_package(Boost REQUIRED ${BOOST_COMPONENT})

include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${SPDLOG_INC_DIR})
include_directories(${JSON_INC_DIR})
include_directories(${Boost_INCLUDE_DIRS})

file(GLOB_RECURSE HEAD_FILE "${PROJECT_SOURCE_DIR}/include/*.h" "${PROJECT_SOURCE_DIR}/include/*.hpp")
file(GLOB_RECURSE SOURCE_FILE "${PROJECT_SOURCE_DIR}/src/*.cpp" "${PROJECT_SOURCE_DIR}/src/*.hpp")
file(GLOB_RECURSE QRC_FILE "${PROJECT_SOURCE_DIR}/qrc/*.qrc")
file(GLOB_RECURSE ASG_DLL "${PROJECT_SOURCE_DIR}/lib/*.lib")
add_executable(${PROJECT_NAME} ${HEAD_FILE} ${SOURCE_FILE} ${QRC_FILE})
target_link_libraries(${PROJECT_NAME} Qt6::Core Qt6::Widgets Qt6::Gui Qt6::Charts)
target_link_libraries(${PROJECT_NAME} spdlog::spdlog)
target_link_libraries(${PROJECT_NAME} nlohmann_json::nlohmann_json)
target_link_libraries(${PROJECT_NAME} ${ASG_DLL})

get_filename_component(DLL_DIR "${PROJECT_SOURCE_DIR}/lib/ASG8X00_x64.dll" DIRECTORY)
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different "${PROJECT_SOURCE_DIR}/lib/ASG8X00_x64.dll" $<TARGET_FILE_DIR:${PROJECT_NAME}>)

#add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
#        COMMAND ${CMAKE_COMMAND} -E copy_directory
#        ${CMAKE_SOURCE_DIR}/qrc/icons
#        $<TARGET_FILE_DIR:${PROJECT_NAME}>/icons)
#add_custom_target(copy_target ALL DEPENDS ${CMAKE_SOURCE_DIR}/qrc/icons)