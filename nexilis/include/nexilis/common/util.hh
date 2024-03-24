#ifndef NEXILIS_COMMON_UTIL_HH
#define NEXILIS_COMMON_UTIL_HH

#include <nexilis/config.hh>
#include <nexilis/logger/log_level.hh>

#include <boost/json/object.hpp>

#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

namespace nexilis
{

class Util
{
public:
    template <typename T>
    static T convertToType(std::vector<uint8_t> bytes)
    {
        if (bytes.size() < sizeof(T))
        {
            // Handle error: insufficient data
            throw std::runtime_error("Conversion failed: Insufficient data");
        }

        T result = 0;

        if (Config::getBigEndian())
        {
            for (size_t i = 0; i < sizeof(T); ++i)
            {
                result |= static_cast<T>(bytes[i]) << (8 * (sizeof(T) - 1 - i));
            }
        }
        else
        {
            for (size_t i = 0; i < sizeof(T); ++i)
            {
                result |= static_cast<T>(bytes[i]) << (8 * i);
            }
        }

        return result;
    }

    static std::string convertToString(std::vector<uint8_t> bytes);

    static uint16_t uint8PairToUint16(uint8_t lowByte, uint8_t highByte);

    static void uint16ToUint8Pair(uint16_t value, uint8_t& lowByte, uint8_t& highByte);

    static std::vector<uint8_t> removeAmountOfBytesFromVector(std::vector<uint8_t> original, uint8_t amount);

    /// Get random size_t between two values.
    static size_t getRandomSizeT(size_t from, size_t to);

    /// Get random size_t value between 0 and max uint64.
    static size_t getRandomUint64();

    static std::vector<uint8_t> convertToByteVector(const char* command_data, size_t lenght);

    static std::vector<uint8_t> convertToByteVector(size_t value);

    static std::vector<uint8_t> convertToByteVector(const boost::json::object& obj);

    static void sendColorMessageToConsole(logger::LogLevel logLevel, const std::string& data);

    static std::string getDateAndTime();
};

} // namespace nexilis

#endif
