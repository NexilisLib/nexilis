#include <gtest/gtest.h>

#include <nexilis/json.hh>

#include <boost/json/parse.hpp>

using namespace nexilis;

TEST(JsonTest, CreateJSONFromMap)
{
    std::map<std::string, boost::json::value> kv;
    kv["name"] = "test";
    kv["count"] = 42;

    auto obj = Json::createJSON(kv);

    EXPECT_EQ(obj.at("name").as_string(), "test");
    EXPECT_EQ(obj.at("count").as_int64(), 42);
}

TEST(JsonTest, CreateJSONEmpty)
{
    std::map<std::string, boost::json::value> kv;
    auto obj = Json::createJSON(kv);
    EXPECT_TRUE(obj.empty());
}

TEST(JsonTest, ConvertToJSONFromBytes)
{
    std::string jsonStr = R"({"key":"value","num":10})";
    nx_data bytes(jsonStr.begin(), jsonStr.end());

    auto obj = Json::convertToJSON(bytes);

    EXPECT_EQ(obj.at("key").as_string(), "value");
    EXPECT_EQ(obj.at("num").as_int64(), 10);
}

TEST(JsonTest, ToString)
{
    boost::json::object obj;
    obj["hello"] = "world";
    obj["number"] = 5;

    std::string result = Json::toString(obj);

    EXPECT_NE(result.find("hello"), std::string::npos);
    EXPECT_NE(result.find("world"), std::string::npos);
    EXPECT_NE(result.find("5"), std::string::npos);
}

TEST(JsonTest, EmplaceMergesObjects)
{
    boost::json::object first;
    first["a"] = 1;

    boost::json::object second;
    second["b"] = 2;

    Json::emplace(first, second);

    EXPECT_EQ(first.at("a").as_int64(), 1);
    EXPECT_EQ(first.at("b").as_int64(), 2);
}

TEST(JsonTest, EmplaceOverlappingKeysKeepsFirst)
{
    boost::json::object first;
    first["key"] = "original";

    boost::json::object second;
    second["key"] = "new";

    Json::emplace(first, second);

    EXPECT_EQ(first.at("key").as_string(), "original");
}

TEST(JsonTest, SaveAndReadJSONFile)
{
    boost::json::object obj;
    obj["test"] = "roundtrip";
    obj["val"] = 99;

    std::string path = std::tmpnam(nullptr);
    path += ".json";

    Json::saveToFile(obj, path);

    auto value = Json::readJSONFromFile(path);
    auto readObj = value.as_object();

    EXPECT_EQ(readObj.at("test").as_string(), "roundtrip");
    EXPECT_EQ(readObj.at("val").as_int64(), 99);

    std::remove(path.c_str());
}

TEST(JsonTest, ReadJSONFromFileNonexistent)
{
    auto value = Json::readJSONFromFile("/tmp/nonexistent_nexilis_test_file_12345.json");
    EXPECT_TRUE(value.is_null());
}
