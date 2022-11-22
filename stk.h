#ifndef STK_H
#define STK_H
#include <stdio.h>
#include <stdlib.h>

enum chesstype{
    kyousya,keima,ginnsyou,kinnsyou,ousyou,hisya,kakugyou,fubyou,space,
};

struct stk{
    int c_row;
    int c_col;
    int n_row;
    int n_col;
    char whose;
    char n_whose;
    enum chesstype kind;
    enum chesstype n_kind;
};
struct whose_stk{
    int top_whose;
    char *whose;
};
struct type_stk{
    int top_type;
    enum chesstype *arrtype;
};



void regret(struct stk *,struct stk ,int *);
void history_record(struct stk *,struct stk ,int *);


#endif