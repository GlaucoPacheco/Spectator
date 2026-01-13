#include "ScenarioRepository.h"
#include "SectionEntryRecorder.h"
#include "ScenarioRunner.h"
#include <Spectator.h>
#include <QCoreApplication>
#include <QtLogging>

using namespace Spectator;
using namespace Spectator::Test;

int main(int argc, char ** argv)
{
    QCoreApplication app(argc, argv);
    auto & scenarioRepository = ScenarioRepository::global();
    if (scenarioRepository.size() != 1)
        qFatal("Failed to store scenario.");
    auto recordedEntries = SectionEntryRecorder::global().recordedEntries();
    if (!recordedEntries.isEmpty())
        qFatal("Recorded entries is expected to be empty.");
    ScenarioRunner::runScenario(scenarioRepository.global()[0]);
    recordedEntries = SectionEntryRecorder::global().recordedEntries();
    if (recordedEntries.isEmpty())
        qFatal("Recorded entries is expected to be non-empty.");
    return 0;
}
