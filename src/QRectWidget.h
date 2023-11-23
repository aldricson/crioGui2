#ifndef QRECTWIDGET_H
#define QRECTWIDGET_H

#include <QObject>
#include <QWidget>
#include <QLineEdit>
#include <QLabel>
#include <QIntValidator>
#include <QGridLayout>

/**
 * @brief The QRectWidget class is a QWidget subclass that allows the user to input the values of a QRect (left, top, width, and height) and provides methods to get and set the QRect.
 */
class QRectWidget : public QWidget
{
    Q_OBJECT
    /**
     * @property rect
     * @brief The QRect property that holds the values of the left, top, width, and height
     */
    Q_PROPERTY(QRect rect READ rect WRITE setRect NOTIFY rectChanged)

public:
    /**
     * @brief Constructs a new QRectWidget with the given parent.
     * @param parent The parent widget.
     */
    QRectWidget(QWidget* parent = nullptr);

    /**
     * @brief Returns the current QRect of the widget.
     * @return The current QRect.
     */
    QRect rect() const;

    /**
     * @brief Sets the current QRect of the widget.
     * @param rect The new QRect.
     */
    void setRect(const QRect &rect);

signals:
    /**
     * @brief Emitted when the QRect of the widget changes.
     * @param rect The new QRect.
     */
    void rectChanged(const QRect &rect);

private:
    QLabel    *leftLabel   = nullptr;
    QLabel    *topLabel    = nullptr;
    QLabel    *widthLabel  = nullptr;
    QLabel    *heightLabel = nullptr;
    QLineEdit *leftEdit    = nullptr;
    QLineEdit *topEdit     = nullptr;
    QLineEdit *widthEdit   = nullptr;
    QLineEdit *heightEdit  = nullptr;
};

#endif // QRECTWIDGET_H
