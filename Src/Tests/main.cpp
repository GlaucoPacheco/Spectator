#include <QCoreApplication>
#include <QtLogging>
#include <QThread>
#include <QList>
#include <QString>
#include <QProcess>
#include <QProcessEnvironment>
#include <QDir>
#include <QtLogging>
#include <QDebug>
#include <QDataStream>
#include <QByteArray>
#include <QStringList>
#include <QByteArrayList>
#include <QSet>
#include <QTextStream>
#include <QFileInfo>
#include <Qt>
#include <cstdio>

using namespace Qt::StringLiterals;

static QTextStream & qStdOut()
{
    static QTextStream textStream(stdout);
    return textStream;
}

static void spectatorFetchesSettingsFromCmdLine();
static void spectatorStoresScenarios();
static void spectatorSupportsInfoMessagesOutsideScenarioScope();
static void spectatorSupportsRequireOutsideScenarioScope();
static void spectatorVisitsAllLeafNodesOfScenarioPathWithoutGeneratorsOnce();

int main(int argc, char **argv)
{
    QCoreApplication app(argc, argv);
    qStdOut() << u"Running Tests"_s << Qt::endl;
    // spectatorFetchesSettingsFromCmdLine();
    // spectatorStoresScenarios();
    // spectatorSupportsInfoMessagesOutsideScenarioScope();
    // spectatorSupportsRequireOutsideScenarioScope();
    spectatorVisitsAllLeafNodesOfScenarioPathWithoutGeneratorsOnce();
    return 0;
}

static void spectatorFetchesSettingsFromCmdLine()
{
    qStdOut() << u"Testing Settings from command line."_s << Qt::endl;
    const auto threadCounts = QStringList() << u""_s
                                            << u"-1"_s
                                            << u"1"_s
                                            << QString::number(QThread::idealThreadCount());
    const auto repetitionCounts = QStringList() << u""_s
                                                << u"1"_s
                                                << u"1024"_s;
    const auto filePathFilters = QStringList() << u""_s
                                               << u"file://somefile"_s
                                               << u"file://another_file/at_sub/directory"_s;
    const auto scenarioNameFilters = QStringList() << u""_s
                                                   << u"TcpSockets exchange data"_s
                                                   << u"RingBuffer wraps data inside buffer"_s;
    const auto tagsList = QList<QStringList>() << QStringList()
                                               << (QStringList() << u"Design Tests"_s  << u"Another Tag"_s)
                                               << (QStringList() << u"TLS Encryption"_s);
    for (const auto threadCount : threadCounts)
    {
        for (const auto repetitionCount : repetitionCounts)
        {
            for (const auto & filePathFilter : filePathFilters)
            {
                for (const auto & scenarioNameFilter : scenarioNameFilters)
                {
                    for (const auto & tags : tagsList)
                    {
                        // Create command line arguments
                        QStringList cmdLineArgs;
                        if (!threadCount.isEmpty())
                            cmdLineArgs << u"-j"_s << threadCount;
                        if (!repetitionCount.isEmpty())
                            cmdLineArgs << u"-r"_s << repetitionCount;
                        if (!filePathFilter.isEmpty())
                            cmdLineArgs << u"-f"_s << filePathFilter;
                        if (!scenarioNameFilter.isEmpty())
                            cmdLineArgs << u"-s"_s << scenarioNameFilter;
                        if (!tags.isEmpty())
                        {
                            for (const auto & tag : tags)
                                cmdLineArgs << u"-t"_s << tag;
                        }
                        // Run test app
                        QProcess testApp;
                        auto testsAppDir = QDir(QCoreApplication::applicationDirPath());
                        auto resourceTestAppFilePath = testsAppDir.absoluteFilePath("Resources/SettingsFromCmdLineTestApp/SettingsFromCmdLineTestApp");
                        testApp.start(resourceTestAppFilePath, cmdLineArgs);
                        if (!testApp.waitForFinished(5000))
                            qFatal("%s%s%s", "Failed to wait for ", qUtf8Printable(resourceTestAppFilePath), " test app to finish.");
                        const auto output = testApp.readAllStandardOutput();
                        // Check result
                        const auto buffer = QByteArray::fromBase64(output);
                        QDataStream dataStream(buffer);
                        qint32 parsedThreadCount = 0;
                        dataStream >> parsedThreadCount;
                        const qint32 expectedThreadCount = threadCount.isEmpty() ? 1 : (threadCount.toInt() == -1 ? QThread::idealThreadCount() : threadCount.toInt());
                        if (parsedThreadCount != expectedThreadCount)
                            qFatal() << QString(u"Thread count does not match with expected value (parsed value = %1; expected value = %2)."_s).arg(parsedThreadCount).arg(expectedThreadCount);
                        qint64 parsedRepetitionCount = 0;
                        dataStream >> parsedRepetitionCount;
                        const qint64 expectedRepetitionCount = repetitionCount.isEmpty() ? 0 : repetitionCount.toLongLong();
                        if (parsedRepetitionCount != expectedRepetitionCount)
                            qFatal() << QString(u"Repetition count does not match with expected value (parsed value = %1; expected value = %2)."_s).arg(parsedRepetitionCount).arg(expectedRepetitionCount);
                        QString parsedFilePathFilter;
                        dataStream >> parsedFilePathFilter;
                        if (parsedFilePathFilter != filePathFilter)
                            qFatal() << QString(u"File path filter does not match with expected value (parsed value = %1; expected value = %2)."_s).arg(parsedFilePathFilter).arg(filePathFilter);
                        QString parsedScenarioNameFilter;
                        dataStream >> parsedScenarioNameFilter;
                        const auto expectedScenarioNameFilter = scenarioNameFilter.isEmpty() ? u""_s : (u"Scenario: "_s + scenarioNameFilter);
                        if (parsedScenarioNameFilter != expectedScenarioNameFilter)
                            qFatal() << QString(u"Scenario name filter does not match with expected value (parsed value = %1; expected value = %2)."_s).arg(parsedScenarioNameFilter).arg(expectedScenarioNameFilter);
                        QStringList parsedTags;
                        dataStream >> parsedTags;
                        QSet<QString> orderedParsedTags;
                        for (const auto &tag : parsedTags)
                            orderedParsedTags.insert(tag);
                        QSet<QString> orderedExpectedTags;
                        for (const auto &tag : tags)
                            orderedExpectedTags.insert(tag);
                        if (orderedParsedTags != orderedExpectedTags)
                            qFatal("Parsed tags do not match expected tags.");
                    }
                }
            }
        }
    }
    qStdOut() << u"PASSED Settings from command line tests."_s << Qt::endl;
}

