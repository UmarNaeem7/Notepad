//built in code blocks ide

#include<iostream>                    
#include<graphics.h>                    //FOR BGI GRAPHICS
#include<process.h>                     
#include<stdlib.h>                      
#include<conio.h>                       //FOR getch()
#include<fstream>                     
#include<string.h>                     
#include<ctype.h>                       //FOR CHARACTER FUNCTIONS
#include<dos.h>                         //FOR delay()
using namespace std;

ifstream fin;                      //FILE INPUT STREAM
ofstream fout;                     //FILE OUTPUT SRTEAM


struct line                               
{
char string[101];          //TO STORE A LINE
int len,x,y;                        //LENGTN OF LINE,CURRENT x-position OF CURSOR
struct line * down,*up;             //POINTERS TO LINK UPPER AND LOWER LINE
}*temp,*head=NULL,*current,*last=NULL;  //GLOBAL VARIABLES

int xmax,ymax,i=-1,curri=0,currx=20,ins=0,top=0,save=1,iter=1;

// xmax = MAX VALUE OF PIXELS IN HORIZONTAL DIRECTION
// ymax = MAX VALUE OF PIXELS IN VERTICAL DIRECTION
// i STORES 1 LESS THAN NO. OF NODES IN DLL
// curri STORES THE SERIAL NO. OF CURRENT WORKING NODE
// currx STORES THE CURRENT VALUE OF x IN PIXELS
// ins KEEPS TRACK OF WHWTHER 'insert' BUTTON IS PRESSED
// top STORES THE SERIAL NO. OF TOPMOST LINE ON EDITOR WINDOW
// save KEEPS TRACK WHETHER THE CURRENTLY EDITED FILE IS SAVED OR NOT
// iter IS INCREMENTED IF 'OPEN' FUNCTION IS USED

char file[20];                     //TO STORE FILE NAME
//ifstream fin;                      //FILE INPUT STREAM
//ofstream fout;                     //FILE OUTPUT SRTEAM

/* THIS FUNCTION CALCULATES CURRENT CURSOR POS^N IN PIXELS
THIS HAS TO BE DONE BCOZ CHARACTER WIDTH IS DIFFERENT IN WINDOWS   */
void calcx()
{
currx=20;                          //INITIALIZE calcx
for(int j=0;j<current->x;j++)
{
if(current->string[j]=='>' || current->string[j]==':'||current->string[j]==';'||current->string[j]=='f'||current->string[j]=='i'||current->string[j]=='l'||current->string[j]=='t')
currx+=5;
else if(current->string[j]=='!')
currx+=2;
else if(current->string[j]=='%')
currx+=7;
else if(current->string[j]=='.')
currx+=4;
else if(current->string[j]=='"'||current->string[j]=='('||current->string[j]==')'||current->string[j]=='['||current->string[j]==']')
currx+=5;
else
currx+=6;
}
}

/* THIS FUNCTION DISPLAYS CURSOR AT REQUIRED POSITION*/
void cursor()
{
setcolor(RED);
outtextxy(currx,(curri-top)*12+23,"_");
outtextxy(currx,(curri-top)*12+24,"_");
}

/* THIS FUNCTION IS TO DISPLAY THE STATUS BAR AT RIGHTMOST
EDGE OF EDITOR SCREEN */
void statusbar()
{
setfillstyle(1,7);
bar(xmax-9,ymax/28,xmax-1,27*ymax/28);   //DRAW DARKGRAY BAR
float percent,bartop,barbot;
if(i<=36)
percent=1;
else
percent=36.0/i;
if(i!=0)
bartop=ymax/28+1+(1.0*top/i)*(26*ymax/28);
else
bartop=ymax/28+1;
barbot=bartop+percent*(26*ymax/28)-1;
if(barbot>=27*ymax/28)
barbot=27*ymax/28-1;
setfillstyle(1,8);
bar(xmax-7,(int)bartop,xmax-3,(int)barbot);     //DRAW LIGHTGRAY BAR
}

