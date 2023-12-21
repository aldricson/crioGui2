#ifndef INIOBJECT_H
#define INIOBJECT_H

#include <QObject>
#include <QSettings>
#include <string>
#include <vector>

class IniObject : public QObject {
    Q_OBJECT

public:
    explicit IniObject(QObject *parent = nullptr);
    virtual ~IniObject();

    int readInteger(const std::string& section, const std::string& key, int defaultValue, const std::string& filename);
    bool writeInteger(const std::string& section, const std::string& key, int value, const std::string& filename);

    double readDouble(const std::string& section, const std::string& key, double defaultValue, const std::string& filename);
    bool writeDouble(const std::string& section, const std::string& key, double value, const std::string& filename);

    std::string readString(const std::string& section, const std::string& key, const std::string& defaultValue, const std::string& filename);
    bool writeString(const std::string& section, const std::string& key, const std::string& value, const std::string& filename);

    unsigned int readUnsignedInteger(const std::string& section, const std::string& key, unsigned int defaultValue, const std::string& filename);
    bool writeUnsignedInteger(const std::string& section, const std::string& key, unsigned int value, const std::string& filename);

    bool readBoolean(const std::string& section, const std::string& key, bool defaultValue, const std::string& filename);
    bool writeBoolean(const std::string& section, const std::string& key, bool value, const std::string& filename);

    bool readStringVector(const std::string& section, const std::string& keyPrefix, unsigned int nbElements, std::vector<std::string>& vectorToFill, const std::string& filename);

private:
    QSettings* createSettingsObject(const std::string& filename);
};

#endif // INIOBJECT_H
