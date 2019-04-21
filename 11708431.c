#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

struct process{
	char pid;
	int arrival_time, burst_time, waiting_time, turnaround_time, remaining_time;
	bool isCompleted;
	int priority;
};


struct Queue 
{ 
    // Initialize front and rear 
    int rear, front; 
  
    // Circular Queue 
    int size; 
    struct process* arr; 
  
    Queue(int s) 
    { 
       front = rear = -1; 
       size = s; 
       arr = new struct process[s]; 
    } 
  
    void enQueue(struct process p); 
    struct process deQueue(); 
    void displayQueue(); 
    void sortByTime();
    bool inQueue(struct process p);
    struct process qFront();
    struct process qBack();
}; 

void Queue::enQueue(struct process p) 
{ 
    if ((front == 0 && rear == size-1) || 
            (rear == (front-1)%(size-1))) 
    { 
        printf("\nQueue is Full"); 
        return; 
    } 
  
    else if (front == -1)
    { 
        front = rear = 0; 
        arr[rear] = p; 
    } 
  
    else if (rear == size-1 && front != 0) 
    { 
        rear = 0; 
        arr[rear] = p; 
    } 
  
    else
    { 
        rear++; 
        arr[rear] = p;
    } 
} 

struct process Queue::qFront()
{
	if(front == -1){
		struct process p;
        p.pid = '-';
        p.arrival_time = -1;
        p.burst_time = -1;
        p.turnaround_time = -1;
        p.remaining_time = -1;
        p.waiting_time = -1;
        p.isCompleted = false;

    	return p;
	}

	struct process p = arr[front];
	return p;
}

struct process Queue::qBack()
{
	if(rear == -1){
		struct process p;
        p.pid = '-';
        p.arrival_time = -1;
        p.burst_time = -1;
        p.turnaround_time = -1;
        p.remaining_time = -1;
        p.waiting_time = -1;
        p.isCompleted = false;

    	return p;
	}

	struct process p = arr[rear];
	return p;
}

struct process Queue::deQueue() 
{ 
    if (front == -1) 
    { 
        printf("\nQueue is Empty"); 

        struct process p;
        p.pid = '-';
        p.arrival_time = -1;
        p.burst_time = -1;
        p.turnaround_time = -1;
        p.remaining_time = -1;
        p.waiting_time = -1;
        p.isCompleted = false;

    	return p;
    } 
  
    struct process P = arr[front];

    struct process p;
    p.pid = '-';
    p.arrival_time = -1;
    p.burst_time = -1;
    p.turnaround_time = -1;
    p.remaining_time = -1;
    p.waiting_time = -1;
    p.isCompleted = false;

    arr[front] = p; 

    if (front == rear) 
    { 
        front = -1; 
        rear = -1; 
    } 
    else if (front == size-1) 
        front = 0; 
    else
        front++; 
  
    return P; 
} 

void Queue::displayQueue() 
{ 
    if (front == -1) 
    { 
        printf("\nQueue is Empty"); 
        return; 
    } 
    printf("\nElements in Queue are: "); 
    if (rear >= front) 
    { 
        for (int i = front; i <= rear; i++) 
            printf("%d ",arr[i].pid); 
    } 
    else
    { 
        for (int i = front; i < size; i++) 
            printf("%d ", arr[i].pid); 
  
        for (int i = 0; i <= rear; i++) 
            printf("%d ", arr[i].pid); 
    } 
} 

void Queue :: sortByTime()
{
	if (front == -1) 
    { 
        printf("\nCannot Sort. Queue is Empty\n"); 
        return; 
    } 

    for(int i = 0; i < size; ++i){
    	struct process p = arr[i];
    	int j = i - 1;

    	while(j>=0){
    		if(arr[j].arrival_time == p.arrival_time){
    			if(arr[j].priority < p.priority){
    				arr[j+1] = arr[j];
    				j = j-1;
    			}else break;
    		}else if(arr[j].arrival_time > p.arrival_time){
    			arr[j+1] = arr[j];
    			j = j-1;
    		}else break;
    	}
    	arr[j+1] = p;
    }

}

bool Queue :: inQueue(struct process p)
{
	for(int i=0;i<size;++i){
		if(arr[i].pid == p.pid)return true;
	}
	return false;
}

int main(void){

	int n,T;

	printf("Enter the number of process: ");
	scanf("%d",&n);
	printf("\n");
	Queue q(n);
	Queue qCompleted(n);

	for(int i=0;i<n;++i){
		struct process p;
		printf("Enter process ID: ");
		scanf(" %c",&p.pid);
		printf("Enter Arrival Time of process %c: ", p.pid);
		scanf("%d",&p.arrival_time);
		printf("Enter Burst Time of process %c: ", p.pid);
		scanf("%d",&p.burst_time);
		printf("Enter Priority of process %c: ", p.pid);
		scanf("%d",&p.priority);

		p.remaining_time = p.burst_time;
		p.isCompleted = false;

		q.enQueue(p);

		printf("\n");
	}

	q.sortByTime();
	
	printf("Enter time quanta T\n");
	scanf("%d",&T);

	int total_time = q.qFront().arrival_time;
	printf("\nProcess execution order: \n");

	while(q.qFront().pid != '-'){
		struct process p = q.deQueue();

		if(p.remaining_time <= T){
			total_time += p.remaining_time;
			p.remaining_time = 0;
			p.isCompleted = true;
			printf("%c -> ", p.pid);
			p.waiting_time = total_time - p.arrival_time - p.burst_time;
			p.turnaround_time = total_time - p.arrival_time;

			qCompleted.enQueue(p);
		}else{
			total_time += T;
			p.remaining_time -= T;
			printf("%c -> ", p.pid);
			q.enQueue(p);
		}
	}
	printf("Finished\n");

	printf("\nName\tArrival Time\tBurst Time\tWaiting Time\tTurnAround Time\n");

	float avg_waiting_time = 0, avg_turnaround_time;
	while(qCompleted.qFront().pid != '-'){
		struct process p = qCompleted.deQueue();
		avg_waiting_time += p.waiting_time*1.0;
		avg_turnaround_time += p.turnaround_time*1.0;
		printf("\n%c\t\t%d\t\t%d\t\t%d\t\t%d\n",p.pid,p.arrival_time,p.burst_time,p.waiting_time,p.turnaround_time);
	}

	printf("\nAverage waiting time is :%f\n",avg_waiting_time/(n*1.0));
	printf("\nAverage turn around time is :%f\n",avg_turnaround_time/(n*1.0));
	return 0;
}

