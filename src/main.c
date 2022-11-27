#include <stdio.h>
#include <stdlib.h>
#include "libev/ev.h"
#include "stk.h"
#define ROW 9
#define COLUMN 9
#define STACKSIZE 1000

int indep=0;
int control;
int now;
ev_io stdin_watcher;
ev_timer timeout_watcherx;
ev_timer timeout_watchery;

static void stdin_cb (EV_P_ ev_io *w, int revents){
    ev_timer_stop (loop, &timeout_watcherx);	
    ev_timer_stop (loop, &timeout_watchery);
	if(control%2){	
		ev_timer_stop (loop, &timeout_watcherx);	
		ev_timer_start (loop, &timeout_watchery);
	}
	else{
		ev_timer_stop (loop, &timeout_watchery);
		ev_timer_start (loop, &timeout_watcherx);
	}
}
static void timeout_cbx (EV_P_ ev_timer *w, int revents){
	if(control==0){
        printf("The time of player x use: ");printf("\033[43m %d second \033[m\n",(int)ev_now(loop)-now);
        indep=(int)ev_now(loop)-now;
    }else{
        printf("The time of player x use: ");printf("\033[43m %d second \033[m\n",(int)ev_now(loop)-now-indep);
        indep=(int)ev_now(loop)-now;
    }
    ev_timer_stop (loop, &timeout_watcherx);
    ev_break(loop, EVBREAK_ONE);
    control+=1;
	
}
static void timeout_cby (EV_P_ ev_timer *w, int revents){
    printf("The time of player y use: ");printf("\033[43m %d second \033[m\n",(int)ev_now(loop)-now-indep);
    indep=(int)ev_now(loop)-now;
    ev_timer_stop (loop, &timeout_watchery);
    ev_break(loop, EVBREAK_ONE);
    control+=1;
	
}

enum chesstype X[]={
    kyousya,keima,ginnsyou,kinnsyou,ousyou,hisya,kakugyou,fubyou,space,
};
enum chesstype Y[]={
    kyousya,keima,ginnsyou,kinnsyou,ousyou,hisya,kakugyou,fubyou,space,
};
const char* chesstype[]={"香車","桂馬","銀將","金將","王將","飛車","角行","步兵","    "};

