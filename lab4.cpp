#include <iostream>
#include <queue>
#include <fstream>
#include <unistd.h>
#include <fcntl.h>
using namespace std;
Process manager[9];
int no_processes;
string visual_choice;
int timer;
string process_carrier[30][30];
int scheduling_policy[5];
int last_instance;
struct Process{
string name;
int no;
int arrival_time;
int service_time;
};
Process* constructor(Process p,string name,int arrival_time,int service_time){
p.name=name;
p.arrival_time=arrival_time;
p.service_time=service_time;
return p;
}
void FCFS(){
timer=0;
queue<Process> temp;
Process running=temp.pop(manager[0]);
while(!temp.empty()&& running.service_time!=0){
process_carrier[]
for(i=0;i<number){


}

}
timer+=1;
}
void RR(){}
void SPN(){}
void SRT(){}
void HRRN(){}
void FB_1(){}
void FB_2i(){}
void Aging(){}
void policy_selector(int policy){

switch (policy){
case 1: FCFS();
        break;
case 2: RR();
        break;
case 3: SPN();
        break;
case 4: SRT();
        break;
case 5: HRRN();
        break;
case 6: FB_1();
        break;
case 7: FB_2i();
        break;
case 8: Aging();
        break;
}
}
void newline(){
cout << endl;
}

void trace_printer(){
string intial_space="------";
cout << visual_choice;
int visual_choice_space=visual_choice.length();
for (int i=visual_choice_space;i<6;i++){
cout << " ";
}
for (int i=0;i<timer;i++){
int instance=i;
while(instance>9){
instance-=10;
}
cout << instance;
cout << " ";
}
newline();
cout << intial_space;
for (int i=0;i<timer;i++){
cout << "--";
}
newline();
for (int i=0;i<no_processes;i++){
string test="A";
cout << test;
int visual_choice_space=test.length();
for (int k=visual_choice_space;k<6;k++){
cout << " ";
}
for (int j=0;j<timer;j++){
cout << "|";
if(j==timer-1){
cout << " ";
break;
}
cout << process_carrier[i][j];
}

newline();
}
cout << intial_space;
for (int i=0;i<timer;i++){
cout << "--";
}
newline();
newline();
}
void builder(){
for (int i=0;i<no_processes;i++){
	for(int j=0;j<timer;j++){
	process_carrier[i][j]=" ";
	}
}
}
int main()
{
no_processes=5;
timer=10;
builder();
visual_choice="FCFS";
Process p1,p2,p3,p4,p5;
p1=constructor(p1,"A",0,0,3);
p2=constructor(p2,"B",1,2,6);
p3=constructor(p3,"C",2,4,4);
manager[0]=p1;
manager[1]=p2;
manager[2]=p3;
    return 0;
}
