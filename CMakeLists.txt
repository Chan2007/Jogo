cmake_minimum_required(VERSION 3.10)
project(Jogo CXX)

set(CMAKE_CXX_STANDARD 98)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Usa o Qt informado pelo ambiente. Caso não existir, utiliza o caminho padrão da máquina local.
if(NOT DEFINED CMAKE_PREFIX_PATH)
    set(CMAKE_PREFIX_PATH "C:/Qt/5.15.2/msvc2019_64")
endif()

# Ativa os compiladores automáticos do Qt para ler a interface (.ui) e eventos (MOC)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(OpenGL REQUIRED)

# Encontra os pacotes do Qt 5 necessários para a interface
find_package(Qt5 REQUIRED COMPONENTS Core Gui Widgets Svg)

set(SFML_ROOT "${CMAKE_SOURCE_DIR}/SFML-2.6.0")
set(SFML_INCLUDE_DIR "${SFML_ROOT}/include")
set(SFML_LIBRARY_DIR "${SFML_ROOT}/lib")
set(SFML_BINARY_DIR "${SFML_ROOT}/bin")

# Procura os arquivos do jogo recursivamente
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "src/*.cpp" "src/*.h")
file(GLOB MATERIAL_WIDGETS_SRC
        "qt-material-widgets/components/*.cpp"
        "qt-material-widgets/components/lib/*.cpp"
)
file(GLOB MATERIAL_WIDGETS_HDR
        "qt-material-widgets/components/*.h"
        "qt-material-widgets/components/lib/*.h"
)

# Cria o executável incluindo as classes do Qt (mainwindow) e o arquivo de design (.ui)
add_executable(Jogo ${SOURCES}
        src/main.cpp
        src/Ente/Menu/Menu.cpp
        src/Ente/Menu/Menu.h
        src/Ente/Menu/Menu.ui
        src/Widgets/qsfmlwidget.cpp
        src/Widgets/qsfmlwidget.h
        src/Widgets/particlewidget.cpp
        src/Widgets/particlewidget.h

        qt-material-widgets/components/resources.qrc
        ${MATERIAL_WIDGETS_SRC}
        ${MATERIAL_WIDGETS_HDR}
)

target_include_directories(Jogo PRIVATE src
        "${SFML_INCLUDE_DIR}"
        "${CMAKE_CURRENT_BINARY_DIR}"
        ${CMAKE_CURRENT_SOURCE_DIR}/qt-material-widgets
        ${CMAKE_CURRENT_SOURCE_DIR}/qt-material-widgets/components
        ${CMAKE_CURRENT_SOURCE_DIR}/qt-material-widgets/components/lib
)

if(MSVC)
    set_target_properties(Jogo PROPERTIES LINK_FLAGS "/SUBSYSTEM:CONSOLE")
endif()

target_link_libraries(Jogo PRIVATE
        # Qt 5
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
        Qt5::Svg

        # SFML
        debug "${SFML_LIBRARY_DIR}/sfml-graphics-d.lib"
        debug "${SFML_LIBRARY_DIR}/sfml-window-d.lib"
        debug "${SFML_LIBRARY_DIR}/sfml-system-d.lib"
        debug "${SFML_LIBRARY_DIR}/sfml-audio-d.lib"
        optimized "${SFML_LIBRARY_DIR}/sfml-graphics.lib"
        optimized "${SFML_LIBRARY_DIR}/sfml-window.lib"
        optimized "${SFML_LIBRARY_DIR}/sfml-system.lib"
        optimized "${SFML_LIBRARY_DIR}/sfml-audio.lib"
        OpenGL::GL
)

if(WIN32)
    # Copia AUTOMATICAMENTE as DLLs do SFML correspondentes ao modo ativo
    add_custom_command(TARGET Jogo POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "$<$<CONFIG:Debug>:${SFML_BINARY_DIR}/sfml-graphics-d-2.dll>$<$<NOT:$<CONFIG:Debug>>:${SFML_BINARY_DIR}/sfml-graphics-2.dll>"
            "$<$<CONFIG:Debug>:${SFML_BINARY_DIR}/sfml-window-d-2.dll>$<$<NOT:$<CONFIG:Debug>>:${SFML_BINARY_DIR}/sfml-window-2.dll>"
            "$<$<CONFIG:Debug>:${SFML_BINARY_DIR}/sfml-system-d-2.dll>$<$<NOT:$<CONFIG:Debug>>:${SFML_BINARY_DIR}/sfml-system-2.dll>"
            "$<$<CONFIG:Debug>:${SFML_BINARY_DIR}/sfml-audio-d-2.dll>$<$<NOT:$<CONFIG:Debug>>:${SFML_BINARY_DIR}/sfml-audio-2.dll>"
            "${SFML_BINARY_DIR}/openal32.dll"
            "$<TARGET_FILE_DIR:Jogo>"
    )

    # Copia AUTOMATICAMENTE as DLLs do Qt correspondentes ao modo ativo mais as dependências gráficos
    add_custom_command(TARGET Jogo POST_BUILD
            COMMAND "$<TARGET_FILE_DIR:Qt5::Core>/windeployqt.exe"
            "$<$<CONFIG:Debug>:--debug>"
            "$<$<CONFIG:Release>:--release>"
            "$<TARGET_FILE:Jogo>"
    )

    # Copia o arquivo de estilo do Qt
    add_custom_command(TARGET Jogo POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${CMAKE_SOURCE_DIR}/src/Ente/Menu/qtstyle.qss"
            "$<TARGET_FILE_DIR:Jogo>/qtstyle.qss"
    )

    # Copia todas as fontes do qt-material-widgets
    add_custom_command(TARGET Jogo POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CMAKE_SOURCE_DIR}/qt-material-widgets/fonts"
            "$<TARGET_FILE_DIR:Jogo>/fonts"
    )
    # Copia todos os assets do jogo
    add_custom_command(TARGET Jogo POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CMAKE_SOURCE_DIR}/assets"
            "$<TARGET_FILE_DIR:Jogo>/assets"
    )
endif()

