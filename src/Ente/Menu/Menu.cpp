#include "Menu.h"
#include "ui_menu.h"

#include <QString>
#include <QSizePolicy>
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include "qtmaterialcheckbox.h"
#include "qtmaterialslider.h"
#include "qtmaterialtextfield.h"
#include "qtmaterialautocomplete.h"
#include "Ente/Entidade/Personagem/Jogador/Jogador.h"

const QStringList Menu::LISTA_CAMPEOES = {
    "NAAFIRI","JHIN","LUX","EVELYNN",
    "GWEN",   "PYKE","SETT","SHACO","VIEGO"
};

Menu::Menu(QWidget *parent): QMainWindow(parent)
    , ui(new Ui::Menu)
    , jogo(Jogo::getJogo())
    , jogoInicializado(false)
    , telas()
    , particulas(NULL)
{
    ui->setupUi(this);


    QtMaterialCheckBox *materialMusic = qobject_cast<QtMaterialCheckBox*>(ui->musicCheckBox);
    if (materialMusic) {
        materialMusic->setTextColor(QColor(0, 0, 0));
        materialMusic->setCheckedColor(QColor(0, 0, 0));
        materialMusic->setUncheckedColor(QColor(100, 100, 100));
    }

    QtMaterialSlider *materialSlider = qobject_cast<QtMaterialSlider*>(ui->volumeSlider);
    if (materialSlider) {
        materialSlider->setThumbColor(QColor(0, 0, 0));
    }

    init_MainMenu();
    init_ConfigMenu();
    telas.setContainer(ui->stackedWidget);
    telas.setInitialScreen(ui->mainPage);

    ui->stackedWidget->raise();
    ui->centralwidget->raise();

    particulas = new Widgets::ParticleWidget(ui->mainPage);
    particulas->lower();
    particulas->show();
    update_Particle();
    update_Stack();

    ui->musicCheckBox->setChecked(true);
    ui->volumeSlider->setRange(0, 100);
    ui->volumeSlider->setValue(50);
    update_VolumeText(ui->volumeSlider->value());

    // Animações de entrada dos botões
    ui->phase1Button->iniciarAnimacaoEntrada(0);
    ui->settingsButton->iniciarAnimacaoEntrada(80);
    ui->exitButton->iniciarAnimacaoEntrada(160);
    ui->backFromSettingsButton->iniciarAnimacaoEntrada(0);
    ui->phase2Button->iniciarAnimacaoEntrada(80);
    ui->loadGameButton->iniciarAnimacaoEntrada(160);
    ui->rankingButton->iniciarAnimacaoEntrada(240);
    ui->backFromPhase1SelectButton->iniciarAnimacaoEntrada(0);
    ui->backFromPhase2SelectButton->iniciarAnimacaoEntrada(0);

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

void Menu::resizeEvent(QResizeEvent *event) {
    QMainWindow::resizeEvent(event);
    update_Particle();
}

void Menu::init_MainMenu() {
    ui->menuPanelLayout->setAlignment(Qt::AlignTop);
    ui->heroTitleLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    ui->heroSubtitleLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    ui->statusLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    ui->phase1Button->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    ui->settingsButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    ui->exitButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    ui->phase1Button->setMaximumHeight(100);
    ui->settingsButton->setMaximumHeight(100);
    ui->exitButton->setMaximumHeight(100);
}

void Menu::init_ConfigMenu() {
    // Configurações
    ui->settingsPageLayout->setAlignment(ui->settingsPanel, Qt::AlignHCenter);
    ui->settingsTitleLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    ui->settingsSubtitleLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    ui->sectionTitleLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    ui->backFromSettingsButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    ui->settingsPanel->setAttribute(Qt::WA_TranslucentBackground);
    ui->settingsPanel->setAutoFillBackground(false);
    ui->backFromSettingsButton->setAlinhamento(Qt::AlignCenter | Qt::AlignVCenter);

    // Carregar jogo
    ui->loadGamePanel->setAttribute(Qt::WA_TranslucentBackground);
    ui->loadGamePanel->setAutoFillBackground(false);
    ui->loadGamePageLayout->setAlignment(ui->loadGamePanel, Qt::AlignHCenter);
    ui->loadGameTitleLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    ui->loadGameSubtitleLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    ui->backFromLoadButton->setAlinhamento(Qt::AlignCenter | Qt::AlignVCenter);

    // Ranking
    ui->rankingPanel->setAttribute(Qt::WA_TranslucentBackground);
    ui->rankingPanel->setAutoFillBackground(false);
    ui->rankingPageLayout->setAlignment(ui->rankingPanel, Qt::AlignHCenter);
    ui->rankingTitleLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    ui->rankingSubtitleLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    ui->backFromRankingButton->setAlinhamento(Qt::AlignCenter | Qt::AlignVCenter);

    // Fase 1
    ui->phase1SelectPanel->setAttribute(Qt::WA_TranslucentBackground);
    ui->phase1SelectPanel->setAutoFillBackground(false);
    ui->phase1SelectPageLayout->setAlignment(ui->phase1SelectPanel, Qt::AlignHCenter);
    ui->phase1SelectPageTitleLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    ui->phase1SelectPageSubtitleLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    ui->backFromPhase1SelectButton->setAlinhamento(Qt::AlignCenter | Qt::AlignVCenter);
    init_MaterialInputs(
        ui->p1Phase1NameInput, ui->p1Phase1ChampionCombo,
        ui->p2Phase1NameInput, ui->p2Phase1ChampionCombo
    );

    // Fase 2
    ui->phase2SelectPanel->setAttribute(Qt::WA_TranslucentBackground);
    ui->phase2SelectPanel->setAutoFillBackground(false);
    ui->phase2SelectPageLayout->setAlignment(ui->phase2SelectPanel, Qt::AlignHCenter);
    ui->phase2SelectPageTitleLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    ui->phase2SelectPageSubtitleLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    ui->backFromPhase2SelectButton->setAlinhamento(Qt::AlignCenter | Qt::AlignVCenter);
    init_MaterialInputs(
        ui->p1Phase2NameInput, ui->p1Phase2ChampionCombo,
        ui->p2Phase2NameInput, ui->p2Phase2ChampionCombo
    );
}

void Menu::animate_Transition(QWidget *origem, QWidget *destino, bool empilhar) {
    if (!origem || !destino || origem == destino) return;
    destino->move(0, 0);
    destino->resize(ui->stackedWidget->size());
    if (empilhar) telas.pushScreen(destino);
    else telas.popScreen();

    destino->setGeometry(ui->centralwidget->rect());
    destino->show();
    destino->raise();

    QGraphicsOpacityEffect *efeitoOrigem = qobject_cast<QGraphicsOpacityEffect*>(origem->graphicsEffect());
    if (!efeitoOrigem) {
        efeitoOrigem = new QGraphicsOpacityEffect(origem);
        origem->setGraphicsEffect(efeitoOrigem);
    }

    QGraphicsOpacityEffect *efeitoDestino = qobject_cast<QGraphicsOpacityEffect*>(destino->graphicsEffect());
    if (!efeitoDestino) {
        efeitoDestino = new QGraphicsOpacityEffect(destino);
        destino->setGraphicsEffect(efeitoDestino);
    }

    efeitoOrigem->setOpacity(1.0);
    efeitoDestino->setOpacity(0.0);

    QPropertyAnimation *fadeOut = new QPropertyAnimation(efeitoOrigem, "opacity");
    fadeOut->setDuration(1000);
    fadeOut->setStartValue(1.0);
    fadeOut->setEndValue(0.0);

    QPropertyAnimation *fadeIn = new QPropertyAnimation(efeitoDestino, "opacity");
    fadeIn->setDuration(1000);
    fadeIn->setStartValue(0.0);
    fadeIn->setEndValue(1.0);

    m_destino = destino;
    m_efeitoOrigem = efeitoOrigem;
    m_efeitoDestino = efeitoDestino;
    connect(fadeOut, SIGNAL(finished()), this, SLOT(onFadeOutFinished()));

    fadeOut->start(QAbstractAnimation::DeleteWhenStopped);
    fadeIn->start(QAbstractAnimation::DeleteWhenStopped);

    ui->centralwidget->update();
    update();
}

void Menu::update_Particle() {
    if (!particulas || !ui || !ui->centralwidget) return;

    const int larguraBase = ui->mainPage->width();
    const int alturaBase  = ui->mainPage->height();
    if (larguraBase <= 0 || alturaBase <= 0) return;

    int largura = static_cast<int>(larguraBase * 2.0f / 5.0f);
    int altura  = static_cast<int>(alturaBase  * 1.5f);
    if (largura < 1) largura = 1;
    if (altura  < 1) altura  = 1;

    const int margemDireita   = 0;
    const int margemInferior  = 0;
    int x = larguraBase - largura - margemDireita;
    int y = alturaBase  - altura  - margemInferior;
    if (x < 0) x = 0;
    if (y < 0) y = 0;

    particulas->setGeometry(x, y, largura, altura);
    particulas->lower();
}

void Menu::update_Stack() {
    if (!particulas || !ui || !ui->stackedWidget) return;

    const bool visivel = ui->stackedWidget->currentWidget() == ui->mainPage;
    particulas->setVisible(visivel);
    if (visivel) particulas->lower();
}

void Menu::update_VolumeText(float value) {
    if (ui && ui->volumeValueLabel)
        ui->volumeValueLabel->setText(QString::number(value) + "%");
}

void Menu::on_phase1Button_clicked() {
    animate_Transition(ui->mainPage, ui->phase1SelectPage, true);
    ui->statusLabel->setText("Seleção de personagens — Fase 1");
}

void Menu::on_phase2Button_clicked() {
    animate_Transition(ui->mainPage, ui->phase2SelectPage, true);
    ui->statusLabel->setText("Seleção de personagens — Fase 2");
}

void Menu::on_settingsButton_clicked() {
    animate_Transition(ui->mainPage, ui->settingsPage, true);
    ui->statusLabel->setText("Configurações abertas.");
}

void Menu::on_backFromSettingsButton_clicked() {
    animate_Transition(ui->settingsPage, ui->mainPage, false);
    ui->statusLabel->setText("Menu principal");
}

void Menu::on_loadGameButton_clicked() {
    animate_Transition(ui->mainPage, ui->loadGamePage, true);
    ui->statusLabel->setText("Carregar jogo.");
}

void Menu::on_backFromLoadButton_clicked() {
    animate_Transition(ui->loadGamePage, ui->mainPage, false);
    ui->statusLabel->setText("Menu principal");
}

void Menu::on_rankingButton_clicked() {
    animate_Transition(ui->mainPage, ui->rankingPage, true);
    ui->statusLabel->setText("Ranking.");
}

void Menu::on_backFromRankingButton_clicked() {
    animate_Transition(ui->rankingPage, ui->mainPage, false);
    ui->statusLabel->setText("Menu principal");
}

void Menu::on_exitButton_clicked() {
    close();
}

void Menu::onFadeOutFinished() {
    ui->stackedWidget->setCurrentWidget(m_destino);
    if (m_efeitoOrigem) {
        QWidget* origemWidget = qobject_cast<QWidget*>(m_efeitoOrigem->parent());
        if (origemWidget) origemWidget->setGraphicsEffect(nullptr);
    }
    if (m_efeitoDestino) {
        m_destino->setGraphicsEffect(nullptr);
    }
    update_Stack();
}
void Menu::on_musicCheckBox_toggled(bool checked) {
    ui->musicCheckBox->setText(checked ? "Ativada" : "Desativada");
}

void Menu::on_volumeSlider_valueChanged(int value) {
    update_VolumeText(static_cast<float>(value));
}

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
    if (combo1) combo1->setDataSource(LISTA_CAMPEOES);
    if (combo2) combo2->setDataSource(LISTA_CAMPEOES);
}

