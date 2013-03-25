/**
 * A thread semaphore
 *
 */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>

void* thread_function(void* arg);
sem_t bin_sem;

#define WORK_SIZE 1024
char work_area[WORK_SIZE] = "\0";

int main(int argc, char* argv[])
{
	int ret;
	pthread_t a_thread;
	void* thread_result;

	ret = sem_init(&bin_sem, 0, 0);
	if (0 != ret)
	{
		perror("Semaphore initialization failed\n");
		exit(EXIT_FAILURE);
	}

	ret = pthread_create(&a_thread, NULL, thread_function, NULL);
	if (0 != ret)
	{
		perror("Thread create failed\n");
		exit(EXIT_FAILURE);
	}

	printf("Input some text. Enter \'end\' to finish\n");
	while (strncmp("end", work_area, 3) != 0)
	{
		fgets(work_area, WORK_SIZE, stdin);
		sem_post(&bin_sem);
	}

	printf("\nWaiting for thread to finish...\n");
	ret = pthread_join(a_thread, &thread_result);
	if (0 != ret)
	{
		perror("Thread join failed\n");
		exit(EXIT_FAILURE);
	}
	printf("Thread joined\n");
	sem_destroy(&bin_sem);
	exit(EXIT_SUCCESS);
}

void* thread_function(void* arg)
{
	sem_wait(&bin_sem);
	while (strncmp("end", work_area, 3) != 0)
	{
		printf("You input %d characters\n", strlen(work_area)-1);
		sem_wait(&bin_sem);
	}
	pthread_exit(NULL);
}
 
