cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
project(pcl-dev)


#pcl
find_package(PCL 1.7 REQUIRED COMPONENTS filters common kdtree octree search surface sample_consensus io)
message(" pcl defauts: ${PCL_INCLUDE_DIRS}")

include_directories(${PCL_INCLUDE_DIRS})
link_directories(${PCL_LIBRARY_DIRS})
add_definitions(${PCL_DEFINITIONS})


#maxmsp
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")
find_package(MaxSDK REQUIRED)
include_directories(${MAXSDK_MAX_INCLUDE_DIR} ${MAXSDK_JIT_INCLUDE_DIR})
link_directories(${MAXSDK_MAX_INCLUDE_DIR} ${MAXSDK_JIT_INCLUDE_DIR})

add_library(pcd_write_test STATIC pcd_write.cpp ${MAXSDK_COMMON})
target_link_libraries(pcd_write_test ${Boost_SYSTEM_LIBRARY_RELEASE} ${PCL_FILTERS_LIBRARY} ${PCL_COMMON_LIBRARY} ${PCL_KDTREE_LIBRARY} ${PCL_SEARCH_LIBRARY} ${PCL_SURFACE_LIBRARY} ${PCL_IO_LIBRARY})
set_target_properties(pcd_write_test PROPERTIES SUFFIX ".mxe")
