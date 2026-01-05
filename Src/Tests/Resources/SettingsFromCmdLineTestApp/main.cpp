#include <Settings.h>
#include <QCoreApplication>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QString>
#include <QTextStream>
#include <cstdio>

using namespace Spectator;
using namespace Qt::StringLiterals;

int main(int argc, char ** argv)
{
    QCoreApplication app(argc, argv);
    const auto settings = Settings::fromCmdLine();
    QJsonObject root;
    root[u"threadCount"_s] = settings.threadCount();
    root[u"repetitionCount"_s] = settings.repetitionCount();
    root[u"filePathFilter"_s] = settings.filePathFilter();
    root[u"scenarioNameFilter"_s] = settings.scenarioNameFilter();
    QJsonArray tags;
    const auto scenarioTagsFilter = settings.scenarioTagsFilter();
    for (const auto &tag : scenarioTagsFilter)
        tags.append(tag);
    root[u"scenarioTagsFilter"_s] = tags;
    QJsonDocument jsonDoc;
    jsonDoc.setObject(root);
    QTextStream outputStream(stdout);
    outputStream << jsonDoc.toJson(QJsonDocument::Compact);
    return 0;
}
