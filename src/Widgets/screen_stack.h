#ifndef SCREEN_STACK_H
#define SCREEN_STACK_H

#include <QStackedWidget>
#include <QVector>

class ScreenStack
{
public:
    explicit ScreenStack(QStackedWidget *container = 0);

    void setContainer(QStackedWidget *container);
    void setInitialScreen(QWidget *screen);
    void pushScreen(QWidget *screen);
    void popScreen();
    QWidget* currentScreen() const;

private:
    QStackedWidget *stackedWidget;
    QVector<QWidget*> historico;
};

#endif // SCREEN_STACK_H
