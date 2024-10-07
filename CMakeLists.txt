cmake_minimum_required(VERSION 3.16)

project(
  WLDU
  VERSION 0.1
  LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.5 REQUIRED COMPONENTS Quick)

qt_standard_project_setup(REQUIRES 6.5)

qt_add_executable(appWLDU src/main.cpp)

qt_add_qml_module(
  appWLDU
  URI
  WLDU
  VERSION
  1.0
  QML_FILES
  src/Main.qml
  src/Profile.qml
  #
)

target_link_libraries(appWLDU PRIVATE Qt6::Quick)

add_custom_target(
  Run
  COMMAND appWLDU
  COMMENT "Running..."
  USES_TERMINAL)
