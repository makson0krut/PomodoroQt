#include <QApplication>
#include <QStackedWidget>
#include "pomodorowindow.h"
#include "mainmenuwindow.h"
#include "statswindow.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    app.setStyleSheet(R"(
    QWidget {
        background-color: #1e1e2e;
        color: #cdd6f4;
        font-family: -apple-system;
    }

    QLabel#stateLabel {
        font-size: 20px;
        color: #89b4fa;
        font-weight: bold;
        qproperty-alignment: AlignCenter;
    }

    QLabel#timeLabel {
        font-size: 56px;
        font-weight: bold;
        color: #f9e2af;
        qproperty-alignment: AlignCenter;
    }

    QPushButton {
        background-color: #313244;
        color: #cdd6f4;
        border: none;
        border-radius: 8px;
        padding: 10px 20px;
        font-size: 14px;
    }

    QPushButton:hover {
        background-color: #45475a;
    }

    QPushButton:pressed {
        background-color: #585b70;
    }
)");
    QStackedWidget stack;

    MainMenuWindow* menu = new MainMenuWindow();
    PomodoroWindow* timerWindow = new PomodoroWindow();
    StatsWindow* statsWindow = new StatsWindow();

    stack.addWidget(menu);
    stack.addWidget(timerWindow);
    stack.addWidget(statsWindow);

    QObject::connect(menu, &MainMenuWindow::timerButtonClicked, [&stack, timerWindow]() {
        timerWindow->resetTimer();
        stack.setCurrentWidget(timerWindow);
        timerWindow->setFocus();
    });
    QObject::connect(timerWindow, &PomodoroWindow::backToMenuRequested, [&stack, menu]() {
        stack.setCurrentWidget(menu);
        menu->setFocus();
    });

    QObject::connect(menu, &MainMenuWindow::statsButtonClicked, [&stack, statsWindow]() {
    statsWindow->loadStats();
    stack.setCurrentWidget(statsWindow);
    statsWindow->setFocus();
});

    QObject::connect(statsWindow, &StatsWindow::backToMenuRequested, [&stack, menu]() {
        stack.setCurrentWidget(menu);
        menu->setFocus();
    });

    stack.setCurrentWidget(menu);
    stack.resize(400, 500);
    stack.show();

    return app.exec();
}