struct board{
    enum chesstype name;
    char whose;//x for x,y for y,z for none
    int promotion;
    int arrive_enemy;
};
struct board board[ROW][COLUMN];
void startgame(){//initialize chess
    for(int i=0;i<ROW;i++){
        int symmetry=0;
        for(int j=0;j<COLUMN;j++){
            board[i][j].name=X[8];
            if(i<=2){
                if(i==0){//first row
                    if(j<=4){
                        symmetry=j;
                    }
                    else{
                        symmetry=4-(j-4);
                        /*
                        j=5,temp=3
                        j=6,temp=2
                        j=7,temp=1
                        j=8,temp=0
                        */
                    }
                    board[i][j].name=Y[symmetry];
                    board[i][j].whose='y';
                }
                else if(i==1 && j==7){
                    board[i][j].name=Y[5];
                    board[i][j].whose='y';
                }
                else if(i==1 && j==1){
                    board[i][j].name=Y[6];
                    board[i][j].whose='y';
                }
                else if(i==2){
                    board[i][j].name=Y[7];
                    board[i][j].whose='y';
                }
                else{
                    board[i][j].whose='z';
                }
            }
            else if(i>=6){
                if(i==8){//last row
                    if(j<=4){
                        symmetry=j;
                    }
                    else{
                        symmetry=4-(j-4);
                        /*
                        j=5,temp=3
                        j=6,temp=2
                        j=7,temp=1
                        j=8,temp=0
                        */
                    }
                    board[i][j].name=X[symmetry];
                    board[i][j].whose='x';
                }
                else if(i==7 && j==1){
                    board[i][j].name=X[5];
                    board[i][j].whose='x';
                }
                else if(i==7 && j==7){
                    board[i][j].name=X[6];
                    board[i][j].whose='x';
                }
                else if(i==6){
                    board[i][j].name=X[7];
                    board[i][j].whose='x';
                }
                else{
                    board[i][j].whose='z';
                }
            }
            else{
                board[i][j].whose='z';
            }
        }
    }
}
void showboard(){//show the whole board
    for(int i=0;i<ROW;i++){
        for(int j=COLUMN-1;j>=0;j--){
            if(board[i][j].whose=='x'){
                printf("(%d,%d)\033[34m%s \033[m",j+1,i+1,chesstype[board[i][j].name]);
            }else if(board[i][j].whose=='y'){
                printf("(%d,%d)\033[31m%s \033[m",j+1,i+1,chesstype[board[i][j].name]);
            }else{
                printf("(%d,%d)%s ",j+1,i+1,chesstype[board[i][j].name]); 
            }
            
        }
        printf("\n");
    }
}
int check_chess(int c_row,int c_col){//check boundary
    if((board[c_row-1][c_col-1].name==space) || c_row>=10 || c_col>=10 ){
        printf("No chess!\n");
        return 0;
    }
    //printf("check_chess ok!\n");
    return 1;
}
int compare(int a,int b){//compare big or small
    if(a>b){
        return 1;
    }
    else if(b>a){
        return 0;
    }
    return 3;//a=b
}
int between_kyousya(int c_row,int c_col,int n_row,char turn){//return 1 for can't move
    if(abs(c_row-n_row==1) && board[n_row-1][c_col-1].whose!=turn){
        return 0;       
    }
    if(compare(c_row,n_row)){
        if(board[n_row-1][c_col-1].whose==turn){
            return 1;
        }
        for(int i=n_row;i<c_row-1;i++){//c>n
            if(board[i][c_col-1].whose==turn || board[i][c_col-1].whose!='z'){
                //printf("ttt%c %c\n",turn,board[i][c_col-1].whose);
                return 1;
            }
            //return 0;
        }
        return 0;
    }
    if(board[n_row-1][c_col-1].whose==turn){
        return 1;
    }
    for(int i=c_row;i<n_row-1;i++){//n>c
        if(board[i][c_col-1].whose==turn ||  board[i][c_col-1].whose!='z'){
            //printf("kkk%c %c\n",turn,board[i][c_col-1].whose);
            return 1;
        }      
    }
    return 0;
}
int between_hisya(int c_row,int c_col,int n_row,int n_col,char turn){//return 1 for can't move
    if(board[n_row-1][n_col-1].whose==turn){
        return 1;
    }
    int higher_row=compare(c_row,n_row);// c>n=1,n>c=0,3 for equal
    int higher_col=compare(c_col,n_col);// c>n=1,n>c=0,3 for equal
    if(higher_row==1){//c>n
        if(c_row-n_row==1 && board[n_row-1][n_col-1].whose==turn){
            return 1;
        }
        for(int i=n_row;i<c_row-1;i++){
            if(board[i][c_col-1].whose==turn || board[i][c_col-1].whose!='z'){
                //printf("111%c %c\n",turn,board[i-1][c_col-1].whose);
                return 1;
            }
        }
        return 0;
    }else if(higher_row==0){//n>c
        if(n_row-c_row==1 && board[n_row-1][n_col-1].whose==turn){
            return 1;
        }
        for(int i=c_row;i<n_row-1;i++){
            if(board[i][c_col-1].whose==turn || board[i][c_col-1].whose!='z'){
                //printf("222%c %c\n",turn,board[i][c_col-1].whose);
                return 1;
            }
        }
        return 0;
    }else if(higher_col==1){//c>n
        if(c_col-n_col==1 && board[n_row-1][n_col-1].whose==turn){
            return 1;
        }
        for(int i=n_col;i<c_col-1;i++){
            if(board[c_row-1][i].whose==turn || board[c_row-1][i].whose!='z'){
                //printf("333%c %c\n",turn,board[c_row-1][i-1].whose);
                return 1;
            }
        }
        return 0;
    }else if(higher_col==0){//n>c
        if(n_col-c_col==1 && board[n_row-1][n_col-1].whose==turn){
            return 1;
        }
        for(int i=c_col;i<n_col-1;i++){
            if(board[c_row-1][i].whose==turn || board[c_row-1][i].whose!='z'){
                //printf("444%c %c\n",turn,board[c_row-1][i-1].whose);
                return 1;
            }
        }
        return 0;
    }
    return 1;
}
int between_kakugyou(int c_row,int c_col,int n_row,int n_col,char turn){//return 1 for can't move
    int dir=0;
    if(n_row>c_row && n_col>c_col){
        dir=1;
    }else if(n_row<c_row && n_col>c_col){
        dir=2;
    }else if(n_row>c_row && n_col<c_col){
        dir=3;
    }else if(n_row<c_row && n_col<c_col){
        dir=4;
    }
    if(board[n_row-1][n_col-1].whose==turn){
        return 1;
    }
    switch(dir){
        case 1:
            if(n_col-c_col==1 && n_row-c_row==1 && board[n_row-1][n_col-1].whose==turn){
                return 1;
            }
            for(int i=c_row;i<n_row-1;i++){
                if(board[i][i].whose==turn || board[i][i].whose!='z'){
                    //printf("aaaa\n");
                    return 1;
                }
            }
            return 0;
            break;
        case 2:
            if(n_col-c_col==1 && c_row-n_row==1 && board[n_row-1][n_col-1].whose==turn){
                return 1;
            }
            for(int i=1;i<=(n_col-c_col-1);i++){
                if(board[c_row-1-1*i][c_col-1+1*i].whose==turn || board[c_row-1-1*i][c_col-1+1*i].whose!='z'){
                    //printf("bbbb\n");
                    return 1;
                }
            }
            return 0;
            break;
        case 3:
            if(c_col-n_col==1 && n_row-c_row==1 && board[n_row-1][n_col-1].whose==turn){
                return 1;
            }
            for(int i=1;i<=(n_row-c_row-1);i++){
                if(board[c_row-1+1*i][c_col-1-1*i].whose==turn || board[c_row-1+1*i][c_col-1-1*i].whose!='z'){
                    //printf("cccc\n");
                    return 1;
                }
            }
            return 0;
            break;;
        case 4:
            if(c_col-n_col==1 && c_row-n_row==1 && board[n_row-1][n_col-1].whose==turn){
                return 1;
            }
            for(int i=n_row;i<c_row-1;i++){
                if(board[i][i].whose==turn || board[i][i].whose!='z'){
                    //printf("dddd\n");
                    return 1;
                }
            }
            return 0;
            break;
        default:
            return 1;
            break;
    }
}
void moving(int c_row,int c_col,int n_row,int n_col,char turn,enum chesstype type){//moving
    //current->space
    board[c_row-1][c_col-1].name=space;
    board[c_row-1][c_col-1].whose='z';
    //destination->current
    board[n_row-1][n_col-1].name=type;
    board[n_row-1][n_col-1].whose=turn;
    showboard();
}
int check_direction(int c_row,int c_col,int n_row,int n_col,char turn){// 0 for wrong 1 for right
    int wrong;
    if(c_row==n_row && c_col==n_col){
        return 0;
    }
    //int sameside=((board[n_row-1][n_col-1].whose=='x'&& turns%2==0) || (board[n_row-1][n_col-1].whose=='y'&& turns%2==1));
    if((board[n_row-1][n_col-1].whose==turn)){
        printf("You can't move! Please enter right destination! same\n");
        //printf("%d %d %d %d %c %c\n",c_col,c_row,n_col,n_row,board[c_row-1][c_col-1].whose,board[n_row-1][n_col-1].whose);
        return 0;    
    }     
    switch(board[c_row-1][c_col-1].name){
        case 0://
            if( (c_row!=n_row && c_col==n_col && n_row<c_row && turn=='x') ||  
                (c_row!=n_row && c_col==n_col && n_row>c_row && turn=='y')){ //can only move forward
                if(between_kyousya(c_row, c_col,n_row,turn)){//checking between space
                    printf("You can't move! Please enter right destination! \n");
                    return 0;
                    break;    
                }
                else{
                    //moving(c_row,c_col,n_row,n_col,turn,board[c_row-1][c_col-1].name);
                    //printf("check_dir ok!\n");
                    return 1;
                    break;
                }
            }
            printf("You can't move! Please enter right destination! \n");//can't move since turns
            return 0;
            break;
        case 1:
            if((c_row-n_row!=2 && c_col-n_col!=1 && turn=='x') || (c_row-n_row!=2 && n_col-c_col!=1 && turn=='x') ||
               (n_row-c_row!=2 && c_col-n_col!=1 && turn=='y') || (n_row-c_row!=2 && n_col-c_col!=1 && turn=='y') ){//only forward and right destination
                printf("You can't move! Please enter right destination! \n"); 
                return 0;
                break;  
            }
            else if(board[n_row-1][n_col-1].whose!=turn){
                //moving(c_row,c_col,n_row,n_col,turn,board[c_row-1][c_col-1].name);
                return 1;
                break;
            }
            printf("You can't move! Please enter right destination! \n");
            return 0;
            break;
        case 2:
            if( turn=='x' && ( abs(n_row-c_row)>=2 || abs(n_col-c_col)>=2 ||
                (n_row==c_row && n_col-c_col==1) || (n_row==c_row && c_col-n_col==1) || (n_col==c_col && n_row-c_row==1) ) ){
                printf("You can't move! Please enter right destination!?? \n");
                return 0;
                break;
            }
            else if( turn=='y' && (abs(n_row-c_row)>=2 || abs(n_col-c_col)>=2 ||
                (n_row==c_row && n_col-c_col==1) || (n_row==c_row && c_col-n_col==1) || (n_col==c_col && c_row-n_row==1)) ){//
                printf("You can't move! Please enter right destination!?? \n");
                return 0;
                break;
            }
            else if(board[n_row-1][n_col-1].whose!=turn){
                //moving(c_row,c_col,n_row,n_col,turn,board[c_row-1][c_col-1].name);
                return 1;
                break;
            }
            printf("You can't move! Please enter right destination!112 \n");
            return 0;
            break;
        case 3:
            if( turn=='x' && (abs(n_row-c_row)>=2 || abs(n_col-c_col)>=2 ||
              (n_col-c_col==1 && n_row-c_row==1) || (c_col-n_col==1 && n_row-c_row==1) ) ){//only around
                printf("You can't move! Please enter right destination!?? \n");
                return 0;
                break;
            }
            else if( turn=='y' && (abs(n_row-c_row)>=2 || abs(n_col-c_col)>=2 ||
              (n_col-c_col==1 && c_row-n_row==1) || (c_col-n_col==1 && c_row-n_row==1) ) ){//only around
                printf("You can't move! Please enter right destination!?? \n");
                return 0;
                break;
            }
            else if(board[n_row-1][n_col-1].whose!=turn){
                //moving(c_row,c_col,n_row,n_col,turn,board[c_row-1][c_col-1].name);
                return 1;
                break;
            }
            printf("You can't move! Please enter right destination!112 \n");
            return 0;
            break;
        case 4:
            if(abs(n_row-c_row)>=2 || abs(n_col-c_col)>=2  ){//only around
                printf("You can't move! Please enter right destination! \n");
                return 0;
                break;
            }
            else if(board[n_row-1][n_col-1].whose!=turn){
                //moving(c_row,c_col,n_row,n_col,turn,board[c_row-1][c_col-1].name);
                return 1;
                break;
            }
            printf("You can't move! Please enter right destination! \n");
            return 0;
            break;
        case 5:
            if( (c_row!=n_row || c_col!=n_col) || (c_row==n_row && c_col==n_col) ){//check whether cross route
                if(between_hisya(c_row, c_col,n_row,n_col,turn)){//check between space
                    printf("You can't move! Please enter right destination! \n"); 
                    return 0;
                    break; 
                }
                else{
                    //moving(c_row,c_col,n_row,n_col,turn,board[c_row-1][c_col-1].name);
                    return 1;
                    break;    
                }  
            }
            printf("You can't move! Please enter right destination!  \n"); 
            return 0;
            break;
        case 6:
            if(abs(n_row-c_row)/abs(n_col-c_col)==1){//only cross
                if(between_kakugyou(c_row, c_col,n_row,n_col,turn)){//check between space
                    printf("You can't move! Please enter right destination! \n"); 
                    return 0;
                    break; 
                }
                else{
                    //moving(c_row,c_col,n_row,n_col,turn,board[c_row-1][c_col-1].name);
                    return 1;
                    break;    
                }  
                
            }
            printf("You can't move! Please enter right destination! 11 \n"); 
            return 0;
            break;
        case 7:
            if(( ((c_row-n_row!=1) || (c_col!=n_col)) && (turn=='x')) ||  ((n_row-c_row!=1 || c_col!=n_col) && turn=='y')){//only forward
                printf("You can't move! Please enter right destination! \n");
                return 0;
                break;
            }
            else if(board[n_row-1][n_col-1].whose!=turn){
                //moving(c_row,c_col,n_row,n_col,turn,board[c_row-1][c_col-1].name);
                return 1;
                break;
            }
            printf("You can't move! Please enter right destination! \n");
            return 0;
            break;
        default:
            printf("Wrong direction!!5\n");
            wrong=0;
            break;
    }
    return wrong;
}
int game_finish(){
    int ousyou_num=0;
    for(int i=0;i<ROW;i++){
        for(int j=0;j<COLUMN;j++){
            if(board[i][j].name==ousyou){
                ousyou_num+=1;
            }
        }
    }
    if(ousyou_num!=2){
        printf("Game finished!!\n");
        return 0;
    }
    return 1;
}
void recover(struct stk *sptr,struct stk s,int *top){
    printf("Recover!\n");
    board[sptr[*top+1].c_row-1][sptr[*top+1].c_col-1].name=sptr[*top+1].kind;
    board[sptr[*top+1].c_row-1][sptr[*top+1].c_col-1].whose=sptr[*top+1].whose;

    board[sptr[*top+1].n_row-1][sptr[*top+1].n_col-1].name=sptr[*top+1].n_kind;
    board[sptr[*top+1].n_row-1][sptr[*top+1].n_col-1].whose=sptr[*top+1].n_whose;
    
    s.c_row=sptr[*top].c_row; s.c_col=sptr[*top].c_col; s.n_row=sptr[*top].n_row; s.n_col=sptr[*top].n_col;
    s.whose=sptr[*top].whose; s.n_whose=sptr[*top].n_whose;
    s.kind=sptr[*top].kind; s.n_kind=sptr[*top].n_kind;
    
    
}
void save(struct stk *sptr,FILE *fp,int *top){
    for(int i=0;i<=*top;i++){
        fprintf(fp," %d %d %d %d \n",
        sptr[i].c_col,sptr[i].c_row,sptr[i].n_col,sptr[i].n_row);
    }
}
void readmove(struct stk *sptr,int track){
  
    board[sptr[track].c_row-1][sptr[track].c_col-1].name=space;
    board[sptr[track].c_row-1][sptr[track].c_col-1].whose='z';
    board[sptr[track].n_row-1][sptr[track].n_col-1].name=sptr[track].kind;
    board[sptr[track].n_row-1][sptr[track].n_col-1].whose=sptr[track].whose;
}
void readback(struct stk *sptr,int track2){
  
    board[sptr[track2].c_row-1][sptr[track2].c_col-1].name=sptr[track2].kind;
    board[sptr[track2].c_row-1][sptr[track2].c_col-1].whose=sptr[track2].whose;
    board[sptr[track2].n_row-1][sptr[track2].n_col-1].name=sptr[track2].n_kind;
    board[sptr[track2].n_row-1][sptr[track2].n_col-1].whose=sptr[track2].n_whose;
}


