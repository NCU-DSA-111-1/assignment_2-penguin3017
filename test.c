#include <stdio.h>
#include <stdlib.h>
#include "libev/ev.h"
#define ROW 9

int turns;
int now;
int c=0;
int indep=0;
ev_io stdin_watcher;
ev_timer timeout_watcherx;
ev_timer timeout_watchery;

static void stdin_cb (EV_P_ ev_io *w, int revents){
    ev_timer_stop (loop, &timeout_watcherx);	
    ev_timer_stop (loop, &timeout_watchery);
	if(turns%2){	
		ev_timer_stop (loop, &timeout_watcherx);	
		ev_timer_start (loop, &timeout_watchery);
	}
	else{
		ev_timer_stop (loop, &timeout_watchery);
		ev_timer_start (loop, &timeout_watcherx);
	}
}
static void timeout_cbx (EV_P_ ev_timer *w, int revents){
	if(turns==0){
        printf("The time of player x use: ");printf("%d\n",(int)ev_now(loop)-now);  //first time
        indep=(int)ev_now(loop)-now;
        printf("indep: %d\n",indep);
    }
    else{
        printf("The time of player x use: ");printf("%d\n",(int)ev_now(loop)-now-indep);
        indep=(int)ev_now(loop)-now;
        printf("cby\n");
        printf("indep: %d\n",indep);
    }
    printf("cbx\n");
    ev_timer_stop (loop, &timeout_watcherx);
    ev_break(loop, EVBREAK_ONE);
    printf("InCBX,turns++\n\n\n");
    turns+=1;
}
static void timeout_cby (EV_P_ ev_timer *w, int revents){
    printf("The time of player y use: ");printf("%d\n",(int)ev_now(loop)-now-indep);
    indep=(int)ev_now(loop)-now;
    printf("indep: %d\n",indep);
    printf("cby\n");
    ev_timer_stop (loop, &timeout_watchery);
    ev_break(loop, EVBREAK_ONE);
    printf("InCBY,turns++\n\n\n");
    turns+=1;
}


int main(){
    turns=0;
    struct ev_loop *loop = EV_DEFAULT;
    now=ev_now(loop);
    int x,y,z;    
    ev_io_init(&stdin_watcher, stdin_cb, /*STDIN_FILENO*/ 0, EV_READ);
	ev_io_start(loop, &stdin_watcher);
	ev_timer_init(&timeout_watcherx, timeout_cbx, 0, 1);
    ev_timer_init(&timeout_watchery, timeout_cby, 0, 1);
    
    while(turns<=50){
        printf("turns: %d\n",turns);
        if(turns%2==0){
            printf("input: ");
            scanf(" %d",&x);
            ev_timer_start(loop, &timeout_watcherx);
            printf("The turn is %d now\n",turns);
            
        }
        else{
            printf("input:");
            scanf(" %d",&x);
            ev_timer_start(loop, &timeout_watchery);
            printf("The turn is %d now\n",turns);
        }
        ev_run(loop, 0);
    }
    printf("Outside!\n");
    ev_timer_stop (loop, &timeout_watcherx);
    ev_timer_stop (loop, &timeout_watchery);
    
}