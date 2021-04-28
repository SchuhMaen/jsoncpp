#include "..\..\include\json\json.h"
#include <iostream>
#include <mr/log_resource.hpp>
#include <mr/config.hpp>
#include <mr/res_printer.hpp>
#include <memory_resource>
#include <string>

#define TEST(x) mr::ls::stream() << #x << " :\n"; x();

void move_assign_test()
{
    const int width = 70;
    const int length = 70;

    {   // different resource
        std::array<char, 512> buffer{};
        std::array<char, 512> buffer2{};
        std::array<char, 512> buffer3{};
        buffer.fill('~');
        buffer2.fill('~');
        buffer3.fill('~');
        std::pmr::monotonic_buffer_resource mr{buffer.data(), buffer.size()};
        std::pmr::monotonic_buffer_resource mr2{buffer2.data(), buffer2.size()};
        std::pmr::monotonic_buffer_resource mr3{buffer3.data(), buffer3.size()};
        mr::log_resource lr{"me_res",&mr};
        mr::log_resource lr2{"other_res",&mr2};
        mr::log_resource lr3{"def_res",&mr3};
        std::pmr::set_default_resource(&lr3);

        Json::Value me{"me",&lr};
        me = Json::Value(Json::arrayValue, &lr2);

        lr.report();
        mr::dump(buffer.begin(), buffer.end(), length, width);

        lr2.report();
        mr::dump(buffer2.begin(), buffer2.end(), length, width);
    }
    {   // same resource
        std::array<char, 512> buffer{};
        std::array<char, 512> buffer2{};
        std::array<char, 512> buffer3{};
        buffer.fill('~');
        buffer2.fill('~');
        buffer3.fill('~');
        std::pmr::monotonic_buffer_resource mr{buffer.data(), buffer.size()};
        std::pmr::monotonic_buffer_resource mr2{buffer2.data(), buffer2.size()};
        std::pmr::monotonic_buffer_resource mr3{buffer3.data(), buffer3.size()};
        mr::log_resource lr{"me_res",&mr};
        mr::log_resource lr2{"other_res",&mr2};
        mr::log_resource lr3{"def_res",&mr3};
        std::pmr::set_default_resource(&lr3);

        Json::Value me{"me",&lr};
        me = Json::Value(Json::arrayValue,&lr);

        lr.report();
        mr::dump(buffer.begin(), buffer.end(), length, width);

        lr2.report();
        mr::dump(buffer2.begin(), buffer2.end(), length, width);

        lr3.report();
        mr::dump(buffer3.begin(), buffer3.end(), length, width);
    }
}

void number_test()
{
    const int width = 70;
    const int length = 70;

    {   // different resource
        std::array<char, 512> buffer{};
        std::array<char, 512> buffer2{};
        buffer.fill('~');
        buffer2.fill('~');
        std::pmr::monotonic_buffer_resource mr{buffer.data(), buffer.size()};
        std::pmr::monotonic_buffer_resource mr2{buffer2.data(), buffer2.size()};
        mr::log_resource lr{"me_res",&mr};
        mr::log_resource lr2{"other_res",&mr2};
        std::pmr::set_default_resource(&lr2);

        //Json::Value me{Json::arrayValue,&lr};
        //me.emplace(0, "gggg"); // limitation. resource has to be manually passed;
        
        std::pmr::map<int,Json::Value> map{&lr};
        auto i = map.lower_bound(0);
        std::pmr::map<int,Json::Value>::value_type def(Json::Value::ArrayIndex(0), Json::Value::nullSingleton());
        i = map.insert(i, def);
        //(*i).second[0].set("fffffffffffffff");


        //std::pmr::map<int, Json::Value> g{&lr};
        //g.emplace(0,Json::Value("gggg"));

        lr.report();
        mr::dump(buffer.begin(), buffer.end(), length, width);
    }
}

int main()
{
    mr::ls::set_log_stream(std::clog);
    TEST(move_assign_test)
    //TEST(number_test)
}