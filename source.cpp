#include <iostream>
#include<vector>
#include<queue>
#include<stack>

using namespace std;

class info{
public:
	info(vector<int> v);
	int timeTR; // turn around time
	int timeW; // waiting time
	int timeR; //  response time
	int index; //  inex to current prog
	int totalburst;
	bool done;
	bool start;
	vector<int> P; // current process

};
info::info(vector<int>v){
	totalburst = timeTR = timeW = 0;
	timeR = -1;
	done = start = false;
	P = v;
}
void vprint(const vector<int>&A); // utility function, prints a vector of ints
bool pempty(vector<info> p); // determines if processes are done
void FCFS(const vector<info> &V); // First Come Fist Serve
void SJF(const vector<info> &V); // Shortest Job First
void MLFQ(const vector<info> &V); // Multi Level Feedback Queue
void vprint(const vector<int>&A); // prints vector for testing
vector<info> runio2(vector<info> &io); // runs io, decrements and returns vector of any that hit 0
int smallest(const vector<info> &ready); // returns smallest index in ready q
void printList(const int &totalRunT, const vector<info> &running, const vector<info> &RR1, const vector<info> &RR2, const vector<info> & FCFSQ, const vector<info> &io); //  prints out data
void main(){

	vector<info> Progs;
	vector<int> P[8];
	P[0] = { 4, 24, 5, 73, 3, 31, 5, 27, 4, 33, 6, 43, 4, 64, 5, 19, 2 };
	P[1] = { 18, 31, 19, 35, 11, 42, 18, 43, 19, 47, 18, 43, 17, 51, 19, 32, 10 };
	P[2] = { 6, 18, 4, 21, 7, 19, 4, 16, 5, 29, 7, 21, 8, 22, 6, 24, 5 };
	P[3] = { 17, 42, 19, 55, 20, 54, 17, 52, 15, 67, 12, 72, 15, 66, 14 };
	P[4] = { 5, 81, 4, 82, 5, 71, 3, 61, 5, 62, 4, 51, 3, 77, 4, 61, 3, 42, 5 }; // intializing vecotrs to processes
	P[5] = { 10, 35, 12, 41, 14, 33, 11, 32, 15, 41, 13, 29, 11 };
	P[6] = { 21, 51, 23, 53, 24, 61, 22, 31, 21, 43, 20 };
	P[7] = { 11, 52, 14, 42, 15, 31, 17, 21, 16, 43, 12, 31, 13, 32, 15 };


	for (int i = 0; i < 8; i++)
	{
		Progs.push_back(P[i]);
		Progs[i].index = i;
	}
	FCFS(Progs);
	SJF(Progs);
	MLFQ(Progs);

}
void FCFS(const vector<info> &V){

	int totalRunT = 0; // totaltime
	vector<info> temp = V;
	vector<info> running; //  process running
	vector<info>  ready; //  ready queue
	vector<info> io; // io processes
	totalRunT = 0; //  reseting run time for each algorithm
	int ResponseTime = 0;
	vector<info> ioindex;
	double totalCPUBurst; 
	double totalCPUused;
	int currindex; // indicated current process being dealt with


	totalCPUBurst = totalCPUused = 0;
	for (size_t i = 0; i < temp.size(); i++) // total cpu burst and total burst
	{
		for (size_t j = 0; j < temp[i].P.size(); j += 2) // calculates total cpu burst
		{

			totalCPUBurst += temp[i].P[j];

		}
		for (size_t j = 0; j < temp[i].P.size(); j++)
		{
			temp[i].totalburst += temp[i].P[j]; //  calculating total bursts for wait time

		}

	}


	for (size_t i = 0; i < temp.size(); i++) // calculate initial response times
	{
		temp[i].timeR = ResponseTime;
		ResponseTime += temp[i].P.front();
	}


	for (size_t i = 0; i < temp.size(); i++) // initializing ready queue at start
	{
		ready.push_back(temp[i]);
		temp[i].P.erase(temp[i].P.begin());

	}



	while (!pempty(temp) || !ready.empty() || !io.empty() || !running.empty()){ //  main control loop [until all vectors are empty]




		if (running.empty()){
			if (!ready.empty()){ // loads running from ready if empty

				running.push_back(ready.front());
				ready.erase(ready.begin());
				running.front().P.front()--;

			}
		}
		else{


			if (running.front().P.front() > 0){ // decrements running
				running.front().P.front()--;
			}
			else{

				currindex = running.front().index; // index of process being calculated
				running.pop_back();

				if (temp[currindex].P.empty()){ // determines when a process is finished
					temp[currindex].timeTR = totalRunT;
					temp[currindex].done = true;
					cout << "Process " << currindex+1 << " finished: " << totalRunT << endl;
				}
				if (!temp[currindex].P.empty()){ // moves next burst to io

					io.push_back(temp[currindex]);
					temp[currindex].P.erase(temp[currindex].P.begin());
				}

				if (!ready.empty()){

					running.push_back(ready.front());
					ready.erase(ready.begin());
					running.front().P.front()--;

				}


			}
		}


		if (!io.empty()){ // runs io, decrements all processes in io by 1, returns index of processes that ereach 0 or less
			ioindex = runio2(io); //  also removes any process from io list that reaches 0 or less
		}

		if (!ioindex.empty()){ // appends anythingthat leaves io to ready

			for (size_t i = 0; i < ioindex.size(); i++)
			{
				if (!temp[ioindex[i].index].P.empty()){
					ready.push_back(temp[ioindex[i].index]);
					temp[ioindex[i].index].P.erase(temp[ioindex[i].index].P.begin());
				}
			}

		}


		totalRunT++;
	}

	totalRunT--; //  Lloop ticks 1 more time, decrementing for logic

	for (size_t i = 0; i < temp.size(); i++){ //  waiting time 

		temp[i].timeW = temp[i].timeTR - temp[i].totalburst;

	}
	cout << "FCFS: " << endl;
	for (size_t i = 0; i < temp.size(); i++){
		cout << "P" << i + 1 << " "
			<< " Tw: " << temp[i].timeW
			<< " Ttr: " << temp[i].timeTR
			<< " Tr: " << temp[i].timeR << endl;
	}
	double wtavg, Ttravg, Travg;
	wtavg = Ttravg = Travg = 0;
	for (size_t i = 0; i < temp.size(); i++) // calculating the average times
	{
		wtavg += temp[i].timeW;
		Ttravg += temp[i].timeTR;
		Travg += temp[i].timeR;

	}


	wtavg = wtavg / temp.size();
	Ttravg = Ttravg / temp.size();
	Travg = Travg / temp.size();
	totalCPUused = totalCPUBurst / totalRunT;
	cout << "TWavg: " << wtavg << " Ttravg: " << Ttravg << " Travg: " << Travg << endl;
	cout << " CPU Utilization: " << totalCPUused * 100 << "%" << endl;



}
void SJF(const vector<info> &V){


	vector<info> temp = V;
	vector<info> running; //  process running
	vector<info>  ready; //  ready queue
	vector<info> io; // io processes
	int totalRunT = 0; // totaltime
	int ResponseTime = 0;
	vector<info> ioindex;
	double totalCPUBurst;
	double totalCPUused;
	int currindex; // indicated current process being dealt with

	totalCPUBurst = totalCPUused = 0;
	for (size_t i = 0; i < temp.size(); i++) // total cpu burst and total burst
	{ 
		for (size_t j = 0; j < temp[i].P.size(); j += 2) // calculates total cpu burst
		{
			totalCPUBurst += temp[i].P[j];
		}
		for (size_t j = 0; j < temp[i].P.size(); j++)
		{
			temp[i].totalburst += temp[i].P[j]; //  calculating total bursts for wait time
		}
	}


	for (size_t i = 0; i < temp.size(); i++) // initializing ready queue at start
	{
		ready.push_back(temp[i]);
		temp[i].P.erase(temp[i].P.begin());

	}


	while (!pempty(temp) || !ready.empty() || !io.empty() || !running.empty()){ //  main control loop [until all vectors are empty]



		if (running.empty()){
			if (!ready.empty()){
				if (ready[smallest(ready)].start == false){ // determines when a process starts for response time
					temp[ready[smallest(ready)].index].start = true;
					temp[ready[smallest(ready)].index].timeR = totalRunT;
				}
				running.push_back(ready[smallest(ready)]);
				ready.erase(ready.begin() + smallest(ready));
				running.front().P.front()--;

			}
		}
		if (!running.empty()){
			

			if (running.front().P.front() > 0){ //  decrements burst in running
				running.front().P.front()--;
			}
			else{

				currindex = running.front().index;
				running.pop_back();

				if (temp[currindex].P.empty()){ // determines if process is done
					temp[currindex].timeTR = totalRunT;
					temp[currindex].done = true;

					cout << "Process " << currindex + 1 << " finished: " << totalRunT << endl;
				}
				if (!temp[currindex].P.empty()){ // moves process to io

					io.push_back(temp[currindex]);
					temp[currindex].P.erase(temp[currindex].P.begin());
				}
				if (!ready.empty()){

					if (ready[smallest(ready)].start == false){ // determines when a process starts, stoes it
						temp[ready[smallest(ready)].index].start = true;
						temp[ready[smallest(ready)].index].timeR = totalRunT;
					}
					running.push_back(ready[smallest(ready)]);
					ready.erase(ready.begin() + smallest(ready));
					running.front().P.front()--;

				}


			}
		}


		if (!io.empty()){ // runs io, decrements all processes in io by 1, returns index of processes that ereach 0 or less
			ioindex = runio2(io); //  also removes any process from io list that reaches 0 or less
		}

		if (!ioindex.empty()){

			for (size_t i = 0; i < ioindex.size(); i++)
			{
				if (!temp[ioindex[i].index].P.empty()){
					ready.push_back(temp[ioindex[i].index]);
					temp[ioindex[i].index].P.erase(temp[ioindex[i].index].P.begin());
				}
			}

		}

		totalRunT++;
	}


	totalRunT--; //  Lloop ticks 1 more time, decrementing for logic



	for (size_t i = 0; i < temp.size(); i++){ //  waiting time 

		temp[i].timeW = temp[i].timeTR - temp[i].totalburst;

	}
	cout << "SJF: " << endl;
	for (size_t i = 0; i < temp.size(); i++){
		cout << "P" << i + 1 << " "
			<< " Tw: " << temp[i].timeW
			<< " Ttr: " << temp[i].timeTR
			<< " Tr: " << temp[i].timeR << endl;
	}
	double wtavg, Ttravg, Travg;
	wtavg = Ttravg = Travg = 0;
	for (size_t i = 0; i < temp.size(); i++) // calculating the average times
	{
		wtavg += temp[i].timeW;
		Ttravg += temp[i].timeTR;
		Travg += temp[i].timeR;

	}

	wtavg = wtavg / temp.size(); // avg wait time
	Ttravg = Ttravg / temp.size(); //  avg turnaround time
	Travg = Travg / temp.size(); // avg response time
	totalCPUused = totalCPUBurst / totalRunT; // calculates total cpu utilization
	cout << "TWavg: " << wtavg << " Ttravg: " << Ttravg << " Travg: " << Travg << endl;
	cout << " CPU Utilization: " << totalCPUused * 100 << "%" << endl;

}
void MLFQ(const vector<info> &V){

	vector<info> temp = V;
	vector<info> running; //  process running
	vector<info>  RR1; //  RoundRobin1
	vector<info>  RR2; // RoundRobin2
	vector<info> FCFSQ; // first come first server, final queue
	vector<info> io; // io processes
	int totalRunT = 0; // totaltime
	int currentQ = 0; // 0 -> RR1, 1->RR2, 2->FCFSQ
	int ResponseTime = 0;
	vector<info> ioindex;
	double totalCPUBurst;
	double totalCPUused;
	int currindex; // indicated current process being dealt with


	int tq1 = 6, tq2 = 11; //  time quantum 1 and 2
	int rr1tq, rr2tq; // used to keep track of time quantam state
	rr1tq = rr2tq = 0;
	totalCPUBurst = totalCPUused = 0;

	for (size_t i = 0; i < temp.size(); i++)
	{
		for (size_t j = 0; j < temp[i].P.size(); j++)
		{
			temp[i].totalburst += temp[i].P[j]; //  calculating total bursts for wait time

		}
		for (size_t j = 0; j < temp[i].P.size(); j += 2) // calculates total cpu burst
		{
			totalCPUBurst += temp[i].P[j];
		}
	}

	for (size_t i = 0; i < temp.size(); i++) // initializing RR1 queue at start
	{
		RR1.push_back(temp[i]);
		temp[i].P.erase(temp[i].P.begin());

	}

	temp[0].timeR = 0;
	for (size_t i = 1; i < RR1.size(); i++) // calculating time Tr
	{

		if (RR1[i - 1].P.front() <= tq1){
			temp[i].timeR = temp[i - 1].timeR + RR1[i - 1].P.front();
		}
		else{
			temp[i].timeR = temp[i - 1].timeR + tq1;
		}

	}


	while (!RR1.empty() || !io.empty() || !RR2.empty() || !FCFSQ.empty() || !running.empty()){ //  main control loop [until all vectors are empty]


		if (running.empty())
		{
			if (!RR1.empty()){ // if rr1 isn't empty loads rr1 process

				running.push_back(RR1.front());
				RR1.erase(RR1.begin());
				running.front().P.front()--;
				rr1tq++;
				currentQ = 0;

			}
			else if (!RR2.empty()){ // checks if rr2 isn't empty then loads that next

				running.push_back(RR2.front());
				RR2.erase(RR2.begin());
				rr2tq++;
				running.front().P.front()--;
				currentQ = 1;

			}
			else if (!FCFSQ.empty()){ // then finally once reach the pits of 
										//hell you are able to acess the fcfsq, allowing your process to finally run

				running.push_back(FCFSQ.front());
				FCFSQ.erase(FCFSQ.begin());
				running.front().P.front()--;
				currentQ = 2;
			}


		}
		else{

			if (running.front().P.front() > 0 && rr1tq < tq1 && rr2tq < tq2){ //decrements running burst, 
				running.front().P.front()--;
				
				if (currentQ == 0){											//	increments counters based on current q
									
					rr1tq++;
				}
				else if (currentQ == 1){
					rr2tq++;
				}
			}
			else{
				rr1tq = rr2tq = 0; // resets counters
				if (running.front().P.front() == 0){

					currindex = running.front().index;
					if (temp[currindex].P.empty() && temp[currindex].done == false){ // determines when process terminates
						temp[currindex].timeTR = totalRunT;
						temp[currindex].done = true;
						cout << "Process " << currindex + 1 << " finished: " << totalRunT << endl;
					}
					if (!temp[currindex].P.empty()){ // moves next burst to io
						io.push_back(temp[currindex]);
						temp[currindex].P.erase(temp[currindex].P.begin());
					}
					running.pop_back();
				}
				else if (currentQ == 0){ // moves to next queue

					RR2.push_back(running.front());
					running.pop_back();
				}
				else if (currentQ == 1){
					FCFSQ.push_back(running.front());
					running.pop_back();
				}

				if (running.empty())
				{
					if (!RR1.empty()){

						running.push_back(RR1.front());
						RR1.erase(RR1.begin());
						running.front().P.front()--;
						rr1tq++;
						currentQ = 0;

					}
					else if (!RR2.empty()){

						running.push_back(RR2.front());
						RR2.erase(RR2.begin());
						running.front().P.front()--;
						rr2tq++;
						currentQ = 1;

					}
					else if (!FCFSQ.empty()){


						running.push_back(FCFSQ.front());
						FCFSQ.erase(FCFSQ.begin());
						running.front().P.front()--;
						currentQ = 2;
					}


				}
				
			}

		}
	
		
		if (!io.empty()){ // runs io, decrements all processes in io by 1, returns index of processes that ereach 0 or less
			ioindex = runio2(io); //  also removes any process from io list that reaches 0 or less
		}

		if (!ioindex.empty()){

			for (size_t i = 0; i < ioindex.size(); i++)
			{
				if (!temp[ioindex[i].index].P.empty()){
					RR1.push_back(temp[ioindex[i].index]);
					temp[ioindex[i].index].P.erase(temp[ioindex[i].index].P.begin());
				}
			}

		}

		totalRunT++;
	}


	totalRunT--; //  Lloop ticks 1 more time, decrementing for logic

	for (size_t i = 0; i < temp.size(); i++){ //  waiting time 

		temp[i].timeW = temp[i].timeTR - temp[i].totalburst;

	}
	cout << "MLFQ: " << endl;
	for (size_t i = 0; i < temp.size(); i++){
		cout << "P" << i + 1 << " "
			<< " Tw: " << temp[i].timeW
			<< " Ttr: " << temp[i].timeTR
			<< " Tr: " << temp[i].timeR << endl;
	}
	double wtavg, Ttravg, Travg;
	wtavg = Ttravg = Travg = 0;
	for (size_t i = 0; i < temp.size(); i++) // calculating the average times
	{
		wtavg += temp[i].timeW;
		Ttravg += temp[i].timeTR;
		Travg += temp[i].timeR;

	}

	wtavg = wtavg / temp.size();
	Ttravg = Ttravg / temp.size();
	Travg = Travg / temp.size();
	totalCPUused = totalCPUBurst / totalRunT;
	cout << "TWavg: " << wtavg << " Ttravg: " << Ttravg << " Travg: " << Travg << endl;
	cout << " CPU Utilization: " << totalCPUused * 100 << "%" << endl;

}
bool pempty(const vector<info> p){


	for (size_t i = 0; i < p.size(); i++)
	{
		if (!p[i].P.empty()) {
			return false;
		}
	}

	return true;

}
void vprint(const vector<int>&A){ // prints vector

	for (size_t i = 0; i < A.size(); i++)
	{
		cout << A[i];
		cout << " ";

	}
	cout << endl;
}
vector<info> runio2(vector<info> &io){

	vector<info> index;
	for (size_t i = 0; i < io.size(); i++)
	{
		io[i].P.front()--;
	}
	for (size_t i = 0; i < io.size(); i++)
	{
		if (io[i].P.front() < 1){

			index.push_back(io[i]);
			io.erase(io.begin() + i);
			
		}

	}
	return index;
}
int smallest(const vector<info> &ready){
	
	int index = 0;

	for (size_t i = 1; i < ready.size(); i++)
	{
		if (ready[i].P.front() < ready[index].P.front()){
			index = i;
		}
	}

	return index;
}
void printList(const int & totalRunT, const vector<info> &running, const vector<info> &RR1, const vector<info> &RR2, const vector<info> & FCFSQ, const vector<info> &io){

	if (!running.empty()){
		cout << "running burst: " << running.front().P.front() << endl;
	}
	else{
		cout << "running burst: empty" << endl;
	}
	if (!RR1.empty()){
		cout << "RR1 list: ";
		for (size_t i = 0; i < RR1.size(); i++)
		{
			cout << RR1[i].P.front() << " ";
		}
		cout << endl;
	}
	else{
		cout << "RR1 list: empty" << endl;
	}
	if (!RR2.empty()){
		cout << "RR2 list: ";
		for (size_t i = 0; i < RR2.size(); i++)
		{
			cout << RR2[i].P.front() << " ";
		}
		cout << endl;
	}
	else{
		cout << "RR2 list: empty" << endl;
	}
	if (!FCFSQ.empty()){
		cout << "FCFS list: ";
		for (size_t i = 0; i < FCFSQ.size(); i++)
		{
			cout << FCFSQ[i].P.front() << " ";
		}
		cout << endl;
	}
	else{
		cout << "FCFS list: empty" << endl;
	}
	if (!io.empty()){
		cout << "io burst: ";
		for (size_t i = 0; i < io.size(); i++)
		{
			cout << io[i].P.front() << " ";
		}
		cout << endl;
	}
	else{
		cout << "io burst: empty" << endl;
	}
	cout << "run time: " << totalRunT << endl;

	cout << "-------------------" << endl;
}
