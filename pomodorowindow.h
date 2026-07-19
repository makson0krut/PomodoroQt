//
// Created by Максим Сустаев on 16.07.2026.
//

#ifndef POMODOROWINDOW_H
#define POMODOROWINDOW_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QTimer>
#include <QKeyEvent>

#include <string>
#include <cstdlib>
#include <fstream>
#include <ctime>

#include "circulartimerwidget.h"

//Описание стадий
enum class PomodoroState {
    Work,
    ShortBreak,
    LongBreak,
};

class PomodoroWindow : public QWidget {
    Q_OBJECT

public:
    PomodoroWindow(QWidget* parent = nullptr);
    ~PomodoroWindow();
    void resetTimer();

private slots:
    void onTick();
    void onStartPauseClicked();
protected:
    void keyPressEvent(QKeyEvent* event) override;
signals:
    void backToMenuRequested();
private:
    //Определяем следующую стадию
    PomodoroState getNextState(PomodoroState current, int completedWorkSessions);
    //Получаем время каждой стадии
    int getDurationForState(PomodoroState state);
    //Выводим в нормальном виде какая стадия
    std::string stateToString(PomodoroState state);
    void updateDisplay();
    //Музыка
    void playSound();
    void startMusic();
    void stopMusic();
    void logStats();


    PomodoroState state;
    int completedWorkSessions;
    int remainingSeconds;
    std::string getCurrentDateTime();
    bool shouldLogStats();


    QLabel* timeLabel;
    QLabel* stateLabel;
    QPushButton* startPauseButton;
    QPushButton* backButton;
    QTimer* timer;
    CircularTimerWidget* circularTimer;
};



#endif //POMODOROWINDOW_H
