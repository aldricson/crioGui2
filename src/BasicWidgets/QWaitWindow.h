#ifndef QWAITWINDOW_H
#define QWAITWINDOW_H

#include <QDialog>
#include <QProgressBar>
#include <QTimer>
#include <QLabel>

class QWaitWindow : public QDialog {
    Q_OBJECT

public:
    explicit QWaitWindow(int waitTime, QWidget *parent = nullptr);

private slots:
    void updateProgress();

protected:
   void showEvent( QShowEvent* event ) override;

private:
    QProgressBar *m_progressBar;
    QTimer *m_timer;
    int m_waitTime;
    QLabel *m_textLabel;
};

#endif // QWAITWINDOW_H
