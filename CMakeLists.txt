cmake_minimum_required(VERSION 3.16)

project(
  WLDU
  VERSION 0.1
  LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.5 REQUIRED COMPONENTS Quick Sql)
qt_policy(SET QTP0001 NEW)

qt_standard_project_setup(REQUIRES 6.5)

qt_add_executable(appWLDU src/main.cpp include/Schedule/ScheduleModel.h
                  src/Schedule/ScheduleModel.cpp)

qt_add_qml_module(
  appWLDU
  URI
  WLDU
  VERSION
  1.0
  QML_FILES
  src/Main.qml
  src/Profile.qml
  src/EditProfile.qml
  src/CutProfilePic.qml
  src/Welcome.qml
  src/Register.qml
  src/Account_SingUp.qml
  src/Student_SignUp.qml
  src/Personal_SignUp.qml
  src/Schedule/Schedule.qml
  RESOURCES
  assets/icons/back.svg
  assets/icons/edit-profile.svg
  assets/icons/user-large.svg
  assets/icons/circle-user.svg
  assets/images/wldu_logo.png
  assets/icons/schedule.svg
  assets/icons/angle-left.svg
  assets/icons/angle-right.svg
  assets/icons/question.svg
  assets/icons/check.svg
  SOURCES
  qmltypes/include/CopyFile.h
  qmltypes/src/CopyFile.cpp
  #
)

target_include_directories(appWLDU PRIVATE include qmltypes/include)
target_link_libraries(appWLDU PRIVATE Qt6::Quick Qt6::Sql)

add_custom_target(
  Run
  COMMAND appWLDU
  COMMENT "Running..."
  USES_TERMINAL)
