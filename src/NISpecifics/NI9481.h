#ifndef NI9481_H
#define NI9481_H

#include "NIDeviceModule.h"
#include <vector>
#include <string>

class NI9481 : public NIDeviceModule {
private:


public:
    explicit NI9481(QObject *parent = nullptr);
    void initModule()                       override;  // Initialize the module with default values
    std::vector<std::string> getChanNames() const override;  // Get the names of the channels
    void loadConfig()  override ;
    void saveConfig()  override ;  // Save the module configuration to a file
};

#endif // NI9481_H
