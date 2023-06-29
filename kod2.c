#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char **argv)
{
    
    if(argc != 2)
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

   
    int l = atoi(argv[1]);
    if( l < 0 || l >13 )
    {
        fprintf(stderr,"argv[1] nie jest w <1..13>\n");
        exit(3);
    }
    if( l == 1 || l == 2)
        exit(1);

    


    int nowa1 = atoi(argv[1])-1;
    char m1[100];
    sprintf(m1, "%d", nowa1);


    int nowa2 = atoi(argv[1])-2;
    char m2[100];
    sprintf(m2, "%d", nowa2);
    
    
    int d1 = fork();

    if(d1 == 0)
        execlp(argv[0],argv[0],m1,NULL);

    else if(d1 > 0 )
    {
        int d2 = fork();

        if(d2 == 0)
            execlp(argv[0],argv[0],m2,NULL);

        else if(d2 > 0)
        {   
            int status1, child1 = waitpid(d1,&status1,0);
            int status2, child2 = waitpid(d2,&status2,0);
            printf("%d   %d      %s  %d\n",getpid(),child1,m1,WEXITSTATUS(status1));
            printf("%d   %d      %s  %d\n",getpid(),child2,m2,WEXITSTATUS(status2));
            printf("%d                %d\n\n",getpid() ,WEXITSTATUS(status1) + WEXITSTATUS(status2));
            return WEXITSTATUS(status1) + WEXITSTATUS(status2);
        }
    }
}
