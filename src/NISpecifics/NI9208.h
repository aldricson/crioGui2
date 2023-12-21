#ifndef NI9208_H
#define NI9208_H

#include "NIDeviceModule.h"
#include <vector>
#include <string>


class NI9208 : public NIDeviceModule {
public:
    explicit NI9208(QObject *parent = nullptr);
    void initModule() override;
    std::vector<std::string> getChanNames() const override;
    void loadConfig() override;
    void saveConfig() override;
};

#endif // NI9208_H
