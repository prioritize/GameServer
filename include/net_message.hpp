#pragma once
#include <vector>
#include <memory>
#include "net_common.hpp"

namespace net {
template <typename T>
struct message_header {
    T id{};
    uint32_t size = 0;
};
template <typename T>
struct message {
    message_header<T> header{};
    std::vector<uint8_t> body;
    
    size_t size() const {
        return sizeof(message_header<T>) + body.size();
    }

    friend std::ostream& operator << (std::ostream& os, const message<T>& msg) {
        os << "ID:" << int(msg.header.id) << " Size:" << msg.header.size;
        return os;
    }

    // This is a function using the bitshift left operator that returns a reference to a message of type T
    // It accepts as arguments a reference to a message of type T and a reference to an object of DataType
    template <typename DataType>
    friend message<T>& operator << (message<T>&  msg, const DataType& data) {
        // Check that the type of data that is being passed in is one of the standard data types
        static_assert(std::is_standard_layout<DataType>::value, "Data is too complex to use");

        // Store the size of the current message
        size_t i = msg.body.size();

        // Resize the msg.body vector  to include the new data
        msg.body.resize(msg.body.size() + sizeof(DataType));

        // Copy the data into the newly allocated vector space
        std::memcpy(msg.body.data() + i, &data, sizeof(DataType));

        // Recalculate the size of the message
        msg.header.size = msg.size();

        // Return the target message so that it can be chained
        return msg;
    }
    template <typename DataType>
    friend message<T>& operator >> (message<T>& msg, DataType& data) {
        static_assert(std::is_standard_layout<DataType>::value, "Data is too complex to use");

        // Calculate the future size of msg.body
        size_t i = msg.body.size() - sizeof(DataType);

        // Copy something the size of DataType from msg.body to data
        std::memcpy(&data, msg.body.data()+i, sizeof(DataType));

        // Shrink the vector to remove read bytes and reset end position
        msg.body.resize(i);

        // Update the header size
        msg.header.size = msg.size();

        // Return the target message so it can be chained
        return msg;
    }
};

template <typename T>
class connection;

template <typename T>
struct owned_message {
    std::shared_ptr<connection<T>> remote = nullptr;
    message<T> msg;
    friend std::ostream& operator <<(std::ostream& os, const owned_message<T>& msg) {
        os << msg.msg;
        return os;
    }
};
}