/* THIS FUNCTION DISPLAYS THE CURRENT STATUS OF FILE BEING EDITED*/
void position()
{
char row[10],column[10];
int rowframe[]={xmax-90,27*ymax/28+1,xmax-69,27*ymax/28+1,xmax-69,ymax-1,xmax-90,ymax-1,xmax-90,27*ymax/28};
int colframe[]={xmax-20,27*ymax/28+1,xmax-1,27*ymax/28+1,xmax-1,ymax-1,xmax-20,ymax-1,xmax-20,27*ymax/28};
setcolor(7);
setfillstyle(1,7);
fillpoly(5,rowframe);              // TO CLEAR PREVIOUSLY WRITTEN DATA
fillpoly(5,colframe);           //         -- DO --
setcolor(8);
itoa(curri+1,row,10);           // CONVERT FORM INTEGER TO ARRAY
itoa(current->x+1,column,10);   //         -- DO --
outtextxy(xmax-120,ymax-15,"Line: ");
outtextxy(xmax-65,ymax-15,"Column: ");
setcolor(RED);
outtextxy(xmax-90,ymax-15,row);       // PRINT CURRENT ROW
outtextxy(xmax-20,ymax-15,column);    // PRINT CURRENT COLUMN
if(ins)
setcolor(RED);
else
setcolor(7);
outtextxy(xmax-70,3,"insert");        //PRINT insert IF INSERT IS ON
bar(250,1,340,ymax/28-1);
if(save==1)
{
setcolor(8);
outtextxy(250,3,"Ready to edit"); //IF FILE IS SAVED PRINT THIS
}
else
{
setcolor(RED);
outtextxy(250,3,"   Not saved");  //ELSE PRINT NOT SAVED
}
statusbar();                          //DISPLAY STATUS BAR
}

/* THIS FUNCTION DRAWS THE BASIC EDITOR SCREEN*/
void disp_editscr()
{
xmax=getmaxx();
ymax=getmaxy();
int editwin[]={1,ymax/28,xmax-1,ymax/28,xmax-1,27*ymax/28,1,27*ymax/28,1,ymax/28};
cleardevice();                      // TO CLEAR THE GRAPHICS SCREEN
setcolor(7);
setbkcolor(8);
setfillstyle(1,7);
bar(1,1,xmax-1,ymax/28);             //NAME FRAME
bar(1,27*ymax/28,xmax-1,ymax-1);    //POSITION FRAME
drawpoly(5,editwin);
settextstyle(2,0,4);
setcolor(8);
outtextxy(3,3,"File :");                //DISPLAY VARIOUS MESSAGES
outtextxy(15,ymax-15," Save");          //        - DO -
outtextxy(78,ymax-15," Save as");       //        - DO -
outtextxy(158,ymax-15," Open");         //        - DO -
outtextxy(215,ymax-15," Quit");         //        - DO -
setcolor(RED);
outtextxy(40,3,file);
outtextxy(3,ymax-15,"F1 Save");              //        - DO -
outtextxy(66,ymax-15,"F2 Save as");             //        - DO -
outtextxy(146,ymax-15,"F3 Open");            //        - DO -
outtextxy(203,ymax-15,"F4 Quit");            //        - DO -
}

/* REFRESH THE FULL EDITOR SCREEN*/
void disp_full()
{
int j=curri;
setcolor(8);
setfillstyle(1,8);
bar(2,ymax/28,xmax-2,27*ymax/28);         //DRAW THE BACKGROUND
struct line *temp1;
temp1=current;
while(j>top)                       //START DISPLAYING FROM SERIAL NO. TOP
{
temp1=temp1->up;
j--;
}
int ycor=20;
setcolor(7);
settextstyle(2,0,4);
while(temp1!=NULL&&j<top+36)           //DISPLAY EACH LINE
{
outtextxy(20,ycor,temp1->string);
ycor+=12;
temp1=temp1->down;
j++;
}
calcx();
cursor();                           //DISPLAY CURSOR AT NEW POSITION
position();                         //DISPLAY UPDATED FILE STATUS
}