void Menu::apply_PlayerSetup(QtMaterialTextField* nameInput1, QtMaterialAutoComplete* combo1,
                             QtMaterialTextField* nameInput2, QtMaterialAutoComplete* combo2)
{
    // Mapeia o texto digitado/selecionado de volta para o enum EscolhaCampeao

    auto resolverCampeao = [](const QString& texto) -> Personagens::EscolhaCampeao {
        static const QMap<QString, Personagens::EscolhaCampeao> tabela = {
            { "NAAFIRI", Personagens::CAMPEAO_NAAFIRI },
            { "JHIN",Personagens::CAMPEAO_JHIN },
            { "LUX", Personagens::CAMPEAO_LUX },
            { "EVELYNN", Personagens::CAMPEAO_EVELYNN },
            { "GWEN", Personagens::CAMPEAO_GWEN },
            { "PYKE", Personagens::CAMPEAO_PYKE },
            { "SETT", Personagens::CAMPEAO_SETT },
            { "SHACO", Personagens::CAMPEAO_SHACO },
            { "VIEGO", Personagens::CAMPEAO_VIEGO },
        };
        return tabela.value(texto.toUpper(), Personagens::CAMPEAO_NAAFIRI);
    };

    // Jogador 1
    QString nomeJ1 = nameInput1 ? nameInput1->text().trimmed() : QString();
    if (nomeJ1.isEmpty()) nomeJ1 = "Anônimo";

    QString textoCombo1 = (combo1 && !combo1->text().trimmed().isEmpty())
                          ? combo1->text().trimmed()
                          : LISTA_CAMPEOES.first();
    Personagens::EscolhaCampeao campeaoJ1 = resolverCampeao(textoCombo1);

    if (jogo->getJogador1()) {
        jogo->getJogador1()->setNome(nomeJ1.toStdString());
        jogo->getJogador1()->setCampeao(campeaoJ1);
    }

    // Jogador 2
    QString nomeJ2 = nameInput2 ? nameInput2->text().trimmed() : QString();
    if (!nomeJ2.isEmpty()) {
        QString textoCombo2 = (combo2 && !combo2->text().trimmed().isEmpty())
                              ? combo2->text().trimmed()
                              : LISTA_CAMPEOES.first();
        Personagens::EscolhaCampeao campeaoJ2 = resolverCampeao(textoCombo2);

        if (jogo->getJogador2()) {
            jogo->getJogador2()->setNome(nomeJ2.toStdString());
            jogo->getJogador2()->setCampeao(campeaoJ2);
            jogo->setJogador2Ativo(true);
        }
    } else {
        jogo->setJogador2Ativo(false);
    }
}

