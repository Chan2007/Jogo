#ifndef JOGO_QSFMLWIDGET_H
#define JOGO_QSFMLWIDGET_H

#include <QWidget>
#include <QHideEvent>
#include <QShowEvent>
#include <QPaintEvent>
#include <SFML/Graphics.hpp>
#include <QTimer>

class QSFMLWidget : public QWidget
{
    Q_OBJECT
public:
    explicit QSFMLWidget(QWidget *parent = 0);
    virtual ~QSFMLWidget();

    // MÉTODOS DE COMPATIBILIDADE PARA O C++03:
    // 1. Resolve o erro: Cannot resolve symbol 'getSize'
    sf::Vector2u getSize() const { return m_window.getSize(); }

    // 2. Resolve o erro: Non-const lvalue reference...
    // Permite que qualquer classe pegue a janela interna do SFML para desenhar
    sf::RenderWindow& getWindow() { return m_window; }

private slots:
    void onTimerTimeout();

protected:
    virtual void OnInit() = 0;
    virtual void OnUpdate() = 0;

    virtual QPaintEngine* paintEngine() const;
    virtual void showEvent(QShowEvent*);
    virtual void hideEvent(QHideEvent*);
    virtual void paintEvent(QPaintEvent*);

    bool m_initialized;
    QTimer m_timer;

    sf::RenderWindow m_window;
};

#endif //JOGO_QSFMLWIDGET_H
