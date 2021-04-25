project(
  'jsoncpp',
  'cpp',

  # Note: version must be updated in three places when doing a release. This
  # annoying process ensures that amalgamate, CMake, and meson all report the
  # correct version.
  # 1. /meson.build
  # 2. /include/json/version.h
  # 3. /CMakeLists.txt
  # IMPORTANT: also update the SOVERSION!!
  version : '1.9.4',
  default_options : [
    'buildtype=release',
    'cpp_std=c++17',
    'warning_level=1'],
  license : 'Public Domain',
  meson_version : '>= 0.49.0')

add_global_arguments(['-LX:/dev/bachelor/code/jsoncpp/benchmark/build/src'], language : 'cpp')

jsoncpp_headers = files([
  'include/json/allocator.h',
  'include/json/assertions.h',
  'include/json/config.h',
  'include/json/json_features.h',
  'include/json/forwards.h',
  'include/json/json.h',
  'include/json/reader.h',
  'include/json/value.h',
  'include/json/version.h',
  'include/json/writer.h',
])
jsoncpp_include_directories = include_directories('include')

install_headers(
  jsoncpp_headers,
  subdir : 'json')

if get_option('default_library') == 'shared' and meson.get_compiler('cpp').get_id() == 'msvc'
  dll_export_flag = '-DJSON_DLL_BUILD'
  dll_import_flag = '-DJSON_DLL'
else
  dll_export_flag = []
  dll_import_flag = []
endif

if meson.get_compiler('cpp').has_argument('-fno-builtin-memset')
  keep_memset_flag = ['-fno-builtin-memset']
else
  keep_memset_flag = []
endif

jsoncpp_lib = library(
  'jsoncpp', files([
    'src/lib_json/json_reader.cpp',
    'src/lib_json/json_value.cpp',
    'src/lib_json/json_writer.cpp',
  ]),
  soversion : 24,
  install : true,
  include_directories : jsoncpp_include_directories,
  cpp_args: dll_export_flag + keep_memset_flag
)

jsoncpp_lib_secure = library(
  'jsoncpp_secure', files([
    'src/lib_json/json_reader.cpp',
    'src/lib_json/json_value.cpp',
    'src/lib_json/json_writer.cpp',
  ]),
  soversion : 24,
  install : true,
  include_directories : jsoncpp_include_directories,
  cpp_args: dll_export_flag + keep_memset_flag + [
    '-DJSONCPP_USING_SECURE_MEMORY_OVERRIDE=1'
  ]
)

import('pkgconfig').generate(
  libraries : jsoncpp_lib,
  version : meson.project_version(),
  name : 'jsoncpp',
  filebase : 'jsoncpp',
  description : 'A C++ library for interacting with JSON')

# for libraries bundling jsoncpp
jsoncpp_dep = declare_dependency(
  include_directories : jsoncpp_include_directories,
  link_with : jsoncpp_lib,
  version : meson.project_version())

# tests
if meson.is_subproject() or not get_option('tests')
  subdir_done()
endif

python = import('python').find_installation()

jsoncpp_test = executable(
  'jsoncpp_test', files([
    'src/test_lib_json/jsontest.cpp',
    'src/test_lib_json/main.cpp',
    'src/test_lib_json/fuzz.cpp',
  ]),
  include_directories : jsoncpp_include_directories,
  link_with : jsoncpp_lib,
  install : false,
  cpp_args: dll_import_flag
)

test(
  'unittest_jsoncpp_test',
  jsoncpp_test)

jsontestrunner = executable(
  'jsontestrunner',
  'src/jsontestrunner/main.cpp',
  include_directories : jsoncpp_include_directories,
  link_with : jsoncpp_lib,
  install : false,
  cpp_args: dll_import_flag
)

test(
  'unittest_jsontestrunner',
  python,
  args : [
    join_paths(meson.current_source_dir(), 'test/runjsontests.py'),
    jsontestrunner,
    join_paths(meson.current_source_dir(), 'test/data')],
  )

test(
  'jsonchecker_jsontestrunner',
  python,
  is_parallel : false,
  args : [
    join_paths(meson.current_source_dir(), 'test/runjsontests.py'),
    '--with-json-checker',
    jsontestrunner,
    join_paths(meson.current_source_dir(), 'test/data')],
    workdir : join_paths(meson.current_source_dir(), 'test/data'),
)

benchmark_dep = [
  meson.get_compiler('cpp').find_library('benchmark'),
  dependency('threads'),
  meson.get_compiler('cpp').find_library('shlwapi'),
  meson.get_compiler('cpp').find_library('mr'),
  ]

g_benchmark_secure = executable(
  'google_benchmark_secure', files([
    'src/google_benchmark/main.cpp'
  ]),
  include_directories : jsoncpp_include_directories,
  link_with : jsoncpp_lib_secure,
  dependencies : benchmark_dep,
  install : false,
  cpp_args : [
    '-DJSONCPP_USING_SECURE_MEMORY_OVERRIDE=1'
  ]
)

g_benchmark = executable(
  'google_benchmark', files([
    'src/google_benchmark/main.cpp'
  ]),
  include_directories : jsoncpp_include_directories,
  link_with : jsoncpp_lib,
  dependencies : benchmark_dep,
  install : false,
)

g_benchmark_args = [
  '--benchmark_out_format=console',
  '--benchmark_repetitions=5'
]

g_benchmark_workdir = join_paths(meson.current_source_dir(), 'test/benchmark_data')

benchmark(
  'benchmark',
  g_benchmark,
  args : g_benchmark_args + ['--benchmark_out=results.txt'],
  workdir : g_benchmark_workdir
)

benchmark(
  'benchmark_secure_alloc',
  g_benchmark_secure,
  args : g_benchmark_args + ['--benchmark_out=results_secure.txt'],
  workdir : g_benchmark_workdir
)

example = executable(
  'example', files([
    'example/readFromString/readFromString.cpp',
  ]),
  include_directories : jsoncpp_include_directories,
  link_with : jsoncpp_lib,
  dependencies : meson.get_compiler('cpp').find_library('mr'),
  install : false,
  cpp_args: dll_import_flag
)