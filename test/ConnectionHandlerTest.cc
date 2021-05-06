// Copyright: 2021 -- Jared Leach
#include <gtest/gtest.h>
#include <boost/chrono.hpp>
// #include <boost/chrono/io/time_point_io.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/date_time/posix_time/posix_time_duration.hpp>
#include "../include/ConnectionHandler.hpp"

using boost::chrono::system_clock;
namespace asio = boost::asio;
namespace posix_time = boost::posix_time;
TEST(TestGoogle, TestGTest) {
    ASSERT_EQ(0, 0);
}
TEST(Chat, TestAsioWait) {
    asio::io_service service;
    asio::deadline_timer timer(service, posix_time::seconds(5));
    timer.async_wait([](auto ... vn){
                std::cout << system_clock::now() << " : timer expired. \n";
            });
    std::cout << system_clock::now() << " : calling run. \n";
    service.run();
    std::cout << system_clock::now() << " : done. \n";
    // boost::asio::ip::tcp::socket test_socket;

    // test_socket.async_send()
}
void timer_expired(std::string id) {
    std::cout << system_clock::now() << " " << id << " enter.\n";
    std::this_thread::sleep_for(std::chrono::seconds(3));
    std::cout << system_clock::now() << " " << id << " leave.\n";
}

TEST(Chat, SingleThread) {
    asio::io_service service;

    asio::deadline_timer timer1(service, posix_time::seconds(5));
    asio::deadline_timer timer2(service, posix_time::seconds(5));

    timer1.async_wait([](auto ... vn){timer_expired("timer1");});
    timer2.async_wait([](auto ... vn){timer_expired("timer2");});

    std::thread butler([&](){service.run();});

    butler.join();
    std::cout << "done.\n";
}

TEST(Chat, TwoThreads) {
    asio::io_service service;

    asio::deadline_timer timer1(service, posix_time::seconds(5));
    asio::deadline_timer timer2(service, posix_time::seconds(5));

    timer1.async_wait([](auto ... vn){timer_expired("timer1");});
    timer2.async_wait([](auto ... vn){timer_expired("timer2");});

    std::thread butler1([&](){service.run();});
    std::thread butler2([&](){service.run();});
    butler1.join();
    butler2.join();
    std::cout << "done.\n";
}

TEST(Chat, Post) {
    asio::io_service service;

    service.post([]{ std::cout << "eat\n"; });
    service.post([]{ std::cout << "drink\n"; });
    service.post([]{ std::cout << "and be merry!\n"; });

    std::thread butler([&]{ service.run(); });
    butler.join();

    std::cout << "done\n";
}

TEST(Chat, Strand) {
    asio::io_service service;
    asio::io_service::strand strand(service);

    asio::deadline_timer timer1(service, posix_time::seconds(5));
    asio::deadline_timer timer2(service, posix_time::seconds(5));

    timer1.async_wait(
        strand.wrap([](auto ... vn) {
            timer_expired("timer1");
        }));
    timer2.async_wait(
        strand.wrap([](auto ... vn) {
            timer_expired("timer2");
        }));
    std::thread butler1([&](){service.run();});
    std::thread butler2([&](){service.run();});

    butler1.join();
    butler2.join();
    std::cout << "done!\n";
}

TEST(Chat, ThreeStrands) {
    asio::io_service service;
    asio::io_service::strand strand(service);
    asio::deadline_timer timer1(service, posix_time::seconds(5));
    asio::deadline_timer timer2(service, posix_time::seconds(5));
    asio::deadline_timer timer3(service, posix_time::seconds(6));

    timer1.async_wait(
        strand.wrap([](auto ... vn) {
        timer_expired("timer1");
    }));
    timer2.async_wait(
        strand.wrap([](auto ... vn) {
            timer_expired("timer2");
    }));
    timer3.async_wait([](auto ... vn) {
        timer_expired("timer3");
    });
    std::thread butler1([&](){service.run();});
    std::thread butler2([&](){service.run();});

    butler1.join();
    butler2.join();
    std::cout << "done\n";
}

TEST(Chat, SimpleExample) {
    std::vector<char> vBuffer(20*1024);
    boost::system::error_code ec;
    asio::io_context context;
    asio::ip::tcp::endpoint endpoint(asio::ip::make_address("51.38.81.49", ec), 80);
    asio::ip::tcp::socket socket(context);
    socket.connect(endpoint, ec);
    if (!ec) {
        std::cout << "Connected\n";
    } else {
        std::cout << "Failed to connect to address:\n" << ec.message() << std::endl;
    }
    if (socket.is_open()) {
        std::string sRequest =
        "GET /index.html HTTP/1.1\r\n"
        "Host: example.com\r\n"
        "Connection: close\r\n\r\n";
        socket.write_some(asio::buffer(sRequest.data(), sRequest.size()), ec);
        socket.wait(socket.wait_read);

        size_t bytes =  socket.available();
        std::cout << "Bytes Available: " << bytes << std::endl;
        if (bytes > 0) {
            std::vector<char> vBuffer(bytes);
            socket.read_some(asio::buffer(vBuffer.data(), vBuffer.size()), ec);
            for (auto c : vBuffer) {
                std::cout << c;
            }
        }
    }
}