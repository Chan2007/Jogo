#include "mainwindow.h"

#include <QGraphicsDropShadowEffect>

#include "ui_mainwindow.h"
#include <QMenuBar>
#include <QPushButton>
#include <QString>
#include <QSizePolicy>
#include <QStatusBar>
#include <QVariantAnimation>

static QString corCss(const QColor &cor)
{
    return QString("rgba(%1, %2, %3, %4)")
        .arg(cor.red())
        .arg(cor.green())
        .arg(cor.blue())
        .arg(cor.alpha());
}

static QString estiloBotao(const QColor &bg, const QColor &borda, bool centralizado, int paddingLeft)
{
    const QString alinhamento = centralizado ? "center" : "left";
    const int paddingRight = centralizado ? 32 : 0;
    return QString(
        "QPushButton {"
        "font-family: 'Segoe UI', 'Arial', 'Helvetica';"
        "font-size: 18px;"
        "font-weight: 900;"
        "background-color: %1;"
        "border: 2px solid %2;"
        "border-radius: 0px;"
        "padding: 30px %3px 30px %4px;"
        "text-align: %5;"
        "}"
    ).arg(corCss(bg)).arg(corCss(borda)).arg(paddingRight).arg(paddingLeft).arg(alinhamento);
}
static QColor interpolarCor(const QColor &inicio, const QColor &fim, double t)
{
    const double inv = 1.0 - t;
    return QColor(
        qBound(0, static_cast<int>(inicio.red() * inv + fim.red() * t), 255),
        qBound(0, static_cast<int>(inicio.green() * inv + fim.green() * t), 255),
        qBound(0, static_cast<int>(inicio.blue() * inv + fim.blue() * t), 255),
        qBound(0, static_cast<int>(inicio.alpha() * inv + fim.alpha() * t), 255));
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , gameTimer(this)
    , jogo()
    , jogoInicializado(false)
    , telas()
    , m_particulas(0)
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
    ui->mainPage->setAttribute(Qt::WA_TranslucentBackground);
    ui->mainPage->setAutoFillBackground(false);
    ui->settingsPage->setAttribute(Qt::WA_TranslucentBackground);
    ui->settingsPage->setAutoFillBackground(false);
    ui->mainMenuPanel->setAttribute(Qt::WA_TranslucentBackground);
    ui->mainMenuPanel->setAutoFillBackground(false);

    aplicarEfeitosVisuais();
    prepararBotaoAnimado(ui->startButton, QColor(247, 247, 247), QColor(208, 208, 208), QColor(154, 154, 154), QColor(0, 0, 0));
    prepararBotaoAnimado(ui->settingsButton, QColor(247, 247, 247), QColor(208, 208, 208), QColor(154, 154, 154), QColor(0, 0, 0));
    prepararBotaoAnimado(ui->exitButton, QColor(247, 247, 247), QColor(208, 208, 208), QColor(154, 154, 154), QColor(0, 0, 0));
    prepararBotaoAnimado(ui->backButton, QColor(247, 247, 247), QColor(208, 208, 208), QColor(154, 154, 154), QColor(0, 0, 0));

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

    reposicionarParticleWidget();
}
void MainWindow::aplicarEfeitosVisuais()
{
    QGraphicsDropShadowEffect *sombraMenu = new QGraphicsDropShadowEffect(ui->mainMenuPanel);
    sombraMenu->setBlurRadius(24.0);
    sombraMenu->setOffset(0, 5);
    sombraMenu->setColor(QColor(0, 0, 0, 120));
    ui->mainMenuPanel->setGraphicsEffect(sombraMenu);

    QGraphicsDropShadowEffect *sombraSettings = new QGraphicsDropShadowEffect(ui->settingsPanel);
    sombraSettings->setBlurRadius(28.0);
    sombraSettings->setOffset(0, 6);
    sombraSettings->setColor(QColor(0, 0, 0, 140));
    ui->settingsPanel->setGraphicsEffect(sombraSettings);
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

    if (empilhar)
        telas.pushScreen(destino);
    else
        telas.popScreen();

    if (ui && ui->stackedWidget) {
        ui->stackedWidget->setCurrentWidget(destino);
    }

    destino->setGeometry(ui->centralwidget->rect());
    destino->show();
    destino->raise();

    if (ui && ui->stackedWidget) {
        ui->stackedWidget->raise();
    }
    ui->centralwidget->raise();

    atualizarVisibilidadeParticleWidget();

    ui->centralwidget->update();
    this->update();
}