int main(int argc,char *argv[]){
    //initial for stack.c 
    control=0;
    struct ev_loop *loop = EV_DEFAULT;//??????????????????????????????????????????????????????????????????????????
    now=ev_now(loop);
    struct stk record[250];
    struct stk temp;
    struct stk readmode[250];
    int top=-1;int *topptr=&top;
    //initial for stack.c 
    FILE *fptr=NULL;
    FILE *readfptr=NULL;
    //fptr = fopen("new_game.txt","r+");
    int start_load;
    printf("Let's play shogi !\n");
     
    if(argc==4 ){
        
        fptr = fopen("new_game.txt","w+");
        ev_io_init(&stdin_watcher, stdin_cb, /*STDIN_FILENO*/ 0, EV_READ);
	    ev_io_start(loop, &stdin_watcher);
	    ev_timer_init(&timeout_watcherx, timeout_cbx, 0, 1);
        ev_timer_init(&timeout_watchery, timeout_cby, 0, 1);
        int turns=0;//%2,=0->x,=1>y
        char turn;//x^y^z
        startgame();
        showboard();
        int current_row;  int current_column;
        int next_row;     int next_column;
        int saved=0; 
        char modesave;
        while(game_finish() && saved==0){//keep until game_finish==0 or save mode
            
            printf("Do you want to save file?(press s for yes n for no) ");
            scanf(" %c",&modesave);
            if(turns%2==0){
                turn='x';   
            }else{
                turn='y';          
            }
            

            if(modesave=='s' ){//|| modesave=='S'
                saved=1;
                printf("Saved!\n");
                ev_timer_stop (loop, &timeout_watcherx);	
                ev_timer_stop (loop, &timeout_watchery);
                save(record,fptr,topptr);
            }
            else{
                printf("The turn is %c now!\n",turn);
                printf("Please enter current column : ");
                scanf("%d", &current_column);
                int current_col2=current_column;
                if(top==-1 && current_column==0 ){
                    printf("You can't regret! Please enter the current/next destination! \n");
                    printf("Please enter current column : ");scanf("%d", &current_column);
                    current_col2=current_column;
                }
                if(current_column==0 && turns!=0){//regretting
                    regret(record,temp,topptr);//pop data
                    recover(record,temp,topptr);//recover board
                    showboard();
                    turns-=1;
                }
                else{
                    printf("Please enter current row : ");scanf("%d", &current_row);
                    printf("Please enter next column : ");scanf("%d", &next_column);
                    printf("Please enter next row : ");   scanf("%d", &next_row); 
                }
                //loading into struct

                if(check_chess(current_row,current_column) && check_direction(current_row,current_column,next_row,next_column,turn) && current_column!=0 && current_column!='s'){
                    temp.c_row=current_row;temp.c_col=current_col2;temp.n_row=next_row;temp.n_col=next_column;
                    temp.whose=turn; temp.n_whose=board[next_row-1][next_column-1].whose;
                    temp.kind=board[current_row-1][current_col2-1].name; temp.n_kind=board[next_row-1][next_column-1].name;
                    moving(current_row,current_column,next_row,next_column,turn,board[current_row-1][current_column-1].name); 
                    history_record(record,temp,topptr);//put into stack
                    turns+=1;
                    
                    if(control%2==0){
                        ev_timer_start(loop, &timeout_watcherx);//////
                    }else{
                        ev_timer_start(loop, &timeout_watchery);
                    }
                    ev_run(loop, 0); /* now wait for events to arrive *///
                }
            }
        }
        ev_timer_stop (loop, &timeout_watcherx);
        ev_timer_stop (loop, &timeout_watchery);
        
    
    }else if(argc==3){
        char nextorlast;
        int top2=-1;
        int track=0;
        int track_mod=0;
        startgame();
        showboard();
        readfptr=fopen("new_game.txt","r+");
        if(readfptr == NULL){
		    printf("open txt fail!!\n");	
		    return -1;
	    }
        printf("Please decide the next step, press f for next step or b for last step :\n");
        scanf(" %c",&nextorlast);
        int temp;
        int i=0;
        int test;
        while((fscanf(readfptr,"%d",&temp))!=EOF){
            switch(i%4){
                case 0:
                    readmode[i/4].c_col=temp;
                    top2+=1;
                    break;
                    
                case 1:
                    readmode[i/4].c_row=temp;
                    top2+=1;
                    break;
                case 2:
                    readmode[i/4].n_col=temp;
                    top2+=1;
                    break;
                case 3:
                    readmode[i/4].n_row=temp;
                    top2+=1;
                    break;
                default:
                    break;
            }
            i+=1;
        }
        int complish=0;
        while(1){
            if(nextorlast=='f'){
                if(track_mod>top2){
                    printf("The all record is finished!\n");
                    break;
                }else{
                    readmode[track].whose=board[readmode[track].c_row-1][readmode[track].c_col-1].whose;
                    readmode[track].kind=board[readmode[track].c_row-1][readmode[track].c_col-1].name;
                    readmode[track].n_whose=board[readmode[track].n_row-1][readmode[track].n_col-1].whose;
                    readmode[track].n_kind=board[readmode[track].n_row-1][readmode[track].n_col-1].name;
                    readmove(readmode,track);
                    showboard();
                    track+=1;
                    track_mod+=4;
                    //printf("\ntrack: %d\n",track);
                    //printf("\ntrack_mod: %d\n",track_mod);
                }
            }else if(nextorlast=='b'){
                if(track_mod>0){
                    readmode[track].whose=board[readmode[track].c_row-1][readmode[track].c_col-1].whose;
                    readmode[track].kind=board[readmode[track].c_row-1][readmode[track].c_col-1].name;
                    readmode[track].n_whose=board[readmode[track].n_row-1][readmode[track].n_col-1].whose;
                    readmode[track].n_kind=board[readmode[track].n_row-1][readmode[track].n_col-1].name;
                    readback(readmode,track-1);
                    showboard();
                    track-=1;
                    track_mod-=4;
                    //printf("\ntrack: %d\n",track);
                    //printf("\ntrack_mod: %d\n",track_mod);
                }
                else{
                    printf("This is first step, you can't go back!\n");
                    break;
                }                
            }else{
                printf("Please press correct command, press f for next step or b for last step :\n");
                scanf(" %c",&nextorlast);    
            }
            printf("Please decide the next step, press f for next step or b for last step :\n");
            scanf(" %c",&nextorlast);  
        }
        
    
}
    /*fclose(fptr);
    fclose(readfptr);*/
    return 0;
}


