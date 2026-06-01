#include "mainwindow.h"

#include <QGraphicsDropShadowEffect>

#include "ui_mainwindow.h"
#include <QPushButton>
#include <QString>
#include <QSizePolicy>
#include <QVariantAnimation>
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include "qtmaterialcheckbox.h"
#include "qtmaterialslider.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , gameTimer(this)
    , jogo()
    , jogoInicializado(false)
    , telas()
    , m_particulas(0)

{
    ui->setupUi(this);
    QtMaterialCheckBox *materialMusic = qobject_cast<QtMaterialCheckBox*>(ui->musicCheckBox);
    if (materialMusic) {
        materialMusic->setTextColor(QColor(0, 0, 0));            // Texto Preto
        materialMusic->setCheckedColor(QColor(0, 0, 0));         // Marcado: Preto
        materialMusic->setUncheckedColor(QColor(100, 100, 100)); // Desmarcado: Cinza
    }

    QtMaterialSlider *materialSlider = qobject_cast<QtMaterialSlider*>(ui->volumeSlider);
    if (materialSlider) {
        materialSlider->setThumbColor(QColor(0, 0, 0)); // Preto absoluto
    }
    configurarTelaPrincipal();
    configurarTelaConfiguracao();
    telas.setContainer(ui->stackedWidget);
    telas.setInitialScreen(ui->mainPage);

    aplicarEfeitosVisuais();


    ui->stackedWidget->raise();
    ui->centralwidget->raise();

    m_particulas = new ParticleWidget(ui->mainPage);
    m_particulas->lower();
    m_particulas->show();
    reposicionarParticleWidget();
    atualizarVisibilidadeParticleWidget();

    connect(&gameTimer, SIGNAL(timeout()), this, SLOT(atualizarJogo()));

    ui->musicCheckBox->setChecked(true);
    ui->volumeSlider->setRange(0, 100);
    ui->volumeSlider->setValue(50);
    atualizarLabelVolume(ui->volumeSlider->value());
    ui->startButton->iniciarAnimacaoEntrada(0);
    ui->settingsButton->iniciarAnimacaoEntrada(80);
    ui->exitButton->iniciarAnimacaoEntrada(160);
    ui->backButton->iniciarAnimacaoEntrada(0);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);

    reposicionarParticleWidget();
}
void MainWindow::aplicarEfeitosVisuais()
{

}

void MainWindow::configurarTelaPrincipal()
{
    ui->menuPanelLayout->setAlignment(Qt::AlignTop);
    ui->heroTitleLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    ui->heroSubtitleLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    ui->statusLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    ui->startButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    ui->settingsButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    ui->exitButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    ui->startButton->setMaximumHeight(100);
    ui->settingsButton->setMaximumHeight(100);
    ui->exitButton->setMaximumHeight(100);
}

void MainWindow::configurarTelaConfiguracao()
{
    ui->settingsPageLayout->setAlignment(ui->settingsPanel, Qt::AlignHCenter);
    ui->settingsTitleLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    ui->settingsSubtitleLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    ui->sectionTitleLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    ui->backButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    ui->settingsPage->setGeometry(ui->centralwidget->rect());
    ui->settingsPanel->setAttribute(Qt::WA_TranslucentBackground);
    ui->settingsPanel->setAutoFillBackground(false);
    ui->backButton->setTextAlignment(Qt::AlignCenter | Qt::AlignVCenter);

}

void MainWindow::animarTransicaoTela(QWidget *origem, QWidget *destino, bool empilhar)
{
    if (!origem || !destino || origem == destino)
        return;

    if (empilhar)
        telas.pushScreen(destino);
    else
        telas.popScreen();

    destino->setGeometry(ui->centralwidget->rect());
    destino->show();
    destino->raise();

    QGraphicsOpacityEffect *efeitoOrigem = qobject_cast<QGraphicsOpacityEffect *>(origem->graphicsEffect());
    if (!efeitoOrigem) {
        efeitoOrigem = new QGraphicsOpacityEffect(origem);
        origem->setGraphicsEffect(efeitoOrigem);
    }

    QGraphicsOpacityEffect *efeitoDestino = qobject_cast<QGraphicsOpacityEffect *>(destino->graphicsEffect());
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

    connect(fadeOut, &QPropertyAnimation::finished, this, [this, destino, efeitoOrigem, efeitoDestino, empilhar]() {
        ui->stackedWidget->setCurrentWidget(destino);
        efeitoOrigem->setOpacity(1.0);
        efeitoDestino->setOpacity(1.0);
        atualizarVisibilidadeParticleWidget();
    });

    fadeOut->start(QAbstractAnimation::DeleteWhenStopped);
    fadeIn->start(QAbstractAnimation::DeleteWhenStopped);

    ui->centralwidget->update();
    update();
}


void MainWindow::reposicionarParticleWidget()
{
    if (!m_particulas || !ui || !ui->centralwidget)
        return;

    const int larguraBase = ui->mainPage->width();
    const int alturaBase = ui->mainPage->height();
    if (larguraBase <= 0 || alturaBase <= 0)
        return;

    int largura = static_cast<int>((larguraBase * 2.0f) / 5.0f);
    int altura = static_cast<int>(alturaBase * 1.0f);
    if (largura < 1)
        largura = 1;
    if (altura < 1)
        altura = 1;

    const int margemDireita = 0;
    const int margemInferior = 0;
    int x = larguraBase - largura - margemDireita;
    int y = alturaBase - altura - margemInferior;
    if (x < 0)
        x = 0;
    if (y < 0)
        y = 0;

    m_particulas->setGeometry(x, y, largura, altura);
    m_particulas->lower();
}

void MainWindow::atualizarVisibilidadeParticleWidget()
{
    if (!m_particulas || !ui || !ui->stackedWidget)
        return;

    const bool mostrar = (ui->stackedWidget->currentWidget() == ui->mainPage);
    m_particulas->setVisible(mostrar);
    if (mostrar)
        m_particulas->lower();
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
    ui->statusLabel->setText("Jogo em execução");
    hide();
    gameTimer.start(16);
}

void MainWindow::on_settingsButton_clicked()
{
    animarTransicaoTela(ui->mainPage, ui->settingsPage, true);
    ui->statusLabel->setText("Configurações abertas.");
}

void MainWindow::on_backButton_clicked()
{
    animarTransicaoTela(ui->settingsPage, ui->mainPage, false);
    ui->statusLabel->setText("Menu principal");
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