/* REFRESH THE LOWER PART ONLY*/
void disp_rem()
{
setcolor(8);
setfillstyle(1,8);
bar(2,(curri-top)*12+21,xmax-2,27*ymax/28);          //REMAINING WINDOW
setcolor(7);
temp=current;
int ycor=(curri-top)*12+20;
int j=curri;
while(temp!=NULL&&j<top+36)           //DISPLAY EACH LINE
{
outtextxy(20,ycor,temp->string);
ycor+=12;
temp=temp->down;
j++;
}
calcx();
cursor();                           //DISPLAY CURSOR AT NEW POSITION
position();                         //DISPLAY UPDATED FILE STATUS
}

/* TO REFRESH ONLY THREE LINES : THE CURRENT ONE, UPPER ONE AND LOWER ONE*/
void disp_three()
{
int subeditwin[]={2,(curri-top)*12+22,xmax-2,(curri-top)*12+22,xmax-2,(curri-top)*12+46,2,(curri-top)*12+46,2,(curri-top)*12+22};
setcolor(8);
setfillstyle(1,8);
fillpoly(5,subeditwin);
setcolor(7);
outtextxy(20,(curri-top)*12+20,current->string);      //DISPLAY THE LINES
outtextxy(20,(curri-top)*12+32,current->down->string); //    - DO -
calcx();
cursor();                           //DISPLAY CURSOR AT NEW POSITION
position();                         //DISPLAY UPDATED FILE STATUS
}

/* TO COPY AN EXISTING FILE INTO LINKED LIST ON OPENING THE FILE*/
void copy()
{
do
{
temp=new (struct line);         //MAKE A NEW NODE
fin. get(temp->string,101);     //READ LINE FROM FILE
temp->x=0;                      // CALCULATE VARIOUS PARAMETERS
temp->len=strlen(temp->string); //          "
temp->string[temp->len]='\0';   //            "
temp->y=i++;                    //            "
if(head==NULL)                  // MAKE THE LIST
{
temp->up=NULL;
head=temp;
}
else
{
last->down=temp;
temp->up=last;
}
temp->down=NULL;
last=temp;
current=temp;
if(fin.get()==EOF)              //CLOSE THE FILE
{
fin.close();
break;
}
}while(1);
}

/* TO HANDLE THE UP ARROW OPERATION */
void uparrow()
{
int subeditwin[]={2,(curri-top)*12+34,xmax-2,(curri-top)*12+34,xmax-2,(curri-top)*12+46,2,(curri-top)*12+46,2,(curri-top)*12+34};
setcolor(8);
setfillstyle(1,8);
fillpoly(5,subeditwin);
setcolor(7);
outtextxy(20,(curri-top)*12+32,current->down->string);
calcx();
cursor();
position();
}

/* TO HANDLE THE DOWN ARROW OPERATION */
void downarrow()
{
int subeditwin[]={2,(curri-top)*12+10,xmax-2,(curri-top)*12+10,xmax-2,(curri-top)*12+22,2,(curri-top)*12+22,2,(curri-top)*12+10};
setcolor(8);
setfillstyle(1,8);
fillpoly(5,subeditwin);
setcolor(7);
outtextxy(20,(curri-top)*12+8,current->up->string);
calcx();
cursor();
position();
}

/* TO HANDLE THE SIDE ARROW OPERATION */
void sidearrow()
{
int subeditwin[]={2,(curri-top)*12+22,xmax-2,(curri-top)*12+22,xmax-2,(curri-top)*12+34,2,(curri-top)*12+34,2,(curri-top)*12+22};
setcolor(8);
setfillstyle(1,8);
fillpoly(5,subeditwin);
setcolor(7);
outtextxy(20,(curri-top)*12+20,current->string);
calcx();
cursor();
position();
}

