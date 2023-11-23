#ifndef QCOLORBUTTON_H
#define QCOLORBUTTON_H

#include <QPushButton>
#include <QColor>
#include <QColorDialog>

/**
 * @brief The QColorButton class is a QPushButton subclass that
 * allows the user to select a color and sets the background color of the button to the selected color.
 */
class QColorButton : public QPushButton
{
    Q_OBJECT
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)

public:
    /**
     * @brief Constructs a new QColorButton with the given parent.
     * @param parent The parent widget.
     */
    explicit QColorButton(QWidget *parent = nullptr);

    /**
     * @brief Returns the current color of the button.
     * @return The current color.
     */
    QColor color() const;

    /**
     * @brief Sets the current color of the button.
     * @param color The new color.
     */
    void setColor(const QColor &color);

signals:
    /**
     * @brief Emitted when the color of the button changes.
     * @param color The new color.
     */
    void colorChanged(const QColor &color);

private:
    QColor m_color;
};

#endif // QCOLORBUTTON_H
