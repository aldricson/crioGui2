#ifndef QCrioModulesDataExtractor_H
#define QCrioModulesDataExtractor_H

#include <QObject>
#include <QStringList>
#include <QComboBox>

class QCrioModulesDataExtractor : public QObject {
    Q_OBJECT

public:
    explicit QCrioModulesDataExtractor(QObject *parent = nullptr);
    void extractCurrentModules(const QStringList &iniFiles, QComboBox *modulesComboBox, QComboBox *channelsComboBox);

};

#endif // QCrioModulesDataExtractor_H
