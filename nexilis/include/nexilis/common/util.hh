#ifndef NEXILIS_COMMON_UTIL_HH
#define NEXILIS_COMMON_UTIL_HH

#include <nexilis/config.hh>
#include <nexilis/logger/log_level.hh>
#include <nexilis/vector2.hh>

#include <boost/json/object.hpp>

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

    /// Convert std::vector<uint8_t> to Vec2f.
    static Vector2f convertToVector2(const std::vector<uint8_t>& bytes);

    /// Return uint16_t from two bytes.
    static uint16_t uint8PairToUint16(uint8_t lowByte, uint8_t highByte);

    /// Separate two bytes.
    /// \param value The two bytes that are separate.
    /// \param lowByte The created low byte.
    /// \param hightByte The created high byte.
    static void uint16ToUint8Pair(uint16_t value, uint8_t& lowByte, uint8_t& highByte);

    /// \defgroup FrontConversions Functions that convert the first bytes of std::vector<uint8_t> into type.

    /// Get the first eight bytes of vector and return it as uint64_t.
    /// \ingroup FrontConversions
    static uint64_t uint64FromFront(const std::vector<uint8_t>& vec);

    /// Get the first four bytes from vector and return it as float.
    static float floatFromFront(const std::vector<uint8_t>& vec);

    /// Remove amount of bytes from the beginning of the vector.
    /// \return The updated vector.
    static std::vector<uint8_t> removeAmountOfBytesFromVector(std::vector<uint8_t> original, uint8_t amount);

    /// \defgroup RandFunctions Functions that generate random values.

    /// Get random uint64_t value between 0 and max uint64.
    /// \ingroup RandFunctions
    static uint64_t getRandomUint64();

    /// Get random integer between values.
    /// \param from The smallest possible value.
    /// \param to The biggest possible value.
    /// \return The random integer.
    /// \ingroup RandFunctions
    static int getRandomInt(int from, int to);

    /// Get random characters from 'A' to 'Z'.
    /// \param charAmount The amount of characters in the string.
    /// \ingroup RandFunctions
    static std::string getRandomString(uint64_t charAmount);

    /// \defgroup BytevectorConversions Functions that converts items to byte vectors.

    /// Byte vector conversions.
    /// \ingroup BytevectorConversions
    static std::vector<uint8_t> convertToByteVector(const char* command_data, uint64_t length);

    /// \ingroup BytevectorConversions
    static std::vector<uint8_t> convertToByteVector(uint64_t value);

    /// \ingroup BytevectorConversions
    static std::vector<uint8_t> convertToByteVector(const boost::json::object& obj);

    /// \ingroup BytevectorConversions
    static std::vector<uint8_t> convertToByteVector(float value);

    /// \ingroup BytevectorConversions
    static std::vector<uint8_t> convertToByteVector(Vector2f value);

    /// Logging.
    static std::string getColorMessage(logger::LogLevel logLevel, const std::string& data);
    static void printColorMessageToConsole(logger::LogLevel logLevel, const std::string& data);
    static void debugUint8Vector(const std::vector<uint8_t>& vector);

    /// Other
    static std::string getDateAndTime();
};

} // namespace nexilis

#endif
