# CMakeList.txt
cmake_minimum_required (VERSION 2.8)

#
# 프로젝트 이름 지정
project ("imageMagickSamples")
message(STATUS "\${PROJECT_NAME} = ${PROJECT_NAME}")

option(BUILD_IMAGE_MAGICK_SAMPLE01 "Enable building imageMagickSample01" ON)
option(BUILD_IMAGE_MAGICK_SAMPLE02 "Enable building imageMagickSample02" ON)
option(BUILD_IMAGE_MAGICK_SAMPLE03 "Enable building imageMagickSample03" ON)
option(BUILD_IMAGE_MAGICK_SAMPLE04 "Enable building imageMagickSample04" ON)

if(BUILD_IMAGE_MAGICK_SAMPLE01)
    add_subdirectory(imageMagickSample01)
endif()
if(BUILD_IMAGE_MAGICK_SAMPLE02)
    add_subdirectory(imageMagickSample02)
endif()
if(BUILD_IMAGE_MAGICK_SAMPLE03)
    add_subdirectory(imageMagickSample03)
endif()
if(BUILD_IMAGE_MAGICK_SAMPLE04)
    add_subdirectory(imageMagickSample04)
endif()

set(CPACK_RESOURCE_FILE_README "${CMAKE_CURRENT_SOURCE_DIR}/README.md")
install(FILES ${CPACK_RESOURCE_FILE_README} DESTINATION .)
