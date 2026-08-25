#include "Stack.h"

#include <qabstractanimation.h>
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>
#include <QStackedWidget>

namespace Widgets {
    Stack::Stack(QStackedWidget *container): stackedWidget(container), stack(), end(NULL), startEffect(NULL), endEffect(NULL) {}

    void Stack::setContainer(QStackedWidget *container) {
        stackedWidget = container;
        stack.clear();
    }

    void Stack::setInitialScreen(QWidget *screen) {
        if (!stackedWidget || !screen) return;

        stack.clear();
        stack.push_back(screen);
        stackedWidget->setCurrentWidget(screen);
    }

    void Stack::pushScreen(QWidget *screen) {
        if (!stackedWidget || !screen) return;

        if (!stack.isEmpty() && stack.back() == screen) {
            stackedWidget->setCurrentWidget(screen);
            return;
        }
        if (stackedWidget->indexOf(screen) == -1)
            stackedWidget->addWidget(screen);

        QWidget *origem = !stack.isEmpty() ? stack.back() : NULL;
        stack.push_back(screen);

        if (origem)
            init_Animation(origem, screen);
        else
            stackedWidget->setCurrentWidget(screen);
    }

    void Stack::popScreen() {
        if (!stackedWidget || stack.size() <= 1) return;

        QWidget *origem = !stack.isEmpty() ? stack.back() : NULL;
        stack.pop_back();
        QWidget *destino = !stack.isEmpty() ? stack.back() : NULL;

        init_Animation(origem, destino);
    }
    void Stack::init_Animation(QWidget *startW, QWidget *endW) {
        if (!startW || !endW || startW == endW) return;

        stackedWidget->setCurrentWidget(endW);

        // Opacidade na tela de origem
        startEffect = qobject_cast<QGraphicsOpacityEffect*>(startW->graphicsEffect());
        if (!startEffect) {
            startEffect = new QGraphicsOpacityEffect(startW);
            startW->setGraphicsEffect(startEffect);
        }

        // Opacidade na tela de destino
        endEffect = qobject_cast<QGraphicsOpacityEffect*>(endW->graphicsEffect());
        if (!endEffect) {
            endEffect = new QGraphicsOpacityEffect(endW);
            endW->setGraphicsEffect(endEffect);
        }

        startEffect->setOpacity(1.0);
        endEffect->setOpacity(0.0);

        QPropertyAnimation* fadeOut = new QPropertyAnimation(startEffect, "opacity");
        fadeOut->setDuration(1000);
        fadeOut->setStartValue(1.0);
        fadeOut->setEndValue(0.0);

        QPropertyAnimation* fadeIn = new QPropertyAnimation(endEffect, "opacity");
        fadeIn->setDuration(1000);
        fadeIn->setStartValue(0.0);
        fadeIn->setEndValue(1.0);

        end = endW;

        // Conecta o fim do fade out ao slot interno da classe
        connect(fadeOut, &QPropertyAnimation::finished, this, &Stack::onFadeOutFinished);

        fadeOut->start(QAbstractAnimation::DeleteWhenStopped);
        fadeIn->start(QAbstractAnimation::DeleteWhenStopped);
    }

    void Stack::onFadeOutFinished() {
        if (startEffect) {
            QWidget* origemWidget = qobject_cast<QWidget*>(startEffect->parent());
            if (origemWidget) origemWidget->setGraphicsEffect(NULL);
            startEffect = NULL;
        }
        if (endEffect) {
            if (end) end->setGraphicsEffect(NULL);
            endEffect = NULL;
        }
        end = NULL;
    }
}
