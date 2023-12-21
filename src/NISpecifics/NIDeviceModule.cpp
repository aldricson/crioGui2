#include "NIDeviceModule.h"

NIDeviceModule::NIDeviceModule(QObject *parent)  : QObject(parent)
{
    m_iniObject = new IniObject(this);
}

NIDeviceModule::~NIDeviceModule()
{

}



bool NIDeviceModule::loadChannels(std::string filename) 
{
   setNbChannel(m_iniObject->readUnsignedInteger("Channels","NumberOfChannels",m_nbChannel,filename));
   setChanMax  (m_iniObject->readDouble("Channels","max",m_analogChanMax,filename));
   setChanMax  (m_iniObject->readDouble("Channels","min",m_analogChanMin,filename));
   bool ok = m_iniObject->readStringVector("channels","channel",m_nbChannel,m_chanNames,filename);
   if (ok)
    {
        //std::cout<<"Channels loaded: ok"<<std::endl;
        return true;
    }
    else
    {
        //std::cout<<"Channels not loaded, defautl values initialized"<<std::endl;
        return false;
    }
}

bool NIDeviceModule::loadCounters(std::string filename) 
{
    setNbCounters(m_iniObject->readUnsignedInteger("Counters","NumberOfCounters",m_nbCounters,filename));
    bool ok = m_iniObject->readStringVector("Counters","Counter",m_nbCounters,m_counterNames,filename);
    if (ok)
    {
        //std::cout<<"Counters loaded: ok"<<std::endl;
    }
    else
    {
        //std::cout<<"Counters not loaded, defautl values initialized"<<std::endl;
    }
    setcounterCountingEdgeMode   (static_cast<moduleCounterEdgeConfig>(m_iniObject->readInteger("Counters","edgeCountingMode" ,m_counterCountingEdgeMode,filename)));
    setCounterCountDirectionMode (static_cast<moduleCounterMode>      (m_iniObject->readInteger("Counters","countingDirection",m_counterCountDirectionMode,filename)));
    setCounterMax                (m_iniObject->readUnsignedInteger("Counters","countingMax",4294967295,filename));
    setCounterMin                (m_iniObject->readUnsignedInteger("Counters","countingMin",0,filename));
    // Successfully loaded all counters info
    return true;
}

bool NIDeviceModule::loadModules(std::string filename)
{
     // Add checks for required keys     
    setModuleType           (static_cast<ModuleType>(m_iniObject->readInteger("Modules","type",m_moduleType,filename)));
    setModuleName           (m_iniObject->readString("Modules","moduleName",m_moduleName,filename));
    setAlias                (m_iniObject->readString("Modules","Alias",m_alias,filename));
    setModuleShuntLocation  (static_cast<moduleShuntLocation>(m_iniObject->readInteger("Modules","shuntLocation",m_shuntLocation,filename)));
    setModuleShuntValue     (m_iniObject->readDouble("Modules","shuntValue",m_shuntValue,filename));
    setModuleTerminalCfg    (static_cast<moduleTerminalConfig>(m_iniObject->readInteger("Modules","terminalConfig",m_moduleTerminalConfig,filename)));
    setModuleUnit           (static_cast<moduleUnit>          (m_iniObject->readInteger("Modules","moduleUnit",m_moduleUnit,filename)));
    return true;
}


void NIDeviceModule::saveChannels(std::string filename)
{
    //std::cout<<"entering save channels "<<m_moduleName<<std::endl;
    m_iniObject->writeUnsignedInteger("Channels", "NumberOfChannels", m_nbChannel,filename);
    m_iniObject->writeDouble("Channels", "max", m_analogChanMax,filename);
    m_iniObject->writeDouble("Channels", "min", m_analogChanMin,filename);

    // Save the channel names
    for (unsigned int i = 0; i < m_nbChannel; ++i)
    {
        std::string key = "Channel" + std::to_string(i);
        m_iniObject->writeString("Channels", key.c_str(), m_chanNames[i],filename);
    }
     //std::cout<<"channels saved"<<m_moduleName<<std::endl;
}

