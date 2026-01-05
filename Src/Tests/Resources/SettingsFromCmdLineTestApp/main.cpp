#include <Settings.h>
#include <QCoreApplication>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QString>
#include <QTextStream>
#include <cstdio>

using namespace Spectator;

int main(int argc, char ** argv)
{
    QCoreApplication app(argc, argv);
    const auto settings = Settings::fromCmdLine();
    QJsonObject root;
    root["threadCount"] = settings.threadCount();
    root["repetitionCount"] = settings.repetitionCount();
    root["filePathFilter"] = settings.filePathFilter();
    root["scenarioNameFilter"] = settings.scenarioNameFilter();
    QJsonArray tags;
    const auto scenarioTagsFilter = settings.scenarioTagsFilter();
    for (const auto &tag : scenarioTagsFilter)
        tags.append(tag);
    root["scenarioTagsFilter"] = tags;
    QJsonDocument jsonDoc;
    jsonDoc.setObject(root);
    QTextStream outputStream(stdout);
    outputStream << jsonDoc.toJson(QJsonDocument::Compact);
    return 0;
}
