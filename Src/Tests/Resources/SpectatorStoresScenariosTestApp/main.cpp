#include "ScenarioRepository.h"
#include <Spectator.h>
#include <QCoreApplication>
#include <QTextStream>
#include <QDataStream>
#include <QByteArray>
#include <QString>
#include <cstdio>

using namespace Spectator;

int main(int argc, char ** argv)
{
    QCoreApplication app(argc, argv);
    QByteArray buffer;
    buffer.reserve(1024);
    QDataStream dataStream(&buffer, QIODeviceBase::WriteOnly);
    auto & scenarioRepository = ScenarioRepository::global();
    dataStream << scenarioRepository.size();
    for (auto i = 0; i < scenarioRepository.size(); ++i)
    {
        const auto & scenario = scenarioRepository[i];
        dataStream << QString(scenario.sourceFile());
        dataStream << scenario.sourceLine();
        dataStream << QString(scenario.scenarioName());
    }
    QTextStream outputStream(stdout);
    outputStream << buffer.toBase64();
    return 0;
}
