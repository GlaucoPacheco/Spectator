#include <QCoreApplication>
#include <QtLogging>
#include <QThread>
#include <QList>
#include <QString>
#include <QProcess>
#include <QDir>
#include <QtLogging>
#include <QDebug>
#include <QDataStream>
#include <QByteArray>
#include <QStringList>
#include <QSet>
#include <QTextStream>
#include <cstdio>

using namespace Qt::StringLiterals;

static QTextStream & qStdOut()
{
    static QTextStream textStream(stdout);
    return textStream;
}

static void testSettingsFromCmdLine();

int main(int argc, char **argv)
{
    QCoreApplication app(argc, argv);
    qStdOut() << u"Running Tests"_s << Qt::endl;
    testSettingsFromCmdLine();
    return 0;
}

static void testSettingsFromCmdLine()
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
