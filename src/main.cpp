#include <iostream>

#include "Ente/Menu/Menu.h"
#include <QApplication>
#include <QFile>
#include <QLocale>
#include <QString>
#include <QStringList>
#include <QTextStream>
#include <QTranslator>
#include "Sistema/Caminho/Encontrar_Caminho.h"

int main(int argc, char *argv[]) {
    QApplication qtWindow(argc, argv);
    qtWindow.setStyle("Fusion");

    // Tradução de língua
    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (int i = 0; i < uiLanguages.size(); ++i) {
        QString locale = uiLanguages.at(i);
        QString baseName = "Jogo_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            qtWindow.installTranslator(&translator);
            break;
        }
    }

    // Carrega e aplica stylesheet
    const std::string qssPath =
    Encontrar_Caminho::acharDiretorio_Arquivo("qtstyle.qss", "src/Ente/Menu/qtstyle.qss");
    if (!qssPath.empty()) {
        QFile styleFile(QString::fromStdString(qssPath));
        if (styleFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream stream(&styleFile);
            const QString stylesheet = stream.readAll();
            styleFile.close();

            // Aplica stylesheet à aplicação inteira
            qtWindow.setStyleSheet(stylesheet);
            std::cout << "Stylesheet carregado com sucesso!" << std::endl;
        }
        else
            std::cerr << "Falha ao abrir arquivo de stylesheet."  << std::endl;
    }
    else
        std::cerr << "Arquivo de estilização não encontrado." << std::endl;

    Jogo* jogoPrincipal = new Jogo();
    Menu menuPrincipal(NULL, jogoPrincipal);

    // Abre a interface do Qt em tela cheia
    menuPrincipal.showMaximized();

    int resultado = qtWindow.exec();

    // Desaloca quando o aplicativo é fechado
    delete jogoPrincipal;
    return resultado;
}
