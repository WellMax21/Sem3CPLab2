#include <cstdlib>
#include <iostream>
#include <string>

#include "ip_filter.h"

//cmd /c "build\ip_filter.exe < ip_filter.tsv > out.txt"

int main()
{
    try
    {
        IpPool ip_pool;

        for(std::string line; std::getline(std::cin, line);)
        {
            if(line.empty())
            {
                continue;
            }
            const auto v = split(line, '\t');
            ip_pool.push_back(parse_ip(v.at(0)));
        }

        sort_reverse(ip_pool);

        auto print = [](const decltype(ip_pool) &pool) {
            for(const auto &ip : pool)
            {
                print_ip(std::cout, ip);
            }
        };

        print(ip_pool);                    
        print(filter(ip_pool, 1));         
        print(filter(ip_pool, 46, 70));    
        print(filter_any(ip_pool, 46));    
    }
    catch(const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