void MainWindow::finalizarAnimacaoTela()
{
    transicaoTelaAtiva = false;
}

void MainWindow::prepararBotaoAnimado(QPushButton *botao, const QColor &bgBase, const QColor &bgHover, const QColor &bordaBase, const QColor &bordaHover)
{
    if (!botao)
        return;

    const bool centralizado = (botao == ui->backButton);
    botao->setProperty("animProgress", 0.0);
    botao->setProperty("bgBase", bgBase);
    botao->setProperty("bgHover", bgHover);
    botao->setProperty("bordaBase", bordaBase);
    botao->setProperty("bordaHover", bordaHover);
    botao->setProperty("botaoCentralizado", centralizado);
    botao->setStyleSheet(estiloBotao(bgBase, bordaBase, centralizado, centralizado ? 0 : 18));
    botao->installEventFilter(this);
}

void MainWindow::iniciarAnimacaoBotao(QPushButton *botao, double destino, int duracaoMs)
{
    if (!botao)
        return;

    const QObjectList filhos = botao->children();
    for (int i = 0; i < filhos.size(); ++i) {
        QVariantAnimation *animAnterior = qobject_cast<QVariantAnimation*>(filhos.at(i));
        if (animAnterior) {
            animAnterior->stop();
            animAnterior->deleteLater();
        }
    }

    QVariantAnimation *animacao = new QVariantAnimation(botao);
    animacao->setDuration(duracaoMs);
    animacao->setEasingCurve(QEasingCurve::Linear);
    animacao->setStartValue(botao->property("animProgress").toDouble());
    animacao->setEndValue(destino);

    connect(animacao, SIGNAL(valueChanged(QVariant)), this, SLOT(atualizarAnimacaoBotao(QVariant)));
    connect(animacao, SIGNAL(finished()), animacao, SLOT(deleteLater()));

    animacao->start();
}

void MainWindow::atualizarAnimacaoBotao(const QVariant &valor)
{
    QVariantAnimation *animacao = qobject_cast<QVariantAnimation*>(sender());
    if (!animacao)
        return;

    QPushButton *botao = qobject_cast<QPushButton*>(animacao->parent());
    if (!botao)
        return;

    const double t = valor.toDouble();
    botao->setProperty("animProgress", t);

    // CORREÇÃO: Passagem direta das propriedades para evitar avisos de variáveis locais não lidas
    const QColor bgAtual = interpolarCor(
        botao->property("bgBase").value<QColor>(),
        botao->property("bgHover").value<QColor>(),
        t
    );

    const QColor bordaAtual = interpolarCor(
        botao->property("bordaBase").value<QColor>(),
        botao->property("bordaHover").value<QColor>(),
        t
    );

    const bool centralizado = botao->property("botaoCentralizado").toBool();
    const int paddingLeft = centralizado ? 0 : static_cast<int>(18 + (8 * t));

    botao->setStyleSheet(estiloBotao(bgAtual, bordaAtual, centralizado, paddingLeft));
}

void MainWindow::animarPaginaConfiguracao(bool entrando)
{
    // CORREÇÃO: Mantido propositalmente vazio para blindar o motor gráfico do QPainter contra colisões de opacidade
    (void)entrando;
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

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    QPushButton *botao = qobject_cast<QPushButton*>(obj);
    if (!botao)
        return QMainWindow::eventFilter(obj, event);

    if (event->type() == QEvent::Enter) {
        iniciarAnimacaoBotao(botao, 1.0, 160);
    } else if (event->type() == QEvent::Leave) {
        iniciarAnimacaoBotao(botao, 0.0, 160);
    } else if (event->type() == QEvent::MouseButtonPress) {
        iniciarAnimacaoBotao(botao, 1.0, 90);
    } else if (event->type() == QEvent::MouseButtonRelease) {
        const bool sobMouse = botao->underMouse();
        iniciarAnimacaoBotao(botao, sobMouse ? 1.0 : 0.0, 120);
    }

    return QMainWindow::eventFilter(obj, event);
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
    ui->statusLabel->setText("Jogo em execucao.");
    hide();
    gameTimer.start(16);
}

void MainWindow::on_settingsButton_clicked()
{
    animarTransicaoTela(ui->mainPage, ui->settingsPage, true);
    ui->statusLabel->setText("Configuracoes abertas.");
}

void MainWindow::on_backButton_clicked()
{
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