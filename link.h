#ifndef LINK_H
#define LINK_H
#include <stdio.h>
#include <stdlib.h>

enum chesstype{
    kyousya,keima,ginnsyou,kinnsyou,ousyou,hisya,kakugyou,fubyou,space,
};

struct stk{//data
    int c_row;
    int c_col;
    int n_row;
    int n_col;
    char whose;
    char n_whose;
    enum chesstype kind;
    enum chesstype n_kind;
};

struct linkstk{//for link list
    struct stk s_link;//push pop 東西
    struct linkstk *next;
    struct linkstk *prev;
};
struct stack{//point to top
    struct linkstk *top;
};
int empty(struct stack *);
void push(struct stk ,struct stack *);
void pop(struct stack *);
void regret(struct stk *,struct stk ,int *);
void history_record(struct stk *,struct stk ,int *);


#endif