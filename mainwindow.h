#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <cstddef>
#include <QColor>
#include <QMainWindow>
#include <QEvent>
#include <QResizeEvent>
#include <QTimer>
#include <QVariant>

#include "particlewidget.h"
#include "src/jogo.h"
#include "src/Widgets/screen_stack.h"

QT_BEGIN_NAMESPACE
namespace Ui {
    class MainWindow;
}
QT_END_NAMESPACE

class QGraphicsOpacityEffect;
class QPushButton;
class QVariantAnimation;
class QGraphicsDropShadowEffect;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = NULL);
    ~MainWindow();

protected:
    bool eventFilter(QObject *obj, QEvent *event);
    virtual void resizeEvent(QResizeEvent *event);

private slots:
    void atualizarJogo();
    void finalizarAnimacaoTela();
    void on_startButton_clicked();
    void on_settingsButton_clicked();
    void on_backButton_clicked();
    void on_musicCheckBox_toggled(bool checked);
    void on_volumeSlider_valueChanged(int value);
    void on_exitButton_clicked();
    void atualizarAnimacaoBotao(const QVariant &valor);

private:
    void aplicarEfeitosVisuais();
    void configurarTelaPrincipal();
    void configurarTelaConfiguracao();
    void animarTransicaoTela(QWidget *origem, QWidget *destino, bool empilhar);
    void iniciarAnimacoesBotoes();
    void animarEntradaWidget(QWidget *widget, int atrasoMs, int deslocamentoX);
    void prepararBotaoAnimado(QPushButton *botao, const QColor &bgBase, const QColor &bgHover, const QColor &bordaBase, const QColor &bordaHover);
    void iniciarAnimacaoBotao(QPushButton *botao, double destino, int duracaoMs);
    void animarPaginaConfiguracao(bool entrando);
    void reposicionarParticleWidget();
    void atualizarVisibilidadeParticleWidget();
    void atualizarLabelVolume(int value);


    Ui::MainWindow *ui;
    QTimer gameTimer;
    Jogo jogo;
    bool jogoInicializado;
    ScreenStack telas;
    bool transicaoTelaAtiva;
    QWidget *telaOrigemAnimada;
    QWidget *telaDestinoAnimada;
    ParticleWidget *m_particulas;
    QGraphicsOpacityEffect *efeitoOrigemTransicao;
    QGraphicsOpacityEffect *efeitoDestinoTransicao;
};
#endif // MAINWINDOW_H
