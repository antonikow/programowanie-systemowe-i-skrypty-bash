#include <stdio.h>
#include <windows.h>
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
    HANDLE MapFile = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, argv[1]);
	if(MapFile == NULL)		//	kod dla gracz 1
	{
	    MapFile = CreateFileMapping(INVALID_HANDLE_VALUE,NULL,PAGE_READWRITE,0,MEM_SIZE,argv[1]);
		if(MapFile == NULL)
		{
			fprintf(stderr, "Nie udalo sie wykonac mapowania\n");
			return 2;
		}
		char *tabwmem = MapViewOfFile(MapFile, FILE_MAP_ALL_ACCESS, 0,0,MEM_SIZE);
		if(tabwmem == NULL)
		{
			fprintf(stderr, "Nie udalo sie udostepnic mapowania w przestrzeni adresowej procesu\n");
			CloseHandle(MapFile);
			return 3;
		}

		int gracz = 1;
		tabwmem[9]='1';
        int nr;
        drukuj(tabwmem);
        while(1)
        {
            if(tabwmem[9] == '1')   //  kolej na ruch gracza 1
            {
                system("cls");
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
                        system("cls");
                        printf("KRZYZYK(gracz 1)\n");                        
                        printf("WYGRANA!\n");   
                        break;   
                    }
                    else if(stan == 2)
                    {
                        tabwmem[9]='5';
                        system("cls");
                        printf("KRZYZYK(gracz 1)\n"); 
                        printf("REMIS!\n");
                        break; 
                    }
                }
                else
                {
                    tabwmem[9]='3';     //  przeciwnik dowie sie ze podano komorke juz zapisana
                    system("cls");
                    printf("KRZYZYK(gracz 1)\n");
                    printf("NIEPOPRAWNY RUCH\n");
                    break ;
                }
                system("cls");
                printf("KRZYZYK(gracz 1)\n");
                drukuj(tabwmem);
                printf("prosze czekac\n");
                tabwmem[9]='2';
            }
            else if(tabwmem[9] == '3')
            {
                system("cls");
                printf("KRZYZYK(gracz 1)\n");
                printf("NIEPOPRAWNY RUCH\n");
                break;
            }
            else if(tabwmem[9] == '4')
            {
                system("cls");
                printf("KRZYZYK(gracz 1)\n");
                printf("PORAZKA!\n");
                break;
            }
            else if(tabwmem[9] == '5')
            {
                system("cls");
                printf("KRZYZYK(gracz 1)\n");
                printf("REMIS!\n");
                break;
            }
        }
        if(!UnmapViewOfFile(tabwmem))
		{
			fprintf(stderr, "Nie udalo sie odlaczyc mapowania\n");
			CloseHandle(MapFile);
			return 4;
		}
		CloseHandle(MapFile);
	}
	else	// kod dla gracz 2
	{
		char *tabwmem = MapViewOfFile(MapFile, FILE_MAP_ALL_ACCESS, 0,0,MEM_SIZE);
		if(tabwmem == NULL)
		{
			fprintf(stderr, "Nie udalo sie udostepnic mapowania w przestrzeni adresowej procesu\n");
			CloseHandle(MapFile);
			return 3;
		}
		int gracz = 2;
		int nr;
		system("cls");
        printf("KOLKO(gracz 2)\n");
        drukuj(tabwmem);
        printf("prosze czekac\n");
        while(1)
        {   
            if(tabwmem[9] == '2')   //  kolej na ruch gracza 2
            {
                system("cls");
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
                        system("cls");
                        printf("KOLKO(gracz 2)\n");                        
                        printf("WYGRANA!\n");   
                        break;  
                    }
                    else if(stan == 2)
                    {
                        tabwmem[9]='5';
                        system("cls");
                        printf("KOLKO(gracz 2)\n"); 
                        printf("REMIS!\n");
                        break; 
                    }
                }   
                else
                {
                    tabwmem[9]='3';     //  przeciwnik dowie sie ze podano komorke juz zapisana
                    system("cls");
                    printf("KOLKO(gracz 2)\n");
                    printf("NIEPOPRAWNY RUCH\n");
                    break ;                   
                }  
                system("cls");
                printf("KOLKO(gracz 2)\n");
                drukuj(tabwmem);
                printf("prosze czekac\n");
                tabwmem[9]='1';
            }
            else if(tabwmem[9] == '3')
            {
                system("cls");
                printf("KOLKO(gracz 2)\n");
                printf("NIEPOPRAWNY RUCH\n");
                break;
            }
            else if(tabwmem[9] == '4')
            {
                system("cls");
                printf("KOLKO(gracz 2)\n");
                printf("PORAZKA!\n");
                break;
            }
            else if(tabwmem[9] == '5')
            {
                system("cls");
                printf("KOLKO(gracz 2)\n");
                printf("REMIS!\n");
                break;
            }
        }	
        if(!UnmapViewOfFile(tabwmem))
		{
			fprintf(stderr, "Nie udalo sie odlaczyc mapowania\n");
			CloseHandle(MapFile);
			return 4;
		}
		CloseHandle(MapFile);
	}
	
	
	return 0;
}
