#include <nexilis/common/util.hh>
#include <nexilis/nexilis_macros.hh>

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

uint16_t Util::uint8PairToUint16(uint8_t lowByte, uint8_t highByte)
{
    return static_cast<uint16_t>(static_cast<uint16_t>(lowByte) | (static_cast<uint16_t>(highByte) << 8));
}

void Util::uint16ToUint8Pair(uint16_t value, uint8_t& lowByte, uint8_t& highByte)
{
    lowByte = static_cast<uint8_t>(value & 0xFF);
    highByte = static_cast<uint8_t>((value >> 8) & 0xFF);
}

size_t Util::getRandomSizeT(size_t from, size_t to)
{
    std::random_device rand_dev;
    std::mt19937_64 generator(rand_dev());
    std::uniform_int_distribution<size_t> dist(from, to);
    return dist(generator);
}

size_t Util::getRandomSizeT()
{
    std::random_device rand_dev;
    std::mt19937_64 generator(rand_dev());
    std::uniform_int_distribution<size_t> dist(0, NEXILIS_MAX);
    return dist(generator);
}

std::vector<uint8_t> Util::removeAmountOfBytesFromVector(std::vector<uint8_t> original, uint8_t amount)
{
    // Return empty vector if the original vector has less elements than we want to remove.
    if (original.size() < amount)
    {
        std::cerr << "Cannot remove more bytes than existing command has.";
        return {};
    }

    return std::vector<uint8_t> (original.begin() + amount, original.end());
}

std::vector<uint8_t> Util::convertToByteVector(const char* command_data, size_t lenght)
{
    std::vector<uint8_t> result;
    result.reserve(lenght);

    for (size_t i = 0; i < lenght; i++)
    {
        result.emplace_back(static_cast<uint8_t>(command_data[i]));
    }
    return result;
}

std::vector<uint8_t> Util::convertToByteVector(size_t value)
{
    std::vector<uint8_t> result(sizeof(size_t));

    if (Config::getBigEndian())
    {
        for (size_t i = 0; i < sizeof(size_t); ++i)
        {
            result[sizeof(size_t) - 1 - i] = static_cast<uint8_t>((value >> (8 * i)) & 0xFF);
        }
    }
    else
    {
        for (size_t i = 0; i < sizeof(size_t); ++i)
        {
            result[i] = static_cast<uint8_t>((value >> (8 * i)) & 0xFF);
        }
    }
    return result;
}

void Util::sendColorMessageToConsole(logger::LogLevel logLevel, const std::string& data)
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

    std::cout << color << data << "\033[0m" << std::endl;
}

}
