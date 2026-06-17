#ifndef JOGO_QSFMLWIDGET_H
#define JOGO_QSFMLWIDGET_H

#include <QWidget>
#include <QHideEvent>
#include <QResizeEvent>
#include <QShowEvent>
#include <QPaintEvent>
#include <SFML/Graphics.hpp>
#include <QTimer>

namespace Widgets {
    class QSFMLWidget : public QWidget
    {
        Q_OBJECT
    public:
        explicit QSFMLWidget(QWidget *parent = 0);
        virtual ~QSFMLWidget();

        sf::Vector2u getSize() const { return m_window.getSize(); } // TODO

        sf::RenderWindow& getWindow() { return m_window; } // TODO

    private slots:
        void onTimerTimeout();

    protected:
        virtual void OnInit() = 0;
        virtual void OnUpdate() = 0;

        virtual QPaintEngine* paintEngine() const;
        virtual void showEvent(QShowEvent*);
        virtual void hideEvent(QHideEvent*);
        virtual void resizeEvent(QResizeEvent*);
        virtual void paintEvent(QPaintEvent*);

        bool m_initialized;
        QTimer m_timer;

        sf::RenderWindow m_window;
    };
}

#endif //JOGO_QSFMLWIDGET_H
