#include "QWaitWindow.h"
#include <QVBoxLayout>

QWaitWindow::QWaitWindow(int waitTime, QWidget *parent)
    : QDialog(parent, Qt::Window | Qt::FramelessWindowHint), m_waitTime(waitTime) {
    m_textLabel = new QLabel("Waiting for a full start", this);
    // Initialize the progress bar
    m_progressBar = new QProgressBar(this);
    m_progressBar->setRange(0, m_waitTime / 50);  // Assuming 50 ms interval
    // Initialize the timer
    m_timer = new QTimer(this);
    m_timer->setInterval(50);  // 50 ms interval
    connect(m_timer, &QTimer::timeout, this, &QWaitWindow::updateProgress);
    // Layout setup
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(m_textLabel,Qt::AlignHCenter);
    layout->addWidget(m_progressBar);
    // Set the QDialog to be modal
    setModal(true);

    // Hide the window when it's created
    hide();
}

void QWaitWindow::updateProgress() {
    int value = m_progressBar->value();
    if (value >= m_progressBar->maximum()) {
        m_timer->stop();
        accept();  // Close the dialog and unlock the application
    } else {
        m_progressBar->setValue(value + 1);
    }
}

void QWaitWindow::showEvent(QShowEvent *event)
{
    m_timer->start();
}
