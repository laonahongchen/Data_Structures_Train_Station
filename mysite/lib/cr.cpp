#include <algorithm>
#include <iostream>
#include <cstring>
#include <cstdio>
#include <vector>
#include <ctime>

namespace CR
{

void searchTicket(char *string)
{
    std::string s =
        "车次 C7673 出发站 小榄 到达站 南朗 出发时间 12:10 到达时间 12:25 软卧 -- 一等座 268 二等座 0 无座 485|车次 G86 出发站 广州南 到达站 上海虹桥 出发时间 08:00 到达时间 14:50 软卧 42 一等座 26 二等座 674 无座 --";
    strcpy(string, s.c_str());
}

} // namespace CR

extern "C"
{
    void searchTicket(char *string)
    {
        CR::searchTicket(string);
    }
}
