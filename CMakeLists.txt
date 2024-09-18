cmake_minimum_required(VERSION 3.20)
project(AUTOr3pair)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_BUILD_TYPE "Release")
set(CGAL_DATA_DIR ".")

# CGAL
find_package( CGAL QUIET COMPONENTS )
if ( CGAL_FOUND )
    include( ${CGAL_USE_FILE} )
    message(STATUS "CGAL found")
    message(STATUS ${CGAL_LIBRARIES})
    message(STATUS ${CGAL_3RD_PARTY_LIBRARIES})
else()
    message(SEND_ERROR "AUTOr3pair requires the CGAL library")
    return()
endif()

find_package(CGAL REQUIRED OPTIONAL_COMPONENTS Qt5)

add_definitions(-DCGAL_USE_BASIC_VIEWER -DQT_NO_KEYWORDS)

include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty)
# Set VAL3DITY_LIBRARY to ON to build val3dity as a library
set(VAL3DITY_LIBRARY ON CACHE BOOL "Build val3dity as a library" FORCE)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../val3dity/ build)

FILE(GLOB SRC_FILES src/*.cpp src/*.h)
add_executable(AUTOr3pair ${SRC_FILES})

target_link_libraries(AUTOr3pair LINK_PUBLIC val3dity CGAL::CGAL CGAL::CGAL_Qt5)



