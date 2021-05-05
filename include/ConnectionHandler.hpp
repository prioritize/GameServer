// Copyright: 2021 -- Jared Leach
#include <vector>
#include <memory>
#include <boost/asio.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/sources/logger.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/system/error_code.hpp>
namespace asio = boost::asio;


template <typename ConnectionHandler>
class asio_generic_server {
    using shared_handler_t = std::shared_ptr<ConnectionHandler>;
 public:
    explicit asio_generic_server(int thread_count)
        : thread_count_(thread_count), acceptor_(io_service_)
        {}
    void start_server(uint16_t port) {
        auto handler = std::make_shared<ConnectionHandler>(io_service_);
        asio::ip::tcp::endpoint endpoint(asio::ip::tcp::v4(), port);
        acceptor_.open(endpoint.protocol());
    }

 private:
    void handle_new_connection(shared_handler_t handler,
                            system::error_code const& error) {
    }
    int thread_count_;
    std::vector<std::thread> thread_pool_;
    asio::io_service io_service_;
    asio::ip::tcp::acceptor acceptor_;
};
