cmake_minimum_required(VERSION 2.8.3)
project(multires_image)

set(DEPS
  mapviz
  roscpp
  swri_math_util
  swri_transform_util
  swri_yaml_util
  tf
)

find_package(catkin REQUIRED COMPONENTS ${DEPS})

find_package(Boost REQUIRED thread)
find_package(OpenCV 3.1 COMPONENTS core imgproc REQUIRED)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5OpenGL REQUIRED)
find_package(Qt5Widgets REQUIRED)
set(QT_LIBRARIES
  ${Qt5Core_LIBRARIES}
  ${Qt5Gui_LIBRARIES}
  ${Qt5OpenGL_LIBRARIES}
  ${Qt5Widgets_LIBRARIES}
)
add_definitions(
  ${Qt5Core_DEFINITIONS}
  ${Qt5Gui_DEFINITIONS}
  ${Qt5OpenGL_DEFINITIONS}
  ${Qt5Widgets_DEFINITIONS}
)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")

catkin_package(
  INCLUDE_DIRS include
  LIBRARIES ${PROJECT_NAME} multires_widget
  DEPENDS Qt5Core Qt5Gui Qt5OpenGL Qt5Widgets
  CATKIN_DEPENDS
    ${DEPS}
)

include_directories(include
    ${catkin_INCLUDE_DIRS}
    ${OpenCV_INCLUDE_DIRS}
)

# Build libtile_cache
set(TILE_SRC_FILES
   src/tile.cpp
   src/tile_cache.cpp
   src/tile_set.cpp
   src/tile_set_layer.cpp)
qt5_wrap_cpp(TILE_SRC_FILES include/${PROJECT_NAME}/tile_cache.h)
add_library(${PROJECT_NAME} ${TILE_SRC_FILES})
target_link_libraries(${PROJECT_NAME}
    ${catkin_LIBRARIES}
    ${Boost_LIBRARIES}
    ${OpenCV_LIBS}
    Qt5::Core
    Qt5::Gui
    Qt5::Widgets
    Qt5::OpenGL
)

# Build libmultires_widget
set(UI_FILES
    src/multires_config.ui
    src/QGLMap.ui
)
set(WIDGET_SRC_FILES
    src/tile_view.cpp
    src/QGLMap.cpp)
qt5_wrap_ui(WIDGET_SRC_FILES ${UI_FILES})
qt5_wrap_cpp(WIDGET_SRC_FILES include/${PROJECT_NAME}/QGLMap.h)
add_library(multires_widget ${WIDGET_SRC_FILES})
target_link_libraries(multires_widget
    ${PROJECT_NAME}
)

# Build nodes
set(VIEW_NODE_SRC_FILES src/nodes/multires_view_node.cpp)
qt5_wrap_cpp(VIEW_NODE_SRC_FILES include/${PROJECT_NAME}/multires_view_node.h)
add_executable(multires_view_node ${VIEW_NODE_SRC_FILES})
target_link_libraries(multires_view_node
    multires_widget
)

# Build mapviz plugin
set(MAPVIZ_SRC_FILES
  src/multires_image_plugin.cpp
  src/multires_view.cpp)
qt5_wrap_ui(MAPVIZ_SRC_FILES src/multires_config.ui)
qt5_wrap_cpp(MAPVIZ_SRC_FILES include/${PROJECT_NAME}/multires_image_plugin.h)
add_library(${PROJECT_NAME}_plugin ${MAPVIZ_SRC_FILES})
target_link_libraries(${PROJECT_NAME}_plugin
    ${PROJECT_NAME}
)

install(DIRECTORY include/${PROJECT_NAME}/
  DESTINATION ${CATKIN_PACKAGE_INCLUDE_DESTINATION}
  FILES_MATCHING PATTERN "*.h"
)

install(TARGETS ${PROJECT_NAME} multires_view_node multires_widget ${PROJECT_NAME}_plugin
  RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
  LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
)

install(DIRECTORY launch
  DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}
)

install(FILES mapviz_plugins.xml
  DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION})
