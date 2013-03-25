/**
 * Try run two thread to access a same golbal variable:
 *
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

void* thread_function(void* arg);

int run_now = 1;

int main(int argc, char* argv[])
{
    int nRet;
    pthread_t a_thread;

    nRet = pthread_create(&a_thread, NULL, thread_function, NULL);
    if (0 != nRet)
    {
        perror("Thread creation failed");
        exit(EXIT_FAILURE);    
    }

	printf("start to change run_now loop in main thread...\n");
    int print_count1 = 0;
	while (print_count1++ <20)
	{
		if (run_now == 1)
		{
			printf("1");
			run_now = 2;
		}
		else
		{
			sleep(1);
		}
	}
}


void* thread_function(void* arg)
{
	printf("start to change run_now loop in thread_function...\n");

	int print_count2 = 0;
	while (print_count2 ++ < 20)
	{
		if (run_now == 2)
		{
			printf("2");
			run_now = 1;
		}
		else
		{
			sleep(1);
		}
	}
	pthread_exit((void*)"thread_function exit");
}
