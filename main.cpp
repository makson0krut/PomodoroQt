#include <QApplication>
#include <QStackedWidget>
#include "pomodorowindow.h"
#include "mainmenuwindow.h"
#include "statswindow.h"
#include "taskwindow.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    app.setStyleSheet(R"(
    QWidget {
        background-color: #1e1e2e;
        color: #cdd6f4;
        font-family: -apple-system;
    }

        QLabel#titleLabel {
        font-size: 32px;
        font-weight: bold;
        color: #f9e2af;
        qproperty-alignment: AlignCenter;
    }

    QPushButton#primaryButton {
        background-color: #89b4fa;
        color: #1e1e2e;
        font-weight: bold;
        padding: 14px 20px;
        font-size: 16px;
    }

    QPushButton#primaryButton:hover {
        background-color: #a6c4fc;
    }

    QPushButton#primaryButton:pressed {
        background-color: #6c9ff0;
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

    QLineEdit, QDateEdit {
        background-color: #313244;
        color: #cdd6f4;
        border: 2px solid #45475a;
        border-radius: 8px;
        padding: 8px 12px;
        font-size: 14px;
    }

    QLineEdit:focus, QDateEdit:focus {
        border: 2px solid #89b4fa;
    }
    QLabel#statsCounter {
        font-size: 22px;
        font-weight: bold;
        color: #9ac3fb;
        padding: 8px;
    }
    QLabel#taskSelectorLabel {
        font-size: 13px;
        color: #9399b2;
    }

    QComboBox#taskSelector {
        background-color: #313244;
        color: #cdd6f4;
        border: 2px solid #45475a;
        border-radius: 8px;
        padding: 8px 12px;
        font-size: 14px;
    }

    QComboBox#taskSelector:hover {
        border: 2px solid #89b4fa;
    }

    QComboBox#taskSelector::drop-down {
        border: none;
        width: 30px;
    }

    QComboBox#taskSelector QAbstractItemView {
        background-color: #313244;
        color: #cdd6f4;
        selection-background-color: #45475a;
        border: 2px solid #45475a;
        border-radius: 8px;
        padding: 4px;
    }
)");
    QStackedWidget stack;

    MainMenuWindow* menu = new MainMenuWindow();
    PomodoroWindow* timerWindow = new PomodoroWindow();
    StatsWindow* statsWindow = new StatsWindow();
    TasksWindow* tasksWindow = new TasksWindow();

    stack.addWidget(menu);
    stack.addWidget(timerWindow);
    stack.addWidget(statsWindow);
    stack.addWidget(tasksWindow);

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
    QObject::connect(menu, &MainMenuWindow::tasksButtonClicked, [&stack, tasksWindow]() {
    tasksWindow->loadTasks();
    stack.setCurrentWidget(tasksWindow);
    tasksWindow->setFocus();
});

    QObject::connect(tasksWindow, &TasksWindow::backToMenuRequested, [&stack, menu]() {
        stack.setCurrentWidget(menu);
        menu->setFocus();
    });
    //Громкость
    QObject::connect(menu, &MainMenuWindow::volumeChanged, timerWindow, &PomodoroWindow::setVolume);

    stack.setCurrentWidget(menu);
    stack.resize(450, 500);
    stack.show();

    return app.exec();
}