QT += core gui widgets
TARGET = Jogo
TEMPLATE = app

# Inclui a pasta src para que o compilador encontre os headers facilmente
INCLUDEPATH += $$PWD/src

# ====================================================================
# Arquivos Base (Apenas os que estão na RAIZ do projeto)
# ====================================================================
HEADERS += mainwindow.h
SOURCES += mainwindow.cpp main.cpp
FORMS   += mainwindow.ui

# Inclui suporte a traduções se o Qt Creator precisar
TRANSLATIONS += GUI_Jogo_pt_BR.ts
CONFIG += lrelease embed_translations

# Remove os padrões modernos do Qt para forçar o comportamento antigo no código
CONFIG -= c++11 c++14 c++17 c++20

# --------------------------------------------------------------------
# Configurações do Compilador MSVC (Visual Studio 2019)
# --------------------------------------------------------------------
QMAKE_CXXFLAGS += /FS /Zc:wchar_t /permissive-
DEFINES += _CRT_SECURE_NO_WARNINGS \
           _SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS

# Procura e adiciona arquivos do projeto recursivamente dentro de src/
# Isso já vai puxar o jogo.cpp, jogo.h, qsfmlwidget.cpp e qsfmlwidget.h automaticamente!
SOURCES += $$files(src/*.cpp, true)
HEADERS += $$files(src/*.h, true)
HEADERS += $$files(src/*.hpp, true)

# --------------------------------------------------------------------
# Integração com o SFML 2.6.0
# --------------------------------------------------------------------
INCLUDEPATH += "$$PWD/SFML-2.6.0/include"
DEPENDPATH  += "$$PWD/SFML-2.6.0/include"
LIBS        += -L"$$PWD/SFML-2.6.0/lib"

CONFIG(debug, debug|release) {
    LIBS += -lsfml-audio-d -lsfml-graphics-d -lsfml-window-d -lsfml-system-d
} else {
    LIBS += -lsfml-audio -lsfml-graphics -lsfml-window -lsfml-system
}

# --------------------------------------------------------------------
# Configuração de Implantação Direta das DLLs do SFML
# --------------------------------------------------------------------
sfml_dlls.files = $$files($$PWD/SFML-2.6.0/bin/*.dll, true)

CONFIG(debug, debug|release) {
    sfml_dlls.path = $$OUT_PWD/debug
    !exists($$OUT_PWD/debug): sfml_dlls.path = $$OUT_PWD
} else {
    sfml_dlls.path = $$OUT_PWD/release
    !exists($$OUT_PWD/release): sfml_dlls.path = $$OUT_PWD
}
CONFIG(debug, debug|release) {
    QMAKE_POST_LINK += xcopy /Y /E "$$shell_path($$PWD/SFML-2.6.0/bin)" "$$shell_path($$OUT_PWD/debug)"
} else {
    QMAKE_POST_LINK += xcopy /Y /E "$$shell_path($$PWD/SFML-2.6.0/bin)" "$$shell_path($$OUT_PWD/release)"
}

INSTALLS += sfml_dlls