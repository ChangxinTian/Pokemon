#include "relatedfunc_client.h"

int randNum()//获取0-9的随机数
{
    const int MIN = 0;
    const int MAX = 9;
    srand((unsigned int)time(nullptr));
    return MIN + rand() % (MAX + MIN - 1);
}

bool isChinese(QString &qstrSrc)
{
    int nCount = qstrSrc.count();
    bool bret = false;
    for(int i=0; i<nCount; ++i){
        QChar cha = qstrSrc.at(i);
        ushort uni = cha.unicode();
        if(uni >= 0x4E00 && uni <= 0x9FA5)
            bret=true;
    }
    return bret;
}
