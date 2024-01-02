#include "QMultiLineTextVisualizer.h"
#include <QScrollBar>
#include <QFile>

QMultiLineTextVisualizer::QMultiLineTextVisualizer(QWidget *parent)
    : QTextBrowser(parent) {
    //setStyleSheet("background-color: black;");
    setReadOnly(true);
    setAutoFillBackground(true);

    QFont terminalFont("Consolas", 10); // You can adjust the font name and size as needed
    if (!terminalFont.exactMatch()) {
        terminalFont = QFont("Courier New", 10); // Fallback font if Consolas is not available
    }
    setFont(terminalFont);
    // Set the vertical scrollbar to always be visible
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    loadStyleStringFromResource();
}

void QMultiLineTextVisualizer::addLastCommand(const QString &lastCommand) {
    QString timestamp = QDateTime::currentDateTime().toString("[yyyy-MM-dd HH:mm:ss]<br/>");
    QString processedCommand = QString(lastCommand).replace("\n", "<br/>");
    appendTextWithColor(timestamp + processedCommand, Qt::green, true);
}

void QMultiLineTextVisualizer::addLastOutput(const QString &lastOutput) {
    QString timestamp = QDateTime::currentDateTime().toString("[yyyy-MM-dd HH:mm:ss]<br/>");
    QString processedOutput = QString(lastOutput).replace("\n", "<br/>");
    appendTextWithColor(timestamp + processedOutput, QColor(0x449ef5), true);
}

void QMultiLineTextVisualizer::addLastError(const QString &lastError) {
    QString timestamp = QDateTime::currentDateTime().toString("[yyyy-MM-dd HH:mm:ss]<br/>");
    QString processedError = QString(lastError).replace("\n", "<br/>");
    appendTextWithColor(timestamp + processedError, Qt::red, true);
}


void QMultiLineTextVisualizer::clearText() {
    clear();
}

void QMultiLineTextVisualizer::loadStyleStringFromResource()
{
        QFile file(":/styles/darkScrollBar.scss");
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            return;
        }

        QTextStream in(&file);
        QString content = in.readAll();
        file.close();

        this->verticalScrollBar()->setStyleSheet(content);
}



void QMultiLineTextVisualizer::appendTextWithColor(const QString &text, const QColor &color, const bool skipLine)
{
    QString coloredText = QString("<span style=\"color:%1;\">%2</span>").arg(color.name(), text);
    append(coloredText);
    if(skipLine) append("");
}

void QMultiLineTextVisualizer::saveRawText(const QString &fileName)
{
    // Get the plain text from the QTextBrowser
    QString plainText = this->toPlainText();
    // Now you can save this plain text to a file or use it as needed
    QFile file(fileName);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream out(&file);
        out << plainText;
        file.close();
    }
}
