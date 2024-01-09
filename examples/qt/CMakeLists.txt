cmake_minimum_required(VERSION 3.21)
project(FinClipQtDemo)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(FINCLIP_QTDEMO_TARGET "FinClipQtDemo")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")

if(${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")
  # 在arm下编译x86的包需要特殊处理
  if(PROJECT_ARCH STREQUAL "x86_64")
    set(CMAKE_OSX_ARCHITECTURES "x86_64" CACHE INTERNAL "" FORCE)
  endif()
endif()

set(FINCLIP_QTDEMO_SRCS src/main.cc)

add_executable(${FINCLIP_QTDEMO_TARGET} ${FINCLIP_QTDEMO_SRCS})

if(NOT DEFINED PROJECT_ARCH)
  set(PROJECT_ARCH ${CMAKE_SYSTEM_PROCESSOR})
endif()

add_library( finclip_wrapper SHARED IMPORTED )

if(${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
  if(PROJECT_ARCH STREQUAL "x86_64")
    set(FINCLIP_SDK_PATH "${PROJECT_SOURCE_DIR}/../../vendor/win/x86_64")
    set_target_properties(finclip_wrapper PROPERTIES IMPORTED_LOCATION "${FINCLIP_SDK_PATH}/FinClipSDKWrapper.dll")
    set_target_properties(finclip_wrapper PROPERTIES IMPORTED_IMPLIB "${FINCLIP_SDK_PATH}/FinClipSDKWrapper.lib")
    set(Qt5_DIR "C:/Qt/5.15.2/msvc2019_64/lib/cmake/Qt5")
  elseif(PROJECT_ARCH STREQUAL "x86")
    set(FINCLIP_SDK_PATH "${PROJECT_SOURCE_DIR}/../../vendor/win/x86")
    set(Qt5_DIR "C:/Qt/5.15.2/msvc2019/lib/cmake/Qt5")
  else()
    
  endif()
  set_target_properties(finclip_wrapper PROPERTIES IMPORTED_LOCATION "${FINCLIP_SDK_PATH}/FinClipSDKWrapper.dll")
  set_target_properties(finclip_wrapper PROPERTIES IMPORTED_IMPLIB "${FINCLIP_SDK_PATH}/FinClipSDKWrapper.lib")
  add_custom_command(TARGET ${FINCLIP_QTDEMO_TARGET} POST_BUILD        # Adds a post-build event to MyTest
    COMMAND ${CMAKE_COMMAND} -E copy_if_different  # which executes "cmake - E copy_if_different..."
    "${FINCLIP_SDK_PATH}/FinClipSDKWrapper.dll"      # <--this is in-file
    $<TARGET_FILE_DIR:${FINCLIP_QTDEMO_TARGET}>)                 # <--this is out-file path
elseif(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
  if(PROJECT_ARCH STREQUAL "x86_64")
    set(FINCLIP_SDK_PATH "${PROJECT_SOURCE_DIR}/../../vendor/linux/x86_64")
    set_target_properties(finclip_wrapper PROPERTIES IMPORTED_LOCATION "${FINCLIP_SDK_PATH}/libFinClipSDKWrapper.so")
  else()
    # arm64
    set(FINCLIP_SDK_PATH "${PROJECT_SOURCE_DIR}/../../vendor/linux/arm64")
    set_target_properties(finclip_wrapper PROPERTIES IMPORTED_LOCATION "${FINCLIP_SDK_PATH}/libFinClipSDKWrapper.so")
  endif()
elseif(${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")
  if(PROJECT_ARCH STREQUAL "x86_64")
    # intel
    set(FINCLIP_SDK_PATH "${PROJECT_SOURCE_DIR}/../../vendor/mac/intel")
    set_target_properties(finclip_wrapper PROPERTIES IMPORTED_LOCATION "${FINCLIP_SDK_PATH}/libFinClipSDKWrapper.so")
  else()
    # arm64
    set(FINCLIP_SDK_PATH "${PROJECT_SOURCE_DIR}/../../vendor/mac/arm64")
    set_target_properties(finclip_wrapper PROPERTIES IMPORTED_LOCATION "${FINCLIP_SDK_PATH}/libFinClipSDKWrapper.so")
  endif()
else()
    message("Operating System: Unknown")
endif()
find_package(
  Qt5
  COMPONENTS Widgets Gui
  REQUIRED)
target_include_directories(${FINCLIP_QTDEMO_TARGET} PRIVATE "${FINCLIP_SDK_PATH}")

message(STATUS "cmake configuration is ${CMAKE_SYSTEM_NAME} ${PROJECT_ARCH} ${FINCLIP_SDK_PATH} ${Qt5_DIR}")

# 
# include_directories(../vendor/json)

target_link_libraries(
  ${FINCLIP_QTDEMO_TARGET}
  finclip_wrapper
  Qt5::Widgets
  Qt5::Gui
)






