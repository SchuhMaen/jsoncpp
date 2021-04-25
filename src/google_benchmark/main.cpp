#include <benchmark/benchmark.h>
#include "json/json.h"
#include <fstream>
#include <iostream>
#include <filesystem>
#include <string>
#include <memory_resource>
#include <mr/log_resource.hpp>

static void BM_StreamParse(benchmark::State& state, const std::string& json_file) 
{
  std::ifstream ifs;
  ifs.open(json_file);

  if(!ifs.is_open())
    state.SkipWithError(("failed to open: " + json_file).c_str());

  std::array<std::byte, 32000> buffer{};

  for (auto _ : state)
  {
    std::pmr::monotonic_buffer_resource mr{buffer.data(), buffer.size()};
    Json::Value root{&mr};
    Json::CharReaderBuilder builder;
    JSONCPP_STRING errs;
    //log_resource lr{&mr};
    //std::pmr::unsynchronized_pool_resource pr{&mr};

    if (!parseFromStream(builder, ifs, &root, &errs)) {
      state.SkipWithError("failed to parse");
      break;
    }
    
    if(root.size() == 0) state.SkipWithError(("failed to open: " + json_file).c_str());
    ifs.clear();
    ifs.seekg(0, std::ios::beg);
  }
}
// Register the function as a benchmark
BENCHMARK_CAPTURE(BM_StreamParse, x.test, std::string("x.json"));

static void BM_StreamParseNoPmr(benchmark::State& state, const std::string& json_file) 
{
  std::ifstream ifs;
  ifs.open(json_file);

  if(!ifs.is_open())
    state.SkipWithError(("failed to open: " + json_file).c_str());

  for (auto _ : state)
  {
    Json::Value root;
    Json::CharReaderBuilder builder;
    JSONCPP_STRING errs;

    if (!parseFromStream(builder, ifs, &root, &errs)) {
      state.SkipWithError("failed to parse");
      break;
    }
    
    if(root.size() == 0) state.SkipWithError(("failed to open: " + json_file).c_str());
    ifs.clear();
    ifs.seekg(0, std::ios::beg);
  }
}
// Register the function as a benchmark
BENCHMARK_CAPTURE(BM_StreamParseNoPmr, x.test, std::string("x.json"));

static void BM_StringParse(benchmark::State& state) 
{
  const std::string rawJson = R"({"Age": 20, "Name": "colin"})";
  const auto rawJsonLength = static_cast<int>(rawJson.length());
  std::array<std::byte, 32000> buffer{};

  for (auto _ : state)
  {
    std::pmr::monotonic_buffer_resource mr{buffer.data(), buffer.size()};
    JSONCPP_STRING err;
    Json::Value root{&mr};
    Json::CharReaderBuilder builder;
    //log_resource lr{&mr};
    const std::unique_ptr<Json::CharReader> reader(builder.newCharReader());

    if (!reader->parse(rawJson.c_str(), rawJson.c_str() + rawJsonLength, &root, &err)) {
      state.SkipWithError("failed to parse");
      break;
    }
  }
}

BENCHMARK(BM_StringParse);

static void BM_StringParseNoPmr(benchmark::State& state) 
{
  const std::string rawJson = R"({"Age": 20, "Name": "colin"})";
  const auto rawJsonLength = static_cast<int>(rawJson.length());

  for (auto _ : state)
  {
    JSONCPP_STRING err;
    Json::Value root;
    Json::CharReaderBuilder builder;
    
    const std::unique_ptr<Json::CharReader> reader(builder.newCharReader());

    if (!reader->parse(rawJson.c_str(), rawJson.c_str() + rawJsonLength, &root, &err)) {
      state.SkipWithError("failed to parse");
      break;
    }
  }
}

BENCHMARK(BM_StringParseNoPmr);

BENCHMARK_MAIN();