project(eus_lp)

cmake_minimum_required(VERSION 2.4.6)

find_package(catkin COMPONENTS cmake_modules)
## find_package(Boost COMPONENTS thread system REQUIRED)

execute_process(COMMAND cmake -E chdir ${PROJECT_SOURCE_DIR} bash install.sh)

set(LPSOLVE_INCLUDE_DIRS /usr/include/lpsolve)
set(LPSOLVE_LIBRARY_DIRS /usr/lib/lp_solve/liblpsolve55.so)
set(MUMPS_LIBRARY_DIRS /usr/lib/libmumps_common.so) ## tmp

## include_directories(${Boost_INCLUDE_DIRS})
include_directories(${LPSOLVE_INCLUDE_DIRS})
include_directories(${PROJECT_SOURCE_DIR}/src)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

catkin_package()

add_executable(eus_lp_test src/test.cpp)
TARGET_LINK_LIBRARIES(eus_lp_test ${LPSOLVE_LIBRARY_DIRS} ${MUMPS_LIBRARY_DIRS})
# add_library(eus_ompl SHARED src/eus_ompl_lib.cpp)
# TARGET_LINK_LIBRARIES(eus_ompl ${OMPL_LIBRARIES} ${Boost_LIBRARIES})
