cmake_minimum_required(VERSION 3.10)
project(Jogo CXX)

set(CMAKE_CXX_STANDARD 98)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(SFML_ROOT "${CMAKE_SOURCE_DIR}/SFML-2.3.2")
set(SFML_INCLUDE_DIR "${SFML_ROOT}/include")
set(SFML_LIBRARY_DIR "${SFML_ROOT}/lib")
set(SFML_BINARY_DIR "${SFML_ROOT}/bin")

file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.h")

add_executable(Jogo ${SOURCES}
)

target_include_directories(Jogo PRIVATE
        src
        "${SFML_INCLUDE_DIR}"
)

target_link_libraries(Jogo PRIVATE
        debug "${SFML_LIBRARY_DIR}/sfml-graphics-d.lib"
        debug "${SFML_LIBRARY_DIR}/sfml-window-d.lib"
        debug "${SFML_LIBRARY_DIR}/sfml-system-d.lib"
        debug "${SFML_LIBRARY_DIR}/sfml-audio-d.lib"
        optimized "${SFML_LIBRARY_DIR}/sfml-graphics.lib"
        optimized "${SFML_LIBRARY_DIR}/sfml-window.lib"
        optimized "${SFML_LIBRARY_DIR}/sfml-system.lib"
        optimized "${SFML_LIBRARY_DIR}/sfml-audio.lib"
)

if(WIN32)
    add_custom_command(TARGET Jogo POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "$<$<CONFIG:Debug>:${SFML_BINARY_DIR}/sfml-graphics-d-2.dll>"
            "$<$<NOT:$<CONFIG:Debug>>:${SFML_BINARY_DIR}/sfml-graphics-2.dll>"
            "$<TARGET_FILE_DIR:Jogo>"
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "$<$<CONFIG:Debug>:${SFML_BINARY_DIR}/sfml-window-d-2.dll>"
            "$<$<NOT:$<CONFIG:Debug>>:${SFML_BINARY_DIR}/sfml-window-2.dll>"
            "$<TARGET_FILE_DIR:Jogo>"
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "$<$<CONFIG:Debug>:${SFML_BINARY_DIR}/sfml-system-d-2.dll>"
            "$<$<NOT:$<CONFIG:Debug>>:${SFML_BINARY_DIR}/sfml-system-2.dll>"
            "$<TARGET_FILE_DIR:Jogo>"
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "$<$<CONFIG:Debug>:${SFML_BINARY_DIR}/sfml-audio-d-2.dll>"
            "$<$<NOT:$<CONFIG:Debug>>:${SFML_BINARY_DIR}/sfml-audio-2.dll>"
            "$<TARGET_FILE_DIR:Jogo>"
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${SFML_BINARY_DIR}/openal32.dll"
            "$<TARGET_FILE_DIR:Jogo>"
    )
endif()