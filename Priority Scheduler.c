#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<malloc.h>
#include<time.h>
#include<pthread.h>
#include<semaphore.h>
struct Process {
	int priority,time,Atime,Btime,id;
	clock_t arrival;
	int flag,completed,p;
	sem_t se;
	struct Process *next;
};
int i=0,z=1;
float WT=0,TAT=0;
clock_t start;
struct Process *front=NULL,*temp,*rear=NULL;
typedef struct Process node;
void *processor(node *S) {
	int y=0;
	clock_t count;
	while(1) {
		sem_wait(&S->se);
		if((S->Atime<=(clock()-start)/CLOCKS_PER_SEC && S->p==1) || y==1) {
			S->p=0;
			y=0;
			count=clock();
		} 
		if(S->flag==1) {
			printf("\nProcess-%d Running \nTimer :%d",S->id,(clock()-start)/CLOCKS_PER_SEC);
			S->flag=0;
			S->arrival=clock();
		}
		if((clock()-count)/CLOCKS_PER_SEC==1) {
			count=clock();
			printf("\nTimer :%d",(clock()-start)/CLOCKS_PER_SEC);
			S->time-=1;
			if(S->time==0) {
				TAT+=(clock()-S->arrival)/CLOCKS_PER_SEC;
				WT+=((clock()-S->arrival)/CLOCKS_PER_SEC)-S->Btime;
				if(rear->next!=NULL) {
					
				if(rear->Atime==(clock()-start)/CLOCKS_PER_SEC){
					rear->next=rear->next->next;
				}
				else {
					rear=rear->next;
				}
				sem_post(&rear->se);
				}
				S->completed=7;
				printf("\nProcess-%d Completed next Process is :%d",S->id,rear->id);
			}
			else if(rear!=S) {
				printf("\nProcess-%d Context Switched to Process-%d",S->id,rear->id);
				y=1;
				sem_post(&rear->se);
				sem_wait(&S->se);
			}
		}
		if(S->completed==7) {
			break;
		}
		sem_post(&S->se);
	}
}

void pop() {
	node *start=rear;
	printf("\nArrival Time :%d",start->next->Atime);
}
void rpush(node *temp) {
	node *start=rear;
	if(rear==NULL) {
		rear=temp;
		rear->next=NULL;
	}
	else{
	int p=temp->priority;
	if (start->priority > p) {
        	temp->next = rear;
        	rear=temp;
    	}
    	else {
        	while (start->next != NULL && start->next->priority< p) {
            		start = start->next;
        	}
        temp->next = start->next;
        start->next = temp;
    }
	}
}
void push() {
	temp=(node *)malloc(sizeof(node));
	printf("\nEnter Priority of %d Process :",(i+1));
	scanf("%d",&temp->priority);
	printf("Enter Arrival Time :");
	scanf("%d",&temp->Atime);
	int p=temp->priority;
	temp->id=i+1;
	printf("Enter Burst Time :");
	scanf("%d",&temp->time);
	temp->Btime=temp->time;
	sem_init(&temp->se,0,0);
	int t=temp->Atime;
	temp->flag=1;
	temp->p=1;
	node *start=front;
	if ((start->Atime > t) || (start->Atime==t && start->priority > temp->priority)){
        	temp->next = front;
        	front=temp;
    	}
    	else {
        	while (start->next != NULL && start->next->Atime <= t) {
            		if(start->next->Atime==t && temp->priority<start->next->priority) {
            			break;
					}
					else
					start = start->next;
            		
        	}
        temp->next = start->next;
        start->next = temp;
    }
}
void main() {
	int n,l=1;
	pthread_t p[10];
	printf("\nEnter No.of Processes :");
	scanf("%d",&n);
	while(i<n) {
		if(front==NULL) {
			front=(node *)malloc(sizeof(node));
			printf("Enter Priority of %d Process :",(i+1));
			scanf("%d",&front->priority);
			printf("Enter Arrival Time :");
			scanf("%d",&front->Atime);
			printf("Enter Burst Time :");
			scanf("%d",&front->time);
			front->flag=1;
			front->id=i+1;
			front->p=1;
			front->Btime=front->time;
			sem_init(&front->se,0,0);
			front->next=NULL;
		}
		else {
			push();
		}
		i++;
	}
	start=clock();
	i=0;
	while(i<n) {
		temp=front;
		if(temp->Atime<=0) {
			printf("\nInvalid Arrival Time for Process-%d",temp->id);
			i++;
			front=front->next;
		}
		if((clock()-start)/CLOCKS_PER_SEC==temp->Atime) {
			if(l==1) {
				l=0;
				sem_post(&temp->se);
			}
			pthread_create(&p[i],NULL,processor,temp);
			front=front->next;
			rpush(temp);
			//printf("\nCreation : %d Priority of Rear : %d",temp->Atime,rear->priority);
			i++;
		}
	}
	for(i=0;i<n;i++) {
		pthread_join(p[i],NULL);
	}
	printf("\nAverage Waiting Time :%f\nAverage Turn Around Time :%f",(float)WT/n,(float)TAT/n);
}

