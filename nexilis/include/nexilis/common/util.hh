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
    /// Convert std::vector<uint8_t> to numeral type.
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
            for (uint64_t i = 0; i < sizeof(T); ++i)
            {
                result |= static_cast<T>(bytes[i]) << (8 * (sizeof(T) - 1 - i));
            }
        }
        else
        {
            for (uint64_t i = 0; i < sizeof(T); ++i)
            {
                result |= static_cast<T>(bytes[i]) << (8 * i);
            }
        }

        return result;
    }

    /// Convert std::vector<uint8_t> to string.
    static std::string convertToString(std::vector<uint8_t> bytes);

    /// Return uint16_t from two bytes.
    static uint16_t uint8PairToUint16(uint8_t lowByte, uint8_t highByte);

    /// Separate two bytes.
    /// \param value The two bytes that are separate.
    /// \param lowByte The created low byte.
    /// \param hightByte The created high byte.
    static void uint16ToUint8Pair(uint16_t value, uint8_t& lowByte, uint8_t& highByte);

    /// Design issue function.
    static std::vector<uint8_t> removeAmountOfBytesFromVector(std::vector<uint8_t> original, uint8_t amount);

    /// Get random uint64_t between two values.
    static size_t getRandomSizeUint64(uint64_t from, uint64_t to);

    /// Get random size_t value between 0 and max uint64.
    static size_t getRandomUint64();

    /// Get random characters from 'A' to 'Z'.
    /// \param charAmount The amount of characters in the string.
    static std::string getRandomString(uint64_t charAmount);

    /// Byte vector conversions.
    static std::vector<uint8_t> convertToByteVector(const char* command_data, uint64_t lenght);
    static std::vector<uint8_t> convertToByteVector(uint64_t value);
    static std::vector<uint8_t> convertToByteVector(const boost::json::object& obj);

    /// Logging.
    static void sendColorMessageToConsole(logger::LogLevel logLevel, const std::string& data);
    static void debugUint8Vector(const std::vector<uint8_t>& vector);

    static std::string getDateAndTime();
};

} // namespace nexilis

#endif