void NIDeviceModule::saveCounters(std::string filename)
{
   //std::cout<<"entering save counters "<<m_moduleName<<std::endl;
   m_iniObject->writeUnsignedInteger("Counters", "NumberOfCounters", m_nbCounters,filename);
   
   // Save the counter names
   for (unsigned int i = 0; i < m_nbCounters; ++i)
   {
       std::string key = "Counter" + std::to_string(i);
       m_iniObject->writeString("Counters", key.c_str(), m_counterNames[i],filename);
   }

   m_iniObject->writeInteger("Counters", "edgeCountingMode", static_cast<int>(m_counterCountingEdgeMode),filename);
   m_iniObject->writeInteger("Counters", "countingDirection", static_cast<int>(m_counterCountDirectionMode),filename);
   m_iniObject->writeUnsignedInteger("Counters", "countingMax", m_counterMax,filename);
   m_iniObject->writeUnsignedInteger("Counters", "countingMin", m_counterMin,filename);
   //std::cout<<"counters saved"<<m_moduleName<<std::endl;
}

void NIDeviceModule::saveModules(std::string filename)
{
   //std::cout<<"entering save modules "<<m_moduleName<<std::endl;
   m_iniObject->writeInteger("Modules", "type", static_cast<int>(m_moduleType),filename);
   m_iniObject->writeString("Modules", "moduleName", m_moduleName,filename);
   m_iniObject->writeString("Modules", "Alias", m_alias,filename);
   m_iniObject->writeInteger("Modules", "shuntLocation", static_cast<int>(m_shuntLocation),filename);
   m_iniObject->writeDouble("Modules", "shuntValue", m_shuntValue,filename);
   m_iniObject->writeInteger("Modules", "terminalConfig", static_cast<int>(m_moduleTerminalConfig),filename);
   m_iniObject->writeInteger("Modules", "moduleUnit", static_cast<int>(m_moduleUnit),filename);
   //std::cout<<"modules saved "<<m_moduleName<<std::endl;
}





void NIDeviceModule::loadFromFile(const std::string &filename)
{ 
    if (!loadChannels(filename))
    {
        std::cerr << "Failed to load channel information from the ini file." << std::endl;   
    }
    if (!loadCounters(filename)) 
    {
            std::cerr << "Failed to load counter information from the ini file." << std::endl;
    }  
    if (!loadModules(filename)) 
    {
        std::cerr << "Failed to load modules information from the ini file." << std::endl;
    } 
}



void NIDeviceModule::saveToFile(const std::string& filename) {   
    saveChannels(filename);
    saveCounters(filename);
    saveModules (filename);
}


std::string NIDeviceModule::getAlias() const
{
    return m_alias;
}

void NIDeviceModule::setAlias(const std::string &newAlias)
{
    m_alias=newAlias;
}

void NIDeviceModule::setChanNames(const std::vector<std::string> &names)
{
    m_chanNames = names;
    Q_EMIT chanNamesChangedSignal(m_chanNames,this);
}

void NIDeviceModule::setCounterNames(const std::vector<std::string> &names)
{
    m_counterNames = names;
    Q_EMIT counterNamesChangedSignal(m_counterNames,this);
}    


void NIDeviceModule::setModuleType(ModuleType newType)
{
    m_moduleType = newType;
}

void NIDeviceModule::setChanMin(double newChanMin)
{
    if (newChanMin<m_analogChanMax)
    {
        m_analogChanMin = newChanMin;
        Q_EMIT chanMinChangedSignal(m_analogChanMin,this);
    }
}

void NIDeviceModule::setcounterCountingEdgeMode(moduleCounterEdgeConfig newCounterCountingEdgeMode)
{
    m_counterCountingEdgeMode = newCounterCountingEdgeMode;
    Q_EMIT counterEdgeConfigChangedSignal(newCounterCountingEdgeMode,this);
}

void NIDeviceModule::setCounterCountDirectionMode(moduleCounterMode newCounterCountMode)
{
    m_counterCountDirectionMode = newCounterCountMode;
    Q_EMIT counterModeChangedSignal(newCounterCountMode,this);
}

void NIDeviceModule::setChanMax(double newChanMax)
{
    if (newChanMax>m_analogChanMin)
    {
        m_analogChanMax = newChanMax;
        Q_EMIT chanMinChangedSignal(m_analogChanMax,this);
    }
}

void NIDeviceModule::setCounterMin(unsigned int newCountersMin)
{
    if (newCountersMin<m_counterMax)
    {
        m_counterMin=newCountersMin;
       Q_EMIT countersMinChangedSignal(m_counterMin,this);
    }
}



void NIDeviceModule::setCounterMax(unsigned int newCountersMax)
{
        if (newCountersMax>m_counterMin)
    {
        m_counterMax=newCountersMax;
        Q_EMIT countersMaxChangedSignal(m_counterMax,this);
    }
}