static void spectatorStoresScenarios()
{
    qStdOut() << u"Testing Scenario Storing."_s << Qt::endl;
    // Run test app
    QProcess testApp;
    auto testsAppDir = QDir(QCoreApplication::applicationDirPath());
    auto resourceTestAppFilePath = testsAppDir.absoluteFilePath("Resources/SpectatorStoresScenariosTestApp/SpectatorStoresScenariosTestApp");
    testApp.start(resourceTestAppFilePath);
    if (!testApp.waitForFinished(5000))
        qFatal("%s%s%s", "Failed to wait for ", qUtf8Printable(resourceTestAppFilePath), " test app to finish.");
    const auto output = testApp.readAllStandardOutput();
    // Check result
    const auto buffer = QByteArray::fromBase64(output);
    QDataStream dataStream(buffer);
    qsizetype scenarioCount = 0;
    dataStream >> scenarioCount;
    if (scenarioCount != 5)
        qFatal("Spectator did not store the correct number of scenarios.");
    struct ScenarioData
    {
        QString sourceFile;
        qint32 sourceLine;
        QString scenarioName;
        inline bool operator==(const ScenarioData & other) const
        {
            return sourceFile == other.sourceFile
                   && sourceLine == other.sourceLine
                   && scenarioName == other.scenarioName;
        }
    };
    QList<ScenarioData> fetchedScenarios;
    for (auto i = 0; i < scenarioCount; ++i)
    {
        fetchedScenarios.append(ScenarioData());
        dataStream >> fetchedScenarios.back().sourceFile;
        dataStream >> fetchedScenarios.back().sourceLine;
        dataStream >> fetchedScenarios.back().scenarioName;
    }
    const QFileInfo thisFileInfo(__FILE__);
    QDir scenariosDir(thisFileInfo.canonicalPath());
    if (!scenariosDir.cd(u"Resources"_s) || !scenariosDir.cd(u"SpectatorStoresScenariosTestApp"_s))
        qFatal("Failed to navigate to directory containing scenario source files.");
    const auto expectedScenariosData = QList<ScenarioData>()
        << ScenarioData{.sourceFile=scenariosDir.absoluteFilePath(u"scenarios_1.cpp"_s), .sourceLine=3, .scenarioName=u"Scenario: A Scenario"_s}
        << ScenarioData{.sourceFile=scenariosDir.absoluteFilePath(u"scenarios_1.cpp"_s), .sourceLine=7, .scenarioName=u"Scenario: Another Scenario"_s}
        << ScenarioData{.sourceFile=scenariosDir.absoluteFilePath(u"scenarios_1.cpp"_s), .sourceLine=11, .scenarioName=u"Scenario: Yet Another Scenario"_s}
        << ScenarioData{.sourceFile=scenariosDir.absoluteFilePath(u"scenarios_2.cpp"_s), .sourceLine=4, .scenarioName=u"Scenario: Spectator is a really fast test framework"_s}
        << ScenarioData{.sourceFile=scenariosDir.absoluteFilePath(u"scenarios_3.cpp"_s), .sourceLine=5, .scenarioName=u"Scenario: Kourier is a blazingly fast HTTP server"_s};
    for (const auto & expectedScenarioData : expectedScenariosData)
    {
        if (!fetchedScenarios.contains(expectedScenarioData))
            qFatal("Spectator failed to store scenario data.");
    }
    qStdOut() << u"PASSED Testing Scenario Storing."_s << Qt::endl;
}

