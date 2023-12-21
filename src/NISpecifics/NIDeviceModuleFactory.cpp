#include "NIDeviceModuleFactory.h"
#include "NI9208.h"
#include "NI9239.h"
#include "NI9423.h"
#include "NI9411.h"
#include "NI9481.h"
// Include other module headers here


NIDeviceModuleFactory::NIDeviceModuleFactory(QObject *parent) : QObject(parent)
{
}


template <typename T>
T* NIDeviceModuleFactory::createAndConfigureModule(QObject* parent) {
    auto result = new T(parent); // Pass the parent to the constructor
    return result;
}


NIDeviceModule* NIDeviceModuleFactory::createModule(const std::string& productName) {
    if (productName == "NI9208") {
        auto module = createAndConfigureModule<NI9208>(this);
        module -> initModule();
        return module;
    }
    else
        if (productName == "NI9239") {
        auto module = createAndConfigureModule<NI9239>(this);
        module->initModule();
        return module;
    }
    else
        if (productName == "NI9423") {
        auto module = createAndConfigureModule<NI9423>(this);
        module->initModule();
        return module;
    }
    else
        if (productName == "NI9411") {
        auto module = createAndConfigureModule<NI9411>(this);
        module->initModule();
        return module;
    }
        else
        if (productName == "NI9481") {
        auto module = createAndConfigureModule<NI9481>(this);
        module->initModule();
        return module;
    }
    // Add other product names and their corresponding classes here


    return nullptr; // or throw an exception if an unknown product name is passed
}
