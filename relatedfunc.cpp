#include "relatedfunc.h"

int randNum()//获取0-9的随机数
{
    const int MIN = 0;
    const int MAX = 9;
    srand((unsigned int)time(nullptr));
    return MIN + rand() % (MAX + MIN - 1);
}
