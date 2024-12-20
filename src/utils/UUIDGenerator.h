#ifndef UUID_GENERATOR_H
#define UUID_GENERATOR_H

#include <string>
#include <random>
#include <sstream>
#include <iomanip>

class UUIDGenerator {
public:
    static std::string generateUUID(){
        static std::random_device rd;
        static std::mt19937 gen(rd());
        static std::uniform_int_distribution<uint32_t> dist(0, 0xFFFFFFFF);

        std::stringstream ss;
        for (int i = 0; i < 4; ++i) {
            ss << std::hex << std::setw(8) << std::setfill('0') << dist(gen);
            if (i < 3) ss << "-";
        }
        return ss.str();
    }
};

#endif
