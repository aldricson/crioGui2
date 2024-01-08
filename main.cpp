#include "mainwindow.h"

#include <QApplication>




int main(int argc, char *argv[])
{
    QApplication crioGui2(argc, argv);

    QFile styleFile(":/styles/dark1.scss");
    if(styleFile.open(QIODevice::ReadOnly))
    {
        QTextStream textStream(&styleFile);
        QString styleSheet = textStream.readAll();
        styleFile.close();
        crioGui2.setStyleSheet(styleSheet);
    }
    MainWindow w;
    w.show();
    //w.showFullScreen();
    return crioGui2.exec();
}
