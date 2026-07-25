#ifndef MAINMENUWINDOW_H
#define MAINMENUWINDOW_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QSlider>

class MainMenuWindow : public QWidget {
    Q_OBJECT

public:
    MainMenuWindow(QWidget* parent = nullptr);

    signals:
        void timerButtonClicked();
    void statsButtonClicked();
    void tasksButtonClicked();
    void volumeChanged(int volumePercent);

private slots:
    void onStubButtonClicked();
    void onVolumeSliderChanged(int value);

private:
    QLabel* titleLabel;
    QPushButton* timerButton;
    QPushButton* tasksButton;
    QPushButton* statsButton;
    QSlider* volumeSlider;
};

#endif // MAINMENUWINDOW_H