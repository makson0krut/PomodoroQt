//
// Created by Максим Сустаев on 16.07.2026.
//

#include "pomodorowindow.h"
#include <QVBoxLayout>
#include <QKeyEvent>
#include <QCoreApplication>
#include <QDir>

PomodoroWindow::PomodoroWindow(QWidget* parent) : QWidget(parent) {
    startMusic();

    //Таймер для красоты
    circularTimer = new CircularTimerWidget(this);
    circularTimer->setMinimumSize(200, 200);


    taskSelectorLabel = new QLabel("Работаю над:", this);
    taskSelectorLabel->setObjectName("taskSelectorLabel");
    taskSelector = new QComboBox(this);
    taskSelector->addItem("Без привязки к задаче");
    taskSelector->addItems(loadIncompleteTasks());

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
    layout->addWidget(taskSelectorLabel);
    layout->addWidget(taskSelector);
    layout->addWidget(stateLabel);
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

    taskSelector->clear();
    taskSelector->addItem("Без привязки к задаче");
    taskSelector->addItems(loadIncompleteTasks());
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
        case PomodoroState::Work: return 5;
        case PomodoroState::ShortBreak: return 2;
        case PomodoroState::LongBreak: return 6;
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
        "января", "февраля", "марта", "апреля", "мая", "июня",
        "июля", "августа", "сентября", "октября", "ноября", "декабря"
    };
    return std::to_string(localTime->tm_mday) + " " + months[localTime->tm_mon];
}
bool PomodoroWindow::shouldLogStats() {
    if (completedWorkSessions % 4 == 0 && completedWorkSessions != 0)
        return true;
    else
        return false;
}
void PomodoroWindow::logStats() {
    std::string path = getStorageDirPath().toStdString() + "/pomodoro_stats.txt";
    std::ofstream file(path, std::ios::app);
    if (file.is_open()) {
        file << getIsoDate() << "\n";
        file.close();
    }
}

std::string PomodoroWindow::getIsoDate() {
    std::time_t now = std::time(nullptr);
    std::tm* localTime = std::localtime(&now);
    char buffer[11];
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d", localTime);
    return std::string(buffer);
}
//Музыка
void PomodoroWindow::playSound() {
    std::system(("afplay /System/Library/Sounds/Glass.aiff &"));
}
void PomodoroWindow::startMusic() {
    std::system(
        "/opt/homebrew/bin/mpv --no-video --shuffle --loop-playlist --volume=50 "
        "--input-ipc-server=/tmp/pomodoro_mpv_socket "
        "~/pomodoro_music/*.mp3 "
        "> /dev/null 2>&1 &"
    );
}
void PomodoroWindow::setVolume(int volumePercent) {
    std::string command = "echo '{\"command\": [\"set_property\", \"volume\", "
        + std::to_string(volumePercent)
        + "]}' | /opt/homebrew/bin/socat - /tmp/pomodoro_mpv_socket";
    std::system(command.c_str());
}
void PomodoroWindow::stopMusic() {
    std::system("pkill mpv");
}
QString PomodoroWindow::getStorageDirPath() {
    QDir dir(QCoreApplication::applicationDirPath());
    dir.cdUp(); // из .../PomodoroQt.app/Contents/MacOS в .../PomodoroQt.app/Contents
    dir.cdUp(); // в .../PomodoroQt.app
    dir.cdUp(); // в папку, содержащую .app
    return dir.absolutePath();
}

QStringList PomodoroWindow::loadIncompleteTasks() {
    QStringList result;
    std::string path = getStorageDirPath().toStdString() + "/pomodoro_tasks.txt";
    std::ifstream file(path);

    if (!file.is_open()) {
        return result;
    }

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) {
            continue;
        }

        size_t firstSep = line.find('|');
        size_t secondSep = line.find('|', firstSep + 1);
        if (firstSep == std::string::npos || secondSep == std::string::npos) {
            continue;
        }

        std::string donePart = line.substr(0, firstSep);
        std::string textPart = line.substr(secondSep + 1);

        if (donePart == "0") { // только невыполненные
            result.append(QString::fromStdString(textPart));
        }
    }

    file.close();
    return result;
}
