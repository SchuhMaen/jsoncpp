#include "json/json.h"
#include <fstream>
#include <iostream>
#include <mr/log_resource.hpp>
#include <mr/config.hpp>
#include <mr/res_printer.hpp>
/** \brief Parse from stream, collect comments and capture error info.
 * Example Usage:
 * $g++ readFromStream.cpp -ljsoncpp -std=c++11 -o readFromStream
 * $./readFromStream
 * // comment head
 * {
 *    // comment before
 *    "key" : "value"
 * }
 * // comment after
 * // comment tail
 */
int main(int argc, char* argv[]) {

  const int width = 200;
  const int length = 1024;
  const bool p_num = false;
  constexpr int buffer_size = 1024;

  std::array<char, buffer_size> buffer{};
  std::array<char, buffer_size> buffer2{};
  std::array<char, buffer_size> buffer3{};

  buffer.fill('~');
  buffer2.fill('~');
  buffer3.fill('~');

  auto buffer_dump = [&]()
  {
      mr::dump(buffer.begin(), buffer.end(), length, p_num, width);
      mr::dump(buffer2.begin(), buffer2.end(), length, p_num, width);
      mr::dump(buffer3.begin(), buffer3.end(), length, p_num, width);
  };

  std::pmr::monotonic_buffer_resource mr{buffer.data(), buffer.size()};
  std::pmr::monotonic_buffer_resource mr2{buffer2.data(), buffer2.size()};
  std::pmr::monotonic_buffer_resource mr3{buffer3.data(), buffer3.size()};
  
  //Json::SecureResource sr{&mr};
  //Json::SecureResource sr2{&mr2};
  //Json::SecureResource sr3{&mr3};

  mr::log_resource::options opt{.log = false};

  mr::log_resource lr{opt,"me_res",&mr};
  mr::log_resource lr2{opt,"other_res",&mr2};
  mr::log_resource lr3{opt,"def_res",&mr3};

  std::pmr::set_default_resource(&lr3);

  Json::Value root{&lr};
  std::ifstream ifs;
  ifs.open(argv[1]);

  Json::CharReaderBuilder builder;
  JSONCPP_STRING errs;

  if (!parseFromStream(builder, ifs, &root, &errs, &lr2)) {
    std::cout << errs << std::endl;
    return EXIT_FAILURE;
  }
  
  lr.report();
  lr2.report();
  lr3.report();

  buffer_dump();

  return EXIT_SUCCESS;
}
