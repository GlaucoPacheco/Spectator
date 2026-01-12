#include "ScenarioRunner.h"
#include <Spectator.h>
#include <QProcessEnvironment>
#include <QString>
#include <QtLogging>
#include <QDebug>
#include <Qt>
#include <QTextStream>
#include <cstdio>

using namespace Qt::StringLiterals;
using namespace Spectator;

struct InfoLogger
{
    InfoLogger()
    {
        if (QProcessEnvironment::systemEnvironment().value(u"INFO_LOCATION"_s) == u"IN_CONSTRUCTOR"_s)
        {
            INFO(u"This is the info message in constructor."_s);
        }
    }

    ~InfoLogger()
    {
        if (QProcessEnvironment::systemEnvironment().value(u"INFO_LOCATION"_s) == u"IN_DESTRUCTOR"_s)
        {
            INFO(u"This is the info message in destructor."_s);
        }
    }
};

static InfoLogger infoLogger;

int main(int argc, char ** argv)
{
    const auto envVarValue = QProcessEnvironment::systemEnvironment().value(u"INFO_LOCATION"_s);
    if (envVarValue != u"IN_CONSTRUCTOR"_s && envVarValue != u"IN_DESTRUCTOR"_s)
    {
        qFatal().noquote() << u"Failed to test INFO logging outside scenario scope. "_s
                           << u"This test requires the environment variable INFO_LOCATION to be set "_s
                           << u"to either IN_CONSTRUCTOR or IN_DESTRUCTOR."_s << Qt::endl;
    }
    else
    {
        QString buffer;
        buffer.reserve(128);
        Spectator::ScenarioRunner::printGlobalStats(buffer);
        QTextStream outputStream(stdout);
        outputStream << u"Buffer Contents: "_s << buffer;
    }
    return 0;
}
