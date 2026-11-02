#pragma once

#include <algorithm>
#include <array>
#include <iostream>
#include <iterator>
#include <stdexcept>
#include <string>
#include <tuple>
#include <vector>

// ("",  '.') -> [""]
// ("11", '.') -> ["11"]
// ("..", '.') -> ["", "", ""]
// ("11.", '.') -> ["11", ""]
// (".11", '.') -> ["", "11"]
// ("11.22", '.') -> ["11", "22"]
inline std::vector<std::string> split(const std::string &str, char d)
{
    std::vector<std::string> r;

    std::string::size_type start = 0;
    std::string::size_type stop = str.find_first_of(d);
    while(stop != std::string::npos)
    {
        r.push_back(str.substr(start, stop - start));

        start = stop + 1;
        stop = str.find_first_of(d, start);
    }

    r.push_back(str.substr(start));

    return r;
}

using IpAddr = std::array<int, 4>;
using IpPool = std::vector<IpAddr>;
using Byte = IpAddr::value_type;

inline IpAddr parse_ip(const std::string &s)
{
    const auto parts = split(s, '.');
    if(parts.size() != 4)
    {
        throw std::runtime_error("bad ip: " + s);
    }

    IpAddr ip{};
    for(std::size_t i = 0; i < ip.size(); ++i)
    {
        const int n = std::stoi(parts[i]);
        if(n < 0 || n > 255)
        {
            throw std::runtime_error("bad ip byte: " + s);
        }
        ip[i] = n;
    }
    return ip;
}

inline void print_ip(std::ostream &os, const IpAddr &ip)
{
    for(auto it = ip.cbegin(); it != ip.cend(); ++it)
    {
        if(it != ip.cbegin())
        {
            os << ".";
        }
        os << *it;
    }
    os << std::endl;
}

inline void sort_reverse(IpPool &pool)
{
    using Ip = IpPool::value_type;
    std::sort(pool.begin(), pool.end(),
              [](const Ip &a, const Ip &b) { return a > b; });
}

template <typename Pred>
IpPool select(const IpPool &pool, Pred pred)
{
    IpPool r;
    std::copy_if(pool.cbegin(), pool.cend(), std::back_inserter(r), pred);
    return r;
}

inline IpPool filter(const IpPool &pool, Byte b1)
{
    using Ip = IpPool::value_type;
    return select(pool, [b1](const Ip &ip) { return ip[0] == b1; });
}

inline IpPool filter(const IpPool &pool, Byte b1, Byte b2)
{
    using Ip = IpPool::value_type;
    const auto key = std::make_tuple(b1, b2);
    return select(pool, [key](const Ip &ip) {
        return std::make_tuple(ip[0], ip[1]) == key;
    });
}

inline IpPool filter_any(const IpPool &pool, Byte b)
{
    using Ip = IpPool::value_type;
    return select(pool, [b](const Ip &ip) {
        return std::any_of(ip.cbegin(), ip.cend(),
                           [b](Byte x) { return x == b; });
    });
}
