#include <gtest/gtest.h>

#include "ip_filter.h"

namespace {

IpPool make_pool(const std::vector<std::string> &strs)
{
    IpPool pool;
    for(const auto &s : strs)
    {
        pool.push_back(parse_ip(s));
    }
    return pool;
}

} 

TEST(IpFilter, Split)
{
    using V = std::vector<std::string>;

    EXPECT_EQ(split("", '.'), (V{""}));
    EXPECT_EQ(split("11", '.'), (V{"11"}));
    EXPECT_EQ(split("..", '.'), (V{"", "", ""}));
    EXPECT_EQ(split("11.", '.'), (V{"11", ""}));
    EXPECT_EQ(split(".11", '.'), (V{"", "11"}));
    EXPECT_EQ(split("11.22", '.'), (V{"11", "22"}));
}


TEST(IpFilter, ReverseSortIsNumeric)
{
    auto pool = make_pool({"1.1.1.1", "1.2.1.1", "1.10.1.1", "46.70.225.39", "5.189.203.46"});

    sort_reverse(pool);

    const auto expected = make_pool({"46.70.225.39", "5.189.203.46", "1.10.1.1", "1.2.1.1", "1.1.1.1"});
    EXPECT_EQ(pool, expected);
}

TEST(IpFilter, Filters)
{
    auto pool = make_pool({"1.1.234.8", "46.70.225.39", "46.70.29.76", "186.46.222.194",
                           "5.189.203.46", "39.46.86.85", "1.231.69.33", "47.70.1.1"});
    sort_reverse(pool);

    EXPECT_EQ(filter(pool, 1), make_pool({"1.231.69.33", "1.1.234.8"}));

    EXPECT_EQ(filter(pool, 46, 70), make_pool({"46.70.225.39", "46.70.29.76"}));

    EXPECT_EQ(filter_any(pool, 46),
              make_pool({"186.46.222.194", "46.70.225.39", "46.70.29.76", "39.46.86.85", "5.189.203.46"}));
}
