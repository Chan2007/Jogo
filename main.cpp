#include <iostream>

#include "mainwindow.h"
#include <QApplication>
#include <QFile>
#include <QLocale>
#include <QString>
#include <QStringList>
#include <QTextStream>
#include <QTranslator>
#include "src/Diretorio/Encontrar_Diretorio.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setStyle("Fusion");

    QTranslator translator;
    QStringList uiLanguages = QLocale::system().uiLanguages();
    for (int i = 0; i < uiLanguages.size(); ++i) {
        QString locale = uiLanguages.at(i);
        QString baseName = "GUI_Jogo_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }

    // Carrega e aplica stylesheet
    const std::string qssPath = Encontrar_Diretorio::acharDiretorio_Arquivo("qtstyle.qss");
    if (!qssPath.empty()) {
        QFile styleFile(QString::fromStdString(qssPath));
        if (styleFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream stream(&styleFile);
            QString stylesheet = stream.readAll();
            styleFile.close();

            // Aplica stylesheet à aplicação inteira
            a.setStyleSheet(stylesheet);
            std::cout << "Stylesheet carregado com sucesso!" << std::endl;
        } else {
            std::cerr << "Falha ao abrir arquivo de stylesheet: " << qssPath << std::endl;
        }
    } else {
        std::cerr << "Arquivo qtstyle.qss nao encontrado no diretorio de assets." << std::endl;
    }

    MainWindow w;
    w.showMaximized(); // Abre a interface do Qt em tela cheia
    return a.exec();
}
