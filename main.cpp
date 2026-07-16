#include <QApplication>
#include <QStackedWidget>
#include "pomodorowindow.h"
#include "mainmenuwindow.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    QStackedWidget stack;

    MainMenuWindow* menu = new MainMenuWindow();
    PomodoroWindow* timerWindow = new PomodoroWindow();

    stack.addWidget(menu);
    stack.addWidget(timerWindow);

    QObject::connect(menu, &MainMenuWindow::timerButtonClicked, [&stack, timerWindow]() {
    stack.setCurrentWidget(timerWindow);
    timerWindow->setFocus();
    });
    QObject::connect(timerWindow, &PomodoroWindow::backToMenuRequested, [&stack, menu]() {
    stack.setCurrentWidget(menu);
    menu->setFocus();
    });

    stack.setCurrentWidget(menu);
    stack.resize(300, 200);
    stack.show();

    return app.exec();
}