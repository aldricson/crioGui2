#include "QMultiLineTextVisualizer.h"

QMultiLineTextVisualizer::QMultiLineTextVisualizer(QWidget *parent)
    : QTextBrowser(parent) {
    setStyleSheet("background-color: black;");
    setReadOnly(true);
    setAutoFillBackground(true);

    QFont terminalFont("Consolas", 10); // You can adjust the font name and size as needed
    if (!terminalFont.exactMatch()) {
        terminalFont = QFont("Courier New", 10); // Fallback font if Consolas is not available
    }
    setFont(terminalFont);
}

void QMultiLineTextVisualizer::addLastCommand(const QString &lastCommand) {
    QString timestamp = QDateTime::currentDateTime().toString("[yyyy-MM-dd HH:mm:ss]");
    appendTextWithColor(timestamp + " " + lastCommand, Qt::green);
}

void QMultiLineTextVisualizer::addLastOutput(const QString &lastOutput) {
    QString timestamp = QDateTime::currentDateTime().toString("[yyyy-MM-dd HH:mm:ss]");
    appendTextWithColor(timestamp + " " + lastOutput, QColor(0x449ef5));
}

void QMultiLineTextVisualizer::addLastError(const QString &lastError) {
    QString timestamp = QDateTime::currentDateTime().toString("[yyyy-MM-dd HH:mm:ss]");
    appendTextWithColor(timestamp + " " + lastError, Qt::red);
}

void QMultiLineTextVisualizer::clearText() {
    clear();
}

void QMultiLineTextVisualizer::appendTextWithColor(const QString &text, const QColor &color) {
    QString coloredText = QString("<span style=\"color:%1;\">%2</span>").arg(color.name(), text);
    append(coloredText);
}
