#include <stdio.h>
#include <stdlib.h>
#include "stk.h"
#define STACKSIZE 1000

int empty(struct stk *sptr,int *top){   
    return (*top==-1);
}
int overflow(struct stk *sptr,int *top){
    if(*top==STACKSIZE-1){
        return 1;
    }
    else{
        return 0;
    }
}
void pop(struct stk *sptr,int *top){
    if(empty(sptr,top)){
        printf("Empty!\n");
    }
    //printf("%d popped out \n",sptr->arr[top]);
    (*top)-=1;
}
void push(struct stk *sptr,struct stk x,int *top){
    if(overflow(sptr,top)==1){
        return ;
    }
    else{
        (*top)+=1;
        sptr[(*top)]=x;
    }
}

void history_record(struct stk *sptr1,struct stk x,int *top){
    push(sptr1,x,top);
}
void regret(struct stk *sptr1,struct stk x,int *top){
    x=sptr1[(*top)];
    pop(sptr1,top);  
}



