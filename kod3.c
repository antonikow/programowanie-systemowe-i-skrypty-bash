#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <process.h>
#include <string.h>

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
        if (!('0' <= znak && znak <= '9'))
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
    
    STARTUPINFO si;
    PROCESS_INFORMATION pi[2];
    char argline1[100];
    char argline2[100];
    sprintf(argline1, "%s %s", argv[0],m1);
	sprintf(argline2, "%s %s",argv[0],m2);
	
    
    memset(&si, 0, sizeof(si));
    memset(&pi, 0, sizeof(pi));
    
    if(CreateProcessA(NULL, argline1, NULL, NULL, 0, 0, NULL,NULL,&si, pi + 0 ) == 0)
    {
    	printf("CreateProcess 1 failed (%d).\n", 1, GetLastError() );
		exit(-1);
	}
	
	
	if(CreateProcessA(NULL, argline2, NULL, NULL, 0, 0, NULL,NULL,&si, pi + 1 ) == 0)
    {
    	printf("CreateProcess 2 failed (%d).\n", 1, GetLastError() );
		exit(-1);
	}
	

	
	WaitForSingleObject(pi[0].hProcess, INFINITE);
	WaitForSingleObject(pi[1].hProcess, INFINITE);
	DWORD status1;
	DWORD status2;
	GetExitCodeProcess(pi[0].hProcess, &status1);
	GetExitCodeProcess(pi[1].hProcess, &status2);
	
	
	
	printf("%d   %d      %s  %d\n", GetCurrentProcessId(), pi[0].dwProcessId,m1, status1);
	printf("%d   %d      %s  %d\n", GetCurrentProcessId(), pi[1].dwProcessId,m2, status2) ;
	printf("%d                 %d\n\n",GetCurrentProcessId(), status1 + status2 );
	
	
	CloseHandle(pi[0].hProcess); CloseHandle(pi[0].hThread);
	CloseHandle(pi[1].hProcess); CloseHandle(pi[1].hThread);
	return status1 + status2;
	
}
