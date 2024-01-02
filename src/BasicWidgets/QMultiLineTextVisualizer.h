#ifndef QMultiLineTextVisualizer_H
#define QMultiLineTextVisualizer_H

#include <QTextBrowser>
#include <QString>
#include <QDateTime>
#include <QFont>
#include <QStyleOption>
#include <QPainter>


class QFile;
class QScrollBar;

class QMultiLineTextVisualizer : public QTextBrowser {
    Q_OBJECT

public:
    explicit QMultiLineTextVisualizer(QWidget *parent = nullptr);
    void addLastCommand(const QString &lastCommand);
    void addLastOutput(const QString &lastOutput);
    void addLastError(const QString &lastError);
    void appendTextWithColor(const QString &text, const QColor &color, const bool skipLine);
    void saveRawText(const QString &fileName);

public slots:
    void clearText();

private:
    void loadStyleStringFromResource();

protected:



};

#endif // QMultiLineTextVisualizer_H
