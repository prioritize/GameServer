#pragma once
#include "net_common.hpp"
#include "net_connection.hpp"
#include "net_tsqueue.hpp"
#include "net_message.hpp"
#include <memory>
#include <string>

namespace net {
template <typename T>
class client_interface {
    client_interface() : m_socket(m_context) {

    }
    virtual ~client_interface() {
        Disconnect();
    }

 protected:
    boost::asio::io_context m_context;
    std::thread thrContext;
    boost::asio::ip::tcp::socket m_socket;
    std::unique_ptr<connection<T>> m_connection;

 private:
    tsqueue<owned_message<T>> m_qMessagesIn;

 public:
    bool Connect(const std::string& host, const uint16_t port) {
        try {
            // Create connection
            // TODO(Jared Leach): Flesh out the make_unique
            m_connection = std::make_unique<connection<T>>();
            boost::asio::tcp::resolver resolver(m_context);
            m_endpoints = resolver.resolve(host, std::to_string(port));

            m_connection->ConnectToServer(m_endpoints);
            thrContext = std::thread([this]() {
                m_context.run();
            });

        } catch (std::exception& e) {
            std::cout << "client exception: " << e.what() << "\n";
            return false;
        }
        return false;
    }
    void Disconnect() {
        if (IsConnected()) {
            m_connection->Disconnect();
        }
        m_context.stop();

        if (thrContext.joinable()) {
            thrContext.join();
            m_connection.release();
        }
    }
    bool IsConnected() {
        if (m_connection) {
            return m_connection->isConnected();
        } else {
            return false;
        }
    }

};
}  // namespace net
