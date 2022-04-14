set(CMAKE_C_COMPILER gcc)
project(FinClipGTK C)

# Use the package PkgConfig to detect GTK+ headers/library files
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK REQUIRED gtk+-3.0)

set(FINCLIP_GTK_DEMO "FinClipGTK")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/$<$<CONFIG:Debug>:>)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/$<$<CONFIG:Debug>:>)

# Add other flags to the compiler
add_definitions(${GTK_CFLAGS_OTHER})
add_executable(${FINCLIP_GTK_DEMO} main.c)
target_include_directories(${FINCLIP_GTK_DEMO} PRIVATE ${GTK_INCLUDE_DIRS})
target_include_directories(${FINCLIP_GTK_DEMO} PRIVATE FinClipSDKWrapper)
add_dependencies(${FINCLIP_GTK_DEMO} FinClipSDKWrapper)
target_link_libraries(${FINCLIP_GTK_DEMO} FinClipSDKWrapper ${GTK_LIBRARIES})

set(OUTPUT_FILE FinClipGTK)
copy_files("${FINCLIP_GTK_DEMO}" "${OUTPUT_FILE}" "${CMAKE_BINARY_DIR}" "${CMAKE_BINARY_DIR}/core/$<CONFIGURATION>")
