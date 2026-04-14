// Copyright (C) 2026 Glauco Pacheco <glaucopacheco@gmail.com>
// SPDX-License-Identifier: AGPL-3.0-only OR MPL-2.0-no-copyleft-exception

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
