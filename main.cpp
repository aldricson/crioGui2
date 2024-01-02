#include "mainwindow.h"

#include <QApplication>


void writeSSHScriptToFile()
{
    // Path to the resource file in the resource system
    QString resourcePath = ":/scripts/ssh.bat"; // Update with actual path in your resource file

    // Open the resource file
    QFile inputFile(resourcePath);
    if (inputFile.open(QIODevice::ReadOnly))
    {
        QTextStream in(&inputFile);

        // Path to the output file in the application directory
        QString outputPath = QCoreApplication::applicationDirPath() + "/ssh.bat";

        // Open the output file
        QFile outputFile(outputPath);
        if (outputFile.open(QIODevice::WriteOnly))
        {
            QTextStream out(&outputFile);
            out << in.readAll(); // Write the contents of the resource file to the output file

            outputFile.close();
        }

        inputFile.close();
    }
}

int main(int argc, char *argv[])
{
    QApplication crioGui2(argc, argv);
    writeSSHScriptToFile();
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
    return crioGui2.exec();
}
