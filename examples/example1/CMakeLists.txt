cmake_minimum_required ( VERSION 3.1 )
set (EX1_PROJECT_NAME Example1)

project(${EX1_PROJECT_NAME})

set(CMAKE_BUILD_TYPE "Release" CACHE STRING "")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_RELEASE} /MTd")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
set(CMAKE_GENERATOR_PLATFORM x64)

set(allegro false CACHE BOOL "Link to allegro")
if (allegro)
    set(allegincludedir "" CACHE STRING "The directory of allegro includes")
    set(alleglibdir "" CACHE STRING "The directories of allegro's libraries")
    if (UNIX)
        set(alleglib "liballegro.so" CACHE STRING "The path of the main allegro library relative to allegdir")
        set(ttflib "liballegro_ttf.so" CACHE STRING "The path of the ttf allegro extension relative to allegdir")
        set(imglib "liballegro_image.so" CACHE STRING "The path of the img allegro extension relative to allegdir")
        set(primlib "liballegro_primitives.so" CACHE STRING "The path of the primtive allegro extension relative to allegdir")
        set(fontlib "liballegro_font.so" CACHE STRING "The path of the font allegro extension relative to allegdir")
    else()
        set(alleglib "allegro-static.lib" CACHE STRING "The path of the main allegro library relative to allegdir")
        set(ttflib "allegro_ttf-static.lib" CACHE STRING "The path of the ttf allegro extension relative to allegdir")
        set(imglib "allegro_image-static.lib" CACHE STRING "The path of the img allegro extension relative to allegdir")
        set(primlib "allegro_primitives-static.lib" CACHE STRING "The path of the primtive allegro extension relative to allegdir")
        set(fontlib "allegro_font-static.lib" CACHE STRING "The path of the font allegro extension relative to allegdir")
    endif()
endif()

set(allegextincludedir "${CMAKE_CURRENT_SOURCE_DIR}/AllegroExt/AllegroExt" CACHE STRING "Include directory of AllegroExt")
set(allegextlibdir "" CACHE STRING "Directory of AllegroExt")
set(allegextlib "AllegroExt" CACHE STRING "Name of AllegroExt library")

set(lightincludedir "${CMAKE_CURRENT_SOURCE_DIR}/../../lighting4" CACHE STRING "Include directory of Lighting4")
set(lightlibdir "" CACHE STRING "Directory of Lighting4 libraries")
set(lightlib "Lighting4" CACHE STRING "Name of Lighting4 library")

set(HEADER_DIR ${CMAKE_CURRENT_SOURCE_DIR}/example1)
set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

set(HEADERS
	${HEADER_DIR}/TestCore.h)

set(SOURCES
	${SOURCE_DIR}/Source.cpp
    ${SOURCE_DIR}/TestCore.cpp)

include_directories(
    ${HEADER_DIR})
    
add_subdirectory(AllegroExt)

include_directories(
    ${allegincludedir}
    ${allegextincludedir}
    ${lightincludedir})

add_executable(${EX1_PROJECT_NAME} ${HEADERS} ${SOURCES})

target_link_libraries(${EX1_PROJECT_NAME}
    ${alleglibdir}${alleglib}
    ${alleglibdir}${ttflib}
    ${alleglibdir}${imglib}
    ${alleglibdir}${primlib}
    ${alleglibdir}${fontlib}
    ${allegextlibdir}${allegextlib}
    ${lightlibdir}${lightlib}
)

set_property(TARGET ${EX1_PROJECT_NAME} PROPERTY CXX_STANDARD 11)
set_property(TARGET ${EX1_PROJECT_NAME} PROPERTY CXX_STANDARD_REQUIRED 11)

add_custom_command(TARGET ${EX1_PROJECT_NAME} PRE_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                       ${CMAKE_CURRENT_SOURCE_DIR}/wdir ${CMAKE_CURRENT_BINARY_DIR})