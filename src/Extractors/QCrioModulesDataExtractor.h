#ifndef QCrioModulesDataExtractor_H
#define QCrioModulesDataExtractor_H

#include <QObject>
#include <QStringList>
#include <QComboBox>
#include <QDir>
#include "./NISpecifics/NIDeviceModule.h"
#include "./NISpecifics/NIDeviceModuleFactory.h"
#include "Globals/globalEnumSpace.h"

using namespace globalEnumSpace;

class QCrioModulesDataExtractor : public QObject
{
    Q_OBJECT
    Q_PROPERTY(std::vector<NIDeviceModule *> fullModuleList READ fullModuleList WRITE setFullModuleList NOTIFY fullModuleListChanged)
    Q_PROPERTY(std::vector<NIDeviceModule *> analogCurrentModuleList READ analogCurrentModuleList WRITE setAnalogCurrentModuleList NOTIFY analogCurrentModuleListChanged)
    Q_PROPERTY(std::vector<NIDeviceModule *> analogVoltageModuleList READ analogVoltageModuleList WRITE setAnalogVoltageModuleList NOTIFY analogVoltageModuleListChanged)
    Q_PROPERTY(std::vector<NIDeviceModule *> analogCountersModuleList READ analogCountersModuleList WRITE setAnalogCountersModuleList NOTIFY analogCountersModuleListChanged)
    Q_PROPERTY(std::vector<NIDeviceModule *> analogCodersModuleList READ analogCodersModuleList WRITE setAnalogCodersModuleList NOTIFY analogCodersModuleListChanged)
public:
    explicit QCrioModulesDataExtractor(QString iniModulesLocalPath,QObject *parent = nullptr);

    void createDevicesFromConfig();

    void extractCurrentModules(QComboBox *modulesComboBox);
    void extractVoltageModules(QComboBox *modulesComboBox);
    void extractAllModules    (QComboBox *modulesComboBox);
    void extractModulesOfType (QComboBox *modulesComboBox, globalEnumSpace::ModuleType type);



    const std::vector<NIDeviceModule *> &fullModuleList() const;
    void setFullModuleList(const std::vector<NIDeviceModule *> &newFullModuleList);

    const std::vector<NIDeviceModule *> &analogCurrentModuleList() const;
    void setAnalogCurrentModuleList(const std::vector<NIDeviceModule *> &newAnalogCurrentModuleList);

    const std::vector<NIDeviceModule *> &analogVoltageModuleList() const;
    void setAnalogVoltageModuleList(const std::vector<NIDeviceModule *> &newAnalogVoltageModuleList);

    const std::vector<NIDeviceModule *> &analogCountersModuleList() const;
    void setAnalogCountersModuleList(const std::vector<NIDeviceModule *> &newAnalogCountersModuleList);

    const std::vector<NIDeviceModule *> &analogCodersModuleList() const;
    void setAnalogCodersModuleList(const std::vector<NIDeviceModule *> &newAnalogCodersModuleList);


signals:
    void fullModuleListChanged          ();
    void analogCurrentModuleListChanged ();
    void analogVoltageModuleListChanged ();
    void analogCountersModuleListChanged();
    void analogCodersModuleListChanged  ();

private:
     QString m_iniModulesLocalPath;

     std::vector<NIDeviceModule*> m_fullModuleList           ;
     std::vector<NIDeviceModule*> m_analogCurrentModuleList  ;
     std::vector<NIDeviceModule*> m_analogVoltageModuleList  ;
     std::vector<NIDeviceModule*> m_analogCountersModuleList ;
     std::vector<NIDeviceModule*> m_analogCodersModuleList   ;

     NIDeviceModuleFactory *m_moduleFactory = nullptr;



};

#endif // QCrioModulesDataExtractor_H
