cmake_minimum_required ( VERSION 3.1 )
set (L4_PROJECT_NAME Lighting4)

project(${L4_PROJECT_NAME})

set(CMAKE_BUILD_TYPE "Release" CACHE STRING "")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_DEBUG} /MT")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELEASE} /MT")
set(CMAKE_GENERATOR_PLATFORM x64)

set(allegro false CACHE BOOL "Link to allegro")
if (allegro)
    set(allegincludedir "" CACHE STRING "The directory of allegro includes")
    set(alleglibdir "" CACHE STRING "The directories of allegro's libraries")
    if (UNIX)
        set(alleglib "liballegro.a" CACHE STRING "The path of the main allegro library relative to allegdir")
        set(ttflib "liballegro_ttf.a" CACHE STRING "The path of the ttf allegro extension relative to allegdir")
        set(imglib "liballegro_image.a" CACHE STRING "The path of the img allegro extension relative to allegdir")
        set(primlib "liballegro_primitives.a" CACHE STRING "The path of the primtive allegro extension relative to allegdir")
        set(fontlib "liballegro_font.a" CACHE STRING "The path of the font allegro extension relative to allegdir")
    else()
        set(alleglib "allegro-static.lib" CACHE STRING "The path of the main allegro library relative to allegdir")
        set(ttflib "allegro_ttf-static.lib" CACHE STRING "The path of the ttf allegro extension relative to allegdir")
        set(imglib "allegro_image-static.lib" CACHE STRING "The path of the img allegro extension relative to allegdir")
        set(primlib "allegro_primitives-static.lib" CACHE STRING "The path of the primtive allegro extension relative to allegdir")
        set(fontlib "allegro_font-static.lib" CACHE STRING "The path of the font allegro extension relative to allegdir")
    endif()
endif()

set(examples false CACHE BOOL "Builds examples")

set(HEADER_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lighting4)
set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

set(HEADERS
	${HEADER_DIR}/AboveLightBlocker.h
    ${HEADER_DIR}/AboveLightSource.h
    ${HEADER_DIR}/AboveShadePoint.h
    ${HEADER_DIR}/CircleLightSource.h
    ${HEADER_DIR}/CircleShadePoint.h
    ${HEADER_DIR}/DirectionalLightSource.h
    ${HEADER_DIR}/DirectionalLightSource.h
    ${HEADER_DIR}/GaussianBlurrer.h
    ${HEADER_DIR}/GaussianKernelData.h
    ${HEADER_DIR}/LightBlocker.h
    ${HEADER_DIR}/LightBlockerContainer.h
    ${HEADER_DIR}/LightLayer.h
    ${HEADER_DIR}/LightRunnable.h
    ${HEADER_DIR}/LightSource.h
    ${HEADER_DIR}/ShadePoint.h)

set(SOURCES
	${SOURCE_DIR}/AboveLightBlocker.cpp
    ${SOURCE_DIR}/AboveLightSource.cpp
    ${SOURCE_DIR}/AboveShadePoint.cpp
    ${SOURCE_DIR}/CircleLightSource.cpp
    ${SOURCE_DIR}/CircleShadePoint.cpp
    ${SOURCE_DIR}/DirectionalLightSource.cpp
    ${SOURCE_DIR}/GaussianBlurrer.cpp
    ${SOURCE_DIR}/GaussianKernelData.cpp
    ${SOURCE_DIR}/LightBlocker.cpp
    ${SOURCE_DIR}/LightBlockerContainer.cpp
    ${SOURCE_DIR}/LightLayer.cpp
    ${SOURCE_DIR}/LightRunnable.cpp
    ${SOURCE_DIR}/LightSource.cpp
    ${SOURCE_DIR}/ShadePoint.cpp)

include_directories(
    ${HEADER_DIR})

include_directories(
    ${allegincludedir})

add_library(${L4_PROJECT_NAME} STATIC ${HEADERS} ${SOURCES})

target_link_libraries(${L4_PROJECT_NAME}
    ${alleglibdir}${alleglib}
    ${alleglibdir}${ttflib}
    ${alleglibdir}${imglib}
    ${alleglibdir}${primlib}
    ${alleglibdir}${fontlib})

if (${examples})
    add_subdirectory(examples/example1)
endif()

set_property(TARGET ${L4_PROJECT_NAME} PROPERTY CXX_STANDARD 11)
set_property(TARGET ${L4_PROJECT_NAME} PROPERTY CXX_STANDARD_REQUIRED 11)