cmake_minimum_required(VERSION 3.10)
project(Jogo CXX)

set(CMAKE_CXX_STANDARD 98)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Ajustar este caminho abaixo para a pasta exata onde o Qt 5 foi instalado
set(CMAKE_PREFIX_PATH "C:/Qt/5.15.19/msvc2019_64")

# Ativa os compiladores automáticos do Qt para ler a interface (.ui) e eventos (MOC)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

# Encontra os pacotes do Qt 5 necessários para a interface
find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)

set(SFML_ROOT "${CMAKE_SOURCE_DIR}/SFML-2.6.0")
set(SFML_INCLUDE_DIR "${SFML_ROOT}/include")
set(SFML_LIBRARY_DIR "${SFML_ROOT}/lib")
set(SFML_BINARY_DIR "${SFML_ROOT}/bin")
set(QT_BINARY_DIR "C:/Qt/5.15.19/msvc2019_64/bin")
set(QT_PLATFORMS_DIR "C:/Qt/5.15.19/msvc2019_64/plugins/platforms")

# Procura os arquivos do jogo recursivamente
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "src/*.cpp" "src/*.h")

# Cria o executável incluindo as classes do Qt (mainwindow) e o arquivo de design (.ui)
add_executable(Jogo ${SOURCES}
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        src/qsfmlwidget.cpp
        src/qsfmlwidget.h
)

target_include_directories(Jogo PRIVATE src
        "${SFML_INCLUDE_DIR}"
        "${CMAKE_CURRENT_BINARY_DIR}"
)

if(MSVC)
    set_target_properties(Jogo PROPERTIES LINK_FLAGS "/SUBSYSTEM:CONSOLE")
endif()

target_link_libraries(Jogo PRIVATE
        # Qt 5
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets

        # SFML
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
    # Copia AUTOMATICAMENTE as DLLs do SFML correspondentes ao modo ativo (CORRIGIDO)
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
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "$<$<CONFIG:Debug>:${QT_BINARY_DIR}/Qt5Cored.dll>$<$<NOT:$<CONFIG:Debug>>:${QT_BINARY_DIR}/Qt5Core.dll>"
            "$<$<CONFIG:Debug>:${QT_BINARY_DIR}/Qt5Guid.dll>$<$<NOT:$<CONFIG:Debug>>:${QT_BINARY_DIR}/Qt5Gui.dll>"
            "$<$<CONFIG:Debug>:${QT_BINARY_DIR}/Qt5Widgetsd.dll>$<$<NOT:$<CONFIG:Debug>>:${QT_BINARY_DIR}/Qt5Widgets.dll>"
            "${QT_BINARY_DIR}/D3Dcompiler_47.dll"
            "${QT_BINARY_DIR}/libEGL.dll"
            "${QT_BINARY_DIR}/libGLESv2.dll"
            "${QT_BINARY_DIR}/opengl32sw.dll"
            "$<TARGET_FILE_DIR:Jogo>"
    )

    # Cria a pasta 'platforms' essencial do Qt e copia a DLL de janelas correta
    add_custom_command(TARGET Jogo POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:Jogo>/platforms"
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "$<$<CONFIG:Debug>:${QT_PLATFORMS_DIR}/qwindowsd.dll>$<$<NOT:$<CONFIG:Debug>>:${QT_PLATFORMS_DIR}/qwindows.dll>"
            "$<TARGET_FILE_DIR:Jogo>/platforms/"
    )
endif()
