#include <nexilis/common/util.hh>
#include <nexilis/nexilis_macros.hh>
#include <nexilis/log.hh>

#include <boost/json/serialize.hpp>

#include <iomanip>
#include <random>

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

Vec2f Util::convertToVector2(const std::vector<uint8_t>& bytes)
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

    return Vec2f(float1, float2);
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

std::vector<uint8_t> Util::convertToByteVector(Vec2f value)
{
    auto vec1 = convertToByteVector(value.x);
    auto vec2 = convertToByteVector(value.y);
    vec1.insert(vec1.end(), vec2.begin(), vec2.end());
    return vec1;
}

uint64_t Util::getRandomSizeUint64(uint64_t from, uint64_t to)
{
    std::random_device rand_dev;
    std::mt19937_64 generator(rand_dev());
    std::uniform_int_distribution<size_t> dist(from, to);
    uint64_t randomValue = dist(generator);

    if (randomValue != 0)
    {
        return randomValue;
    }
    else
    {
        return getRandomSizeUint64(from, to);
    }
}

uint64_t Util::getRandomUint64()
{
    std::random_device rand_dev;
    std::mt19937_64 generator(rand_dev());
    std::uniform_int_distribution<uint64_t> dist(0, NEXILIS_MAX);
    uint64_t randomValue = dist(generator);

    assert(typeid(randomValue) == typeid(uint64_t));

    if (randomValue != 0)
    {
        return randomValue;
    }
    else
    {
        return getRandomUint64();
    }
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
    for (uint8_t byte : vector)
    {
        Log::debug("Commandbyte hex: ", std::hex, static_cast<int>(byte));
        Log::debug("Commandbyte char: ", static_cast<char>(byte));
    }
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

uint64_t Util::extractUint64FromVector(const std::vector<uint8_t>& data)
{
    uint64_t result = 0;

    if (Config::getBigEndian())
    {
        for (auto byte : data)
        {
            if (byte == 0xFF)
            {
                break;
            }
            result = (result << 8) | byte;
        }
    }
    else
    {
        for (uint64_t i = 0; i < data.size(); ++i)
        {
            if (data[i] == 0xFF)
            {
                break;
            }
            result |= static_cast<uint64_t>(data[i]) << (i * 8);
        }
    }

    return result;
}

std::vector<uint8_t> Util::removeItemsUntilFF(const std::vector<uint8_t>& data)
{
    auto ffPosition = std::find(data.begin(), data.end(), 0xFF);

    if (ffPosition != data.end())
    {
        return std::vector<uint8_t>(ffPosition + 1, data.end());
    }
    else
    {
        return {};
    }
}

bool Util::containsFF(const std::vector<uint8_t>& data)
{
    for (auto byte : data)
    {
        if (byte == 0xFF)
        {
            return true;
        }
    }
    return false;
}

uint64_t Util::getMessageIdFromNexilisMessage(const std::vector<uint8_t>& data)
{
    auto a = removeItemsUntilFF(data);
    return extractUint64FromVector(a);
}

} // namespace nexilis
