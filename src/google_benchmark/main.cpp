#include <benchmark/benchmark.h>
#include "json/json.h"
#include <fstream>
#include <iostream>
#include <filesystem>
#include <string>
#include <memory_resource>
#include <mr/log_resource.hpp>
/*
static void BM_StreamParse(benchmark::State& state, const std::string& json_file) 
{
  std::ifstream ifs;
  ifs.open(json_file);

  if(!ifs.is_open())
    state.SkipWithError(("failed to open: " + json_file).c_str());

  Json::CharReaderBuilder builder;
  std::array<std::byte, 128000> buffer{};
  std::array<std::byte, 128000> buffer2{};

  std::pmr::monotonic_buffer_resource mr{buffer.data(), buffer.size()};
  std::pmr::monotonic_buffer_resource pr{buffer2.data(), buffer2.size()};

  for (auto _ : state)
  {
    mr.release();
    pr.release();

    Json::Value root{&mr};
    JSONCPP_STRING errs;

    if (!parseFromStream(builder, ifs, &root, &errs, &pr)) {
      state.SkipWithError(errs.c_str());
      break;
    }
    
    //if(root.size() == 0) state.SkipWithError(("failed to open: " + json_file).c_str());
    ifs.clear();
    ifs.seekg(0, std::ios::beg);
  }
}
// Register the function as a benchmark
BENCHMARK_CAPTURE(BM_StreamParse, x.test, std::string("test.json"));

static void BM_StreamParseNoPmr(benchmark::State& state, const std::string& json_file) 
{
  std::ifstream ifs;
  ifs.open(json_file);

  if(!ifs.is_open())
    state.SkipWithError(("failed to open: " + json_file).c_str());

  Json::CharReaderBuilder builder;

  for (auto _ : state)
  {
    Json::Value root;
    JSONCPP_STRING errs;

    if (!parseFromStream(builder, ifs, &root, &errs)) {
      state.SkipWithError(errs.c_str());
      break;
    }
    
    if(root.size() == 0) state.SkipWithError(("failed to open: " + json_file).c_str());
    ifs.clear();
    ifs.seekg(0, std::ios::beg);
  }
}
// Register the function as a benchmark
BENCHMARK_CAPTURE(BM_StreamParseNoPmr, x.test, std::string("test.json"));*/

static void BM_Depth(benchmark::State& state, const std::string& json_file) 
{
  Json::CharReaderBuilder builder;
  JSONCPP_STRING errs;
  std::ifstream ifs;

  const std::string full_file_name = json_file + std::to_string(state.range(0)) + ".json";
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
    //r_parser.release();
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
    //r_parser.release();
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
    //r_parser.release();
  }
}
// Register the function as a benchmark
BENCHMARK_CAPTURE(BM_String, json.test, std::string("jsongen_str_"))->DenseRange(0, 2000, 200);

BENCHMARK_MAIN();