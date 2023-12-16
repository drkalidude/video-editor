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