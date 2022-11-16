#include <iostream>
#include <queue>
#include <fstream>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
using namespace std;
string processlist[30];
int no_processes;
string visual_choice;
string policy_choice;
int policies;
int timer;
string process_carrier[30][30];
int scheduling_policy[5];
int last_instance;
class Process{
public:
int id;
string name;
int arrival_time;
int service_time;
int end_time;
};
bool operator<(const Process &p1, const Process &p2){
if (p1.service_time==p2.service_time){
return p1.arrival_time>p2.arrival_time;
}
else
return p1.service_time>p2.service_time;
}
Process manager[9];
queue<Process> processes;
void FCFS(){
queue<Process> temp=processes;
queue<Process> container;
Process running;
running.name="NULL";
for(int i=0;i<last_instance;i++){
if(!temp.empty()){
if(temp.front().arrival_time==i){
container.push(temp.front());
temp.pop();}
}
if((running.name=="NULL"|| running.service_time==0)&&!container.empty() ){
running=container.front();
container.pop();
}

queue<Process> temp_copy=container;
while(!temp_copy.empty()){
process_carrier[temp_copy.front().id][i]=".";
temp_copy.pop();
}
if(running.name!="NULL"){
process_carrier[running.id][i]="*";
running.service_time--;
}
}
}
void RR(){
int quantum;
queue<Process> temp=processes;
queue<Process> container;
Process running;
running.name="NULL";
for(int i=0;i<last_instance;i++){
if(!temp.empty()){
if(temp.front().arrival_time==i){
container.push(temp.front());
temp.pop();}
}
if(running.name!="NULL"){
if(running.service_time!=0 && quantum==0){
container.push(running);
running.name="NULL";
}}
if((running.name=="NULL"|| running.service_time==0)&&!container.empty()){
quantum=1;
running=container.front();
container.pop();
}

queue<Process> temp_copy=container;
while(!temp_copy.empty()){
process_carrier[temp_copy.front().id][i]=".";
temp_copy.pop();
}
if(running.name!="NULL"){
process_carrier[running.id][i]="*";
running.service_time--;
quantum--;

}

}
}
void SPN(){
queue<Process> temp=processes;
priority_queue<Process> container;
Process running;
for(int i=0;i<last_instance;i++){
if(!temp.empty()){
if(temp.front().arrival_time==i){
container.push(temp.front());
temp.pop();
}
}
if(running.service_time==0&&!container.empty()){
running=container.top();
container.pop();
}
priority_queue<Process> temp_copy=container;
while(!temp_copy.empty()){
process_carrier[temp_copy.top().id][i]=".";
temp_copy.pop();
}
if(running.service_time!=0){
process_carrier[running.id][i]="*";
running.service_time--;
}
}}
void SRT(){
queue<Process> temp=processes;
priority_queue<Process> container;
Process running;
running.name="NULL";
for(int i=0;i<last_instance;i++){
if(!temp.empty()){
if(temp.front().arrival_time==i){
container.push(temp.front());
temp.pop();}
}
if(running.name!="NULL"){
if(running.service_time>container.top().service_time){
container.push(running);
running=container.top();
container.pop();
}}

if(running.name=="NULL"&&!container.empty()){
running=container.top();
container.pop();}
if(running.name!="NULL"){
if(running.service_time==0){
running=container.top();
container.pop();
}
}
priority_queue<Process> temp_copy=container;
while(!temp_copy.empty()){
process_carrier[temp_copy.top().id][i]=".";
temp_copy.pop();
}

if(running.service_time!=0){
process_carrier[running.id][i]="*";
running.service_time--;
}
}
}
void HRRN(){}
void FB_1(){}
void FB_2i(){}
void Aging(){}
void policy_selector(int policy){

switch (policy){
case 1: policy_choice="FCFS";
        FCFS();
        break;
case 2: policy_choice="RR  ";
        RR();
        break;
case 3: policy_choice="SPN ";
        SPN();
        break;
case 4: policy_choice="STR ";
        SRT();
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
cout << policy_choice;
int visual_choice_space=visual_choice.length();
for (int i=visual_choice_space;i<=6;i++){
cout << " ";
}
for (int i=0;i<=last_instance;i++){
int instance=i;
while(instance>9){
instance-=10;
}
cout << instance;
cout << " ";
}
newline();
cout << intial_space;
for (int i=0;i<=last_instance;i++){
cout << "--";
}
newline();
for (int i=0;i<no_processes;i++){
string test=processlist[i];
cout << test;
int visual_choice_space=test.length();
for (int k=visual_choice_space;k<6;k++){
cout << " ";
}
for (int j=0;j<last_instance+1;j++){
cout << "|";
if(j==last_instance+1){
cout << " ";
break;
}
cout << process_carrier[i][j];
}

newline();
}
cout << intial_space;
for (int i=0;i<=last_instance;i++){
cout << "--";
}
newline();
newline();
}
void builder(){
for (int i=0;i<no_processes;i++){
	for(int j=0;j<last_instance+1;j++){
	process_carrier[i][j]=" ";
	}
}
}
void process_parsing(){
for (int i=0;i<no_processes;i++){
Process p1;
p1.id=i;
char temp[30];
cin >> temp;
char* token = strtok(temp,",");
p1.name=token;
processlist[i]=token;
token = strtok(NULL, ",");
p1.arrival_time=stoi(token);
token = strtok(NULL, ",");
p1.service_time=stoi(token);
manager[i]=p1;
processes.push(p1);
}
}
int main()
{
string temp;
cin >> visual_choice;
cin >> policies;
cin >> last_instance;
cin >> no_processes;
process_parsing();
builder();
policy_selector(policies);
trace_printer();
    return 0;
}
