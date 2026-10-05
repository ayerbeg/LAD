#----------------------------------------------------------------------------
# Setup the project

cmake_minimum_required(VERSION 3.28.2)
#cmake_minimum_required(VERSION 2.8...3.27)

project(LAD)

#----------------------------------------------------------------------------
# Find Geant4 package, activating all available UI and Vis drivers by default
# You can set WITH_GEANT4_UIVIS to OFF via the command line or ccmake/cmake-gui
# to build a batch mode only executable
#
option(WITH_GEANT4_UIVIS "Build example with Geant4 UI and Vis drivers" ON)
if(WITH_GEANT4_UIVIS)
  find_package(Geant4 REQUIRED ui_all vis_all)
else()
  find_package(Geant4 REQUIRED)
endif()

#----------------------------------------------------------------------------
# Find ROOT (required package)
#
find_package(ROOT REQUIRED)

#----------------------------------------------------------------------------
# Setup Geant4 include directories and compile definitions
# Setup include directory for this project
#
include(${Geant4_USE_FILE})

include_directories(${PROJECT_SOURCE_DIR}/include
                    ${PROJECT_SOURCE_DIR}/shared/include
                    ${Geant4_INCLUDE_DIR}
                    ${ROOT_INCLUDE_DIRS})

file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc
                  ${PROJECT_SOURCE_DIR}/shared/src/*.cc)

#----------------------------------------------------------------------------
# Locate sources and headers for this project
# NB: headers are included so they will show up in IDEs
#
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

#----------------------------------------------------------------------------
# Add the executable, and link it to the Geant4 libraries
#
add_executable(LAD LAD.cc ${sources} ${headers})
target_link_libraries(LAD ${Geant4_LIBRARIES} ${ROOT_LIBRARIES} )

#----------------------------------------------------------------------------
# Optional, machine-specific: force the Qt GUI to use X11 (XWayland) instead
# of native Wayland, where the Geant4 OpenGL Qt viewer may not render properly.
# Default OFF. Enable with -DLAD_FORCE_X11=ON. An explicitly set
# QT_QPA_PLATFORM in the environment always takes precedence.
#
option(LAD_FORCE_X11 "Force Qt GUI to use X11/XWayland (QT_QPA_PLATFORM=xcb)" OFF)
if(LAD_FORCE_X11)
  target_compile_definitions(LAD PRIVATE LAD_FORCE_X11)
  message(WARNING "LAD_FORCE_X11=ON: LAD will force the Qt GUI to use X11/XWayland "
                  "(QT_QPA_PLATFORM=xcb) unless QT_QPA_PLATFORM is already set. "
                  "This is a machine-specific workaround; disable with -DLAD_FORCE_X11=OFF.")
elseif(DEFINED ENV{WAYLAND_DISPLAY})
  message(STATUS "LAD: Wayland session detected. If the Geant4 Qt viewer does not "
                 "render properly, run with QT_QPA_PLATFORM=xcb or reconfigure with "
                 "-DLAD_FORCE_X11=ON.")
endif()

#----------------------------------------------------------------------------
# Copy all scripts to the build directory, i.e. the directory in which we
# build B4d. This is so that we can run the executable directly because it
# relies on these scripts being in the current working directory.
#
set(LAD_SCRIPTS
  LAD.ini
  LAD.out
  LAD.in
  gui.mac
  init_vis.mac
  run1.mac
  run2.mac
  vis.mac
  )

foreach(_script ${LAD_SCRIPTS})
  configure_file(
    ${PROJECT_SOURCE_DIR}/${_script}
    ${PROJECT_BINARY_DIR}/${_script}
    COPYONLY
    )
endforeach()

#----------------------------------------------------------------------------
# Install the executable to 'bin' directory under CMAKE_INSTALL_PREFIX
#
#install(TARGETS LAD DESTINATION bin)
