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

void MainWindow::on_nextButton_clicked()
{
    currentIndex = (currentIndex + 1) % playlist->mediaCount();
    playlist->setCurrentIndex(currentIndex);
}

void MainWindow::on_previousButton_clicked()
{
    currentIndex = (currentIndex - 1 + playlist->mediaCount()) % playlist->mediaCount();
    playlist->setCurrentIndex(currentIndex);
}

void MainWindow::on_playPauseButton_clicked()
{
    if (isPause) {
        player->play();
        ui->startStopButton->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
    }
    else {
        player->pause();
        ui->startStopButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    }

    isPause = !isPause;
}

void MainWindow::on_startStopButton_clicked()
{
    on_playPauseButton_clicked();
}

void MainWindow::updatePlaylistView()
{
    ui->videoListWidget->clear();

    for (int i = 0; i < playlist->mediaCount(); ++i) {
        QMediaContent media = playlist->media(i);
        QUrl url = media.canonicalUrl();
        QString fileName = url.fileName();
        int orderNumber = getVideoOrderNumber(fileName);
        QString displayText = QString("%1. %2").arg(orderNumber).arg(fileName);
        ui->videoListWidget->addItem(displayText);
    }
}

int MainWindow::getVideoOrderNumber(const QString &fileName) const
{
    // Получить порядковый номер файла в плейлисте
    int orderNumber = 0;
    for (int i = 0; i < playlist->mediaCount(); ++i) {
        QMediaContent media = playlist->media(i);
        QUrl url = media.canonicalUrl();
        if (url.fileName() == fileName) {
            orderNumber = i;
            break;
        }
        
    }
    return orderNumber;
}

void MainWindow::updateVideoListOrder()
{
    // Обновление порядка элементов в виджете списка
    for (int i = 0; i < playlist->mediaCount(); ++i) {
        QListWidgetItem *item = ui->videoListWidget->item(i);
        if (item) {
            QMediaContent media = playlist->media(i);
            QUrl url = media.canonicalUrl();
            QString fileName = url.fileName();
            int orderNumber = getVideoOrderNumber(fileName);
            QString displayText = QString("%1. %2").arg(orderNumber).arg(fileName);
            item->setText(displayText);
        }
    }
}

void MainWindow::on_videoListWidget_itemDoubleClicked(QListWidgetItem *item)
{
    int clickedInex = ui->videoListWidget->row(item);
    if (clickedInex >= 0 && clickedInex < playlist->mediaCount()) {
        currentIndex = clickedInex;
        playlist->setCurrentIndex(currentIndex);
    }
}

void MainWindow::on_mediaChanged(const QMediaContent &media) {
    Q_UNUSED(media);
    updatePlaylistView();
    updateVideoListOrder();
}

void MainWindow::on_pushButton_sound_clicked()
{
    if(isSound == true) ui->pushButton_sound->setIcon(style()->standardIcon(QStyle::SP_MediaVolume));
    else ui->pushButton_sound->setIcon(style()->standardIcon(QStyle::SP_MediaVolumeMuted));

    isSound = !isSound;
    player->setMuted(isSound);

}

void MainWindow::on_verticalSlider_volume_valueChanged(int value)
{
    player->setVolume(value);
}

void MainWindow::on_pushButton_up_clicked()
{
    QListWidgetItem *selectedItem = ui->videoListWidget->currentItem();
    if (selectedItem) {
        int currentIndex = ui->videoListWidget->row(selectedItem);
        if (currentIndex > 0) {
            QMediaContent currentItem = playlist->media(currentIndex);

            playlist->removeMedia(currentIndex);

            playlist->insertMedia(currentIndex - 1, currentItem);

            updatePlaylistView();
            updateVideoListOrder();
            ui->videoListWidget->setCurrentItem(selectedItem);

        } else {}
    } else {}
}

void MainWindow::on_pushButton_down_clicked()
{
    QListWidgetItem *selectedItem = ui->videoListWidget->currentItem();
    if (selectedItem) {
        int currentIndex = ui->videoListWidget->row(selectedItem);
        if (currentIndex < ui->videoListWidget->count() - 1) {
            QMediaContent currentItem = playlist->media(currentIndex);

            playlist->removeMedia(currentIndex);

            playlist->insertMedia(currentIndex + 1, currentItem);

            updatePlaylistView();
            updateVideoListOrder();
            ui->videoListWidget->setCurrentItem(selectedItem);

            qDebug() << "Updated playlist order:";
            for (int i = 0; i < playlist->mediaCount(); ++i) {
                qDebug() << i << ": " << playlist->media(i).canonicalUrl().fileName();
            }
        } else {
            qDebug() << "Выбранный элемент уже находится внизу списка.";
        }
    } else {
        qDebug() << "Выберите видео для перемещения вниз.";
    }
}

void MainWindow::on_videoListWidget_itemClicked(QListWidgetItem *item)
{
    int clickedIndex = ui->videoListWidget->row(item);
        if (clickedIndex >= 0 && clickedIndex < playlist->mediaCount()) {
            currentIndex = clickedIndex;
            playlist->setCurrentIndex(currentIndex);
            player->play();
            ui->startStopButton->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
        }
}

void MainWindow::on_openVideoPlayerButton_clicked()
{
    QListWidgetItem *currentItem = ui->videoListWidget->currentItem();
    if (currentItem) {
        player->stop();
        ui->startStopButton->setIcon(style()->standardIcon(QStyle::SP_MediaPause));

        QString videoFilePath = playlist->media(currentIndex).canonicalUrl().toLocalFile();

        Dialog *dialog = new Dialog(this);
        dialog->openVideoPlayerWindow(videoFilePath);
        connect(dialog, &Dialog::dialogClosed, this, &MainWindow::handleDialogClosed);
        dialog->exec();
    }
}

void MainWindow::handleDialogClosed(const QString &startTime, const QString &endTime)
{
    QTime startTimeValue = QTime::fromString(startTime, "hh:mm:ss");
    QTime endTimeValue = QTime::fromString(endTime, "hh:mm:ss");

    qint64 startTimeMs = QTime(0, 0).msecsTo(startTimeValue);
    qint64 endTimeMs = QTime(0, 0).msecsTo(endTimeValue);


    if (currentIndex >= 0 && currentIndex < videoDataList.size()) {
        videoDataList[currentIndex].startTime = startTimeMs;
        videoDataList[currentIndex].endTime = endTimeMs;
    }

    qDebug() << "Start Time: " << startTimeMs << "ms";
    qDebug() << "End Time: " << endTimeMs << "ms";
}

void MainWindow::updateDuration(qint64 duration) {
    if (duration || mDuration) {
        QTime currentTime((duration / 3600) % 60, (duration / 60) % 60, duration % 60, (duration * 1000) % 1000);
        QTime totalTime((mDuration / 3600) % 60, (mDuration / 60) % 60, mDuration % 60, (mDuration * 1000) % 1000);
        QString format = "";
        if (mDuration > 3600) format = "hh:mm:ss";
        else format = "mm:ss";

        ui->label_time_now->setText(currentTime.toString(format));
        ui->label_time_total->setText(totalTime.toString(format));
    }
}

void MainWindow::onDialogClosed(const QString &label1Text, const QString &label2Text) {
    qDebug() << "Label 1 Text: " << label1Text;
    qDebug() << "Label 2 Text: " << label2Text;
}