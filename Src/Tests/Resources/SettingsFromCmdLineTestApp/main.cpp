#include <Settings.h>
#include <QCoreApplication>
#include <QTextStream>
#include <QDataStream>
#include <QByteArray>
#include <cstdio>

using namespace Spectator;

int main(int argc, char ** argv)
{
    QCoreApplication app(argc, argv);
    const auto settings = Settings::fromCmdLine();
    QByteArray buffer;
    QDataStream dataStream(&buffer, QIODeviceBase::WriteOnly);
    dataStream << settings.threadCount();
    dataStream << settings.repetitionCount();
    dataStream << settings.filePathFilter();
    dataStream << settings.scenarioNameFilter();
    dataStream << settings.scenarioTagsFilter();
    QTextStream outputStream(stdout);
    outputStream << buffer.toBase64();
    return 0;
}
