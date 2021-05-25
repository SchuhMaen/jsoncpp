#include <benchmark/benchmark.h>
#include "json/json.h"
#include <fstream>
#include <iostream>
#include <filesystem>
#include <string>
#include <memory_resource>
#include <mr/log_resource.hpp>

static void BM_Json(benchmark::State& state) 
{
  Json::CharReaderBuilder builder;
  JSONCPP_STRING errs;
  std::ifstream ifs;

  const std::string full_file_name = "example.json";
  ifs.open(full_file_name);

  if(!ifs.is_open())
      state.SkipWithError(("failed to open: " + full_file_name).c_str());

  std::array<std::byte, 32768> buffer_value;
  std::array<std::byte, 32768> buffer_parser;
  std::pmr::monotonic_buffer_resource r_value{buffer_value.data(), buffer_value.size()};
  std::pmr::monotonic_buffer_resource r_parser{buffer_parser.data(), buffer_parser.size()};

  Json::OStringStream ssin;
  ssin << ifs.rdbuf();
  Json::String doc = ssin.str();
  char const* begin = doc.data();
  char const* end = begin + doc.size();

  std::unique_ptr<Json::CharReader> const reader(builder.newCharReader(&r_parser));

  for (auto _ : state)
  {
    r_value.release();
    Json::Value root{&r_value};
    if (!reader->parse(begin, end, &root, &errs)) {
      std::cerr << errs << std::endl;
    }
  }
}
// Register the function as a benchmark
BENCHMARK(BM_Json);

static void BM_Depth(benchmark::State& state, const std::string& json_file) 
{
  Json::CharReaderBuilder builder;
  JSONCPP_STRING errs;
  std::ifstream ifs;

  const std::string full_file_name = json_file + std::to_string(state.range(0)) + ".json";
  ifs.open(full_file_name);

  if(!ifs.is_open())
      state.SkipWithError(("failed to open: " + full_file_name).c_str());

  std::array<std::byte, 2048> buffer_value;
  std::array<std::byte, 2048> buffer_parser;
  std::pmr::monotonic_buffer_resource r_value{buffer_value.data(), buffer_value.size()};
  std::pmr::monotonic_buffer_resource r_parser{buffer_parser.data(), buffer_parser.size()};

  Json::OStringStream ssin;
  ssin << ifs.rdbuf();
  Json::String doc = ssin.str();
  char const* begin = doc.data();
  char const* end = begin + doc.size();

  std::unique_ptr<Json::CharReader> const reader(builder.newCharReader(&r_parser));

  for (auto _ : state)
  {
    r_value.release();
    Json::Value root{&r_value};
    if (!reader->parse(begin, end, &root, &errs)) {
      std::cerr << errs << std::endl;
    }
  }
}
// Register the function as a benchmark
BENCHMARK_CAPTURE(BM_Depth, json.test, std::string("jsongen_depth_"))->DenseRange(0, 1000, 100);

static void BM_Member(benchmark::State& state, const std::string& json_file) 
{
  Json::CharReaderBuilder builder;
  JSONCPP_STRING errs;
  std::ifstream ifs;

  const std::string full_file_name = json_file + std::to_string(state.range(0)) + ".json";
  ifs.open(full_file_name);

  if(!ifs.is_open())
      state.SkipWithError(("failed to open: " + full_file_name).c_str());

  std::array<std::byte, 2048> buffer_value;
  std::array<std::byte, 2048> buffer_parser;
  std::pmr::monotonic_buffer_resource r_value{buffer_value.data(), buffer_value.size()};
  std::pmr::monotonic_buffer_resource r_parser{buffer_parser.data(), buffer_parser.size()};

  Json::OStringStream ssin;
  ssin << ifs.rdbuf();
  Json::String doc = ssin.str();
  char const* begin = doc.data();
  char const* end = begin + doc.size();

  std::unique_ptr<Json::CharReader> const reader(builder.newCharReader(&r_parser));

  for (auto _ : state)
  {
    r_value.release();
    Json::Value root{&r_value};
    if (!reader->parse(begin, end, &root, &errs)) {
      std::cerr << errs << std::endl;
    }
  }
}
// Register the function as a benchmark
BENCHMARK_CAPTURE(BM_Member, json.test, std::string("jsongen_member_"))->DenseRange(0, 1000, 100);

static void BM_String(benchmark::State& state, const std::string& json_file) 
{
  Json::CharReaderBuilder builder;
  JSONCPP_STRING errs;
  std::ifstream ifs;

  const std::string full_file_name = json_file + std::to_string(state.range(0)) + ".json";
  ifs.open(full_file_name);

  if(!ifs.is_open())
      state.SkipWithError(("failed to open: " + full_file_name).c_str());

  std::array<std::byte, 2048> buffer_value;
  std::array<std::byte, 2048> buffer_parser;
  std::pmr::monotonic_buffer_resource r_value{buffer_value.data(), buffer_value.size()};
  std::pmr::monotonic_buffer_resource r_parser{buffer_parser.data(), buffer_parser.size()};

  Json::OStringStream ssin;
  ssin << ifs.rdbuf();
  Json::String doc = ssin.str();
  char const* begin = doc.data();
  char const* end = begin + doc.size();

  std::unique_ptr<Json::CharReader> const reader(builder.newCharReader(&r_parser));

  for (auto _ : state)
  {
    r_value.release();
    Json::Value root{&r_value};
    if (!reader->parse(begin, end, &root, &errs)) {
      std::cerr << errs << std::endl;
    }
  }
}