/* TO HANDLE THE ENTER KEY OPERATION */
void newline()
{
int j;
temp=new (struct line);           //MAKE NEW NODE
temp->up=current;                 // FILL APPROPRIATE DATA
temp->down=current->down;
if(current->down!=NULL)
current->down->up=temp;
else
last=temp;
current->down=temp;
for(j=current->x;j<=current->len;j++)
temp->string[j-current->x]=current->string[j];
current->string[current->x]='\0';
temp->len=current->len-current->x;
current->len=current->x;
sidearrow();
current=temp;
current->x=0;
current->y=curri++;               //UPDATE GLOBAL VARIABLES
i++;
if(curri==top+36)
{
top++;
disp_full();
}
else
disp_rem();
}

void home()             /* TO HANDLE THE 'HOME' OPERATION */
{
current->x=0;
sidearrow();
}
void end()              /* TO HANDLE THE 'END' OPERATION */
{
current->x=current->len;
sidearrow();
}
void del()              /* TO HANDLE THE DELETE OPERATION */
{
int j;
if(curri!=i&&current->x==current->len)
{
temp=current;
if(current->len+current->down->len<=100)
{
if(current!=head)
{
current->down->up=current->up;
current->up->down=current->down;
}
else
{
current->down->up=NULL;
head=head->down;
}
current=current->down;
current->x=temp->len;
current->len+=temp->len;
strcat(temp->string,current->string);
strcpy(current->string,temp->string);
i--;
delete(temp);
}
else
{
setcolor(4);
outtextxy(xmax/2+100,3,"Next line is too long");
}
disp_rem();
}
else if(current->x<current->len)
{
for(j=current->x;j<current->len;j++)
current->string[j]=current->string[j+1];
current->len--;
sidearrow();
}
else
{
setcolor(4);
outtextxy(xmax/2+100,3,"Nothing to delete");
}
}

void validchar(char typed)         //TO INSERT A VALID CHARACTER IN LINE
{
if(current->len<100)
{
for(int j=current->len-1;j>=current->x;j--)
current->string[j+1]=current->string[j];
if((int)typed==34)
current->string[current->x]='\"';
else
current->string[current->x]=typed;
current->x++;
current->len++;
current->string[current->len]='\0';
sidearrow();
}
else
{
setcolor(4);
outtextxy(xmax/2+100,3,"Maximum limit reached");
}
}
void insert(char typed)      //THIS FUNCTION IS USED WHEN INSERT KEY IS ON
{
if((int)typed==34)
current->string[current->x]='\"';
else
current->string[current->x]=typed;
if(current->x<current->len)
current->x++;
else
{
current->x++;
current->len++;
current->string[current->len]='\0';
}
sidearrow();
}

void bkspace()     // TO HANDLE THE BKSPACE OPERATION
{
if(curri!=0&&current->x==0)
{
temp=current;
if(current->len+current->up->len<=100)
{
if(current->down!=NULL)
{
current->down->up=current->up;
current->up->down=current->down;
}
else
{
current->up->down=NULL;
last=current->up;
}
current=current->up;
current->x=current->len;
strcat(current->string,temp->string);
current->len=strlen(current->string);
curri--;
i--;
delete(temp);
}
else
{
setcolor(4);
outtextxy(xmax/2+100,3,"Proceeding line is too long");
}
if(curri==top-1)
{
top--;
disp_full();
}
else
disp_rem();
}
else if(current->x!=0)
{
for(int j=current->x-1;j<current->len;j++)
current->string[j]=current->string[j+1];
current->x--;
current->len--;
sidearrow();
}
else if(current->x==0)
{
setcolor(4);
outtextxy(xmax/2+100,3,"Nowhere to go");
}
}

