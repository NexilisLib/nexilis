#include <nexilis/common/util.hh>
#include <nexilis/nexilis_macros.hh>
#include <nexilis/log.hh>

#include <boost/json/serialize.hpp>

#include <bitset>
#include <iomanip>
#include <random>
#include <sstream>

namespace nexilis
{

std::string Util::convertToString(std::vector<uint8_t> bytes)
{
    std::string result;
    for (uint8_t b : bytes)
    {
        result += static_cast<char>(b);
    }
    return result;
}

Vector2f Util::convertToVector2(const std::vector<uint8_t>& bytes)
{
    // Ensure the vector has enough bytes for two floats
    if (bytes.size() < sizeof(float) * 2)
    {
        throw std::invalid_argument("The input vector does not contain enough bytes for two floats.");
    }

    // Variables to hold the float values
    float float1, float2;

    // Copy the first 4 bytes.
    memcpy(&float1, bytes.data(), sizeof(float));
    // Copy the next 4 bytes.
    memcpy(&float2, bytes.data() + sizeof(float), sizeof(float));

    return Vector2f(float1, float2);
}

uint16_t Util::uint8PairToUint16(uint8_t lowByte, uint8_t highByte)
{
    return static_cast<uint16_t>(static_cast<uint16_t>(lowByte) | (static_cast<uint16_t>(highByte) << 8));
}

void Util::uint16ToUint8Pair(uint16_t value, uint8_t& lowByte, uint8_t& highByte)
{
    lowByte = static_cast<uint8_t>(value & 0xFF);
    highByte = static_cast<uint8_t>((value >> 8) & 0xFF);
}

std::vector<uint8_t> Util::convertToByteVector(uint64_t value)
{
    std::vector<uint8_t> result(sizeof(uint64_t));

    if (Config::getBigEndian())
    {
        for (uint64_t i = 0; i < sizeof(uint64_t); ++i)
        {
            result[sizeof(uint64_t) - 1 - i] = static_cast<uint8_t>((value >> (8 * i)) & 0xFF);
        }
    }
    else
    {
        for (uint64_t i = 0; i < sizeof(uint64_t); ++i)
        {
            result[i] = static_cast<uint8_t>((value >> (8 * i)) & 0xFF);
        }
    }
    return result;
}

std::vector<uint8_t> Util::convertToByteVector(float value)
{
    std::vector<uint8_t> bytes(sizeof(float));
    memcpy(bytes.data(), &value, sizeof(float));
    return bytes;
}

std::vector<uint8_t> Util::convertToByteVector(Vector2f value)
{
    auto vec1 = convertToByteVector(value.x);
    auto vec2 = convertToByteVector(value.y);
    vec1.insert(vec1.end(), vec2.begin(), vec2.end());
    return vec1;
}

uint64_t Util::getRandomUint64()
{
    std::random_device rand_dev;
    std::mt19937_64 generator(rand_dev());
    std::uniform_int_distribution<uint64_t> dist(0, NEXILIS_MAX);

    uint64_t randomValue;
    do
    {
        randomValue = dist(generator);
    }
    while (std::bitset<64>(randomValue).count() < 32); // Ensure at least 32 bits are set

    assert((std::is_same<decltype(randomValue), uint64_t>::value));

    return randomValue;
}

int Util::getRandomInt(int from, int to)
{
    std::random_device rand_dev;
    std::mt19937_64 generator(rand_dev());
    std::uniform_int_distribution<uint64_t> dist(from, to);
    int randomValue = dist(generator);

    assert(randomValue >= from);
    assert(randomValue <= to);
    return randomValue;
}

std::string Util::getRandomString(uint64_t charAmount)
{
    std::random_device rand_dev;
    std::mt19937 eng(rand_dev());
    std::uniform_int_distribution<int> distribution('A', 'Z');

    std::string randomString;
    for (uint64_t i = 0; i < charAmount; ++i)
    {
        randomString += static_cast<char>(distribution(eng));
    }
    return randomString;
}

std::vector<uint8_t> Util::removeAmountOfBytesFromVector(std::vector<uint8_t> original, uint8_t amount)
{
    // Return empty vector if the original vector has less elements than we want to remove.
    if (original.size() < amount)
    {
        std::cerr << "Cannot remove more bytes than existing command has.";
        return {};
    }

    return std::vector<uint8_t>(original.begin() + amount, original.end());
}

std::vector<uint8_t> Util::convertToByteVector(const char* command_data, uint64_t length)
{
    std::vector<uint8_t> result;
    result.reserve(length);

    for (size_t i = 0; i < length; i++)
    {
        result.emplace_back(static_cast<uint8_t>(command_data[i]));
    }
    return result;
}

std::vector<uint8_t> Util::convertToByteVector(const boost::json::object& obj)
{
    std::string jsonString = boost::json::serialize(obj);
    std::vector<uint8_t> byteStream(jsonString.begin(), jsonString.end());
    return byteStream;
}

std::string Util::getColorMessage(logger::LogLevel logLevel, const std::string &data)
{
    std::string color;
    switch (logLevel)
    {
        case logger::LogLevel::DEBUG:
        case logger::LogLevel::INFO:
            color = "\033[37m";
            break;
        case logger::LogLevel::WARNING:
            color = "\033[33m";
            break;
        case logger::LogLevel::ERROR:
        case logger::LogLevel::CRITICAL:
            color = "\033[31m";
            break;
    }
    std::stringstream ss;
    ss << color << data << "\033[0m" << std::endl;
    return ss.str();
}

void Util::printColorMessageToConsole(logger::LogLevel logLevel, const std::string& data)
{
    std::cout << getColorMessage(logLevel, data);
}

void Util::debugUint8Vector(const std::vector<uint8_t>& vector)
{
    std::stringstream ss;
    for (uint8_t byte : vector)
    {
        ss << std::hex << static_cast<int>(byte) << " " << static_cast<char>(byte) << "\t";
    }
    ss << std::dec << std::endl;
    Log::debug(ss.str());
}

uint64_t Util::uint64FromFront(const std::vector<uint8_t>& vec)
{
    if (vec.size() < 8)
    {
        // Not enough data to read 8 bytes
        Log::error("Not enough data to read 8 bytes");
        return 0;
    }

    uint64_t value = 0;

    if (Config::getBigEndian())
    {
        // Big-endian: Most significant byte is at the lowest address
        for (int i = 0; i < 8; ++i)
        {
            value |= static_cast<uint64_t>(vec[i]) << ((7 - i) * 8);
        }
    }
    else
    {
        // Little-endian: Least significant byte is at the lowest address
        for (int i = 0; i < 8; ++i)
        {
            value |= static_cast<uint64_t>(vec[i]) << (i * 8);
        }
    }
    return value;
}

float Util::floatFromFront(const std::vector<uint8_t>& vec)
{
    if (vec.size() < 4)
    {
        Log::error("Not enough data to read 4 bytes");
        return 0.f;
    }

    float result;
    std::memcpy(&result, vec.data(), sizeof(float));
    return result;
}

std::string Util::getDateAndTime()
{
    // Get the current time.
    auto now = std::chrono::system_clock::now();

    // Convert to time_t.
    std::time_t currentTime = std::chrono::system_clock::to_time_t(now);

    // Convert to local time struct.
    std::tm* localTime = std::localtime(&currentTime);

    // Format the time.
    std::stringstream ss;
    ss << std::put_time(localTime, "%Y-%m-%d_%H:%M:%S");
    return ss.str();
}


} // namespace nexilis
