#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <math.h>
#include <time.h>


double wart = 0.0;

HANDLE mutex;

void leibniz(int n)
{
	for(int i=0;i<=n;i++)
    {
        double w = pow(-1,i)/(2*i+1);
        wart = wart +  w ;
    }
}

struct struktzakres
{
	int rozmiar;
	int pierwszy;	
};

DWORD WINAPI thread(LPVOID data)
{
	struct  struktzakres *zakres = data;
	printf("Thread #%lxd   size=%d   first=%d\n",GetCurrentThreadId(), zakres->rozmiar, zakres->pierwszy);
	double sum = 0.0;
	int until = zakres->pierwszy + zakres->rozmiar;
	for(int i=zakres->pierwszy;i<until;i++)
	{
		sum = sum + pow(-1,i)/(2*i+1);
	}
	WaitForSingleObject(mutex, INFINITE);
	wart = wart + sum;
	ReleaseMutex(mutex);
	printf("Thread #%lxd sum=%.17g\n",GetCurrentThreadId(), sum);
	return 0;
}

int main(int argc, char **argv)
{
    
    if(argc != 3)
    {
        fprintf(stderr,"zla ilosc argc\n");
        exit(1);
    }

    int i = 0;
    int naturalna = 1;
    while(argv[1][i] != '\0')
    {
        int znak = argv[1][i];
        if (!(48 <= znak && znak <= 57))
        {
            naturalna = 0;
            break;
        }
        ++i;
    }
    if(!naturalna)
    {
        fprintf(stderr,"argv[1] nie jest liczba naturalna\n");
        exit(2);
    }
    
    i = 0;
    naturalna = 1;
    while(argv[2][i] != '\0')
    {
        int znak = argv[2][i];
        if (!(48 <= znak && znak <= 57))
        {
            naturalna = 0;
            break;
        }
        ++i;
    }
    if(!naturalna)
    {
        fprintf(stderr,"argv[1] nie jest liczba naturalna\n");
        exit(2);
    }

   

    int n = atoi(argv[1]);
    if( n <= 1 || n >1000000000 )
    {
        fprintf(stderr,"argv[1] nie jest w (1..1000000000)\n");
        exit(3);
    }
    int w = atoi(argv[2]);
    if( w <= 1 || w >=100 )
    {
        fprintf(stderr,"argv[2] nie jest w (1..100)\n");
        exit(3);
    }
    
    HANDLE threads[w];
    struct struktzakres args[w];
    DWORD tidy[w];
    mutex=CreateMutex(NULL,0,NULL);
    if (mutex == NULL)
    {
    	printf("CreateMutex() failed (%d).\n", GetLastError());
    	exit(-1);
	}
    int reszta = n % w;
    
    clock_t start = clock();
    if(!reszta)	//brak reszty
    {
    	for(int i=0;i<w;i++)
    	{
    		args[i].rozmiar = n/w;
    		args[i].pierwszy = i*args[i].rozmiar;
    		threads[i]=CreateThread(NULL,0,thread,args+i,0,tidy+i);
		}
	}
	else	//reszta
	{
		int bezreszty=n-reszta;
		for(int i=0;i<w;i++)
		{
			if( i != w-1)	//iteracje poza ostatnia
			{
				args[i].rozmiar = bezreszty/w;
				args[i].pierwszy= i*args[i].rozmiar;
				threads[i]=CreateThread(NULL,0,thread,args+i,0,tidy+i);
			}
			else	//ostatnia iteracja
			{
				args[i].pierwszy = 1+i*args[i-1].rozmiar;
				args[i].rozmiar = bezreszty/w +reszta;
				threads[i]=CreateThread(NULL,0,thread,args+i,0,tidy+i);
			}
		}
	}
    
    for(int i=0;i<w;i++)
    {
    	WaitForSingleObject(threads[i],INFINITE);
    	CloseHandle(threads[i]);
	}
	clock_t stop = clock();
	CloseHandle(mutex);
	printf("w/Threads: PI=%.20lf time=%g\n",wart*4, (double)(stop-start)/CLOCKS_PER_SEC  );
	
	
	wart=0.0;
	start = clock();
	leibniz(n);
	stop = clock();
	
	printf("wo/Threads: PI=%.20lf time=%g\n",wart*4, (double)(stop-start)/CLOCKS_PER_SEC  );
	
	return 0;
}
