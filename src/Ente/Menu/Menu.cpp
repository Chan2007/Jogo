#include "Menu.h"
#include "ui_menu.h"

#include <QString>
#include <QGraphicsOpacityEffect>
#include <QFont>
#include "qtmaterialcheckbox.h"
#include "qtmaterialslider.h"
#include "qtmaterialtextfield.h"
#include "qtmaterialautocomplete.h"
#include "Widgets/Ranking.h"


Menu::Menu(QWidget *parent, Jogo* j): QMainWindow(parent), ui(new Ui::Menu), jogo(j), Stack(), ParticleWidget(NULL)
{
    ui->setupUi(this);

    QtMaterialCheckBox *materialMusic = qobject_cast<QtMaterialCheckBox*>(ui->musicCheckBox);
    if (materialMusic) {
        materialMusic->setTextColor(QColor(0, 0, 0));
        materialMusic->setCheckedColor(QColor(0, 0, 0));
        materialMusic->setUncheckedColor(QColor(100, 100, 100));
    }

    QtMaterialSlider *materialSlider = qobject_cast<QtMaterialSlider*>(ui->volumeSlider);
    if (materialSlider)
        materialSlider->setThumbColor(QColor(0, 0, 0));

    init_InputPlayer();
    Stack.setContainer(ui->stackedWidget);
    Stack.setInitialScreen(ui->mainPage);

    ParticleWidget = new Widgets::ParticleWidget(ui->mainPage);
    ParticleWidget->show();
    ParticleWidget->lower();
    ParticleWidget->updateWidget();
    update_Stack();

    update_VolumeText(ui->volumeSlider->value());

    // Animações de entrada dos botões
    ui->phase1Button->init_Animation(0);
    ui->phase2Button->init_Animation(0);
    ui->loadGameButton->init_Animation(0);
    ui->rankingButton->init_Animation(0);
    ui->settingsButton->init_Animation(0);
    ui->exitButton->init_Animation(0);

    ui->backFromSettingsButton->init_Animation(0);
    ui->backFromPhase1SelectButton->init_Animation(0);
    ui->backFromPhase2SelectButton->init_Animation(0);
    ui->backFromLoadButton->init_Animation(0);
    ui->backFromRankingButton->init_Animation(0);

    ui->startPhase1Button->setAutoFillBackground(true);
    ui->startPhase1Button->setForegroundColor(QColor(255, 255, 255));

    ui->startPhase1Button->setBackgroundMode(Qt::OpaqueMode);
    ui->startPhase1Button->setBackgroundColor(QColor(100, 100, 100));

    ui->startPhase2Button->setAutoFillBackground(true);
    ui->startPhase2Button->setForegroundColor(QColor(255, 255, 255));

    ui->startPhase2Button->setBackgroundMode(Qt::OpaqueMode);
    ui->startPhase2Button->setBackgroundColor(QColor(100, 100, 100));
}

Menu::~Menu() { delete ui; }



void Menu::init_InputPlayer() {

    init_MaterialInputs(
        ui->p1Phase1NameInput, ui->p1Phase1ChampionCombo,
        ui->p2Phase1NameInput, ui->p2Phase1ChampionCombo
    );
    init_MaterialInputs(
        ui->p1Phase2NameInput, ui->p1Phase2ChampionCombo,
        ui->p2Phase2NameInput, ui->p2Phase2ChampionCombo
    );
}

void Menu::update_Stack() {
    if (!ParticleWidget || !ui || !ui->stackedWidget) return;

    const bool visivel = ui->stackedWidget->currentWidget() == ui->mainPage;
    ParticleWidget->setVisible(visivel);
    if (visivel) ParticleWidget->lower();
}

void Menu::update_VolumeText(float value) {
    if (ui && ui->volumeValueLabel)
        ui->volumeValueLabel->setText(QString::number(value) + "%");
}

void Menu::on_phase1Button_clicked() {
    Stack.pushScreen(ui->phase1SelectPage);
    ui->statusLabel->setText("Seleção de personagens — Fase 1");
}

void Menu::on_phase2Button_clicked() {
    Stack.pushScreen(ui->phase2SelectPage);
    ui->statusLabel->setText("Seleção de personagens — Fase 2");
}

void Menu::on_settingsButton_clicked() {
    Stack.pushScreen(ui->settingsPage);
    ui->statusLabel->setText("Configurações abertas.");
}

