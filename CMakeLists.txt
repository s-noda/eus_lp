project(eus_lp)

cmake_minimum_required(VERSION 2.4.6)

find_package(catkin COMPONENTS cmake_modules)
## find_package(Boost COMPONENTS thread system REQUIRED)

execute_process(COMMAND cmake -E chdir ${PROJECT_SOURCE_DIR} bash install.sh)

set(LPSOLVE_INCLUDE_DIRS /usr/include/lpsolve)
set(LPSOLVE_LIBRARY_DIRS /usr/lib/lp_solve/liblpsolve55.so)
set(CLP_INCLUDE_DIRS /usr/include/coin)
set(CLP_LIBRARY_DIRS /usr/lib/libClp.so)
## set(MUMPS_LIBRARY_DIRS /usr/lib/libmumps_common.so)

## include_directories(${Boost_INCLUDE_DIRS})
include_directories(${LPSOLVE_INCLUDE_DIRS})
include_directories(${CLP_INCLUDE_DIRS})
include_directories(${PROJECT_SOURCE_DIR}/src)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

catkin_package()

set(CMAKE_CXX_FLAGS "-O3")

add_executable(eus_lpsolve_test src/lpsolve_test.cpp)
TARGET_LINK_LIBRARIES(eus_lpsolve_test ${LPSOLVE_LIBRARY_DIRS})
add_executable(eus_clp_test src/clp_test.cpp)
TARGET_LINK_LIBRARIES(eus_clp_test ${CLP_LIBRARY_DIRS})

# add_library(eus_ompl SHARED src/eus_ompl_lib.cpp)
# TARGET_LINK_LIBRARIES(eus_ompl ${OMPL_LIBRARIES} ${Boost_LIBRARIES})