void Menu::launch_Phase(Jogo::EstadoTela fase, QtMaterialTextField* nameInput1,
                        QtMaterialAutoComplete* combo1, QtMaterialTextField* nameInput2,
                        QtMaterialAutoComplete* combo2)
{
    if (Jogo::estaAberto()) return;

    apply_PlayerSetup(nameInput1, combo1, nameInput2, combo2);

    hide();
    jogo->inicializar();
    jogo->setVolume(static_cast<float>(ui->volumeSlider->value()));
    jogo->setMusica(ui->musicCheckBox->isChecked());
    jogo->mudarEstado(fase);
    jogo->executar(); // bloqueia aqui durante o gameplay

    showNormal();
    raise();
    activateWindow();
    ui->statusLabel->setText("Menu principal");
}


void Menu::on_backFromPhase1SelectButton_clicked() {
    animate_Transition(ui->phase1SelectPage, ui->mainPage, false);
    ui->statusLabel->setText("Menu principal");
}

void Menu::on_startPhase1Button_clicked() {
    // apply_PlayerSetup decide se é 1 ou 2 jogadores
    launch_Phase(Jogo::TelaFase1,
       ui->p1Phase1NameInput, ui->p1Phase1ChampionCombo,
       ui->p2Phase1NameInput, ui->p2Phase1ChampionCombo);
}

void Menu::on_backFromPhase2SelectButton_clicked() {
    animate_Transition(ui->phase2SelectPage, ui->mainPage, false);
    ui->statusLabel->setText("Menu principal");
}

void Menu::on_startPhase2Button_clicked() {
    launch_Phase(Jogo::TelaFase2,
       ui->p1Phase2NameInput, ui->p1Phase2ChampionCombo,
       ui->p2Phase2NameInput, ui->p2Phase2ChampionCombo);
}