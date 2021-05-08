#include <iostream>
#include "../include/net_message.hpp"

enum class CustomMsgTypes: uint32_t {
    FireBullet,
    MovePlayer
};

int main() {
    net::message<CustomMsgTypes> msg;
    msg.header.id = CustomMsgTypes::FireBullet;
    int a = 1;
    bool b = true;
    float c = 3.14159f;
    struct {
        float x;
        float y;
    } d[5];
    msg << a << b << c << d;
    a = 99;
    b = false;
    c = 99.0f;

    msg >> a >> b >> c >> d;
    std::cout << a << " " << b << " " << c << " " << d << std::endl;
    return 0;
}