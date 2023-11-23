#ifndef QIpAddressEditor_H
#define QIpAddressEditor_H

#include <QWidget>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QIntValidator>
#include <QKeyEvent>

class QIpAddressEditor : public QWidget {
    Q_OBJECT

public:
    explicit QIpAddressEditor(QWidget *parent = nullptr);
    QString ipAddress() const;

private:
    QLineEdit *byte1;
    QLineEdit *byte2;
    QLineEdit *byte3;
    QLineEdit *byte4;
    QIntValidator *validator;
    QHBoxLayout *layout;

    bool eventFilter(QObject *obj, QEvent *event) override;
    void moveNext(QLineEdit *currentEdit);
    void movePrev(QLineEdit *currentEdit);

signals:
    void ipAddressChanged(const QString &ipAddress);
};

#endif // QIpAddressEditor_H
