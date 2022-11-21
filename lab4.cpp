#include <iostream>
#include <queue>
#include <fstream>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <vector>
#include <algorithm>
#include <bits/stdc++.h>
#include <string>
#include <math.h>
using namespace std;
vector<string> processlist;
int no_processes;
int second_parameter;
string visual_choice;                       //trace or stats
string policy_choice;
vector<vector<int>> policies;
vector<int>finish_times;
vector<vector<string>> process_trace_mapper;      //used to store proccess status at certain time to draw el trace
vector <int> scheduling_policy;                  //used to store the different proccessing policies to perform
int last_instance;                              // last time instance in the trace map
class Process{
    public:
        int id;
        string name;
        int arrival_time;
        int service_time;
        int wait_time=0;
        int end_time;
};
queue<Process> processes;
vector<Process> info_processes;
bool operator<(const Process &p1, const Process &p2){    //used for priority_queue in order to enter class Process instances

    if (p1.service_time==p2.service_time){
        return p1.arrival_time>p2.arrival_time;
    }
    else
        return p1.service_time>p2.service_time;
}


vector<Process> sorter(vector<Process> vec){
sort(vec.begin(),vec.end(), []( Process &p1,  Process &p2){
                    if(((p1.wait_time+p1.service_time)/p1.service_time)==((p2.wait_time+p2.service_time)/p2.service_time))
                    return p1.service_time<p2.service_time;
                    else
                    return ((float)(p1.wait_time+p1.service_time)/(float)p1.service_time)>((float)(p2.wait_time+p2.service_time)/(float)p2.service_time);
});
return vec;
}
void FCFS(){
    queue<Process> temp=processes;            // copy of proccess in a queue in order to insert them to ready queue at their time
    queue<Process> container;                  //queue used for the scheduling policy
    Process running;                          //simulates the running process
    running.name="NULL";
    for(int i=0;i<last_instance;i++){
        if(!temp.empty()){
            if(temp.front().arrival_time==i){
                container.push(temp.front());            // insert a proccess into the ready queue when at it's entry time
                temp.pop();
        }
        }
        if((running.name=="NULL"|| running.service_time==0)&&!container.empty() ){         // deqeue the first proccess in the queue proccess is runnung
            running=container.front();
            container.pop();
        }

        queue<Process> temp_copy=container;                                       // processes in the ready queue are waiting
        while(!temp_copy.empty()){
            process_trace_mapper[temp_copy.front().id][i]=".";                         // simulating waiting in the 2d array
            temp_copy.pop();
        }
        if(running.name!="NULL"&&running.service_time!=0){                                                 //simulating running
            process_trace_mapper[running.id][i]="*";
            running.service_time--;                                          // service time decrementation
            if(running.service_time==0)
                finish_times.at(running.id)=i+1;
        }
    }
}
void RR(){
    int quantum=second_parameter;
    queue<Process> temp=processes;
    queue<Process> container;
    Process running;
    running.name="NULL";
    for(int i=0;i<last_instance;i++){
        if(!temp.empty()){
            if(temp.front().arrival_time==i){
                container.push(temp.front());
                temp.pop();
            }
        }
        if(running.name!="NULL"){
            if(running.service_time!=0 && quantum==0){
                container.push(running);
                running.name="NULL";
            }
        }
        if((running.name=="NULL"|| running.service_time==0)&&!container.empty()){
            quantum=second_parameter;
            running=container.front();
            container.pop();
        }

        queue<Process> temp_copy=container;
        while(!temp_copy.empty()){
            process_trace_mapper[temp_copy.front().id][i]=".";
            temp_copy.pop();
        }
        if(running.name!="NULL"&&running.service_time!=0){
            process_trace_mapper[running.id][i]="*";
            running.service_time--;
            quantum--;
            if(running.service_time==0)
               finish_times.at(running.id)=i+1;
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
            process_trace_mapper[temp_copy.top().id][i]=".";
            temp_copy.pop();
        }
        if(running.service_time!=0){
            process_trace_mapper[running.id][i]="*";
            running.service_time--;
            if(running.service_time==0)
                finish_times.at(running.id)=i+1;
        }
    }
}
void SRT(){
    queue<Process> temp=processes;
    priority_queue<Process> container;
    Process running;
    running.name="NULL";
    for(int i=0;i<last_instance;i++){
        if(!temp.empty()){
            if(temp.front().arrival_time==i){
                container.push(temp.front());
                temp.pop();
            }
        }
        if(running.name!="NULL"){
            if(running.service_time>container.top().service_time){
                container.push(running);
                running=container.top();
                container.pop();
            }
        }

        if((running.name=="NULL"  || running.service_time==0)&&!container.empty()){
            running=container.top();
            container.pop();
        }

        priority_queue<Process> temp_copy=container;
        while(!temp_copy.empty()){
            process_trace_mapper[temp_copy.top().id][i]=".";
            temp_copy.pop();
        }

        if(running.service_time!=0){
            process_trace_mapper[running.id][i]="*";
            running.service_time--;
            if(running.service_time==0)
                 finish_times.at(running.id)=i+1;
        }
    }
}
void HRRN(){
    queue<Process> temp=processes;
    vector<Process> container;
    Process running;
    for(int i=0;i<last_instance;i++){
        if(!temp.empty()){
            if(temp.front().arrival_time==i){
                container.push_back(temp.front());
                 container=sorter(container);
                 temp.pop();
            }
        }
        if(running.service_time==0&&container.capacity()!=0){
        container=sorter(container);
            running=container.at(0);
            container.erase(container.begin());
           container=sorter(container);
        }
        for(int j=0;j<container.size();++j){
            process_trace_mapper[container.at(j).id][i]=".";
            container.at(j).wait_time++;
        }
        container=sorter(container);
        if(running.service_time!=0){
            process_trace_mapper[running.id][i]="*";
            running.service_time--;
            if(running.service_time==0)
                finish_times.at(running.id)=i+1;
        }
    }}
