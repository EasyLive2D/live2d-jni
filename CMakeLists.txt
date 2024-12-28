cmake_minimum_required(VERSION 3.13)

project(LAppModelWrapper)

set(CMAKE_BUILD_TYPE "Release")
set(ENABLE_LOG TRUE)

message("${CMAKE_BUILD_TYPE}")

set(LIB_NAME LAppModelWrapper)
set(PROJECT_ROOT ${CMAKE_CURRENT_SOURCE_DIR})
set(RES_PATH ${CMAKE_CURRENT_SOURCE_DIR}/Resources)

set(CMAKE_CONFIGURATION_TYPES "Release")

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
  set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/static)
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /Zi")
  set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} /Zi")
  add_compile_options("/utf-8" "/wd4018" "/wd4244" "/wd4996")
  add_link_options("/NODEFAULTLIB:LIBCMT" "/DEBUG")
else()
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib/${CMAKE_BUILD_TYPE})
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE})
  set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/static/${CMAKE_BUILD_TYPE})
endif()  

add_subdirectory(Main)

# 创建 Java 扩展模块
add_library(LAppModelWrapper SHARED lapp_model.cpp live2d.cpp)
target_link_libraries(LAppModelWrapper PRIVATE Main)
target_include_directories(LAppModelWrapper PRIVATE 
  D:/jdk/jdk-17.0.9/include 
  D:/jdk/jdk-17.0.9/include/win32
)

# Configure for Linux / Windows
if(CMAKE_SYSTEM_NAME MATCHES "Linux")
  set(MODULE_NAME lib${LIB_NAME}.so)
  set(OUTPUT_NAME live2d.so)
else(CMAKE_SYSTEM_NAME MATCHES "Windows")
  set(MODULE_NAME ${LIB_NAME}.dll)
  set(OUTPUT_NAME live2d.dll)
endif()

# Post build commands
add_custom_command(
  TARGET ${LIB_NAME}
  POST_BUILD
  COMMAND
  ${CMAKE_COMMAND} -E
    copy $<TARGET_FILE_DIR:${LIB_NAME}>/${MODULE_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/live2d-lwjgl-example/live2d/src/main/resources/${OUTPUT_NAME}

  COMMAND
  ${CMAKE_COMMAND} -E
    copy ${CMAKE_CURRENT_SOURCE_DIR}/com/arkueid/live2d/Live2D_v3.java ${CMAKE_CURRENT_SOURCE_DIR}/live2d-lwjgl-example/live2d/src/main/java/com/arkueid/live2d/Live2D_v3.java
)
