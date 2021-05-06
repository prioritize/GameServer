#include "../include/ConnectionHandler.hpp"

std::vector<char> vBuffer(20*1024);

void GrabSomeData(asio::ip::tcp::socket& socket) {
    socket.async_read_some(asio::buffer(vBuffer.data(), vBuffer.size()),
    [&](boost::system::error_code ec, std::size_t length) {
        if (!ec) {
            std::cout << "\n\nRead " << length << " bytes\n\n";
            for (size_t i = 0; i < length; i++) {
                std::cout << vBuffer[i];
                GrabSomeData(socket);
            }
        }
    });
}