void pageup()              // TO HANDLE THE PAGE UP OPERATION
{
if(top>35)
{
top-=36;
curri-=36;
for(int j=0;j<36;j++)
current=current->up;
}
else
{
top=0;
curri=0;
current=head;
}
disp_full();
calcx();
position();
cursor();
}

void pagedown()         // TO HANDLE THE PAGE DOWN OPERATION
{
if(top+35<i)
{
top+=36;
curri+=36;
for(int j=0;j<36;j++)
current=current->down;
}
else
{
top=i;
curri=i;
current=last;
}
disp_full();
calcx();
position();
cursor();
}

void getfile(char *nam,int n)        // GET NEW FILE NAME
{
setfillstyle(1,7);
setcolor(7);
bar(40,90,600,140);              //FILE FREME
setcolor(RED);
settextstyle(1,0,3);
if(n==1)
outtextxy(50,100," ENTER FILE NAME :");
else if(n==0)
outtextxy(50,100,"  SAVE AS FILE : ");
else
outtextxy(50,100,"ENTER NEW FILE NAME :");

int j=-1;
l1:do
{
nam[++j]= getch();
if((int)nam[j]==8&&j!=0)
j-=2;
nam[j+1]='\0';
setfillstyle(1,7);
setcolor(7);
bar(330,90,600,140);        //STATE FRAME
setcolor(BLUE);
outtextxy(330,100,nam);
}while((int)nam[j]!=13);
nam[j]='\0';
if(j==0)
goto l1;
}

void savefile()            // TO SAVE THE FILE CURRENTLY BEING EDITED
{
    ifstream fin;                      //FILE INPUT STREAM
ofstream fout;                     //FILE OUTPUT SRTEAM

temp=head;
fout.open(file,ios::out);
while(temp!=NULL)
{
fout. write(temp->string,temp->len);
if(temp->down!=NULL)
fout. put('\n');
temp=temp->down;
}
fout.close();
save=1;
calcx();
position();
}

void save_as()            // TO HANDLE SAVE AS FUNCTION
{
    ifstream fin;                      //FILE INPUT STREAM
ofstream fout;                     //FILE OUTPUT SRTEAM

char file_as[15];
getfile(file_as,0);
temp=head;
fout.open(file_as,ios::out);
while(temp!=NULL)
{
fout. write(temp->string,temp->len);
if(temp->down!=NULL)
fout. put('\n');
temp=temp->down;
}
fout.close();
delay(500);
disp_full();
}

void quit()                //TO HANDLE THE QUIT OPERATION
{
char ch;
if(save==0)
{
setfillstyle(1,7);
setcolor(7);
bar(40,90,600,140);
setcolor(RED);
settextstyle(1,0,3);
outtextxy(50,100,"WANT TO SAVE CHANGES (Y/N):");
ch=getch();
if(tolower(ch)=='y')
savefile();
}
do                        //FREE ALL THE ALLOCATED MEMORY
{
temp=head;
head=head->down;
delete(temp);
}while(temp!=NULL);
}

void newfile();

