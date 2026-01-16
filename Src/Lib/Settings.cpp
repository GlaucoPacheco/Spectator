// Copyright (C) 2026 Glauco Pacheco <glaucopacheco@gmail.com>
// SPDX-License-Identifier: AGPL-3.0-only OR CDDL-1.0

#include "Settings.h"
#include <QCommandLineParser>
#include <QThread>

namespace Spectator
{

Settings Settings::fromCmdLine()
{
    QCommandLineParser parser;
    parser.addOptions({{"j", "Sets the number of threads to use for running the tests.", "<thread count>", "1"},
                       {"r", "Sets how many times the tests should be repeated.", "<repetition count>", "0"},
                       {"f", "Sets the source file for filtering scenarios. Only scenarios belonging to the given source file are run.", "<scenario filename>", ""},
                       {"s", "Sets the scenario name for filtering scenarios. Only scenarios matching the given name are run.", "<scenario name>", ""},
                       {"t", "Adds the scenario tag for filtering scenarios. Only scenarios tagged with the given tag are run.", "<scenario tag>", ""}});
    if (QCoreApplication::instance() == nullptr) [[unlikely]]
        qFatal("Failed to fetch settings from command line. Current QCoreApplication instance is null. Please, create a QCoreInstance before trying to run the scenarios.");
    else if (!parser.parse(QCoreApplication::arguments())) [[unlikely]]
    {
        qFatal("%s%s%s", "Failed to fetch settings from command line. "
                         "Failed to parse command line arguments",
                         qUtf8Printable(QCoreApplication::arguments().join(' ')), ".");
    }
    Settings settings;
    if (parser.isSet("j"))
    {
        const auto values = parser.values("j");
        if (values.size() != 1) [[unlikely]]
            qFatal("Failed to parse command line options. Thread count (-j option) must be specified only once.");
        else [[likely]]
        {
            bool ok = false;
            auto const threadCount = values[0].toInt(&ok);
            if (!ok || threadCount < -1 || threadCount == 0 || threadCount > QThread::idealThreadCount())
                qFatal("Failed to parse command line options. Invalid argument value. "
                        "The option -j must have as values either a positive integer equal to or "
                        "lesser than the value returned by QThread::idealThreadCount or -1, in "
                        "which case, QThread::idealThreadCount() threads will be used for running the filtered scenarios.");
            settings.m_threadCount = threadCount > 0 ? threadCount : QThread::idealThreadCount();
        }
    }
    if (parser.isSet("r"))
    {
        const auto values = parser.values("r");
        if (values.size() != 1) [[unlikely]]
            qFatal("Failed to parse command line options. Repetition count (-r option) must be specified only once.");
        else [[likely]]
        {
            bool ok = false;
            auto const repetitionCount = values[0].toLongLong(&ok);
            if (!ok || repetitionCount <= 0)
                qFatal("Failed to parse command line options. Invalid argument value. "
                       "The option -r must have as value a positive integer.");
            settings.m_repetitionCount = repetitionCount;
        }
    }
    if (parser.isSet("f"))
    {
        const auto values = parser.values("f");
        if (values.size() != 1) [[unlikely]]
            qFatal("Failed to parse command line options. Only one file path at a time can be used for filtering scenarios (-f can only be specified once).");
        else [[likely]]
            settings.m_filePathFilter = values[0];
    }
    if (parser.isSet("s"))
    {
        const auto values = parser.values("s");
        if (values.size() != 1) [[unlikely]]
            qFatal("Failed to parse command line options. Only one scenario name at a time can be used for filtering scenarios (-s can only be specified once).");
        else [[likely]]
            settings.m_scenarioNameFilter = QString(values[0]).prepend(QStringLiteral(u"Scenario: "));
    }
    if (parser.isSet("t"))
        settings.m_scenarioTagsFilter = parser.values("t");
    return settings;
}

}