static void spectatorSupportsInfoMessagesOutsideScenarioScope()
{
    qStdOut() << u"Testing INFO messages outside scenario scope."_s << Qt::endl;
    // Run test app
    const auto options = QStringList() << u"IN_CONSTRUCTOR"_s << u"IN_DESTRUCTOR"_s;
    for (const auto & option : options)
    {
        QProcess testApp;
        QProcessEnvironment processEnvironment = QProcessEnvironment::systemEnvironment();
        processEnvironment.insert(u"INFO_LOCATION"_s, option);
        testApp.setProcessEnvironment(processEnvironment);
        auto testsAppDir = QDir(QCoreApplication::applicationDirPath());
        auto resourceTestAppFilePath = testsAppDir.absoluteFilePath("Resources/SpectatorSupportsInfoMessagesOutsideScenarioScopeTestApp/SpectatorSupportsInfoMessagesOutsideScenarioScopeTestApp");
        testApp.start(resourceTestAppFilePath);
        if (!testApp.waitForStarted(5000))
            qFatal("%s%s%s", "Failed to wait for ", qUtf8Printable(resourceTestAppFilePath), " test app to start.");
        if (!testApp.waitForFinished(5000))
            qFatal("%s%s%s", "Failed to wait for ", qUtf8Printable(resourceTestAppFilePath), " test app to finish.");
        const auto output = testApp.readAllStandardOutput();
        if (output.isEmpty())
            qFatal().noquote() << "INFO logging outside scenario scope test failed: " << testApp.readAllStandardError() << Qt::endl;
        else
        {
            const auto expectedMessagesInOutput = QByteArrayList()
                << "Buffer Contents: Global Scope"
                << "Assertions: 0"
                << ((option == u"IN_CONSTRUCTOR"_s) ? "INFO: This is the info message in constructor." : "INFO: This is the info message in destructor.");
            for (const auto & expectedMessage : expectedMessagesInOutput)
            {
                if (!output.contains(expectedMessage))
                {
                    qFatal().noquote() << "INFO logging outside scenario scope test failed: "
                                       << "Expected message \"" << expectedMessage << "\" was not found in process output."
                                       << Qt::endl << "Process output: " << output
                                       << Qt::endl;
                }
            }
        }
    }
    qStdOut() << u"PASSED Testing INFO messages outside scenario scope."_s << Qt::endl;
}

