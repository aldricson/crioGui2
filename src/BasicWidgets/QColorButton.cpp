#include "QColorButton.h"

/**
 * @brief Constructs a new QColorButton with the given parent.
 * Connects the clicked() signal to a slot that opens a color dialog and sets the new color as the button's background color.
 * @param parent The parent widget.
 */
QColorButton::QColorButton(QWidget *parent) : QPushButton(parent),
                                                  m_color(Qt::white)
{
    connect(this, &QPushButton::clicked, [this]()
    {
        QColor color = QColorDialog::getColor(m_color,
                                              nullptr,
                                              "Select Color");
        if (color.isValid())
        {
            setColor(color);
        }
    });
    setColor(m_color);
}

/**
 * @brief Returns the current color of the button.
 * @return The current color.
 */
QColor QColorButton::color() const
{
    return m_color;
}

/**
 * @brief Sets the current color of the button.
 * @param color The new color.
 */
void QColorButton::setColor(const QColor &color)
{
    if (m_color == color)
        return;
    m_color = color;
    setStyleSheet(QString("background-color: %1").arg(color.name()));
    emit colorChanged(m_color);
}