void NIDeviceModule::setNbDigitalOutputs(unsigned int newNbDigitalOutputs)
{
    m_nbDigitalOutputs = newNbDigitalOutputs;
    Q_EMIT nbDigitalOutputsChangedSignal(newNbDigitalOutputs,this);

}

void NIDeviceModule::setModuleUnit(moduleUnit newUnit)
{
    m_moduleUnit = newUnit;
    Q_EMIT moduleUnitChangedSignal(m_moduleUnit,this);

}


std::string NIDeviceModule::getModuleName() const
{
  return m_moduleName;
}

unsigned int NIDeviceModule::getNbChannel() const
{
    return m_nbChannel;
}

unsigned int NIDeviceModule::getNbCounters() const
{
    return m_nbCounters;
}

unsigned int NIDeviceModule::getSlotNb() const
{
    return m_slotNumber;
}

unsigned int NIDeviceModule::getNbDigitalIOPorts() const
{
    return m_nbDigitalIoPort;
}

std::string NIDeviceModule::getModuleInfo() const
{
    return m_moduleInfo;
}

std::vector<std::string> NIDeviceModule::getChanNames() const
{
    return m_chanNames;
}

std::vector<std::string> NIDeviceModule::getCounterNames() const
{
    return m_counterNames;
}

ModuleType NIDeviceModule::getModuleType() const
{
    return m_moduleType;
}

moduleShuntLocation NIDeviceModule::getModuleShuntLocation() const
{
    return m_shuntLocation;
}

moduleCounterEdgeConfig NIDeviceModule::getcounterCountingEdgeMode() const
{
    return m_counterCountingEdgeMode;
}

double NIDeviceModule::getModuleShuntValue() const
{
    return m_shuntValue;
}

moduleCounterMode NIDeviceModule::getCounterCountDirectionMode() const
{
    return m_counterCountDirectionMode;
}

unsigned int NIDeviceModule::getNbDigitalOutputs() const
{
    return m_nbDigitalOutputs;
}

moduleTerminalConfig NIDeviceModule::getModuleTerminalCfg() const
{
    return m_moduleTerminalConfig;
}

double NIDeviceModule::getChanMin() const
{
    return m_analogChanMin;
}

double NIDeviceModule::getChanMax() const
{
    return m_analogChanMax;
}

unsigned int NIDeviceModule::getminCounters() const
{
    return m_counterMin;
}

unsigned int NIDeviceModule::getmaxCounters() const
{
    return m_counterMax;
}

moduleUnit NIDeviceModule::getModuleUnit() const
{
    return m_moduleUnit;
}

void NIDeviceModule::setModuleName(const std::string &newModuleName)
{
    m_moduleName = newModuleName;
    Q_EMIT moduleNameChangedSignal(m_moduleName,this);
}

void NIDeviceModule::setNbChannel(unsigned int newNbChannels)
{
    m_nbChannel = newNbChannels;
    //emit signal
    Q_EMIT nbChannelsChangedSignal(newNbChannels,this);
}

void NIDeviceModule::setNbCounters(unsigned int newNbCounters)
{
    m_nbCounters = newNbCounters;
    Q_EMIT nbCountersChangedSignal(m_nbCounters,this);
}

void NIDeviceModule::setNbDigitalIOPorts(unsigned int newNbPorts)
{
    m_nbDigitalIoPort = newNbPorts;
    //emit signal
    Q_EMIT nbDigitalIoPortsChangedSignal(newNbPorts,this);
}

void NIDeviceModule::setModuleInfo(std::string newModuleInfo)
{
     m_moduleInfo = newModuleInfo;
        //emit signal
    Q_EMIT moduleInfoChangedSignal(newModuleInfo,this);

    
}

void NIDeviceModule::setModuleShuntLocation(moduleShuntLocation newLocation)
{
    m_shuntLocation = newLocation;
    Q_EMIT moduleShuntLocationChangedSignal(m_shuntLocation,this);
}

void NIDeviceModule::setModuleShuntValue(double newValue)
{
    m_shuntValue = newValue;
    Q_EMIT moduleShuntValueChangedSignal(m_shuntValue,this);
}

void NIDeviceModule::setModuleTerminalCfg(moduleTerminalConfig newTerminalConfig)
{
    m_moduleTerminalConfig = newTerminalConfig;
    Q_EMIT moduleTerminalConfigChangedSignal(m_moduleTerminalConfig,this);
}

void NIDeviceModule::setSlotNb(unsigned int newSlot)
{
    m_slotNumber = newSlot;
    Q_EMIT moduleSlotNumberChangedSignal(newSlot,this);
}
