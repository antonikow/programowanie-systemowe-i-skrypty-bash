#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <pthread.h>
#include <time.h>

double wart =1.000000000;


pthread_mutex_t mutex=PTHREAD_MUTEX_INITIALIZER;

void wallis(int n)
{
    for(int i=1;i<n;i++)
    {
        double w = (2.0*i/(2.0*i-1)) * (2.0*i/(2.0*i+1)); 
        wart = wart *  w ;
    }
}


struct struktzakres
{
    int rozmiar;
    int pierwszy;
};

void *thread(void *data)
{
    struct struktzakres *zakres = data;
    printf("Thread #%lxd :size=%d   first=%d\n",pthread_self(), zakres->rozmiar, zakres->pierwszy);
    double product = 1.0;
    int until = zakres->pierwszy+zakres->rozmiar;
    for(int i=zakres->pierwszy; i<until;i++)
    {
        product = product * (2.0*i/(2.0*i-1)) * (2.0*i/(2.0*i+1));
    }
   pthread_mutex_lock(&mutex);    
    wart=wart*product;
    pthread_mutex_unlock(&mutex);
    printf("Thread #%lxd prod=%.17g\n",pthread_self(),product);
    return (void *)zakres;    
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
    
    
    pthread_t threads[w];
    char data[w][50];
    struct struktzakres args[w];
    int reszta = n % w;

    clock_t start = clock();
    if (!reszta)    //brak reszty
    {
        for(int i=0;i<w;i++)
        {
            args[i].rozmiar = n/w;
            args[i].pierwszy = 1+ i * args[i].rozmiar;
            pthread_create(threads+i,NULL,thread,(void *)&args[i]);
        }
    }
    else        //reszta
    {
        int bezreszty = n-reszta;
        for(int i=0;i<w;i++)
        {
            if(i != w-1)    //iteracje poza ostatnia
            {
                args[i].rozmiar = bezreszty/w;
                args[i].pierwszy = 1+ i * args[i].rozmiar;
                pthread_create(threads+i,NULL,thread,(void *)&args[i]);
            }
            else    //ostatni iteracja
            {
                args[i].pierwszy = 1+ i * args[i-1].rozmiar;
                args[i].rozmiar = bezreszty/w + reszta;
                pthread_create(threads+i,NULL,thread,(void *)&args[i]);
            }
        }
    }
    
    for(int i=0;i<w;i++)
    {
        pthread_join(threads[i], NULL);
    }
    clock_t stop = clock();
    printf("w/Threads: PI=%.20lf time=%g\n",wart*2, (double)(stop-start)/CLOCKS_PER_SEC);
    
     

    wart=1.0;
    start = clock();
    wallis(n);
    stop = clock();
    
    printf("wo/Threads: PI=%.20lf time=%g\n",wart*2, (double)(stop-start)/CLOCKS_PER_SEC);

    return 0;
}
