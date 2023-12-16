#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QVideoWidget>
#include <QtMultimedia>
#include <QtMultimediaWidgets>
#include <QtCore>
#include <QtWidgets>
#include <QtGui>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = nullptr);
    ~Dialog();
    void openVideoPlayerWindow(const QString &videoFilePath);

signals:
    void dialogClosed(const QString &label1Text, const QString &label2Text);


private:
    Ui::Dialog *ui;
    QMediaPlayer *player;
    QVideoWidget *video;
    QString videoFilePath;
    qint64 mDuration;
    bool isPause;

    void updateDuration(qint64 duration);
    void setupVideoWidget();
    void cropVideo();

private slots:
    void on_pushButton_close_clicked();
    void on_pushButton_play_stop_clicked();
    void durationChanged(qint64 duration);
    void changed(qint64 duration);

};

#endif
