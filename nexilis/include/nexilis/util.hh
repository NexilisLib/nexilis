#ifndef NEXILIS_COMMON_UTIL_HH
#define NEXILIS_COMMON_UTIL_HH

#include <nexilis/server/config.hh>
#include <nexilis/types/vector2.hh>
#include <nexilis/logger/log_level.hh>
#include <nexilis/nexilis_macros.hh>

#include <boost/json/object.hpp>

#include <string>

namespace nexilis
{

class Util
{
public:
    /// Convert nx_data to numeral type.
    template <typename T>
    static T convertToType(nx_data bytes)
    {
        if (bytes.size() < sizeof(T))
        {
            // Handle error: insufficient data
            throw std::runtime_error("Conversion failed: Insufficient data");
        }

        T result = 0;

        if (server::Config::getBigEndian())
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

    /// Convert nx_data to string.
    static std::string convertToString(nx_data bytes);

    /// Convert nx_data to Vec2f.
    static Vector2f convertToVector2(const nx_data& bytes);

    /// Return uint16_t from two bytes.
    static uint16_t uint8PairToUint16(uint8_t lowByte, uint8_t highByte);

    /// Separate two bytes.
    /// \param value The two bytes that are separate.
    /// \param lowByte The created low byte.
    /// \param hightByte The created high byte.
    static void uint16ToUint8Pair(uint16_t value, uint8_t& lowByte, uint8_t& highByte);

    /// \defgroup FrontConversions Functions that convert the first bytes of nx_data into type.

    /// Get the first eight bytes of vector and return it as uint64_t.
    /// \ingroup FrontConversions
    static uint64_t uint64FromFront(const nx_data& vec);

    /// Get the first four bytes from vector and return it as float.
    static float floatFromFront(const nx_data& vec);

    /// Remove amount of bytes from the beginning of the vector.
    /// \return The updated vector.
    static nx_data removeAmountOfBytesFromVector(nx_data original, uint8_t amount);

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
    static nx_data convertToByteVector(const char* command_data, uint64_t length);

    /// \ingroup BytevectorConversions
    static nx_data convertToByteVector(uint64_t value);

    /// \ingroup BytevectorConversions
    static nx_data convertToByteVector(const boost::json::object& obj);

    /// \ingroup BytevectorConversions
    static nx_data convertToByteVector(float value);

    /// \ingroup BytevectorConversions
    static nx_data convertToByteVector(Vector2f value);
    
    /// \ingroup BytevectorConversions
    static nx_data convertToByteVector(const std::string& value);

    /// Logging.
    static std::string getColorMessage(logger::LogLevel logLevel, const std::string& data);
    static void printColorMessageToConsole(logger::LogLevel logLevel, const std::string& data);
    static void debugUint8Vector(const nx_data& vector);

    /// Other
    static std::string getDateAndTime();
};

} // namespace nexilis

#endif
