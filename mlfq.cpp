#include<stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <bits/stdc++.h>
using namespace std;
struct Process_Data
{
	int Num;
	int Pid;  //Process Id
	int Ar_time; // Arrival Time
	int Br_time; // Bruest Time
	int Priority; // Priority
	int Fn_time; // Finish Time
	int Rm_time; // Remaining Time 
	int Wt_time; //Waiting Time
	int St_time; // start Time
	int Res_time;

};

struct Process_Data current;
typedef struct Process_Data P_d ;

bool idsort(const P_d& x , const P_d& y)
{
	return x.Pid < y.Pid;
}
/** Sorting on the base of arrival time if that match then on Priority of Priority also  match than on the base of Process Id**/
bool arrivalsort( const P_d& x ,const P_d& y)
{
	if(x.Ar_time < y.Ar_time)
		return true;
	else if(x.Ar_time > y.Ar_time)
		return false;
	if(x.Priority < y.Priority)
		return true;
	else if(x.Priority > y.Priority)
		return false;
	if(x.Pid < y.Pid)
		return true;

	return false;
}


bool Numsort( const P_d& x ,const P_d& y)
{
	return x.Num < y.Num;
}
/*Sorting on the base of Priority if that same then on the base of PID*/
struct comPare
{
	bool operator()(const P_d& x ,const P_d& y)
	{
		if( x.Priority > y.Priority )
			return true;
		else if( x.Priority < y.Priority )
			return false;
		if( x.Pid > y.Pid )
			return true;

		return false;
		
	}
	
};

/**To check the Input **/
void my_check(vector<P_d> mv)
{
	for(unsigned int i= 0; i < mv.size() ;i++)
	{
		cout<<" Pid :"<<mv[i].Pid<<" Ar_time : "<<mv[i].Ar_time<<" Br_time : "<<mv[i].Br_time<<" Priority : "<<mv[i].Priority<<endl;
	}

}

int main()
{
	int i;
	vector< P_d > input;
	vector<P_d> input_copy;
	P_d temp;
	int pq_process = 0; // for PQ process
	int rq_process = 0; // for RQ process
	int Ar_time;
	int Br_time;
	int Pid;
	int Priority;
	int n;
	int clock;
	int total_exection_time = 0;
	cin>>n;
	for( i= 0; i< n; i++ )
	{
		cin>>Pid>>Ar_time>>Br_time>>Priority;
		temp.Num = i+1;
		temp.Ar_time = Ar_time;
		temp.Br_time = Br_time;
		temp.Rm_time = Br_time;
		temp.Pid = Pid;
		temp.Priority = Priority;
		input.push_back(temp);
	}
	input_copy = input;
	sort( input.begin(), input.end(), arrivalsort );
    //cout<<"arrivalsort : "<<endl;
    //my_check( input ); // To check the sort unomment it
    total_exection_time = total_exection_time + input[0].Ar_time;
    for( i= 0 ;i< n; i++ )
    {
    	if( total_exection_time >= input[i].Ar_time )
    	{
    		total_exection_time = total_exection_time +input[i].Br_time;
    	}
    	else
    	{
    		int diff = (input[i].Ar_time - total_exection_time);
    		total_exection_time = total_exection_time + diff + Br_time;

    	}
    }

	int Ghant[total_exection_time]={0}; //Ghant Chart
	for( i= 0; i< total_exection_time; i++ )
	{
		Ghant[i]=-1;
	}
	//cout<<"total_exection_time : "<<total_exection_time<<endl;

	priority_queue < P_d ,vector<Process_Data> ,comPare> pq; //Priority Queue PQ

	queue< P_d > rq; //Round Robin Queue RQ
	int cpu_state = 0; //idle if 0 then Idle if 1 the Busy
	int quantum = 4 ; //Time Quantum
	current.Pid = -2;
	current.Priority = 999999;

	for ( clock = 0; clock< total_exection_time; clock++ )
	{
		/**Insert the process with same Arrival time in Priority Queue**/
		for( int j = 0; j< n ; j++ )
		{
			if(clock == input[j].Ar_time)
			{
				pq.push(input[j]);
			}
		}
		

		if(cpu_state == 0) //If CPU idle
		{
			if(!pq.empty())
			{
				current = pq.top();
				cpu_state = 1;
				pq_process = 1;
				pq.pop();
				quantum = 4; 
			}
			else if(!rq.empty())
			{
				current = rq.front();
				cpu_state = 1;
				rq_process = 1;
				rq.pop();
				quantum = 4;
			}
		}
		else if(cpu_state == 1) //If cpu has any procss
		{
			if(pq_process == 1 && (!pq.empty()))
			{
				if(pq.top().Priority < current.Priority ) //If new process has high priority
				{
					rq.push(current); //push current in RQ
					current = pq.top();
					pq.pop();
					quantum = 4; 
				}
			}
			else if(rq_process == 1 && (!pq.empty())) //If process is from RQ and new process come  in PQ
			{
				rq.push(current);
				current = pq.top();
				pq.pop();
				rq_process = 0;
				pq_process = 1;
				quantum = 4 ;
			}
			

		}


		if(current.Pid != -2) // Process Execution
		{
			current.Rm_time--;
			quantum--;
			Ghant[clock] = current.Pid;
			if(current.Rm_time == 0) //If process Finish
			{
				cpu_state = 0 ;
				quantum = 4 ;
				current.Pid = -2;
				current.Priority = 999999;
				rq_process = 0;
				pq_process = 0;
			}
			else if(quantum == 0 ) //If time Qunatum of a current running process Finish
			{
				rq.push(current);
				current.Pid = -2;
				current.Priority = 999999;
				rq_process = 0;
				pq_process = 0;
				cpu_state=0;

			}

		}
		
	}


	sort( input.begin(), input.end(), idsort );
	
	for(int i=0;i<n;i++)
	{
		for(int k=total_exection_time;k>=0;k--)
		{
			if(Ghant[k]==i+1)
			{
				input[i].Fn_time=k+1;
				break;

			}
		}
	}
	for(int i=0;i<n;i++)
	{
		for(int k=0;k<total_exection_time;k++)
		{

			if(Ghant[k]==i+1)
			{
				input[i].St_time=k;
				break;
			}
		}
	}
	
	sort( input.begin(), input.end(), Numsort );

	for(int i=0;i<n;i++)
	{
		input[i].Res_time=input[i].St_time-input[i].Ar_time;
		input[i].Wt_time=(input[i].Fn_time-input[i].Ar_time)-input[i].Br_time;

	}
	
	for(int i=0;i<n;i++)
	{
		cout<<input[i].Pid<<" "<<input[i].Res_time<<" "<<input[i].Fn_time<<" "<<input[i].Wt_time<<endl;
		
	}	
	return 0;
}
