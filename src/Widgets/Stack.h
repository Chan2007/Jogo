#ifndef SCREEN_STACK_H
#define SCREEN_STACK_H

#include <QObject>
#include <QVector>

class QWidget;
class QStackedWidget;
class QGraphicsOpacityEffect;
namespace Widgets {
    class Stack: public QObject {
        Q_OBJECT
    public:
        explicit Stack(QStackedWidget* container = NULL);

        void setContainer(QStackedWidget* container);
        void setInitialScreen(QWidget* screen);
        void pushScreen(QWidget* screen);
        void popScreen();

    private slots:
        void onFadeOutFinished();
    private:
        void init_Animation(QWidget* startW, QWidget* endW);
        QStackedWidget* stackedWidget;
        QVector<QWidget*> stack;
        QWidget *end;
        QGraphicsOpacityEffect* startEffect;
        QGraphicsOpacityEffect* endEffect;
    };
}

#endif // SCREEN_STACK_H
