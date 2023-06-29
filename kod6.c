#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>

#define MEM_SIZE sizeof(char)*(9+1)

void drukuj(char *tab)
{
    printf("  %c   |  %c   |  %c   \n",tab[0],tab[1],tab[2]);
    printf("------+-----+------\n");
    printf("  %c   |  %c   |  %c   \n",tab[3],tab[4],tab[5]);
    printf("------+-----+------\n");
    printf("  %c   |  %c   |  %c   \n",tab[6],tab[7],tab[8]);
}
int wynik(char *tab)
{
    //  1 wygrana
    //  2 remis
    //  0 gra wciaz trwa  

    if(tab[0] == tab[1] && tab[1] == tab[2] & tab[0] != '\0')
        return 1;
    else if(tab[3] == tab[4] && tab[4] == tab[5] && tab[3] != '\0')
        return 1;
    else if(tab[6] == tab[7] && tab[7] == tab[8] && tab[6] != '\0')
        return 1;
    else if(tab[0] == tab[3] && tab[3] == tab[6] && tab[0] != '\0')
        return 1;
    else if(tab[1] == tab[4] && tab[4] == tab[7] && tab[1] != '\0')
        return 1;
    else if(tab[2] == tab[5] && tab[5] == tab[8] && tab[2] != '\0')
        return 1;
    else if(tab[0] == tab[4] && tab[4] == tab[8] && tab[0] != '\0')
        return 1;
    else if(tab[6] == tab[4] && tab[4] == tab[2] && tab[6] != '\0')
        return 1;

    //  czy remis
    int i;
    for(i=0;i<9;i++)
    {
        if(tab[i] == '\0')
            break;
    }
    if(i == 9)
        return 2;
    else
        return 0;
}


int main(int argc, char **argv)
{
    
    if(argc != 2)
    {
        fprintf(stderr,"zla ilosc argc\n");
        return 1;
    }
    key_t key = ftok(argv[1], 'a');
    if(key ==-1)
    {
        fprintf(stderr, "blad tworzac klucz\n");   
        return 2;
    }
    int shmid = shmget(key,MEM_SIZE , 0666 | IPC_CREAT | IPC_EXCL );   
    int gracz; 
    if(shmid < 0)   // gdy gracz 2
    {
        int gracz = 2;
        shmid = shmget(key, MEM_SIZE, 0666);
        if(shmid < 0)
        {
            fprintf(stderr, "blad przydzielajac pamiec wspoldzielona\n");
            return 3;
        }        
        char *tabwmem = shmat(shmid, NULL, 0);
        if(tabwmem == (char *)-1)
        {
            fprintf(stderr,"blad dolaczajac pamiec wspoldzielona\n");
            return 4;
        }
        int nr;
        printf("KOLKO(gracz 2)\n");
        drukuj(tabwmem);
        printf("prosze czekac\n");
        while(1)
        {   
            if(tabwmem[9] == '2')   //  kolej na ruch gracza 2
            {
                system("clear");
                printf("KOLKO(gracz 2)\n");
                drukuj(tabwmem);
                puts("Ruch?");
                scanf("%d",&nr);
                if(tabwmem[nr-1] == '\0')
                {
                    tabwmem[nr-1]='O';
                    int stan = wynik(tabwmem);
                    if(stan == 1)
                    {
                        tabwmem[9]='4';    //   przeciwnik natrafiajac na to dowie sie o przegranej
                        system("clear");
                        printf("KOLKO(gracz 2)\n");                        
                        printf("WYGRANA!\n");   
                        break;  
                    }
                    else if(stan == 2)
                    {
                        tabwmem[9]='5';
                        system("clear");
                        printf("KOLKO(gracz 2)\n"); 
                        printf("REMIS!\n");
                        break; 
                    }
                }   
                else
                {
                    tabwmem[9]='3';     //  przeciwnik dowie sie ze podano komrke juz zapisana
                    system("clear");
                    printf("KOLKO(gracz 2)\n");
                    printf("NIEPOPRAWNY RUCH\n");
                    break ;                   
                }  
                system("clear");
                printf("KOLKO(gracz 2)\n");
                drukuj(tabwmem);
                printf("prosze czekac\n");
                tabwmem[9]='1';
            }
            else if(tabwmem[9] == '4')
            {
                system("clear");
                printf("KOLKO(gracz 2)\n");
                printf("PORAZKA!\n");
                break;
            }
            else if(tabwmem[9] == '5')
            {
                system("clear");
                printf("KOLKO(gracz 2)\n");
                printf("REMIS!\n");
                break;
            }
        }
        if(shmdt(tabwmem) == -1)
        {
            fprintf(stderr, "blad odlaczania pamieci wspoldzielonej\n");
            return 5;
        }
    }
    else    // gdy gracz 1
    {
        gracz = 1;
        char *tabwmem = shmat(shmid, NULL, 0);
        if(tabwmem == (char *)-1)
        {
            fprintf(stderr,"blad dolaczajac pamiec wspoldzielona\n");
            return 4;
        }

        tabwmem[9]='1';
        int nr;
        drukuj(tabwmem);
        while(1)
        {
            if(tabwmem[9] == '1')   //  kolej na ruch gracza 1
            {
                system("clear");
                printf("KRZYZYK(gracz 1)\n");
                drukuj(tabwmem);
                puts("Ruch?");
                scanf("%d",&nr);
                if(tabwmem[nr-1] == '\0')
                {
                    tabwmem[nr-1]='X';
                    int stan = wynik(tabwmem);
                    if(stan == 1)
                    {
                        tabwmem[9]='4';     //   przeciwnik natrafiajac na to dowie sie o przegranej
                        system("clear");
                        printf("KRZYZYK(gracz 1)\n");                        
                        printf("WYGRANA!\n");   
                        break;   
                    }
                    else if(stan == 2)
                    {
                        tabwmem[9]='5';
                        system("clear");
                        printf("KRZYZYK(gracz 1)\n"); 
                        printf("REMIS!\n");
                        break; 
                    }
                }
                else if(tabwmem[9] == '3')
                    break;
                else
                {
                    tabwmem[9]='3';     //  przeciwnik dowie sie ze podano komrke juz zapisana
                    system("clear");
                    printf("KRZYZYK(gracz 1)\n");
                    printf("NIEPOPRAWNY RUCH\n");
                    break ;
                }
                system("clear");
                printf("KRZYZYK(gracz 1)\n");
                drukuj(tabwmem);
                printf("prosze czekac\n");
                tabwmem[9]='2';
            }
            else if(tabwmem[9] == '3')
            {
                system("clear");
                printf("KRZYZYK(gracz 1)\n");
                printf("NIEPOPRAWNY RUCH\n");
                break;
            }
            else if(tabwmem[9] == '4')
            {
                system("clear");
                printf("KRZYZYK(gracz 1)\n");
                printf("PORAZKA!\n");
                break;
            }
            else if(tabwmem[9] == '5')
            {
                system("clear");
                printf("KRZYZYK(gracz 1)\n");
                printf("REMIS!\n");
                break;
            }
        }
        if(shmdt(tabwmem) == -1)
        {
            fprintf(stderr,"blad odlaczania bloku pamieci wspoldzielonej\n");
            return 5;
        }
        if(shmctl(shmid, IPC_RMID, NULL) == -1)
        {
            fprintf(stderr,"blad zwalniania bloku pamieci wspoldzielonej\n");
            return 6;
        }
    }
    return 0;
}
