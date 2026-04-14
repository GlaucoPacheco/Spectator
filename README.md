# Introduction

 Spectator is a C++ test framework for behavior-driven design and was created to support Kourier development.

Kourier provides a high-performance C++ server that you interact with through the intuitive Qt signals and slots. Kourier is thoroughly tested using generated data, leading to multiple runs of the same scenario path. Thus, a fast test framework capable of running tests on all available cores significantly reduces test execution time.

Beyond being fast, to test network interaction, we must be able to go back to the event loop while waiting for network events to happen. Spectator provides all of the niceties of having a Qt event loop running behind the scenes and can process events while waiting for semaphores.

Below, we show how intuitive Spectator makes testing network interaction using QTcpSocket as an example:

```cpp
#include <Spectator>
#include <QTcpServer>
#include <QTcpSocket>
#include <QHostAddress>
#include <QByteArray>
#include <QSemaphore>
#include <QDeadlineTimer>
#include <memory>


SCENARIO("QTcpSocket sends data to connected peer before disconnecting")
{
    GIVEN("Two connected sockets")
    {
        QTcpServer server;
        REQUIRE(server.listen(QHostAddress::LocalHost));
        std::unique_ptr<QTcpSocket> pServerPeer;
        QSemaphore serverPeerConnectedSemaphore;
        QObject::connect(&server, &QTcpServer::newConnection, [&]()
        {
            REQUIRE(pServerPeer.get() == nullptr);
            pServerPeer.reset(server.nextPendingConnection());
            REQUIRE(pServerPeer.get() != nullptr);
            pServerPeer->setParent(nullptr);
            REQUIRE(server.nextPendingConnection() == nullptr);
            serverPeerConnectedSemaphore.release(1);
        });
        QTcpSocket clientPeer;
        QSemaphore clientPeerConnectedSemaphore;
        QObject::connect(&clientPeer, &QTcpSocket::connected, [&]()
        {
            clientPeerConnectedSemaphore.release(1);
        });
        clientPeer.connectToHost(server.serverAddress(), server.serverPort());
        REQUIRE(TRY_ACQUIRE(clientPeerConnectedSemaphore, QDeadlineTimer(5000)));
        REQUIRE(TRY_ACQUIRE(serverPeerConnectedSemaphore, QDeadlineTimer(5000)));

        WHEN("client peer sends data to server peer before disconnecting")
        {
            const auto sentData = GENERATE(AS(QByteArray), "Hello Peer!", "The test will be ran again with this text as sent data.");
            clientPeer.write(sentData);
            clientPeer.disconnectFromHost();

            THEN("server peer receives sent data before disconnecting")
            {
                QSemaphore serverPeerDisconnectedSemaphore;
                QObject::connect(pServerPeer.get(), &QTcpSocket::disconnected, [&]
                {
                    serverPeerDisconnectedSemaphore.release(1);
                });
                REQUIRE(TRY_ACQUIRE(serverPeerDisconnectedSemaphore, QDeadlineTimer(5000)));
                REQUIRE(pServerPeer->readAll() == sentData);
            }
        }
    }
}
```

You can build and run the test above following the [tutorial](Src/Tutorial/README.md).

## Command-line arguments

| Option     | Information       |
| ------------- | -------- |
| -r       | Sets how many times the tests must be repeated. The default value is 0. |
| -f  | Filters scenarios by file path. Only scenarios belonging to the given file are run. The default value is an empty string.  |
| -s | Filters scenario by name. Only scenarios matching the given name are run. The default value is an empty string.  |
| -t | Filters scenarios by tag. Only scenarios tagged with the given tag are run. The default value is an empty string.  |
| -j | Sets the thread count used to run scenario paths. Must be a value between 1 and the value returned by QThread::idealThreadCount() or -1, in which case all available cores are used for running scenario paths. The default value is 1, making all tests run in a single thread. |

## License

Spectator is dual-licensed under AGPL-3.0-only OR MPL-2.0-no-copyleft-exception.
