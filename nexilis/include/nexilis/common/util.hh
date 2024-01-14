#ifndef NEXILIS_COMMON_UTIL_HH
#define NEXILIS_COMMON_UTIL_HH

#include <string>
#include <vector>
#include <cstdint>
#include <iostream>

namespace nexilis
{

class Util
{
public:
    static std::string convertToString(std::vector<uint8_t> bytes)
    {
        std::string result;
        for (uint8_t b : bytes)
        {
            result += static_cast<char>(b);
        }
        return result;
    }

    static uint16_t uint8PairToUint16(uint8_t lowByte, uint8_t highByte)
    {
        return static_cast<uint16_t>(static_cast<uint16_t>(lowByte) | (static_cast<uint16_t>(highByte) << 8));
    }

    static void uint16ToUint8Pair(uint16_t value, uint8_t& lowByte, uint8_t& highByte)
    {
        lowByte = static_cast<uint8_t>(value & 0xFF);
        highByte = static_cast<uint8_t>((value >> 8) & 0xFF);
    }

    static std::vector<uint8_t> removeAmountOfBytesFromVector(std::vector<uint8_t> original, uint8_t amount)
    {
        // Return empty vector if the original vector has less elements than we want to remove.
        if (original.size() < amount)
        {
            std::cerr << "Cannot remove more bytes than existing command has.";
            return {};
        }

        return std::vector<uint8_t> (original.begin() + amount, original.end());
    }

    template <typename T>
    static T convertToType(std::vector<uint8_t> bytes)
    {
        if (bytes.size() < sizeof(T))
        {
            // Handle error: insufficient data
            throw std::runtime_error("Conversion failed: Insufficient data");
        }

        T result = 0;

        // Assuming little-endian byte order
        for (size_t i = 0; i < sizeof(T); ++i)
        {
            result |= static_cast<T>(bytes[i]) << (8 * i);
        }

        return result;
    }

    static std::vector<uint8_t> convertToByteVector(const char* command_data, size_t lenght)
    {
        std::vector<uint8_t> result;
        result.reserve(lenght);

        for (size_t i = 0; i < lenght; i++)
        {
            result.emplace_back(static_cast<uint8_t>(command_data[i]));
        }
        return result;
    }
    static std::vector<uint8_t> convertToByteVector(size_t value)
    {
        std::vector<uint8_t> result;

        for (size_t i = 0; i < sizeof(size_t); ++i)
        {
            // Extract the i-th byte and push it into the vector
            uint8_t byte = static_cast<uint8_t>((value >> (8 * i)) & 0xFF);
            result.push_back(byte);
        }

        return result;
    }

};

}

#endif
