#include "QIpAddressEditor.h"

QIpAddressEditor::QIpAddressEditor(QWidget *parent) : QWidget(parent) {
    byte1 = new QLineEdit(this);
    byte2 = new QLineEdit(this);
    byte3 = new QLineEdit(this);
    byte4 = new QLineEdit(this);

    byte1->installEventFilter(this);
    byte2->installEventFilter(this);
    byte3->installEventFilter(this);
    byte4->installEventFilter(this);

    validator = new QIntValidator(0, 255, this);
    byte1->setValidator(validator);
    byte2->setValidator(validator);
    byte3->setValidator(validator);
    byte4->setValidator(validator);

    byte1->setMaximumWidth(60);
    byte2->setMaximumWidth(60);
    byte3->setMaximumWidth(60);
    byte4->setMaximumWidth(60);

    layout = new QHBoxLayout(this);
    layout->addWidget(byte1);
    layout->addWidget(byte2);
    layout->addWidget(byte3);
    layout->addWidget(byte4);
    setLayout(layout);
}

QString QIpAddressEditor::ipAddress() const
{
        return QString("%1.%2.%3.%4").arg(byte1->text(),byte2->text(),byte3->text(),byte4->text());
}

bool QIpAddressEditor::eventFilter(QObject *obj, QEvent *event) {
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        QLineEdit *edit = qobject_cast<QLineEdit *>(obj);
        if (edit && keyEvent) {
            switch (keyEvent->key()) {
                case Qt::Key_Period:
                case Qt::Key_Comma:
                case Qt::Key_Right:
                    moveNext(edit);
                    return true;
                case Qt::Key_Left:
                    movePrev(edit);
                    return true;
                default:
                    break;
            }
        }
    }
    // Call base class method for default processing
    return QWidget::eventFilter(obj, event);
}


void QIpAddressEditor::moveNext(QLineEdit *currentEdit) {
    if (currentEdit == byte1) byte2->setFocus();
    else if (currentEdit == byte2) byte3->setFocus();
    else if (currentEdit == byte3) byte4->setFocus();
}

void QIpAddressEditor::movePrev(QLineEdit *currentEdit) {
    if (currentEdit == byte4) byte3->setFocus();
    else if (currentEdit == byte3) byte2->setFocus();
    else if (currentEdit == byte2) byte1->setFocus();
}

void QIpAddressEditor::setIp(const QString &ipAddress)
{
    QStringList ipParts = ipAddress.split('.');
    if (ipParts.size() != 4)
    {
        // Handle the error - either log it, show a message to the user, or throw an exception.
        return;
    }

    bool ok;
    int byteValue;

    // Setting each byte after validating
    for (int i = 0; i < 4; ++i)
    {
        byteValue = ipParts[i].toInt(&ok);
        if (!ok || byteValue < 0 || byteValue > 255)
        {
            // Handle the error for invalid byte value
            return;
        }

        switch (i)
        {
            case 0: byte1->setText(ipParts[i]); break;
            case 1: byte2->setText(ipParts[i]); break;
            case 2: byte3->setText(ipParts[i]); break;
            case 3: byte4->setText(ipParts[i]); break;
            default: break; // This should never happen
        }
    }

    // Emit signal if needed
    emit ipAddressChanged(ipAddress);
}

