/**
 * A thread mutext
 *
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>

void* thread_function(void * arg);
pthread_mutex_t work_mutex; /* protects both work_area and time_to_exit */

#define WORK_SIZE 1024
char work_area[WORK_SIZE] = "\0";
int time_to_exit = 0;

int main(int argc, char* argv[])
{
	int ret;
	pthread_t a_thread;
	void* thread_result;
	ret =  pthread_mutex_init(&work_mutex, NULL);
	if (0 != ret)
	{
		perror("Mutex initialization failed\n");
		exit(EXIT_FAILURE);
	}

	ret = pthread_create(&a_thread, NULL, thread_function, NULL);
	if (0 != ret)
	{
		perror("create thread failed\n");
		exit(EXIT_FAILURE);
	}

	pthread_mutex_lock(&work_mutex);
	printf("Input some text, Enter 'end' to finish\n");
	while (!time_to_exit)
	{
		fgets(work_area, WORK_SIZE, stdin);
		pthread_mutex_unlock(&work_mutex);
		if (work_area[0] != '\0') 
		{
			pthread_mutex_unlock(&work_mutex);
			sleep(1);
		}
		else
		{
			break;
		}
	}

	pthread_mutex_unlock(&work_mutex);
	printf("\nWaiting for thread to finish...\n");
	ret = pthread_join(a_thread, &thread_result);
	if (0 != ret)
	{
		perror("thread join failed\n");
		exit(EXIT_FAILURE);
	}
	printf("thread joined\n");
	pthread_mutex_destroy(&work_mutex);
	exit(EXIT_SUCCESS);
}

void* thread_function(void* arg)
{
	sleep(1);
	pthread_mutex_lock(&work_mutex);
	while (strncmp("end", work_area, 3) != 0)
	{
		printf("You input %d characters\n", strlen(work_area)-1);
		work_area[0] = '\0';
		pthread_mutex_unlock(&work_mutex);
		sleep(1);
		pthread_mutex_lock(&work_mutex);
		while (work_area[0] == '\0')
		{
			pthread_mutex_unlock(&work_mutex);
			sleep(1);
			pthread_mutex_lock(&work_mutex);
		}
	}
	time_to_exit = 1;
	work_area[0] = '\0';
	pthread_mutex_unlock(&work_mutex);
	pthread_exit(NULL);
}



 