BENCHMARK_CAPTURE(BM_String, json.test, std::string("jsongen_str_"))->DenseRange(0, 2000, 200);

static void BM_Object(benchmark::State& state) 
{
  Json::CharReaderBuilder builder;
  JSONCPP_STRING errs;

  const std::string rawJson = R"({})";
  const auto rawJsonLength = static_cast<int>(rawJson.length());

  std::array<std::byte, 1024> buffer_value;
  std::array<std::byte, 1024> buffer_parser;
  std::pmr::monotonic_buffer_resource r_value{buffer_value.data(), buffer_value.size()};
  std::pmr::monotonic_buffer_resource r_parser{buffer_parser.data(), buffer_parser.size()};
  
  const std::unique_ptr<Json::CharReader> reader(builder.newCharReader(&r_parser));

  for (auto _ : state)
  {
    r_value.release();
    Json::Value root{&r_value};
    if (!reader->parse(rawJson.c_str(), rawJson.c_str() + rawJsonLength, &root, &errs)) {
      std::cerr << errs << std::endl;
    }
  }
}
// Register the function as a benchmark
BENCHMARK(BM_Object);

static void BM_Array(benchmark::State& state) 
{
  Json::CharReaderBuilder builder;
  JSONCPP_STRING errs;

  const std::string rawJson = R"([])";
  const auto rawJsonLength = static_cast<int>(rawJson.length());

  std::array<std::byte, 1024> buffer_value;
  std::array<std::byte, 1024> buffer_parser;
  std::pmr::monotonic_buffer_resource r_value{buffer_value.data(), buffer_value.size()};
  std::pmr::monotonic_buffer_resource r_parser{buffer_parser.data(), buffer_parser.size()};
  
  const std::unique_ptr<Json::CharReader> reader(builder.newCharReader(&r_parser));

  for (auto _ : state)
  {
    r_value.release();
    Json::Value root{&r_value};
    if (!reader->parse(rawJson.c_str(), rawJson.c_str() + rawJsonLength, &root, &errs)) {
      std::cerr << errs << std::endl;
    }
  }
}
// Register the function as a benchmark
BENCHMARK(BM_Array);

static void BM_Integer(benchmark::State& state) 
{
  Json::CharReaderBuilder builder;
  JSONCPP_STRING errs;

  const std::string rawJson = R"({"int": 20})";
  const auto rawJsonLength = static_cast<int>(rawJson.length());

  std::array<std::byte, 1024> buffer_value;
  std::array<std::byte, 1024> buffer_parser;
  std::pmr::monotonic_buffer_resource r_value{buffer_value.data(), buffer_value.size()};
  std::pmr::monotonic_buffer_resource r_parser{buffer_parser.data(), buffer_parser.size()};
  
  const std::unique_ptr<Json::CharReader> reader(builder.newCharReader(&r_parser));

  for (auto _ : state)
  {
    r_value.release();
    Json::Value root{&r_value};
    if (!reader->parse(rawJson.c_str(), rawJson.c_str() + rawJsonLength, &root, &errs)) {
      std::cerr << errs << std::endl;
    }
  }
}
// Register the function as a benchmark
BENCHMARK(BM_Integer);

static void BM_Real(benchmark::State& state) 
{
  Json::CharReaderBuilder builder;
  JSONCPP_STRING errs;

  const std::string rawJson = R"({"real": 20.123456789})";
  const auto rawJsonLength = static_cast<int>(rawJson.length());

  std::array<std::byte, 1024> buffer_value;
  std::array<std::byte, 1024> buffer_parser;
  std::pmr::monotonic_buffer_resource r_value{buffer_value.data(), buffer_value.size()};
  std::pmr::monotonic_buffer_resource r_parser{buffer_parser.data(), buffer_parser.size()};
  
  const std::unique_ptr<Json::CharReader> reader(builder.newCharReader(&r_parser));

  for (auto _ : state)
  {
    r_value.release();
    Json::Value root{&r_value};
    if (!reader->parse(rawJson.c_str(), rawJson.c_str() + rawJsonLength, &root, &errs)) {
      std::cerr << errs << std::endl;
    }
  }
}
// Register the function as a benchmark
BENCHMARK(BM_Real);

static void BM_Bool(benchmark::State& state) 
{
  Json::CharReaderBuilder builder;
  JSONCPP_STRING errs;

  const std::string rawJson = R"({"bool": true})";
  const auto rawJsonLength = static_cast<int>(rawJson.length());

  std::array<std::byte, 1024> buffer_value;
  std::array<std::byte, 1024> buffer_parser;
  std::pmr::monotonic_buffer_resource r_value{buffer_value.data(), buffer_value.size()};
  std::pmr::monotonic_buffer_resource r_parser{buffer_parser.data(), buffer_parser.size()};
  
  const std::unique_ptr<Json::CharReader> reader(builder.newCharReader(&r_parser));

  for (auto _ : state)
  {
    r_value.release();
    Json::Value root{&r_value};
    if (!reader->parse(rawJson.c_str(), rawJson.c_str() + rawJsonLength, &root, &errs)) {
      std::cerr << errs << std::endl;
    }
  }
}
// Register the function as a benchmark
BENCHMARK(BM_Bool);

BENCHMARK_MAIN();