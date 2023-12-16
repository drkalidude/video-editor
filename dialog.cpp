// dialog.cpp
#include "dialog.h"
#include "ui_dialog.h"
#include <QTime>
#include <QFileDialog>
#include <QStyle>

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog),
    player(new QMediaPlayer(this)),
    video(new QVideoWidget(this)),
    isPause(false)
{
    ui->setupUi(this);

    connect(player, &QMediaPlayer::durationChanged, this, &Dialog::durationChanged);
    connect(player, &QMediaPlayer::positionChanged, this, &Dialog::changed);

    ui->horizontalSlider->setRange(0, player->duration() / 1000);
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::openVideoPlayerWindow(const QString &videoFilePath)
{
    video->setGeometry(5, 5, ui->groupBox_video->width() - 10, ui->groupBox_video->height() - 10);
    player->setVideoOutput(video);
    player->setMedia(QUrl::fromLocalFile(videoFilePath));
    video->setParent(ui->groupBox_video);
    video->show();
    player->play();
}

void Dialog::on_pushButton_close_clicked()
{
    QString label1Text = ui->timeEdit_start->text();
    QString label2Text = ui->timeEdit_end->text();

    emit dialogClosed(label1Text, label2Text);
    close();
}

void Dialog::on_pushButton_play_stop_clicked()
{
    if (isPause) {
        player->play();
        ui->pushButton_play_stop->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
    } else {
        player->pause();
        ui->pushButton_play_stop->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    }

    isPause = !isPause;
}

void Dialog::updateDuration(qint64 duration) {
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

void Dialog::durationChanged(qint64 duration) {
    mDuration = duration / 1000;
    ui->horizontalSlider->setMaximum(mDuration);
}

void Dialog::changed(qint64 duration) {
    if (!ui->horizontalSlider->isSliderDown()) ui->horizontalSlider->setValue(duration / 1000);
    updateDuration(duration / 1000);
}