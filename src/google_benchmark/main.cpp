#include <benchmark/benchmark.h>
#include <json/json.h>
#include <fstream>
#include <iostream>
#include <filesystem>
#include <string>

static void BM_Depth(benchmark::State& state, const std::string& json_file) 
{
  Json::CharReaderBuilder builder;
  JSONCPP_STRING errs;
  std::ifstream ifs;

  const std::string full_file_name = json_file + std::to_string(state.range(0)) + ".json";
  ifs.open(full_file_name);

  if(!ifs.is_open())
      state.SkipWithError(("failed to open: " + full_file_name).c_str());

  for (auto _ : state)
  {
    Json::Value root;
    if (!parseFromStream(builder, ifs, &root, &errs)) {
      std::cerr << errs << std::endl;
    }

    ifs.clear();
    ifs.seekg(0, std::ios::beg);
  }
}
// Register the function as a benchmark
BENCHMARK_CAPTURE(BM_Depth, json.test, std::string("jsongen_depth_"))->DenseRange(3, 9, 3);

BENCHMARK_MAIN();