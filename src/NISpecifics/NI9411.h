#ifndef NI9411_H
#define NI9411_H

#include "NIDeviceModule.h"
#include <vector>
#include <string>

class NI9411 : public NIDeviceModule {
private:


public:
    explicit NI9411(QObject *parent = nullptr);
    void initModule()                       override;  // Initialize the module with default values
    std::vector<std::string> getChanNames() const override;  // Get the names of the channels
    void loadConfig()  override ;
    void saveConfig()  override ;  // Save the module configuration to a file
};

#endif // NI9411_H
