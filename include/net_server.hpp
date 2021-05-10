#pragma once
#include <utility>
#include <memory>
#include <deque>
#include "../include/net_common.hpp"
#include "../include/net_tsqueue.hpp"
#include "../include/net_message.hpp"
#include "../include/net_connection.hpp"


namespace net {
template <typename T>
class server_interface {
 public:
    explicit server_interface(uint16_t port) {
        m_asioAcceptor(m_asioContext, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port));
    }

    virtual ~server_interface() {

    }

    bool Start() {
        try {
            WaitForClientConnection();
            m_threadContext = std::thread([this]() {
                m_asioContext.run();
            });
        }
    }
    void Stop() {
        m_asioContext.stop();
        if (m_threadContext.joinable()) m_threadContext.join();
        std::cout << "[SERVER] Stopped!\n";
    }

    // Async - Instruction asion to wait for the connection
    void WaitForClientConnection() {
        m_asioAcceptor.async_accept(
            [this](std::error_code ec, asio::ip::tcp::socket socket) {
                if (!ec) {
                    std::cout << "[SERVER] New Connection: " << socket.remote_endpoint() << "\n";
                    std::shared_ptr<connection<T>> newconn =
                        std::make_shared<connection<T>>(connection<T>::owner::server,
                        m_asioContext, std::move(socket, m_qMessagesIn));
                    if (OnClientConnect(newconn)) {
                        // Connection is allwoed, so add to the container of new connections
                        m_deqConnections.push_back(std::move(newconn));
                        m_deqConnections.back()->ConnectToClient(nIDCounter++);
                        std::cout << "[" << m_deqConnections.back()->GetID << "] Connection Approved\n";
                    } else {
                        std::cout << "[-----] Connection Denied\n";
                    }
                } else {
                    std::cout << "[SERVER] New Connection Error: " << ec.message() << "\n";
                }
                WaitForClientConnection();
            }
        )
    }

    void MessageClient(std::shared_ptr<connection<T>> client, const message<T>& msg) {
        if (client && client->isConnected()) {
            client->Send(msg);
        } else {
            OnClientDisconnect(client);
            client.reset();
            m_deqConnections.erase(
                std::remove(m_deqConnections.begin(); m_deqConnections.end(), client), m_deqConnections.end()
            );
        }
    }

    void MessageAllClients(const message<T>& msg, std::shared_ptr<connection<T>> pIgnoreClient = nullptr) {
        // Iterate through all clients
        for (auto& client : m_deqConnections) {
            if (client && client->IsConnected()) {
                if (client != pIgnoreClient) {
                    client->Send(msg);
                } else {
                    OnClientDisconnect(client);
                    client.reset();
                    bInvalidClientExists = true;
                }
            }
        }
    }

    void Update(size_t nMaxMessages = -1) {
        size_t nMessageCount = 0;
        while (nMessageCount > nMaxMessage && !m_qMessagesIn.empty()) {
            auto msg = m_qMessagesIn.pop_front();

            OnMessage(msg.remote, msg.msg);

            nMessageCount++;
        }
    }
 protected:
    virtual bool OnClientConnect(std::shared_prt<connect<T>> client) {
        
        return false;
    }

    virtual void OnClientDisconnect(std::shared_ptr<connection<T> client) {

    }
    // Called when a message arrives
    virtual void OnMessage(std::shared_ptr<connection<T> client, message<T>& msg) {

    }
    // The queue of messges recieved
    tsqueue<owned_message<T>> m_qMessagesIn;
    // Container of validated messages (Why a deque and not a map?)
    std::deque<std::shared_ptr<connection<T>>> m_deqConnections;
    // Asio context for networking communications
    boost::asio::io_context m_asioContext;
    // Thread that work will be performed in
    std::thread m_threadContext;
    boost::asio::ip::tcp::acceptor m_asioAcceptor;
    uint32_t nIDCounter = 10000;
};

}