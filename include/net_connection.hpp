#pragma once
#include "net_common.hpp"
#include "net_tsqueue.hpp"
#include "net_message.hpp"

namespace net {
template <typename T>
class connection : public std::enable_shared_from_this<connection<T>> {
 public:
    connection() {

    }
    virtual ~connection() {

    }
 public:
    bool ConnectToServer();
    bool Disconnect();
    bool isConnected() const;
 public:
    bool Send(const message<T>& msg);
 protected:
    boost::asio::ip::tcp::socket m_socket;
    boost::asio::io_context& m_asioContext;
    tsqueue<message<T>> m_qMessagesOut;
    tsqueue<owned_message>& m_qMessagesIn;
};
}

