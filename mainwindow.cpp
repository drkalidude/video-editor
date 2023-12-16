#include <cstdlib>
#include <QString>
#include <QDebug>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dialog.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow),
      videoWidget(new QVideoWidget(this)),
      player(new QMediaPlayer(this)),
      playlist(new QMediaPlaylist(this)),
      currentIndex(0),
      isPause(false),
      isSound(true)
{

    ui->setupUi(this);

    setupVideoWidget();
    player->setVideoOutput(videoWidget);
    player->setPlaylist(playlist);

    ui->verticalSlider_volume->setMinimum(0);
    ui->verticalSlider_volume->setMaximum(100);
    ui->verticalSlider_volume->setValue(30);

    player->setVolume(ui->verticalSlider_volume->value());

    ui->startStopButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    ui->nextButton->setIcon(style()->standardIcon(QStyle::SP_MediaSeekBackward));
    ui->previousButton->setIcon(style()->standardIcon(QStyle::SP_MediaSeekForward));
    ui->pushButton_sound->setIcon(style()->standardIcon(QStyle::SP_MediaVolume));
    ui->horizontalSlider_time->setRange(0, player->duration() / 1000);

    connect(player, &QMediaPlayer::durationChanged, this, &MainWindow::durationChanged);
    connect(player, &QMediaPlayer::positionChanged, this, &MainWindow::changed);
    connect(player, &QMediaPlayer::mediaChanged, this, &MainWindow::on_mediaChanged);
    connect(ui->videoListWidget, &QListWidget::itemDoubleClicked, this, &MainWindow::on_videoListWidget_itemDoubleClicked);
    connect(ui->videoListWidget, &QListWidget::itemClicked, this, &MainWindow::on_videoListWidget_itemClicked);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupVideoWidget()
{
    player->setVideoOutput(videoWidget);
    videoWidget->setGeometry(5, 5, ui->groupBox_video->width() - 10, ui->groupBox_video->height() - 10);
    videoWidget->setParent(ui->groupBox_video);
    videoWidget->show();
}

void MainWindow::on_addButton_clicked()
{
    QStringList fileNames = QFileDialog::getOpenFileNames(this, tr("Open Video Files"), "", tr("Video Files (*.mp4 *.avi)"));

    if (!fileNames.isEmpty()) {
        for (const QString &fileName : fileNames) {
            playlist->addMedia(QUrl::fromLocalFile(fileName));
        }

        updatePlaylistView();
        updateVideoListOrder();

        if (playlist->mediaCount() == fileNames.count()) {
            currentIndex = 0;
        } else {
            currentIndex = playlist->mediaCount() - fileNames.count();
        }
        playlist->setCurrentIndex(currentIndex);
    }
}

void MainWindow::on_addButton_clicked()
{
    QStringList fileNames = QFileDialog::getOpenFileNames(this, tr("Open Video Files"), "", tr("Video Files (*.mp4 *.avi)"));

    if (!fileNames.isEmpty()) {
        for (const QString &fileName : fileNames) {
            playlist->addMedia(QUrl::fromLocalFile(fileName));
        }

        updatePlaylistView();
        updateVideoListOrder();

        // Если список был пуст, установите текущий индекс на первый добавленный файл
        // Также исправим логику условия, чтобы она корректно учитывала уже существующие файлы
        if (playlist->mediaCount() == fileNames.count()) {
            currentIndex = 0;
        } else {
            currentIndex = playlist->mediaCount() - fileNames.count();
        }
        playlist->setCurrentIndex(currentIndex);
    }
}
