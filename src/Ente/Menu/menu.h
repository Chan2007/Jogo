#ifndef MENU_H
#define MENU_H

#include <cstddef>
#include <QMainWindow>
#include <QResizeEvent>
#include <QVariant>

#include "jogo.h"
#include "Ente/Ente.h"
#include "Widgets/screen_stack.h"
#include "Widgets/menu_button.h"
#include "Widgets/particlewidget.h"

#include "qtmaterialtextfield.h"
#include "qtmaterialautocomplete.h"
#include "qtmaterialflatbutton.h"

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
        void on_phase1Button_clicked();
        void on_backFromSettingsButton_clicked();
        void on_musicCheckBox_toggled(bool checked);
        void on_volumeSlider_valueChanged(int value);
        void on_exitButton_clicked();
        void on_settingsButton_clicked();

        void on_phase2Button_clicked();
        void on_loadGameButton_clicked();
        void on_rankingButton_clicked();
        void on_backFromLoadButton_clicked();
        void on_backFromRankingButton_clicked();

        void on_backFromPhase1SelectButton_clicked();
        void on_startPhase1Button_clicked();

        void on_backFromPhase2SelectButton_clicked();
        void on_startPhase2Button_clicked();

        void onFadeOutFinished();

    private:
        void init_MainMenu();
        void init_ConfigMenu();
        void animate_Transition(QWidget *origem, QWidget *destino, bool empilhar);
        void update_Particle();
        void update_Stack();
        void update_VolumeText(float value);

        void init_MaterialInputs(QtMaterialTextField* nameInput1, QtMaterialAutoComplete* combo1,
                                 QtMaterialTextField* nameInput2, QtMaterialAutoComplete* combo2);

        void apply_PlayerSetup(QtMaterialTextField* nameInput1, QtMaterialAutoComplete* combo1,
                               QtMaterialTextField* nameInput2, QtMaterialAutoComplete* combo2);

        void launch_Phase(Jogo::EstadoTela fase,
                          QtMaterialTextField* nameInput1, QtMaterialAutoComplete* combo1,
                          QtMaterialTextField* nameInput2, QtMaterialAutoComplete* combo2);

        Ui::Menu *ui;
        QTimer gameTimer;
        Jogo* jogo;
        bool jogoInicializado;
        Widgets::ScreenStack telas;
        Widgets::ParticleWidget* particulas;

        QWidget* m_destino;
        QGraphicsOpacityEffect* m_efeitoOrigem;
        QGraphicsOpacityEffect* m_efeitoDestino;

        // Lista de campeões compartilhada entre todas as páginas de seleção
        static const QStringList LISTA_CAMPEOES;
};
#endif // MENU_H