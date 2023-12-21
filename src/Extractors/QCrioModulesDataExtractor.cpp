#include "QCrioModulesDataExtractor.h"
#include <QSettings>
#include <QFileInfo>
#include <map>


QCrioModulesDataExtractor::QCrioModulesDataExtractor(QString iniModulesLocalPath, QObject *parent) : QObject(parent)
{
    // Constructor implementation (if needed)
    m_iniModulesLocalPath = iniModulesLocalPath;
    m_moduleFactory = new NIDeviceModuleFactory(this);
}

void QCrioModulesDataExtractor::createDevicesFromConfig()
{
    QDir dir(m_iniModulesLocalPath);
    QStringList filters;
    // Set the file name filter pattern: start with "NI", followed by anything, then an underscore and an unsigned integer, and ends with ".ini"
    filters << "NI*_[0-9]*.ini";
    dir.setNameFilters(filters);
    // List all files in the directory matching the pattern
    QFileInfoList fileList = dir.entryInfoList(QDir::Files | QDir::NoDotAndDotDot);

    foreach (const QFileInfo& fileInfo, fileList)
    {
        QString fileName = fileInfo.fileName();

        // Find the position of the underscore in the file name
        int underscoreIndex = fileName.indexOf('_');
        if (underscoreIndex != -1) {
            // Extract the part of the file name before the underscore
            std::string shortedModuleName = fileName.left(underscoreIndex).toStdString();
            auto module = m_moduleFactory->createModule(shortedModuleName);
            if (module)
            {
                module->loadFromFile(m_iniModulesLocalPath.toStdString()+fileName.toStdString());
                m_fullModuleList.push_back(module);
                if (module->getModuleType()==ModuleType::isAnalogicInputCurrent)
                    m_analogCurrentModuleList.push_back(module);
                else if (module->getModuleType()==ModuleType::isAnalogicInputVoltage)
                    m_analogVoltageModuleList.push_back(module);
                else if (module->getModuleType()==ModuleType::isCounter)
                    m_analogCountersModuleList.push_back(module);
                else if (module->getModuleType()==ModuleType::isCoder)
                    m_analogCodersModuleList.push_back(module);
            }
        }
    }
}

void QCrioModulesDataExtractor::extractCurrentModules(QComboBox *modulesComboBox)
{
    if (!modulesComboBox) return; // Safety check

    modulesComboBox->blockSignals(true);
    modulesComboBox->clear();
    std::map<QString, NIDeviceModule*> aliasModuleMap; // Map alias to module
    for (int i = 0; i < m_analogCurrentModuleList.size(); ++i)
    {
        auto module = m_analogCurrentModuleList[i];
        if (module)
        {
            QString alias = QString::fromStdString(module->getAlias());
            if (!alias.isEmpty())
            {
                aliasModuleMap[alias] = module; // Associate alias with module
            }
        }
    }
    for (const auto& pair : aliasModuleMap)
    {
        // Store the module pointer as user data
        modulesComboBox->addItem(pair.first, QVariant::fromValue(pair.second));
    }

    modulesComboBox->blockSignals(false);
}


void QCrioModulesDataExtractor::extractVoltageModules(QComboBox *modulesComboBox)
{
    if (!modulesComboBox) return; // Safety check

    modulesComboBox->blockSignals(true);
    modulesComboBox->clear();

    std::map<QString, NIDeviceModule*> aliasModuleMap; // Map alias to module

    for (int i = 0; i < m_analogVoltageModuleList.size(); ++i)
    {
        auto module = m_analogVoltageModuleList[i];
        if (module)
        {
            QString alias = QString::fromStdString(module->getAlias());
            if (!alias.isEmpty())
            {
                aliasModuleMap[alias] = module; // Associate alias with module
            }
        }
    }

    for (const auto& pair : aliasModuleMap)
    {
        // Store the module pointer as user data
        modulesComboBox->addItem(pair.first, QVariant::fromValue(pair.second));
    }

    modulesComboBox->blockSignals(false);
}

void QCrioModulesDataExtractor::extractAllModules(QComboBox *modulesComboBox)
{
    if (!modulesComboBox) return; // Safety check

    modulesComboBox->blockSignals(true);
    modulesComboBox->clear();

    std::map<QString, NIDeviceModule*> aliasModuleMap; // Map alias to module

    for (int i = 0; i < m_fullModuleList.size(); ++i)
    {
        auto module = m_fullModuleList[i];
        if (module)
        {
            QString alias = QString::fromStdString(module->getAlias());
            if (!alias.isEmpty())
            {
                aliasModuleMap[alias] = module; // Associate alias with module
            }
        }
    }

    for (const auto& pair : aliasModuleMap)
    {
        // Store the module pointer as user data
        modulesComboBox->addItem(pair.first, QVariant::fromValue(pair.second));
    }

    modulesComboBox->blockSignals(false);
}

void QCrioModulesDataExtractor::extractModulesOfType(QComboBox *modulesComboBox, ModuleType type)
{

}



const std::vector<NIDeviceModule *> &QCrioModulesDataExtractor::fullModuleList() const
{
    return m_fullModuleList;
}

void QCrioModulesDataExtractor::setFullModuleList(const std::vector<NIDeviceModule *> &newFullModuleList)
{
    if (m_fullModuleList == newFullModuleList)
        return;
    m_fullModuleList = newFullModuleList;
    emit fullModuleListChanged();
}

const std::vector<NIDeviceModule *> &QCrioModulesDataExtractor::analogCurrentModuleList() const
{
    return m_analogCurrentModuleList;
}

void QCrioModulesDataExtractor::setAnalogCurrentModuleList(const std::vector<NIDeviceModule *> &newAnalogCurrentModuleList)
{
    if (m_analogCurrentModuleList == newAnalogCurrentModuleList)
        return;
    m_analogCurrentModuleList = newAnalogCurrentModuleList;
    emit analogCurrentModuleListChanged();
}

const std::vector<NIDeviceModule *> &QCrioModulesDataExtractor::analogVoltageModuleList() const
{
    return m_analogVoltageModuleList;
}

void QCrioModulesDataExtractor::setAnalogVoltageModuleList(const std::vector<NIDeviceModule *> &newAnalogVoltageModuleList)
{
    if (m_analogVoltageModuleList == newAnalogVoltageModuleList)
        return;
    m_analogVoltageModuleList = newAnalogVoltageModuleList;
    emit analogVoltageModuleListChanged();
}

const std::vector<NIDeviceModule *> &QCrioModulesDataExtractor::analogCountersModuleList() const
{
    return m_analogCountersModuleList;
}

void QCrioModulesDataExtractor::setAnalogCountersModuleList(const std::vector<NIDeviceModule *> &newAnalogCountersModuleList)
{
    if (m_analogCountersModuleList == newAnalogCountersModuleList)
        return;
    m_analogCountersModuleList = newAnalogCountersModuleList;
    emit analogCountersModuleListChanged();
}

const std::vector<NIDeviceModule *> &QCrioModulesDataExtractor::analogCodersModuleList() const
{
    return m_analogCodersModuleList;
}

void QCrioModulesDataExtractor::setAnalogCodersModuleList(const std::vector<NIDeviceModule *> &newAnalogCodersModuleList)
{
    if (m_analogCodersModuleList == newAnalogCodersModuleList)
        return;
    m_analogCodersModuleList = newAnalogCodersModuleList;
    emit analogCodersModuleListChanged();
}


