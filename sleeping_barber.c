#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>
#include<errno.h>
#include<sys/ipc.h>
#include<semaphore.h>
 
#define N 25
 
sem_t barber_ready, mutex, customer_ready;
int free_seats=N;
int jc[2] = {0,1};
 
void *barber();
void *customer();
  
void *barber()
{
	while(1)
	{
		sem_wait(&customer_ready);            
		sem_wait(&mutex);
		free_seats += 1;
		printf("El barbero corto el cabello, el numero de clientes es %d.\n",25-free_seats);
		sem_post(&barber_ready);
		sem_post(&mutex);
		sleep(2);       
	}
}
 
void *customer()
{
	while(1)
	{
		sem_wait(&mutex);
		if(free_seats>0)
		{
			free_seats-=1;
			printf("Ha llegado un cliente, el numero de clientes es %d\n",25-free_seats);
			sem_post(&customer_ready);
			sem_post(&mutex);
			sem_wait(&barber_ready);
			sleep(1);
		}
		else
		{
			sem_post(&mutex);
			sleep(1);
		}
	}
}

int main(int a,char *ar[])
{
	pthread_t t_barber,t_costumer;
	sem_init(&mutex,0,1);
	sem_init(&customer_ready,0,0);
	sem_init(&barber_ready,0,1);
 
	pthread_create(&t_barber,NULL,barber,&jc[0]);
	pthread_create(&t_costumer,NULL,customer,&jc[1]);
	pthread_join(t_costumer,NULL);
	pthread_join(t_barber,NULL);
 
	exit(0);
}
