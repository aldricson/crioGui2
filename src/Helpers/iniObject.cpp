#include "iniObject.h"
#include <QVariant>
#include <QString>
#include <QStringList>

IniObject::IniObject(QObject *parent) : QObject(parent) {
    // Constructor implementation
}

IniObject::~IniObject() {
    // Destructor implementation
}

int IniObject::readInteger(const std::string& section, const std::string& key, int defaultValue, const std::string& filename) {
    QSettings settings(QString::fromStdString(filename), QSettings::IniFormat);
    settings.beginGroup(QString::fromStdString(section));
    int value = settings.value(QString::fromStdString(key), defaultValue).toInt();
    settings.endGroup();
    return value;
}

bool IniObject::writeInteger(const std::string& section, const std::string& key, int value, const std::string& filename) {
    QSettings settings(QString::fromStdString(filename), QSettings::IniFormat);
    settings.beginGroup(QString::fromStdString(section));
    settings.setValue(QString::fromStdString(key), value);
    settings.endGroup();
    return true;
}

// Read and Write for Double
double IniObject::readDouble(const std::string& section, const std::string& key, double defaultValue, const std::string& filename) {
    QSettings settings(QString::fromStdString(filename), QSettings::IniFormat);
    settings.beginGroup(QString::fromStdString(section));
    double value = settings.value(QString::fromStdString(key), defaultValue).toDouble();
    settings.endGroup();
    return value;
}

bool IniObject::writeDouble(const std::string& section, const std::string& key, double value, const std::string& filename) {
    QSettings settings(QString::fromStdString(filename), QSettings::IniFormat);
    settings.beginGroup(QString::fromStdString(section));
    settings.setValue(QString::fromStdString(key), value);
    settings.endGroup();
    return true;
}

// Read and Write for String
std::string IniObject::readString(const std::string& section, const std::string& key, const std::string& defaultValue, const std::string& filename) {
    QSettings settings(QString::fromStdString(filename), QSettings::IniFormat);
    settings.beginGroup(QString::fromStdString(section));
    QString value = settings.value(QString::fromStdString(key), QString::fromStdString(defaultValue)).toString();
    settings.endGroup();
    return value.toStdString();
}

bool IniObject::writeString(const std::string& section, const std::string& key, const std::string& value, const std::string& filename) {
    QSettings settings(QString::fromStdString(filename), QSettings::IniFormat);
    settings.beginGroup(QString::fromStdString(section));
    settings.setValue(QString::fromStdString(key), QString::fromStdString(value));
    settings.endGroup();
    return true;
}

// Read and Write for Unsigned Integer
unsigned int IniObject::readUnsignedInteger(const std::string& section, const std::string& key, unsigned int defaultValue, const std::string& filename) {
    QSettings settings(QString::fromStdString(filename), QSettings::IniFormat);
    settings.beginGroup(QString::fromStdString(section));
    unsigned int value = settings.value(QString::fromStdString(key), defaultValue).toUInt();
    settings.endGroup();
    return value;
}

bool IniObject::writeUnsignedInteger(const std::string& section, const std::string& key, unsigned int value, const std::string& filename) {
    QSettings settings(QString::fromStdString(filename), QSettings::IniFormat);
    settings.beginGroup(QString::fromStdString(section));
    settings.setValue(QString::fromStdString(key), value);
    settings.endGroup();
    return true;
}

// Read and Write for Boolean
bool IniObject::readBoolean(const std::string& section, const std::string& key, bool defaultValue, const std::string& filename) {
    QSettings settings(QString::fromStdString(filename), QSettings::IniFormat);
    settings.beginGroup(QString::fromStdString(section));
    bool value = settings.value(QString::fromStdString(key), defaultValue).toBool();
    settings.endGroup();
    return value;
}

bool IniObject::writeBoolean(const std::string& section, const std::string& key, bool value, const std::string& filename) {
    QSettings settings(QString::fromStdString(filename), QSettings::IniFormat);
    settings.beginGroup(QString::fromStdString(section));
    settings.setValue(QString::fromStdString(key), value);
    settings.endGroup();
    return true;
}




bool IniObject::readStringVector(const std::string& section, const std::string& keyPrefix, unsigned int nbElements, std::vector<std::string>& vectorToFill, const std::string& filename) {
    QSettings settings(QString::fromStdString(filename), QSettings::IniFormat);
    settings.beginGroup(QString::fromStdString(section));
    vectorToFill.clear();
    for (unsigned int i = 0; i < nbElements; ++i) {
        QString key = QString::fromStdString(keyPrefix) + QString::number(i);
        std::string value = settings.value(key, "").toString().toStdString();
        vectorToFill.push_back(value);
    }
    settings.endGroup();
    return true;
}

QSettings* IniObject::createSettingsObject(const std::string& filename) {
    return new QSettings(QString::fromStdString(filename), QSettings::IniFormat, this);
}
