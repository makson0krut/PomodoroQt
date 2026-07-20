//
// Created by Максим Сустаев on 16.07.2026.
//

#include "pomodorowindow.h"
#include <QVBoxLayout>
#include <QKeyEvent>

PomodoroWindow::PomodoroWindow(QWidget* parent) : QWidget(parent) {
    startMusic();

    //Таймер для красоты
    circularTimer = new CircularTimerWidget(this);
    circularTimer->setMinimumSize(200, 200);

    //Переменны которые передаем в лейбл
    state = PomodoroState::Work;
    completedWorkSessions = 0;
    remainingSeconds = getDurationForState(state);
    //Создаем лейбл что будет написанно на экране
    stateLabel = new QLabel(QString::fromStdString(stateToString(state)), this);
    timeLabel = new QLabel(this);
    startPauseButton =  new QPushButton("Start", this);
    backButton = new QPushButton("В меню", this);
    //Даем виджетам имена для стилизации
    stateLabel->setObjectName("stateLabel");
    timeLabel->setObjectName("timeLabel");

    //Создаем новый слой
    QVBoxLayout* layout = new QVBoxLayout(this);

    layout->setContentsMargins(20, 20, 20, 20);
    layout->setSpacing(15);

    //Порядок вывода
    layout->addWidget(stateLabel);
    layout->addWidget(circularTimer);
    layout->addWidget(timeLabel);
    layout->addWidget(startPauseButton);
    layout->addWidget(backButton);
    setLayout(layout);

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &PomodoroWindow::onTick);
    connect(startPauseButton, &QPushButton::clicked, this, &PomodoroWindow::onStartPauseClicked);
    connect(backButton, &QPushButton::clicked, this, &PomodoroWindow::backToMenuRequested);

}

PomodoroWindow::~PomodoroWindow() {
    stopMusic();
}
void PomodoroWindow::resetTimer() {
    timer->stop();
    state = PomodoroState::Work;
    completedWorkSessions = 0;
    remainingSeconds = getDurationForState(state);
    stateLabel->setText(QString::fromStdString(stateToString(state)));
    startPauseButton->setText("Start");
    circularTimer->setTime(remainingSeconds, getDurationForState(state));
    updateDisplay();
}
//Получаем какая стадия будет следующей
PomodoroState PomodoroWindow::getNextState(PomodoroState current, int completedWorkSessions) {
    switch (current) {
        case PomodoroState::LongBreak: return PomodoroState::Work;
        case PomodoroState::ShortBreak: return PomodoroState::Work;
        case PomodoroState::Work:
            if (completedWorkSessions % 4 == 0 && completedWorkSessions != 0)
                return PomodoroState::LongBreak;
            else return PomodoroState::ShortBreak;
    }
    return PomodoroState::Work;
}
int PomodoroWindow::getDurationForState(PomodoroState state) {
    switch (state) {
        case PomodoroState::Work: return 25*60;
        case PomodoroState::ShortBreak: return 5*60;
        case PomodoroState::LongBreak: return 10*60;
    }
    return 5;
}
//Обработка нажития для выхода и паузы
void PomodoroWindow::keyPressEvent(QKeyEvent* event) {
    if (event->key() == Qt::Key_P) {
        onStartPauseClicked();
    } else if (event->key() == Qt::Key_Escape) {
        emit backToMenuRequested();
    }
}
//Нормальный вывод для пользователя какая стадия
std::string PomodoroWindow::stateToString(PomodoroState state) {
    switch (state) {
        case PomodoroState::Work: return "Work";
        case PomodoroState::ShortBreak: return "ShortBreak";
        case PomodoroState::LongBreak: return "LongBreak";
    }
    return "Unknown";
}
//Показ времени в нормальном виде
void PomodoroWindow::updateDisplay() {
    int minutes = remainingSeconds / 60;
    int seconds = remainingSeconds % 60;

    QString text = QString("%1:%2")
    .arg(minutes, 2, 10, QChar('0'))
    .arg(seconds, 2, 10, QChar('0'));

    timeLabel->setText(text);
}

//При запсуке программы стоит пауза таймера
// ReSharper disable once CppMemberFunctionMayBeConst
void PomodoroWindow::onStartPauseClicked() {
    if (timer->isActive()) {
        timer->stop();
        startPauseButton->setText("Start");
    } else {
        timer->start(1000);
        startPauseButton->setText("Pause");
    }
}

//Переход на другую стадию без 00:00
void PomodoroWindow::onTick() {
    remainingSeconds--;
    if (remainingSeconds <= 0) {
        if (state == PomodoroState::Work) {
            completedWorkSessions++;
            if (shouldLogStats()) {
                logStats();
            }
        }
        state = getNextState(state, completedWorkSessions);
        remainingSeconds = getDurationForState(state);
        stateLabel->setText(QString::fromStdString(stateToString(state)));
        playSound();
    }
    circularTimer->setTime(remainingSeconds, getDurationForState(state));
    updateDisplay();
}

std::string PomodoroWindow::getCurrentDateTime() {
    std::time_t now = std::time(nullptr);
    std::tm* localTime = std::localtime(&now);

    static const std::string months[] = {
        "Января", "Февраля", "Марта", "Апреля", "Мая", "Июня",
        "Июля", "Августа", "Сентября", "Октября", "Ноября", "Декабря"
    };

    int day = localTime->tm_mday;
    int monthIndex = localTime->tm_mon;

    return std::to_string(day) + " " + months[monthIndex];
}
bool PomodoroWindow::shouldLogStats() {
    if (completedWorkSessions % 4 == 0 && completedWorkSessions != 0)
        return true;
    else
        return false;
}
void PomodoroWindow::logStats() {
    std::ofstream file("pomodoro_stats.txt", std::ios::app);
    if (file.is_open()) {
        file << getCurrentDateTime() << " - Цикл завершён\n";
        file.close();
    }
}
//Музыка
void PomodoroWindow::playSound() {
    std::system(("afplay /System/Library/Sounds/Glass.aiff &"));
}
void PomodoroWindow::startMusic() {
    std::system("(ls ~/pomodoro_music/*.mp3 | sort -R | while read f; do afplay \"$f\"; done) &");
}
void PomodoroWindow::stopMusic() {
    std::system("pkill -f 'pomodoro_music'");
    std::system("pkill afplay");
}
