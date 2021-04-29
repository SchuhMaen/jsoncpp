#include "..\..\include\json\json.h"
#include <iostream>
#include <mr/log_resource.hpp>
#include <mr/config.hpp>
#include <mr/res_printer.hpp>
#include <memory_resource>
#include <string>

class ConversionTest
{
  public:
  //ConversionTest(std::pmr::memory_resource* f) {}
  ConversionTest(std::pmr::memory_resource* a) {}
  ConversionTest(bool f, std::pmr::polymorphic_allocator<std::byte> a = {}) {}

  
};
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
  mr::ls::set_log_stream(std::clog);
  //const std::string rawJson = R"({"Age": 20, "Name": "colin is to short so i will lengthen it.", "Obj": { "f": 3 }, "ar": [2,3]})";
  const std::string rawJson = R"({
    "short_string": "short",
    "long_string": "This is a pretty long string, but I don't want to type this out so I will just copy this. This is a pretty long string, but I don't want to type this out so I will just copy this. This is a pretty long string, but I don't want to type this out so I will just copy this. This is a pretty long string, but I don't want to type this out so I will just copy this. This is a pretty long string, but I don't want to type this out so I will just copy this.",
    "integdddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddder": 42,
    "float": 42.3,
    "array":
        [
            1,
            2,
            3,
            4,
            5
        ]
  })";
  const auto rawJsonLength = static_cast<int>(rawJson.length());

  JSONCPP_STRING err;
  Json::CharReaderBuilder builder;

  std::array<char, 100> buffer{};
  buffer.fill('~');
  std::array<char, 32000> buffer_parser{};
  buffer_parser.fill('~');
  std::array<char, 32000> buffer_def{};
  buffer_def.fill('~');
  
  std::pmr::monotonic_buffer_resource mr{buffer.data(), buffer.size()};
  std::pmr::monotonic_buffer_resource pr{buffer_parser.data(), buffer_parser.size()};
  std::pmr::monotonic_buffer_resource dr{buffer_def.data(), buffer_def.size()};

  mr::log_resource::options opt{.log = false};

  mr::log_resource lr{opt, "value_res", &mr};
  mr::log_resource lf{"parse_res", &pr};
  mr::log_resource ld{opt, "def_res", &dr};

  Json::Value root{&lr};
  
  const std::unique_ptr<Json::CharReader> reader(builder.newCharReader(&lf));
  std::pmr::set_default_resource(&ld);
  if (!reader->parse(rawJson.c_str(), rawJson.c_str() + rawJsonLength, &root,
                      &err)) {
    std::cout << err << std::endl;
    return EXIT_FAILURE;
  }

  /* const std::string short_string = root["short_string"].asString();
  std::cout << short_string << std::endl;
  const std::string long_string = root["long_string"].asString();
  std::cout << long_string << std::endl;
  const int integer = root["integer"].asInt();
  std::cout << integer << std::endl;
  const float floating = root["float"].asFloat();
  std::cout << floating << std::endl;
  const Json::Value& array = root["array"];
  std::cout << array[0] << std::endl;
  std::cout << array[1] << std::endl;
  std::cout << array[2] << std::endl;
  std::cout << array[3] << std::endl;*/
  //lr.report();
  //mr::dump(buffer.begin(), buffer.end(), 1000);
  //ld.report();
  //mr::dump(buffer_def.begin(), buffer_def.end(), 1000);
  mr.release();
  return EXIT_SUCCESS;
}
