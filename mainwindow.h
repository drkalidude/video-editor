#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QVideoWidget>
#include <QPushButton>
#include <QListWidgetItem>
#include <QTime>
#include <QList>
#include "dialog.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

struct VideoData {
    QString filePath;
    qint64 startTime;
    qint64 endTime;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_addButton_clicked();

private:
    Ui::MainWindow *ui;
    QMediaPlayer *player;
    QVideoWidget *video;
    QMediaPlaylist *playlist;
    QVideoWidget *videoWidget;
    qint64 mDuration;
    int currentIndex;
    bool isPause;
    bool isSound;
    QList<VideoData> videoDataList;

    void setupVideoWidget();

};

#endif 


