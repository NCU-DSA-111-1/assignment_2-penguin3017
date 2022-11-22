#include <stdio.h>
#include <stdlib.h>
#include "link.h"
#define STACKSIZE 1000


int empty(struct stack *ptr){   
    if(ptr->top==NULL)
        return 1;
    return 0;
}
void pop(struct stack *ptr){
    struct linkstk *temp=ptr->top;
    if(temp->prev!=NULL){
        ptr->top=temp->prev;
        ptr->top->next=NULL;printf("ty2!\n");
        free(temp);    
    }
    else{//temp->prev==NULL
        ptr->top=NULL;
        free(temp);
    }
    return;
}
void push(struct stk data,struct stack *ptr){
    struct linkstk *temp=(struct linkstk *)malloc(sizeof(struct linkstk));
    temp->s_link=data;
    temp->next=NULL;
    temp->prev=NULL;
    if(ptr->top){
        ptr->top->next=temp;
        temp->prev=ptr->top;
    }
    ptr->top=temp;
    return;
}
/*
void history_record(struct stk *sptr1,struct stk x,int *top){
    push(sptr1,x,top);
}

void regret(struct stk *sptr1,struct stk x,int *top){
    x=sptr1[(*top)];
    pop(sptr1,top);  
}
*/


