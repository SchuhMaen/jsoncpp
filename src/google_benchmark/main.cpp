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

static void BM_Json(benchmark::State& state) 
{
  Json::CharReaderBuilder builder;
  JSONCPP_STRING errs;
  std::ifstream ifs;

  const std::string full_file_name = "example.json";
  ifs.open(full_file_name);

  if(!ifs.is_open())
      state.SkipWithError(("failed to open: " + full_file_name).c_str());

  Json::OStringStream ssin;
  ssin << ifs.rdbuf();
  Json::String doc = ssin.str();
  char const* begin = doc.data();
  char const* end = begin + doc.size();

  std::unique_ptr<Json::CharReader> const reader(builder.newCharReader());

  for (auto _ : state)
  {
    Json::Value root;
    if (!reader->parse(begin, end, &root, &errs)) {
      std::cerr << errs << std::endl;
    }
  }
}
// Register the function as a benchmark
BENCHMARK(BM_Json);

BENCHMARK_MAIN();