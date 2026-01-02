#include <Settings.h>
#include <QCoreApplication>

using namespace Spectator;

int main(int argc, char ** argv)
{
    QCoreApplication app(argc, argv);
    const auto settings = Settings::fromCmdLine();
    return QCoreApplication::exec();
}
