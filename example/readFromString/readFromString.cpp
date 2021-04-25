#include "..\..\include\json\json.h"
#include <iostream>
#include <mr/log_resource.hpp>
#include <memory_resource>
#include <string>
/**
 * \brief Parse a raw string into Value object using the CharReaderBuilder
 * class, or the legacy Reader class.
 * Example Usage: 
 * $g++ readFromString.cpp -ljsoncpp -std=c++11 -o readFromString
 * $g++ readFromString.cpp -ljsoncpp-24 -std=c++17 -o readFromString
 * $./readFromString
 * colin
 * 20
 */
int main() {
  const std::string rawJson = R"({"Age": 20, "Name": "colin", "Obj": { "f": 3 }})";
  const auto rawJsonLength = static_cast<int>(rawJson.length());
  JSONCPP_STRING err;
  std::array<std::byte, 32000> buffer{};
  Json::CharReaderBuilder builder;
  std::pmr::monotonic_buffer_resource mr{buffer.data(), buffer.size()};
  log_resource lr{&mr};
  Json::Value root{&lr};
  const std::unique_ptr<Json::CharReader> reader(builder.newCharReader(&lr));
  if (!reader->parse(rawJson.c_str(), rawJson.c_str() + rawJsonLength, &root,
                      &err)) {
    std::cout << "error" << std::endl;
    return EXIT_FAILURE;
  }

  const std::string name = root["Name"].asString();
  const int age = root["Age"].asInt();

  std::cout << sizeof(Json::Value) << std::endl;
  std::cout << age << std::endl;
  return EXIT_SUCCESS;
}