//  THIS FUNCTION IS THE MAIN DRIVER OF THE PROGRAM. IT USES VARIOUS
//FUNCTIONS DEFINED ABOVE TO EXECUTE THE TASK
void editor()
{

getfile(file,iter);            //GET FILENAME TO BE OPENED
int j,tp,bm,lt,rt;
char typed;
fin.open(file,ios_base::in);       //OPEN THE FILE IN READ MODE
if(!fin)                                // IF CAN'T OPEN
{                                       // THEN CREAYE NEW FILE
setcolor(GREEN);
settextstyle(3,0,2);
outtextxy(150,250,"CREATING NEW FILE ");
outtextxy(350,250,file);
settextstyle(2,0,6);
outtextxy(350,400,"PRESS ANY KEY TO CONTINUE...");
getch();
current=new (struct line);           //MAKE A NODE FOR WORKING
current->up=NULL;                    //ASSIGN VARIOUS PARAMETERS
current->down=NULL;
i++;
current->x=0;
current->y=0;
current->len=0;
current->string[0]='\0';
head=current;
}
else                               // IF FILE OPENS
{
copy();                        //COPY ITS CONTENT TO DLL
fin.close();
current=head;
}
calcx();
disp_editscr();                   //DRAW EDITOR SCREEN
disp_full();                      // DISPLAY TEXT ON SCREEN
int check;
do                 // NOW DO THE FOLLOWING FOR ANY KEY PRESSED BY USER
{
check=getch();
if((check>=32&&check<=126))     //IF VALID/NON-CONTROL CHARACTER
{
typed=(char)check;          //CONVERT IT TO CHARACTER
check=-1;
}
setcolor(7);
setfillstyle(1,7);
bar(xmax/2+100,1,xmax-1,ymax/28-1);      // MESSAGE FRAME
switch(check)
{
case 13:save=0;             // IF ENTER KEY IS PRESSED
newline();          // CALL THIS FUNCTION
break;
case 0 :typed=getch();      // A SPECIAL KEY IS PRESSED
switch(typed)
{
case 72:if(curri>0)    // UP ARROW PRESSED
{
current=current->up;
curri--;
if(curri==top-1)
{
top--;
disp_full();
}
else
uparrow();
}
else
{
setcolor(4);
outtextxy(xmax/2+100,3,"Already at the top");
}
break;
case 80:if(curri<i)          //DOWN ARROW PRESSED
{
current=current->down;
curri++;
if(curri==top+36)
{
top++;
disp_full();
}
else
downarrow();
}
else
{
setcolor(4);
outtextxy(xmax/2+100,3,"Already at the bottom");
}
break;
case 75:if(current->x!=0)    //LEFT ARROW PRESSED
{
current->x-=1;
sidearrow();
}
else
{
setcolor(4);
outtextxy(xmax/2+100,3,"Already at left margin");
}
break;
case 77:if(current->x<current->len)  //RIGHT ARROW PRESSED
{
current->x+=1;
sidearrow();
}
else
{
setcolor(4);
outtextxy(xmax/2+100,3,"Already at right margin");
}
break;
case 83:save=0;                  //DELETE KEY PRESSED
del();
break;
case 82:if(!ins)                 //INSERT KEY PRESSED
ins=1;
else
ins=0;
position();
break;
case 71:home();                  //HOME KEY PRESSED
break;
case 79:end();                   //END KEY PRESSED
break;
case 73:pageup();                //PAGE_UP KEY PRESSED
break;
case 81:pagedown();              //PAGE_DOWN KEY PRESSED
break;
case 59:savefile();              //F1 PRESSED
break;
case 60:save_as();               //F2 PRESSED
break;
case 61:newfile();               //F3 PRESSED
break;
case 62:quit();                  //F4 PRESSED
closegraph();
exit(0);
break;
}
break;
case -1:save=0;       //INSERT THE VALID CHARACTER IN THE CURRENT LINE
if(!ins)
validchar(typed);
else
insert(typed);
break;
case 8 :save=0;       //BACK SPACE PRESSED
bkspace();
break;
}
}while(1);
}

// TO OPEN A NEW FILE WHILE EDITING
void newfile()
{
    ifstream fin;                      //FILE INPUT STREAM
ofstream fout;                     //FILE OUTPUT SRTEAM

quit();                 //QUIT FROM CURRENT FILE
head=NULL;              //RESET ALL GLOBAL VARIABLES
last=NULL;
i=-1;
curri=0;
currx=20;
ins=0;
top=0;
save=1;
iter++;
cleardevice();          //START FROM BEGINING FOR NEW FILE
setbkcolor(8);
delay(500);
editor();
}

// THE MAIN FUNCTION
int main()
{
ifstream fin;                      //FILE INPUT STREAM
ofstream fout;                     //FILE OUTPUT SRTEAM

initwindow(700,700,"sample");

editor();          // CALLING THE MAIN DRIVER FUNCTION
}