static void spectatorSupportsRequireOutsideScenarioScope()
{
    qStdOut() << u"Testing REQUIRE outside scenario scope."_s << Qt::endl;
    // Run test app
    const auto options = QStringList() << u"SUCCEED_IN_CONSTRUCTOR"_s
                                       << u"FAIL_IN_CONSTRUCTOR"_s
                                       << u"SUCCEED_IN_DESTRUCTOR"_s
                                       << u"FAIL_IN_DESTRUCTOR"_s;
    for (const auto & option : options)
    {
        QProcess testApp;
        QProcessEnvironment processEnvironment = QProcessEnvironment::systemEnvironment();
        processEnvironment.insert(u"REQUIRE_TYPE"_s, option);
        testApp.setProcessEnvironment(processEnvironment);
        auto testsAppDir = QDir(QCoreApplication::applicationDirPath());
        auto resourceTestAppFilePath = testsAppDir.absoluteFilePath("Resources/SpectatorSupportsRequireOutsideScenarioScopeTestApp/SpectatorSupportsRequireOutsideScenarioScopeTestApp");
        testApp.start(resourceTestAppFilePath);
        if (!testApp.waitForStarted(5000))
            qFatal("%s%s%s", "Failed to wait for ", qUtf8Printable(resourceTestAppFilePath), " test app to start.");
        if (!testApp.waitForFinished(5000))
            qFatal("%s%s%s", "Failed to wait for ", qUtf8Printable(resourceTestAppFilePath), " test app to finish.");
        const QFileInfo thisFileInfo(__FILE__);
        QDir scenariosDir(thisFileInfo.canonicalPath());
        if (!scenariosDir.cd(u"Resources"_s) || !scenariosDir.cd(u"SpectatorSupportsRequireOutsideScenarioScopeTestApp"_s))
            qFatal("Failed to navigate to directory containing scenario source files.");
        const auto mainFilePath = scenariosDir.absoluteFilePath(u"main.cpp"_s);
        QByteArrayList expectedMessages;
        if (option == u"SUCCEED_IN_CONSTRUCTOR"_s)
        {
            expectedMessages = QByteArrayList() << "Global Scope" << "Assertions: 1";
        }
        else if (option == u"FAIL_IN_CONSTRUCTOR"_s)
        {
            expectedMessages = QByteArrayList() << QByteArray("REQUIRE(false) failed at file://").append(mainFilePath.toUtf8()).append(":24.");
        }
        else if (option == u"SUCCEED_IN_DESTRUCTOR"_s)
        {
            expectedMessages = QByteArrayList() << "Global Scope" << "Assertions: 0";
        }
        else if (option == u"FAIL_IN_DESTRUCTOR"_s)
        {
            expectedMessages = QByteArrayList() << "Global Scope" << "Assertions: 0" << QByteArray("REQUIRE(false) failed at file://").append(mainFilePath.toUtf8()).append(":36.");
        }
        else
            qFatal().noquote() << "Require outside scenario scope test failed: Invalid option value of: " << option << Qt::endl;
        const auto output = testApp.readAllStandardOutput() + testApp.readAllStandardError();
        if (output.isEmpty())
            QTextStream(stdout) << "Require outside scenario scope test failed: test app did not generate neither of standard/error output." << Qt::endl;
        else
        {
            for (const auto & expectedMessage : expectedMessages)
            {
                if (!output.contains(expectedMessage))
                {
                    qFatal().noquote() << "REQUIRE outside scenario scope test failed: "
                                       << "Expected message \"" << expectedMessage << "\" was not found in process output."
                                       << Qt::endl << "Process output: " << output
                                       << Qt::endl;
                }
            }
        }
    }
    qStdOut() << u"PASSED Testing Require outside scenario scope."_s << Qt::endl;
}

static void spectatorVisitsAllLeafNodesOfScenarioPathWithoutGeneratorsOnce()
{
    qStdOut() << u"Testing Spectator Visits All Leaf Nodes Of Scenario Path Without Generators Once."_s << Qt::endl;
    // Run test app
    QProcess testApp;
    auto testsAppDir = QDir(QCoreApplication::applicationDirPath());
    auto resourceTestAppFilePath = testsAppDir.absoluteFilePath("Resources/SpectatorVisitsAllLeafNodesOfScenarioPathWithoutGeneratorsOnceTestApp/SpectatorVisitsAllLeafNodesOfScenarioPathWithoutGeneratorsOnceTestApp");
    testApp.start(resourceTestAppFilePath);
    if (!testApp.waitForFinished(5000))
        qFatal("%s%s%s", "Failed to wait for ", qUtf8Printable(resourceTestAppFilePath), " test app to finish.");
    if (testApp.exitCode() != 0 || testApp.exitStatus() != QProcess::NormalExit)
    {
        qStdOut() << "Process Failed:" << Qt::endl << testApp.readAllStandardError() << Qt::endl;
        qFatal("Testing Spectator Visits All Leaf Nodes Of Scenario Path Without Generators Once failed.");
    }
    qStdOut() << u"PASSED Testing Spectator Visits All Leaf Nodes Of Scenario Path Without Generators Once."_s << Qt::endl;
}
