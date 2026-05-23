#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMenuBar>
#include <QSizePolicy>
#include <QStatusBar>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , backgroundWidget(NULL)
    , gameTimer(this)
    , jogo()
    , jogoInicializado(false)
    , telas()
    , transicaoTelaAtiva(false)
    , telaOrigemAnimada(0)
    , telaDestinoAnimada(0)
    , efeitoOrigemTransicao(0)
    , efeitoDestinoTransicao(0)
{
    ui->setupUi(this);
    configurarTelaPrincipal();
    configurarTelaConfiguracao();
    telas.setContainer(ui->stackedWidget);
    telas.setInitialScreen(ui->mainPage);

    if (menuBar())
        menuBar()->hide();
    if (statusBar())
        statusBar()->hide();

    ui->centralwidget->setAttribute(Qt::WA_TranslucentBackground);
    ui->centralwidget->setAutoFillBackground(false);
    ui->stackedWidget->setAttribute(Qt::WA_TranslucentBackground);
    ui->stackedWidget->setAttribute(Qt::WA_NoSystemBackground);
    ui->stackedWidget->setAutoFillBackground(false);
    ui->stackedWidget->setAttribute(Qt::WA_TranslucentBackground);
    ui->stackedWidget->setAttribute(Qt::WA_NoSystemBackground);
    ui->stackedWidget->setAutoFillBackground(false);
    ui->mainPage->setAttribute(Qt::WA_TranslucentBackground);
    ui->mainPage->setAutoFillBackground(false);
    ui->settingsPage->setAttribute(Qt::WA_TranslucentBackground);
    ui->settingsPage->setAutoFillBackground(false);
    ui->mainMenuPanel->setAttribute(Qt::WA_TranslucentBackground);
    ui->mainMenuPanel->setAutoFillBackground(false);

    aplicarEfeitosVisuais();

    backgroundWidget = new BackgroundWidget(ui->centralwidget);
    backgroundWidget->setAttribute(Qt::WA_TransparentForMouseEvents);
    backgroundWidget->setFocusPolicy(Qt::NoFocus);
    backgroundWidget->show();
    backgroundWidget->stackUnder(ui->stackedWidget);
    ui->stackedWidget->raise();
    ajustarFundo();

    connect(&gameTimer, SIGNAL(timeout()), this, SLOT(atualizarJogo()));

    ui->musicCheckBox->setChecked(true);
    ui->volumeSlider->setRange(0, 100);
    ui->volumeSlider->setValue(50);
    atualizarLabelVolume(ui->volumeSlider->value());
    ui->statusLabel->setText("Selecione a jornada ou ajuste a atmosfera sonora.");
    iniciarAnimacoesBotoes();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);
    ajustarFundo();

    // Garante que settingsPage ocupe a tela inteira quando visível
    if (ui && ui->settingsPage && telas.currentScreen() == ui->settingsPage) {
        ui->settingsPage->setGeometry(ui->centralwidget->rect());
    }
}

void MainWindow::ajustarFundo()
{
    if (!backgroundWidget || !ui || !ui->centralwidget)
        return;

    backgroundWidget->setGeometry(ui->centralwidget->rect());
}

void MainWindow::aplicarEfeitosVisuais()
{
    ui->mainMenuPanel->setGraphicsEffect(0);
    ui->settingsPanel->setGraphicsEffect(0);
}

void MainWindow::configurarTelaPrincipal()
{
    ui->menuPanelLayout->setAlignment(Qt::AlignTop);
    ui->mainPageLayout->setAlignment(ui->mainMenuPanel, Qt::AlignLeft);
    ui->mainPageLayout->setAlignment(ui->heroTitleLabel, Qt::AlignLeft);
    ui->mainPageLayout->setAlignment(ui->heroSubtitleLabel, Qt::AlignLeft);
    ui->mainPageLayout->setAlignment(ui->statusLabel, Qt::AlignLeft);
    ui->heroTitleLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    ui->heroSubtitleLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    ui->statusLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    ui->mainMenuPanel->setMaximumWidth(380);
    ui->heroTitleLabel->setMaximumWidth(520);
    ui->heroSubtitleLabel->setMaximumWidth(520);
    ui->statusLabel->setMaximumWidth(520);
    ui->startButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    ui->settingsButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    ui->exitButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    ui->startButton->setMinimumHeight(46);
    ui->settingsButton->setMinimumHeight(46);
    ui->exitButton->setMinimumHeight(46);
    ui->startButton->setMaximumHeight(46);
    ui->settingsButton->setMaximumHeight(46);
    ui->exitButton->setMaximumHeight(46);
}

