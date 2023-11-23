#ifndef QMultiLineTextVisualizer_H
#define QMultiLineTextVisualizer_H

#include <QTextBrowser>
#include <QString>
#include <QDateTime>
#include <QFont>

class QMultiLineTextVisualizer : public QTextBrowser {
    Q_OBJECT

public:
    explicit QMultiLineTextVisualizer(QWidget *parent = nullptr);
    void addLastCommand(const QString &lastCommand);
    void addLastOutput(const QString &lastOutput);
    void addLastError(const QString &lastError);

public slots:
    void clearText();

private:
    void appendTextWithColor(const QString &text, const QColor &color);
};

#endif // QMultiLineTextVisualizer_H
