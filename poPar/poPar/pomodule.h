#ifndef POMODULE_H
#define POMODULE_H
#include <iostream>
#include <ostream>

class pomodule
{

public:
    const static int PARAM_ROW = 3;
    const static int PARAM_COL = 16;
    pomodule();
    friend std::ostream & operator<<(std::ostream& out,const pomodule& obj);

    void setName(const std::string str);
    std::string getName();

    void setAtuo(bool a);
    bool getIsAtuo();

    void setSnAuto(bool a);
    bool getSnAuto();

    void setExIso(bool a);
    bool getExIso();

    void setStrgByStr(int stat,char *str);
    int *getStrg(int stat);

    int getValueByIndex(int index_row,int index_col){
        return strg[index_row][index_col];
    }

    int *getDayStrg(){return strg[0];};
    int *getWdrStrg(){return strg[1];};
    int *getNightStrg(){return strg[2];};


    bool setStrgByIndex(int stat,int index,int value);

private:
    std::string name;
    bool is_auto;
    bool is_sn_auto;
    bool is_ex_iso;
    int strg[PARAM_ROW][PARAM_COL];

};

#endif // POMODULE_H
