#ifndef QMODBUSSETUPVIEWER_H
#define QMODBUSSETUPVIEWER_H

#include <QWidget>
#include <QSettings>
#include <QGroupBox>
#include <QLineEdit>
#include <QPushButton>
#include <QIntValidator>
#include <QRegularExpressionValidator>
#include <QRegularExpression>
#include "QBetterSwitchButton.h"
#include "stringUtils.h"
#include "QIpAddressEditor.h"

class QLineEdit;
class QSpinBox;
class QPushButton;
class QModbusSetupViewer : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QString fileName READ fileName WRITE setFileName NOTIFY fileNameChanged)


public:
    explicit QModbusSetupViewer(QWidget *parent = nullptr);

    const QString &fileName() const;
    void setFileName(const QString &newFileName);

public slots:
    void loadFromFile();
    void saveToFile();
    void uploadToServer();

signals:
    void fileNameChanged();

private:
    QSettings *settings                    = nullptr;

    QGroupBox           *m_containerGroupBox          = nullptr; // GroupBox for Modbus Setup
    QGroupBox           *m_compatibilityLayerGroupBox = nullptr;
    QGroupBox           *m_modbusCapacitiesGroupBox   = nullptr;
    QGroupBox           *m_networkGroupBox            = nullptr;
    QLineEdit           *coilsLineEdit                = nullptr;
    QLineEdit           *discreteInputsLineEdit       = nullptr;
    QLineEdit           *holdingRegistersLineEdit     = nullptr;
    QLineEdit           *inputRegistersLineEdit       = nullptr;
    QLineEdit           *listeningPortLineEdit        = nullptr;
    QIpAddressEditor    *listeningInterfaceIpEdit     = nullptr;
    QBetterSwitchButton *compatibilityLayerSwitch     = nullptr;
    QLineEdit           *nbAnalogsInLineEdit          = nullptr;
    QLineEdit           *nbAnalogsOutLineEdit         = nullptr;
    QLineEdit           *nbCountersLineEdit           = nullptr;
    QPushButton         *reloadButton                 = nullptr;
    QPushButton         *saveButton                   = nullptr;
    QPushButton         *uploadButton                 = nullptr;

    QString m_fileName;

    void blockAllSignals(const bool &blocked);
    void setAllValidators();
};

#endif // QMODBUSSETUPVIEWER_H