void FB_1(){}
void FB_2i(){}
void Aging(){}
void policy_selector(int policy){

switch (policy){
    case 1: policy_choice="FCFS";
            FCFS();
            break;
    case 2: policy_choice="RR-";
            policy_choice.append(to_string(second_parameter));
            RR();
            break;
    case 3: policy_choice="SPN";
            SPN();
            break;
    case 4: policy_choice="SRT";
            SRT();
            break;
    case 5: policy_choice="HRRN";
            HRRN();
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
void stats_printer(){
    cout << policy_choice << endl;
    cout << "Process    |";
    for (int i=0;i<no_processes;i++){
        cout << "  "<<processlist.at(i)<< "  |";
    }
    newline();
    cout << "Arrival    |";
    for (int i=0;i<no_processes;i++){
        string number=to_string(info_processes.at(i).arrival_time);
        int length=number.length();
        for(int j=2;j>=length;j--)
            cout<<" ";
        cout<<info_processes.at(i).arrival_time;
        cout<< "  |";
    }
    newline();
    cout << "Service    |";
    for (int i=0;i<no_processes;i++){
        string number=to_string(info_processes.at(i).service_time);
        int length=number.length();
        for(int j=2;j>=length;j--)
            cout<<" ";
        cout<<info_processes.at(i).service_time;
        cout<< "  |";
    }
    cout<< " Mean|";
    newline();
    cout << "Finish     |";
    for (int i=0;i<no_processes;i++){
        string number=to_string(finish_times.at(i));
        int length=number.length();
        for(int j=2;j>=length;j--)
            cout<<" ";
        cout<<finish_times.at(i);
        cout<< "  |";
    }
    cout<< "-----|";
    newline();
    cout << "Turnaround |";
    float total_turnaround=0;
    for (int i=0;i<no_processes;i++){
        int turnaround_time=finish_times.at(i)-info_processes.at(i).arrival_time;
        total_turnaround+=turnaround_time;
        string number=to_string(turnaround_time);
        int length=number.length();
        for(int j=2;j>=length;j--)
            cout<<" ";
        cout<<turnaround_time;
        cout<< "  |";
    }
    float avg=((float)(total_turnaround)/(float)(no_processes));
    string number=to_string((int)avg);
    int length=number.length();
    for(int j=2;j>length;j--)
        cout<<" ";
    cout <<fixed<<setprecision(2)<<avg;
    cout<< "|";
    newline();
    cout << "NormTurn   |";
    float total_normalised=0;
    for (int i=0;i<no_processes;i++){
        float normalised_time=((float)(finish_times.at(i)-info_processes.at(i).arrival_time))/(float)info_processes.at(i).service_time;
        total_normalised+=normalised_time;
        string number=to_string((int)normalised_time);
        int length=number.length();
        for(int j=2;j>length;j--)
            cout<<" ";
        cout<<fixed<<setprecision(2)<<normalised_time;
        cout<< "|";
    }
    avg=((float)(total_normalised)/(float)(no_processes));
    number=to_string((int)avg);
    length=number.length();
    for(int j=2;j>length;j--)
        cout<<" ";
    cout <<fixed<<setprecision(2)<<avg;
    cout<< "|";
    newline();
    newline();
}
void trace_printer(){
    string intial_space="------";
    cout << policy_choice;
    int visual_choice_space=policy_choice.length();
    for (int i=0;i<6-visual_choice_space;i++){
        cout << " ";
    }
    for (int i=0;i<=last_instance;i++){        // printer for time sequence of trace
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
    for (int i=0;i<no_processes;i++){                      //printer for main table row
        string test=processlist.at(i);
        cout << test;
        int visual_choice_space=test.length();
        for (int k=visual_choice_space;k<6;k++){          //space after proccess name
            cout << " ";
        }
        for (int j=0;j<last_instance+1;j++){
            cout << "|";
            if(j==last_instance+1){
                cout << " ";                             //space after row
                break;
            }
            cout << process_trace_mapper[i][j];              //printer for proccess status (* for running " " not ready . for waiting)
        }
        newline();
    }
    cout << intial_space;                               //printer for line after proccesses dynamic
    for (int i=0;i<=last_instance;i++){
        cout << "--";
    }
    newline();
    newline();
    }
void builder(){                                         //builder that builds 2d array for storing process status at certain time
    for (int i=0;i<no_processes;i++){
        vector <string>row_info;
        for(int j=0;j<last_instance+1;j++){
            row_info.push_back(" ");
        }
        process_trace_mapper.push_back(row_info);
        row_info.clear();
    }
}
void process_parsing(){                                 //tokenistion  of procces from input textfile
    for (int i=0;i<no_processes;i++){
        Process p1;
        p1.id=i;
        char temp[30];
        cin >> temp;
        char* token = strtok(temp,",");
        p1.name=token;
        processlist.push_back(token);
        token = strtok(NULL, ",");
        p1.arrival_time=stoi(token);
        token = strtok(NULL, ",");
        p1.service_time=stoi(token);
        processes.push(p1);
        info_processes.push_back(p1);
    }
}
void policies_pharsing(){
    string temp;
    cin >> temp;
    stringstream line(temp);
    string token;
    string innertoken;
    while(getline(line,token,',')){
        vector<int> temp;
        stringstream innerline(token);
        getline(innerline,innertoken,'-');
        if (token==innertoken){
            temp.push_back(stoi(token));
        }
        else{
            temp.push_back(stoi(innertoken));
            getline(innerline,innertoken,'-');
            temp.push_back(stoi(innertoken));
            }
        policies.push_back(temp);
    }
}
void initialise_finish_times(){
for (int i=0;i<no_processes;i++)
    finish_times.push_back(-1);
}
int main()
{
    string temp;
    cin >> visual_choice;
    policies_pharsing();
    cin >> last_instance;
    cin >> no_processes;
    initialise_finish_times();
    process_parsing();
    for(int i=0;i<policies.size();i++){
        process_trace_mapper.clear();
        builder();
        if(policies[i][0]==2 || policies[i][0]==7)
            second_parameter=policies[i][1];
        policy_selector(policies[i][0]);
        if(visual_choice=="trace")
            trace_printer();
       else if(visual_choice=="stats")
            stats_printer();
    }

    return 0;
}
