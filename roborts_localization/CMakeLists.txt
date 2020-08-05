project(roborts_localization)
cmake_minimum_required(VERSION 3.1)

set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_STANDARD 14)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake_module)

find_package(Threads REQUIRED)
find_package(Eigen3 REQUIRED)
find_package(Glog REQUIRED)
find_package(catkin REQUIRED COMPONENTS
  roscpp
  tf
  nav_msgs
  message_filters
  sensor_msgs
  )
catkin_package()

add_subdirectory(amcl)

include_directories(
    src
)

add_executable(
    localization_node
    localization_node.cpp
    localization_node.h
    localization_math.cpp
    )

target_link_libraries(localization_node
        PUBLIC
        localization::amcl
        ${EIGEN3_LIBRARIES}
        ${catkin_LIBRARIES}
        ${GLOG_LIBRARY}
        )

target_include_directories(localization_node PUBLIC
        ${catkin_INCLUDE_DIRS})


add_executable(
        uwb_node
        uwb_node.cpp
        uwb_publisher.cpp
)

target_link_libraries(uwb_node
        PUBLIC
        roborts_base
        ${catkin_LIBRARIES}
        ${GLOG_LIBRARY}
        )

target_include_directories(uwb_node PUBLIC
        ../roborts_base/roborts_sdk/hardware
        ${catkin_INCLUDE_DIRS}
        )



 install(TARGETS localization_node
   ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
   LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
   RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
 )

install(TARGETS uwb_node
        ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
        LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
        RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
        )

 install(DIRECTORY config
            DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}
    )

 install(DIRECTORY amcl
            DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}
    )
