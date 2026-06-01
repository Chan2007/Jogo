#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <cstddef>
#include <QMainWindow>
#include <QResizeEvent>
#include <QVariant>

#include "src/Widgets/particlewidget.h"
#include "src/jogo.h"
#include "src/Widgets/screen_stack.h"
#include "src/Widgets/menu_button.h"

QT_BEGIN_NAMESPACE
namespace Ui {
    class MainWindow;
}
QT_END_NAMESPACE

class QGraphicsOpacityEffect;
class QPushButton;
class QVariantAnimation;
class QGraphicsDropShadowEffect;

class MainWindow : public QMainWindow{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = NULL);
    ~MainWindow();

protected:
    virtual void resizeEvent(QResizeEvent *event);

private slots:
    void atualizarJogo();
    void on_startButton_clicked();
    void on_backButton_clicked();
    void on_musicCheckBox_toggled(bool checked);
    void on_volumeSlider_valueChanged(int value);
    void on_exitButton_clicked();
    void on_settingsButton_clicked();


private:
    void aplicarEfeitosVisuais();
    void configurarTelaPrincipal();
    void configurarTelaConfiguracao();
    void animarTransicaoTela(QWidget *origem, QWidget *destino, bool empilhar);
    void animarPaginaConfiguracao(bool entrando);
    void reposicionarParticleWidget();
    void atualizarVisibilidadeParticleWidget();
    void atualizarLabelVolume(int value);


    Ui::MainWindow *ui;
    QTimer gameTimer;
    Jogo jogo;
    bool jogoInicializado;
    ScreenStack telas;
    ParticleWidget *m_particulas;
};
#endif // MAINWINDOW_H
