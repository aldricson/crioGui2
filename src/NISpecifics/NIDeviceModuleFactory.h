/*#ifndef NIDEVICEMODULEFACTORY_H
#define NIDEVICEMODULEFACTORY_H

#include "NIDeviceModule.h"
#include <string>
#include <memory>
#include "cPosixFileHelper.h"

class NIDeviceModuleFactory {
public:
    static NIDeviceModule* createModule(const std::string& productName);

private:
        template <typename T>
    static T* createAndConfigureModule();
};

#endif // NIDEVICEMODULEFACTORY_H*/


#ifndef NIDEVICEMODULEFACTORY_H
#define NIDEVICEMODULEFACTORY_H

#include <QObject>
#include "NIDeviceModule.h"
#include <string>
#include <memory>

class NIDeviceModuleFactory : public QObject {
    Q_OBJECT

public:
    explicit NIDeviceModuleFactory(QObject *parent = nullptr);
    NIDeviceModule* createModule(const std::string& productName);

private:
    template <typename T>
    static T* createAndConfigureModule(QObject* parent);
};

#endif // NIDEVICEMODULEFACTORY_H

