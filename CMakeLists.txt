cmake_minimum_required(VERSION 3.21)
project(verlet-sdl)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
if(NOT MSVC)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
endif()

if(WIN32)
  list(APPEND CMAKE_PREFIX_PATH "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty")
  set(THIRDPARTY_DIR "${CMAKE_SOURCE_DIR}/thirdparty")
  set(SDL2_TTF_DIR "${THIRDPARTY_DIR}/SDL2_ttf-2.20.2")
  set(SDL2_IMG_DIR "${THIRDPARTY_DIR}/SDL2_image-2.8.2")
  set(Eigen3_DIR "${THIRDPARTY_DIR}/eigen-3.4.0/")
endif()

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)
# find_package(Eigen3 REQUIRED)
#
# If downloading Eigen from source, you will need to run CMake in Eigen before
# find_package(Eigen3 REQUIRED) will work.
#
# Then, remove the include to ${Eigen3_DIR} and add Eigen3::Eigen to link
# libraries.

file(GLOB_RECURSE SOURCES "src/*.cpp")

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(
  ${PROJECT_NAME} PRIVATE ${SDL2_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR}/src
                          ${Eigen3_DIR})
# Add Eigen3::Eigen
target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES} SDL2_image::SDL2_image
                      SDL2_ttf::SDL2_ttf)

add_custom_command(
  TARGET ${PROJECT_NAME}
  POST_BUILD
  COMMAND
    ${CMAKE_COMMAND} -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/assets"
    $<TARGET_FILE_DIR:${PROJECT_NAME}>"/assets")
if(WIN32)
  add_custom_command(
    TARGET ${PROJECT_NAME}
    POST_BUILD
    COMMAND
      ${CMAKE_COMMAND} -E copy_if_different "${SDL2_DIR}/../lib/x64/SDL2.dll"
      $<TARGET_FILE_DIR:${PROJECT_NAME}>)
  add_custom_command(
    TARGET ${PROJECT_NAME}
    POST_BUILD
    COMMAND
      ${CMAKE_COMMAND} -E copy_if_different
      "${SDL2_IMG_DIR}/lib/x64/SDL2_image.dll"
      $<TARGET_FILE_DIR:${PROJECT_NAME}>)
  add_custom_command(
    TARGET ${PROJECT_NAME}
    POST_BUILD
    COMMAND
      ${CMAKE_COMMAND} -E copy_if_different
      "${SDL2_TTF_DIR}/lib/x64/SDL2_ttf.dll" $<TARGET_FILE_DIR:${PROJECT_NAME}>)
endif()
