#ifndef QRECTFWIDGET_H
#define QRECTFWIDGET_H

#include <QObject>
#include <QWidget>
#include <QLineEdit>
#include <QLabel>
#include <QIntValidator>
#include <QGridLayout>

/**
 * @brief The QRectFWidget class is a QWidget subclass that allows the user to input the values 
 * of a QRectF (left, top, width, and height) and provides methods to get and set the QRectF.
 */
class QRectFWidget : public QWidget
{
    Q_OBJECT
    /**
     * @property rectF
     * @brief The QRectF property that holds the values of the left, top, width, and height
     */
    Q_PROPERTY(QRectF rectF READ rectF WRITE setRectF NOTIFY rectFChanged)

public:
    /**
     * @brief Constructs a new QRectFWidget with the given parent.
     * @param parent The parent widget.
     */
    QRectFWidget(QWidget* parent = nullptr);

    /**
     * @brief Returns the current QRectF of the widget.
     * @return The current QRectF.
     */
    QRectF rectF() const;

    /**
     * @brief Sets the current QRect of the widget.
     * @param rect The new QRectF.
     */
    void setRectF(const QRectF &rectF);

signals:
    /**
     * @brief Emitted when the QRectF of the widget changes.
     * @param rectF The new QRectF.
     */
    void rectFChanged(const QRectF &rectF);

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

#endif // QRectFWidget_H
