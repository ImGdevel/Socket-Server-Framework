#ifndef TRANSPORT_PROTOCOL_H
#define TRANSPORT_PROTOCOL_H

#include <string>
#include <stdexcept>

enum class TransportProtocol {
    TCP,
    UDP
};

inline TransportProtocol fromString(const std::string& protocol) {
    if (protocol == "TCP" || protocol == "tcp") {
        return TransportProtocol::TCP;
    }
    if (protocol == "UDP" || protocol == "udp") {
        return TransportProtocol::UDP;
    }
    throw std::invalid_argument("Unsupported transport protocol: " + protocol);
}

inline std::string toString(TransportProtocol protocol) {
    switch (protocol) {
        case TransportProtocol::TCP: return "TCP";
        case TransportProtocol::UDP: return "UDP";
        default: throw std::invalid_argument("Unknown transport protocol");
    }
}

#endif // TRANSPORT_PROTOCOL_H