void MainWindow::configurarTelaConfiguracao()
{
    ui->settingsPageLayout->setAlignment(ui->settingsPanel, Qt::AlignHCenter);
    ui->settingsTitleLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    ui->settingsSubtitleLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    ui->sectionTitleLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    ui->backButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    ui->backButton->setMinimumHeight(42);
    ui->backButton->setMaximumHeight(42);

    // Garante que o painel de configurações ocupe a tela inteira
    ui->settingsPage->setGeometry(ui->centralwidget->rect());
    ui->settingsPanel->setAttribute(Qt::WA_TranslucentBackground);
    ui->settingsPanel->setAutoFillBackground(false);
}

void MainWindow::iniciarAnimacoesBotoes()
{
    Q_UNUSED(this);
}

void MainWindow::animarEntradaWidget(QWidget *widget, int atrasoMs, int deslocamentoX)
{
    Q_UNUSED(widget);
    Q_UNUSED(atrasoMs);
    Q_UNUSED(deslocamentoX);
}

void MainWindow::animarTransicaoTela(QWidget *origem, QWidget *destino, bool empilhar)
{
    if (!origem || !destino || origem == destino)
        return;

    // Garante que BackgroundWidget está sempre visível
    if (backgroundWidget) {
        backgroundWidget->show();
        backgroundWidget->raise();
        backgroundWidget->update();
    }

    if (empilhar)
        telas.pushScreen(destino);
    else
        telas.popScreen();

    // Garante que contentLayout está acima do BackgroundWidget
    if (ui && ui->stackedWidget) {
        ui->stackedWidget->raise();
    }
}

void MainWindow::finalizarAnimacaoTela()
{
    transicaoTelaAtiva = false;
}

void MainWindow::atualizarLabelVolume(int value)
{
    if (ui && ui->volumeValueLabel)
        ui->volumeValueLabel->setText(QString::number(value) + "%");
}

void MainWindow::atualizarJogo()
{
    if (!jogo.estaAberto()) {
        gameTimer.stop();
        jogoInicializado = false;
        showNormal();
        if (backgroundWidget) {
            backgroundWidget->show();
            backgroundWidget->update();
        }
        raise();
        activateWindow();
        ui->statusLabel->setText("A janela do jogo foi fechada.");
        return;
    }

    jogo.atualizar();
}

void MainWindow::on_startButton_clicked()
{
    if (!jogoInicializado) {
        if (!jogo.inicializar()) {
            ui->statusLabel->setText("Falha ao inicializar o jogo.");
            return;
        }

        jogoInicializado = true;
        jogo.setVolumeMusica(static_cast<float>(ui->volumeSlider->value()));
        jogo.setMusicaLigada(ui->musicCheckBox->isChecked());
    }

    jogo.iniciarGameplay();
    ui->statusLabel->setText("Jogo em execucao.");
    if (backgroundWidget)
        backgroundWidget->hide();
    hide();
    gameTimer.start(16);
}

void MainWindow::on_settingsButton_clicked()
{
    // Garante que BackgroundWidget está visível
    if (backgroundWidget) {
        backgroundWidget->show();
        backgroundWidget->update();
    }

    animarTransicaoTela(ui->mainPage, ui->settingsPage, true);
    ui->statusLabel->setText("Configuracoes abertas.");
}

void MainWindow::on_backButton_clicked()
{
    // Garante que BackgroundWidget está visível
    if (backgroundWidget) {
        backgroundWidget->show();
        backgroundWidget->update();
    }

    animarTransicaoTela(ui->settingsPage, ui->mainPage, false);
    ui->statusLabel->setText("Menu principal.");
}

void MainWindow::on_musicCheckBox_toggled(bool checked)
{
    ui->musicCheckBox->setText(checked ? "Ativada" : "Desativada");

    if (jogoInicializado)
        jogo.setMusicaLigada(checked);
}

void MainWindow::on_volumeSlider_valueChanged(int value)
{
    atualizarLabelVolume(value);

    if (jogoInicializado)
        jogo.setVolumeMusica(static_cast<float>(value));
}

void MainWindow::on_exitButton_clicked()
{
    jogo.fechar();
    close();
}
