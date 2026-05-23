#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <cstddef>
#include <QMainWindow>
#include <QResizeEvent>
#include <QTimer>
#include "src/backgroundwidget.h"
#include "src/jogo.h"
#include "src/Widgets/screen_stack.h"

QT_BEGIN_NAMESPACE
namespace Ui {
    class MainWindow;
}
QT_END_NAMESPACE

class QGraphicsOpacityEffect;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = NULL);
    ~MainWindow();

protected:
    void resizeEvent(QResizeEvent *event);

private slots:
    void atualizarJogo();
    void finalizarAnimacaoTela();
    void on_startButton_clicked();
    void on_settingsButton_clicked();
    void on_backButton_clicked();
    void on_musicCheckBox_toggled(bool checked);
    void on_volumeSlider_valueChanged(int value);
    void on_exitButton_clicked();

private:
    void ajustarFundo();
    void aplicarEfeitosVisuais();
    void configurarTelaPrincipal();
    void configurarTelaConfiguracao();
    void animarTransicaoTela(QWidget *origem, QWidget *destino, bool empilhar);
    void iniciarAnimacoesBotoes();
    void animarEntradaWidget(QWidget *widget, int atrasoMs, int deslocamentoX);
    void atualizarLabelVolume(int value);

    Ui::MainWindow *ui;
    BackgroundWidget *backgroundWidget;
    QTimer gameTimer;
    Jogo jogo;
    bool jogoInicializado;
    ScreenStack telas;
    bool transicaoTelaAtiva;
    QWidget *telaOrigemAnimada;
    QWidget *telaDestinoAnimada;
    QGraphicsOpacityEffect *efeitoOrigemTransicao;
    QGraphicsOpacityEffect *efeitoDestinoTransicao;
};
#endif // MAINWINDOW_H
