#ifndef MENU_H
#define MENU_H

#include <cstddef>
#include <QMainWindow>
#include <QResizeEvent>
#include <QVariant>

#include "Widgets/particlewidget.h"
#include "jogo.h"
#include "Widgets/screen_stack.h"
#include "Widgets/menu_button.h"

QT_BEGIN_NAMESPACE
namespace Ui {
    class Menu;
}
QT_END_NAMESPACE

class QGraphicsOpacityEffect;
class QPushButton;
class QVariantAnimation;
class QGraphicsDropShadowEffect;

class Menu: public QMainWindow, public Ente {
    Q_OBJECT
    public:
        explicit Menu(QWidget *parent = NULL);
        ~Menu();

    protected:
        virtual void resizeEvent(QResizeEvent *event);

    private slots:
        void on_startButton_clicked();
        void on_backButton_clicked();
        void on_musicCheckBox_toggled(bool checked);
        void on_volumeSlider_valueChanged(int value);
        void on_exitButton_clicked();
        void on_settingsButton_clicked();

        void onFadeOutFinished();

    private:
        void configurarTelaPrincipal();
        void configurarTelaConfiguracao();
        void animarTransicaoTela(QWidget *origem, QWidget *destino, bool empilhar);
        void atualizarParticula();
        void atualizarPilhaParticula();
        void atualizarTextoVolume(float value);


        Ui::Menu *ui;
        QTimer gameTimer;
        Jogo jogo;
        bool jogoInicializado;
        ScreenStack telas;
        ParticleWidget* particulas;

        QWidget *m_destino;
        QGraphicsOpacityEffect *m_efeitoOrigem;
        QGraphicsOpacityEffect *m_efeitoDestino;
};
#endif // MENU_H