void Menu::on_backFromSettingsButton_clicked() {
    Stack.popScreen();
    ui->statusLabel->setText("Menu principal");
}

void Menu::on_loadGameButton_clicked() {
    Stack.pushScreen(ui->loadGamePage);
    ui->statusLabel->setText("Carregar jogo.");
}

void Menu::on_backFromLoadButton_clicked() {
    Stack.popScreen();
    ui->statusLabel->setText("Menu principal");
}

void Menu::on_rankingButton_clicked() {
    Widgets::Ranking::loadRanking(ui->rankingTable);
    Stack.pushScreen(ui->rankingPage);
    ui->statusLabel->setText("Ranking.");
}

void Menu::on_backFromRankingButton_clicked() {
    Stack.popScreen();
    ui->statusLabel->setText("Menu principal");
}

void Menu::on_exitButton_clicked() {
    close();
}

void Menu::on_musicCheckBox_toggled(bool checked) {
    ui->musicCheckBox->setText(checked ? "Ativada" : "Desativada");
}

void Menu::on_volumeSlider_valueChanged(int value) {
    update_VolumeText(static_cast<float>(value));
}

void Menu::on_backFromPhase1SelectButton_clicked() {
    Stack.popScreen();
    ui->statusLabel->setText("Menu principal");
}

void Menu::on_startPhase1Button_clicked() {
    executar(Jogo::TelaFase1,
       ui->p1Phase1NameInput, ui->p1Phase1ChampionCombo,
       ui->p2Phase1NameInput, ui->p2Phase1ChampionCombo);
}

void Menu::on_backFromPhase2SelectButton_clicked() {
    Stack.popScreen();
    ui->statusLabel->setText("Menu principal");
}

void Menu::on_startPhase2Button_clicked() {
    executar(Jogo::TelaFase2,
       ui->p1Phase2NameInput, ui->p1Phase2ChampionCombo,
       ui->p2Phase2NameInput, ui->p2Phase2ChampionCombo);
}

// -------------------------------------------------------------------------
// COMPONENTES DE INTERFACE (QT-MATERIAL-WIDGETS)
// Baseado no código open-source de laserpants (Licença BSD 3-Clause).
// Repositório original: https://github.com/laserpants/qt-material-widgets
// -------------------------------------------------------------------------

void Menu::init_MaterialInputs(QtMaterialTextField* nameInput1, QtMaterialAutoComplete* combo1,
                               QtMaterialTextField* nameInput2, QtMaterialAutoComplete* combo2)
{
    if (nameInput1) {
        nameInput1->setLabel("Nome do Jogador 1");
        nameInput1->setInkColor(QColor(0, 0, 0));
        nameInput1->setInputLineColor(QColor(0, 0, 0));
        nameInput1->setLabelColor(QColor(80, 80, 80));
    }
    if (nameInput2) {
        nameInput2->setLabel("Nome do Jogador 2");
        nameInput2->setInkColor(QColor(0, 0, 0));
        nameInput2->setInputLineColor(QColor(0, 0, 0));
        nameInput2->setLabelColor(QColor(80, 80, 80));
    }

    // Alimenta o AutoComplete com a lista de campeões disponíveis
    if (combo1) combo1->setDataSource(Fases::Fase::CAMPEOES);
    if (combo2) combo2->setDataSource(Fases::Fase::CAMPEOES);
}

void Menu::executar(Jogo::EstadoTela fase, QtMaterialTextField* nameInput1, QtMaterialAutoComplete* combo1,
               QtMaterialTextField* nameInput2, QtMaterialAutoComplete* combo2)
{
    if (Jogo::estaAberto()) return;

    std::string nomeJ1 = nameInput1 ? nameInput1->text().trimmed().toStdString() : "";
    QString campJ1 = combo1 ? combo1->text().trimmed() : "";

    std::string nomeJ2 = nameInput2 ? nameInput2->text().trimmed().toStdString() : "";
    QString campJ2 = combo2 ? combo2->text().trimmed() : "";
    bool j2Ativo = !nomeJ2.empty();

    hide();
    jogo->inicializar();
    jogo->mudarEstado(fase, nomeJ1, campJ1, nomeJ2, campJ2, j2Ativo);
    jogo->executar();
    this->show();
}
