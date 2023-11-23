#ifndef QReadCurrentTestWidget_H
#define QReadCurrentTestWidget_H

#include <QWidget>
#include <QGroupBox>
#include <QLabel>
#include <QComboBox>
#include <QPushButton>
#include <QGridLayout>

class QReadCurrentTestWidget : public QWidget {
    Q_OBJECT

public:
    QReadCurrentTestWidget(QWidget *parent = nullptr);


    QComboBox *getModulesComboBox() const;
    void setModulesComboBox(QComboBox *newModulesComboBox);

    QComboBox *getChannelComboBox() const;
    void setChannelComboBox(QComboBox *newChannelComboBox);

signals:
    void modulesComboBoxChanged();

    void channelComboBoxChanged();

private slots:
    void onReadOneShotClicked();
    void onPollClicked();

private:
    QGroupBox    *groupBox;
    QLabel       *moduleLabel;
    QLabel       *channelLabel;
    QComboBox    *modulesComboBox;
    QComboBox    *channelComboBox;
    QPushButton  *readOneShotButton;
    QPushButton  *pollButton;
    QLabel       *resultLabel;


    void setupUi();
    Q_PROPERTY(QComboBox *modulesComboBox READ getModulesComboBox WRITE setModulesComboBox NOTIFY modulesComboBoxChanged)
    Q_PROPERTY(QComboBox *channelComboBox READ getChannelComboBox WRITE setChannelComboBox NOTIFY channelComboBoxChanged)
};

#endif // QReadCurrentTestWidget_H
