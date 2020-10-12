cmake_minimum_required(VERSION 3.0.2)
project(wtr_serial)

# 设置C++标准
set(CMAKE_CXX_STANDARD 11)
add_compile_options(-std=c++11)

# 链接头文件
include_directories(${PROJECT_SOURCE_DIR}/include)



find_package(serial)

find_package(catkin REQUIRED COMPONENTS
        roscpp
        rospy
        std_msgs
        pcl_ros
        cv_bridge
        )


catkin_package(
        #  INCLUDE_DIRS include
        #  LIBRARIES obeject_detect
        #  CATKIN_DEPENDS roscpp rpspy std_msgs
        #  DEPENDS system_lib
)

###########
## Build ##
###########

include_directories(
        include ${catkin_INCLUDE_DIRS}
        ${catkin_INCLUDE_DIRS}
)



set(THIRD_PARTY_LIBS
        ${serial_LIBRARIES}
        ${catkin_LIBRARIES}
        )


add_executable(main App/main.cpp)
add_dependencies(main ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
target_link_libraries(main
        ${THIRD_PARTY_LIBS}
        )

add_executable(controller App/controller.cpp)
add_dependencies(controller ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
target_link_libraries(controller
        ${THIRD_PARTY_LIBS}
        )
