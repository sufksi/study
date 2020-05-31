#include "pomodule.h"
#include <cstring>

pomodule::pomodule():is_auto(1),is_sn_auto(1),is_ex_iso(0)
{

}

std::ostream & operator<<(std::ostream& out,const pomodule& obj)
{
    out<<"modName:"<<obj.name<<"\tatuo="<<obj.is_auto<<"\tsn_atuo="<<obj.is_sn_auto<<"\tex_iso="<<obj.is_ex_iso<<"\tstrg:\n";
    for(int j = 0;j<3;j++)
    {
        for(int i=0;i<16;i++)
        {
            out<<obj.strg[j][i]<<" ";
        }
        out<<std::endl;
    }
    out<<std::endl;
    return out;
}

void pomodule::setName(const std::string str)
{
    name = str;
}
std::string pomodule::getName()
{
    return name;
}

void pomodule::setAtuo(bool a)
{
    is_auto = a;
}
bool pomodule::getIsAtuo()
{
    return is_auto;
}

void pomodule::setSnAuto(bool a)
{
    is_sn_auto = a;
}
bool pomodule::getSnAuto()
{
    return is_sn_auto;
}

void pomodule::setExIso(bool a)
{
    is_ex_iso = a;
}
bool pomodule::getExIso()
{
    return is_ex_iso;
}

void pomodule::setStrgByStr(int stat,char *str)
{
    char tmp[10];
    char *index = str;
    int i = 0;
    int j = 0;
    while(*index != '\0')
    {
        tmp[i++] = *index++;

        if(',' == *index || '\0' == *index)
        {
            tmp[i] = '\0';
            index++;
            if(tmp[0] == '0' && 'x' == tmp[1])
            {
                sscanf(tmp,"%x",&strg[stat][j++]);
            }
            else
            {
                sscanf(tmp,"%d",&strg[stat][j++]);
            }
            i = 0;
        }
    }
}

int *pomodule::getStrg(int stat)
{
    return strg[stat];
}

bool pomodule::setStrgByIndex(int stat,int index,int value)
{
    strg[stat][index] = value;
    return true;
}
