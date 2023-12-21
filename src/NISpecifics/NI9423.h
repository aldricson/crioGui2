#ifndef NI9423_H
#define NI9423_H

#include "NIDeviceModule.h"
#include <vector>
#include <string>


class NI9423 : public NIDeviceModule {
public:
    explicit NI9423(QObject *parent = nullptr);
    void initModule() override;
    std::vector<std::string> getChanNames() const override;
    void loadConfig() override;
    void saveConfig() override;
};

#endif // NI9423_H
