#include "QCrioModulesDataExtractor.h"
#include <QSettings>
#include <QFileInfo>

QCrioModulesDataExtractor::QCrioModulesDataExtractor(QObject *parent) : QObject(parent)
{
    // Constructor implementation (if needed)
}

void QCrioModulesDataExtractor::extractCurrentModules(const QStringList &iniFiles, QComboBox *modulesComboBox, QComboBox *channelsComboBox)
{
    if (!modulesComboBox || !channelsComboBox)
    {
        return; // Safety check to ensure pointers are valid
    }

    modulesComboBox->blockSignals(true);
    channelsComboBox->blockSignals(true);

    modulesComboBox->clear();
    channelsComboBox->clear();

    foreach (const QString &file, iniFiles)
    {
        QFileInfo fileInfo(file);
        if (fileInfo.fileName().contains("NI9208", Qt::CaseInsensitive))
        {
            QSettings settings(file, QSettings::IniFormat);

            // Modules
            settings.beginGroup("modules");
            QString alias = settings.value("alias").toString();
            if (!alias.isEmpty())
            {
                modulesComboBox->addItem(alias);
            }
            settings.endGroup();

            // Channels
            settings.beginGroup("channels");
            int numberOfChannels = settings.value("numberofchannels", 0).toInt();
            for (int i = 0; i < numberOfChannels; ++i)
            {
                QString channelKey = QString("channel%1").arg(i);
                QString channelValue = settings.value(channelKey).toString();
                if (!channelValue.isEmpty())
                {
                    channelsComboBox->addItem(channelValue);
                }
            }
            settings.endGroup();
        }
    }

    modulesComboBox->blockSignals(false);
    channelsComboBox->blockSignals(false);
}
