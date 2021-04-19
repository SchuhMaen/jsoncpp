#include <benchmark/benchmark.h>
#include <json/json.h>
#include <fstream>
#include <iostream>
#include <filesystem>
#include <string>

static void BM_StreamParse(benchmark::State& state, const std::string& json_file) 
{
  for (auto _ : state)
  {
    Json::Value root;
    Json::CharReaderBuilder builder;
    JSONCPP_STRING errs;
    std::ifstream ifs;
    ifs.open(json_file);

    if(!ifs.is_open())
      state.SkipWithError(("failed to open: " + json_file).c_str());

    if (!parseFromStream(builder, ifs, &root, &errs)) {
      std::cout << errs << std::endl;
    }
  }
}
// Register the function as a benchmark
BENCHMARK_CAPTURE(BM_StreamParse, json.test, std::string("test.json"));

BENCHMARK_MAIN();