#include <stdio.h> 
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include <windows.h>

int size = 10;

typedef struct Tile
{
    int type;
    int isbomb;
    int see;
    int autour;
    int check;

}Tile;



void remplir(Tile **grid);
void victoire(Tile **grid, int bombe, int* finish);
void modif(Tile **grid, int bombe);
int jouer(Tile **grid, int* caseLibres, int* finish, int bombe, int* drapeau, int* firstMove, int* nbrindice);
void first(Tile **grid, int i, int j, int* caseLibres);
void nombreAutour(Tile **grid);
void affichage(Tile **grid);
int askNumber(const char* Message, int Min, int Max);
void ninvisible(Tile **grid);



int main()
{
    int nbrindice = 0;
    int premier = 1;
    int finish = 0;
    int drapeau = 0;
    int restart = 1;

    printf("/////////////////////");
    printf("\n");
    printf("// JEU DU DEMINEUR //");
    printf("\n");
    printf("/////////////////////");
    Sleep(4000);
    system("cls");

    printf("////////////////////");
    printf("\n");
    printf("// INITIALISATION //");
    printf("\n");
    printf("////////////////////");
    printf("\n");
    //Si restart avec changement de taille de grille
    while (1)
    {
        system("cls");
        restart = 0;
        size = askNumber("Veuillez saisir la taille de votre grille", 5, 60);

        Tile** grid = malloc(size * sizeof(Tile*));
        for (int t = 0; t < size; t++)
        {
            grid[t] = malloc(size * sizeof(Tile));
        }

        int caseLibres = size * size;
        printf("\n");
        int bombe = askNumber("Veuillez saisir la quantite de bombe sur le terrain", 1, ((size * size) - 1));
        printf("\n");
        printf("Aide   0 : OUI   1 : NON");
        printf("\n");

        int firstMove = askNumber("Aide", 0, 1);
        modif(grid, bombe);
        
        while (caseLibres > 0) {
            system("cls");
            printf("\n");
            affichage(grid);
            /*printf("\n");
            ninvisible(grid);*/
            jouer(grid, &finish, &caseLibres, bombe, &drapeau, &firstMove, &nbrindice);
            if (finish == 1) {
                caseLibres = 0;
            }
        }
        system("cls");
        int redemarrer = askNumber("Voulez - vous rejouer au demineur ? 1 pour Oui, 0 pour Non ", 0,1);
        if (redemarrer == 0)
        {
            break;
        }
        
        for (int t = 0; t < size; t++)
        {
            free(grid[t]);
        }
        free(grid);
    
    }

    
}
... (427 lignes restantes)
Réduire
message.txt
15 Ko
Minipoto — 03/03/2023 12:23
ça va le code ?
urrikki — 03/03/2023 12:27
Ouais ça va
Minipoto — Hier à 12:45
il est sur Github le code ? vu que c'est toi qui l'avais remodifié en fin d'heure vendredi
urrikki — Hier à 12:46
nan je lui ai envoyé en message et en .c
mais aujourd'hui c'est le code en console qu'il faut donner donc je lai pas modifier
Minipoto — Hier à 12:47
aaah ok
urrikki — Hier à 12:47
j'ai envoyé celui au dessu s
Minipoto — Hier à 12:47
ok ok
urrikki — Hier à 12:47
si tu peut le mettre sur git ca serais quand meme cool
Minipoto — Hier à 12:48
ok, je te fais sa dans l'aprem ^^'
urrikki — Hier à 12:53
ca marche
Minipoto — Hier à 22:37
normal que le fishier dans lequel tu as mis le demineur s'appelle GL ?
urrikki — Hier à 22:37
Ouais
C'était pour le sdl
Minipoto — Hier à 22:38
ok, je mets juste le .c sur le git
urrikki — Hier à 22:38
Ouais azy
Minipoto — Hier à 22:39
https://github.com/Minipotal/demineur
GitHub
GitHub - Minipotal/demineur: demineur semestre 2, semaine 2 à 4
demineur semestre 2, semaine 2 à 4. Contribute to Minipotal/demineur development by creating an account on GitHub.
GitHub - Minipotal/demineur: demineur semestre 2, semaine 2 à 4
tu vois le main.c ?
urrikki — Hier à 22:39
Ouais
Minipoto — Hier à 22:40
stonks alors
urrikki — Hier à 22:41
Merci
Minipoto — Hier à 22:41
lien github ajouté au dossier de rendu
allez, je vais prendre ma douche et me coucher, bonne nuit a toi
urrikki — Hier à 22:42
Cool à demain alors
Bn
urrikki — Aujourd’hui à 08:08
#include <stdio.h> 
#include <SDL.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include <windows.h>

int size = 10;

typedef struct Tile
{
    int type;
    int isbomb;
    int see;
    int autour;
    int check;

}Tile;



void remplir(Tile** grid);
void victoire(Tile** grid, int bombe, int* finish);
void modif(Tile** grid, int bombe);
int jouer(Tile** grid, int* caseLibres, int* finish, int bombe, int* drapeau, int* firstMove, int* nbrindice);
void first(Tile** grid, int i, int j, int* caseLibres);
void nombreAutour(Tile** grid);
void affichage(Tile** grid, int finish , SDL_Window* window , SDL_Renderer* renderer);
int askNumber(const char* Message, int Min, int Max);
void ninvisible(Tile** grid);
void exitError(const char* Message);
void detruire(SDL_Renderer* renderer, SDL_Window* window);



int main()
{
    int nbrindice = 0;
    int premier = 1;
    int finish = 0;
    int drapeau = 0;
    int restart = 1;

    printf("/////////////////////");
    printf("\n");
    printf("// JEU DU DEMINEUR //");
    printf("\n");
    printf("/////////////////////");
    Sleep(4000);
    system("cls");

    printf("////////////////////");
    printf("\n");
    printf("// INITIALISATION //");
    printf("\n");
    printf("////////////////////");
    printf("\n");
    //Si restart avec changement de taille de grille

    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;

    if (SDL_CreateWindowAndRenderer(size * 31, size * 31, 0, &window, &renderer) != 0)
    {
        exitError("Initialisation fenetre");
    }

    while (1)
    {
        system("cls");
        restart = 0;
        size = askNumber("Veuillez saisir la taille de votre grille", 5, 60);

        Tile** grid = malloc(size * sizeof(Tile*));
        for (int t = 0; t < size; t++)
        {
            grid[t] = malloc(size * sizeof(Tile));
        }

        int caseLibres = size * size;
        printf("\n");
        int bombe = askNumber("Veuillez saisir la quantite de bombe sur le terrain", 1, ((size * size) - 1));
        printf("\n");
        printf("Aide   0 : OUI   1 : NON");
        printf("\n");

        int firstMove = askNumber("Aide", 0, 1);
        modif(grid, bombe);

        while (caseLibres > 0) {
            system("cls");
            printf("\n");
            affichage(grid, finish , window , renderer);
            printf("\n");
            ninvisible(grid);
            jouer(grid, &finish, &caseLibres, bombe, &drapeau, &firstMove, &nbrindice , window , renderer);
            if (finish == 1) {
                caseLibres = 0;
            }
        }
... (722 lignes restantes)
Réduire
message.txt
23 Ko
﻿
urrikki#2613
#include <stdio.h> 
#include <stdlib.h>
#include <conio.h>
#include <time.h>
//#include <SDL.h>
#include <windows.h>

int size = 10;

typedef struct Tile
{
    int type;
    int isbomb;
    int see;
    int autour;
    int check;

}Tile;



void remplir(Tile **grid);
void victoire(Tile **grid, int bombe, int* finish);
void modif(Tile **grid, int bombe);
int jouer(Tile **grid, int* caseLibres, int* finish, int bombe, int* drapeau, int* firstMove, int* nbrindice);
void first(Tile **grid, int i, int j, int* caseLibres);
void nombreAutour(Tile **grid);
void affichage(Tile **grid);
int askNumber(const char* Message, int Min, int Max);
void ninvisible(Tile **grid);



int main()
{
    int nbrindice = 0;
    int premier = 1;
    int finish = 0;
    int drapeau = 0;
    int restart = 1;

    printf("/////////////////////");
    printf("\n");
    printf("// JEU DU DEMINEUR //");
    printf("\n");
    printf("/////////////////////");
    Sleep(4000);
    system("cls");

    printf("////////////////////");
    printf("\n");
    printf("// INITIALISATION //");
    printf("\n");
    printf("////////////////////");
    printf("\n");
    //Si restart avec changement de taille de grille
    while (1)
    {
        system("cls");
        restart = 0;
        size = askNumber("Veuillez saisir la taille de votre grille", 5, 60);

        Tile** grid = malloc(size * sizeof(Tile*));
        for (int t = 0; t < size; t++)
        {
            grid[t] = malloc(size * sizeof(Tile));
        }

        int caseLibres = size * size;
        printf("\n");
        int bombe = askNumber("Veuillez saisir la quantite de bombe sur le terrain", 1, ((size * size) - 1));
        printf("\n");
        printf("Aide   0 : OUI   1 : NON");
        printf("\n");

        int firstMove = askNumber("Aide", 0, 1);
        modif(grid, bombe);
        
        while (caseLibres > 0) {
            system("cls");
            printf("\n");
            affichage(grid);
            /*printf("\n");
            ninvisible(grid);*/
            jouer(grid, &finish, &caseLibres, bombe, &drapeau, &firstMove, &nbrindice);
            if (finish == 1) {
                caseLibres = 0;
            }
        }
        system("cls");
        int redemarrer = askNumber("Voulez - vous rejouer au demineur ? 1 pour Oui, 0 pour Non ", 0,1);
        if (redemarrer == 0)
        {
            break;
        }
    
    }

    
}

void nombreAutour(Tile **grid) {

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (grid[i][j].isbomb == 0) {
                int count = 0;
                for (int x = i - 1; x <= i + 1; x++) {
                    for (int y = j - 1; y <= j + 1; y++) {
                        if (x >= 0 && x < size && y >= 0 && y < size && grid[x][y].isbomb == 1) {
                            count++;
                        }
                    }
                }
                grid[i][j].autour = count;
            }
        }
    }
}

void victoire(Tile **grid, int bombe, int* finish) {
    int nobomb = 0;
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++)
        {
            if (grid[i][j].isbomb == 0 && grid[i][j].see == 1) {
                nobomb++;
            }
        }
    }
    if (nobomb == ((size * size) - bombe)) {
        while (1) {
            system("cls");
            printf("//////////////");
            printf("\n");
            printf("// VICTOIRE //");
            printf("\n");
            printf("//////////////");
            Sleep(4500);
            *finish = 1;
        }
    }
}

void tristan(Tile **grid)
{
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (grid[i][j].isbomb == 0) {
                grid[i][j].see = 1;
            }
        }
    }
}

void remplir(Tile **grid) {
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            grid[i][j].type = 0;
            grid[i][j].isbomb = 0;
            grid[i][j].see = 0;
            grid[i][j].autour = 0;
            grid[i][j].check = 0;
        }
    }
}

void modif(Tile **grid, int bombe) {
    remplir(grid);
    srand((unsigned int)time(NULL));
    int compteurBombe = 0;
    while (1) {
        int i = rand() % size;
        int j = rand() % size;

        if (grid[i][j].isbomb == 0) {
            grid[i][j].isbomb = 1;
            grid[i][j].autour = 3;
            compteurBombe += 1;
        }

        if (compteurBombe == bombe) {
            break;
        }
    }
    nombreAutour(grid);
}

void first(Tile **grid, int i, int j, int* caseLibres)
{
    if (grid[i][j].autour == 0) {
        if ((i - 1) >= 0 && grid[i - 1][j].check == 0)
        {
            if (grid[i - 1][j].isbomb == 0)
            {
                grid[i - 1][j].check = 1;
                grid[i - 1][j].see = 1;
                *caseLibres -= 1;
                if (grid[i - 1][j].autour == 0) {
                    first(grid, i - 1, j, caseLibres);
                }
            }
        }
        if ((i + 1) < size && grid[i + 1][j].check == 0 )
        {
            if (grid[i + 1][j].isbomb == 0)
            {
                grid[i + 1][j].check = 1;
                grid[i + 1][j].see = 1;
                *caseLibres -= 1;
                if (grid[i + 1][j].autour == 0) {
                    first(grid, i + 1, j, caseLibres);
                }
            }
        }
        if ((j + 1) < size && grid[i][j + 1].check == 0)
        {
            if (grid[i][j + 1].isbomb == 0)
            {
                grid[i][j + 1].check = 1;
                grid[i][j + 1].see = 1;
                *caseLibres -= 1;
                if (grid[i][j + 1].autour == 0) {
                    first(grid, i, j + 1, caseLibres);
                }
            }
        }
        if ((j - 1) >= 0 && grid[i][j - 1].check == 0 )
        {
            if (grid[i][j - 1].isbomb == 0)
            {
                grid[i][j - 1].check = 1;
                grid[i][j - 1].see = 1;
                *caseLibres -= 1;
                if (grid[i][j - 1].autour == 0) {
                    first(grid, i, j - 1, caseLibres);
                }
            }
        }
        if ((i + 1) < size && (j + 1) < size && grid[i + 1][j + 1].check == 0 )
        {
            if (grid[i + 1][j + 1].isbomb == 0)
            {
                grid[i + 1][j + 1].check = 1;
                grid[i + 1][j + 1].see = 1;
                *caseLibres -= 1;
                if (grid[i + 1][j + 1].autour == 0) {
                    first(grid, i + 1, j + 1, caseLibres);
                }
            }
        }
        if ((i - 1) >= 0 && (j - 1) >= 0 && grid[i - 1][j - 1].check == 0 )
        {
            if (grid[i - 1][j - 1].isbomb == 0)
            {
                grid[i - 1][j - 1].check = 1;
                grid[i - 1][j - 1].see = 1;
                *caseLibres -= 1;
                if (grid[i - 1][j - 1].autour == 0) {
                    first(grid, i - 1, j - 1, caseLibres);
                }
            }
        }
        if ((j - 1) >= 0 && (i + 1) < size && grid[i + 1][j - 1].check == 0)
        {
            if (grid[i + 1][j - 1].isbomb == 0)
            {
                grid[i + 1][j - 1].check = 1;
                grid[i + 1][j - 1].see = 1;
                *caseLibres -= 1;
                if (grid[i + 1][j - 1].autour == 0) {
                    first(grid, i + 1, j - 1, caseLibres);
                }
            }
        }
        if ((i - 1) >= 0 && (j + 1) < size && grid[i - 1][j + 1].check == 0 )
        {
            if (grid[i - 1][j + 1].isbomb == 0)
            {
                grid[i - 1][j + 1].check = 1;
                grid[i - 1][j + 1].see = 1;
                *caseLibres -= 1;
                if (grid[i - 1][j + 1].autour == 0) {
                    first(grid, i - 1, j + 1, caseLibres);
                }
            }
        }
    }
}

int askNumber(const char* Message, int Min, int Max)
{
    int integer;
    while (1)
    {
        printf("\n");
        printf("%s: ", Message);
        int iError = scanf_s("%d", &integer);
        while (getchar() != '\n');

        if (iError == 1 && integer >= Min && integer <= Max)
        {
            integer;
            break;
        }

        printf("Choisir un nombre entre %d et %d afin de respecter les limites du jeu !", Min, Max);
    }

    return integer;
}

int jouer(Tile **grid, int* finish, int* caseLibres, int bombe, int* drapeau, int* firstMove, int* nbrindice)
{
    printf("\n");
    printf("Action  1:CLIQUE  2:DRAPEAU  3:INDICE");
    printf("\n");


    int action = askNumber("action", 1, 4);
    if (action == 1 || action == 2) {
        int ligne = askNumber("ligne", 1, size)-1;
        int col = askNumber("colonne", 1, size)-1;
        if (action == 1)
        {
            if (grid[ligne][col].isbomb == 1) {
                system("cls");
                printf("/////////////");
                printf("\n");
                printf("// DEFAITE //");
                printf("\n");
                printf("/////////////");
                Sleep(4500);
                *finish = 1;
            }
            else {
                if (grid[ligne][col].see == 0) {
                    grid[ligne][col].type = 2;
                    grid[ligne][col].see = 1;
                    *caseLibres -= 1;
                    victoire(grid, bombe, finish);
                    if (*firstMove == 0)
                    {
                        first(grid, ligne, col, caseLibres);
                    }
                }
                else {
                    *caseLibres += 1;
                }
                return *finish;
            }
        }
        if (action == 2)
        {
            if (*drapeau <= bombe) {
                if (grid[ligne][col].see == 0)
                {
                    grid[ligne][col].type = 3;
                    grid[ligne][col].autour = 4;
                    grid[ligne][col].see = 1;
                    *caseLibres -= 1;
                    *drapeau += 1;
                }
                else {
                    *caseLibres += 1;
                }
            }


        }
    }
    if (action == 3)
    {
        if (*nbrindice != 1) {
            *nbrindice = 1;
            printf("\n");
            system("cls");
            printf("Un indice coute 3.99 euros");
            printf("\n");
            printf("Achat en cours ...");
            Sleep(4500);
            system("cls");
            printf("Achat confirme !");
            Sleep(4000);
            int produit = 0;
            int iProduit = 0;
            int jProduit = 0;
            while (1) {
                int i = rand() % size;
                int j = rand() % size;

                if (grid[i][j].see == 0 && grid[i][j].isbomb == 0) {
                    grid[i][j].see = 1;
                    iProduit = i + 1;
                    jProduit = j + 1;
                    produit = 1;
                }

                if (produit == 1) {
                    break;
                }
            }
            system("cls");
            printf("La case ligne %d et colonne %d a ete revele", iProduit, jProduit);
            Sleep(4800);
            victoire(grid, bombe, finish);
            *caseLibres -= 1;
        }
        else {
            printf("Vous avez deja utilisé votre indice");
            Sleep(4000);
        }
    }
    if (action == 4)
    {
        printf("\n");
        system("cls");
        printf("Le programme Tristan coute 400 euros");
        printf("\n");
        printf("Achat en cours ...");
        Sleep(4500);
        system("cls");
        printf("Achat confirme !");
        Sleep(4000);
        system("cls");
        printf("Lancement du programme Tristan ...");
        Sleep(4000);
        tristan(grid);
        victoire(grid, bombe, finish);
        return *finish;

    }

    return *finish;
}

void affichage(Tile **grid)
{

    printf(" %c |", 3);
    for(int a=0; a<size; a++)
    {
        if (a < 10)
        {
            printf("%2d ", a + 1);
        }
        else if (a >= 10)
        {
            printf("%d ", a + 1);
        }
    }
    printf("\n");

    printf("___|");
    for (int a = 0; a < size; a++)
            printf("___");
    printf("\n");


    for (int i = 0; i < size; i++)
    {
        if (i < 9)
            printf("%2d |", i + 1);
        else
            printf("%d |", i + 1);
        for (int j = 0; j < size; j++)
        {
            if (grid[i][j].see == 1) {
                if (grid[i][j].isbomb == 1 || grid[i][j].type == 3) {
                        printf(" %c ", grid[i][j].autour);
                }
                else {
                        printf("%2d ", grid[i][j].autour);
                }
            }
            else {
                printf("   ");
            }
        }
        printf("\n");
    }
}

void ninvisible(Tile **grid)
{
    printf(" %c |", 3);
    for (int a = 0; a < size; a++)
    {
        if (a < 10)
        {
            printf("%2d ", a + 1);
        }
        else if (a >= 10)
        {
            printf("%d ", a + 1);
        }
    }
    printf("\n");

    printf("___|");
    for (int a = 0; a < size; a++)
        printf("___");
    printf("\n");


    for (int i = 0; i < size; i++)
    {
        if (i < 9)
            printf(" %d |", i + 1);
        else
            printf("%d |", i + 1);
        for (int j = 0; j < size; j++)
        {

            if (grid[i][j].isbomb == 1 || grid[i][j].type == 3) {
                printf(" %c ", grid[i][j].autour);
            }
            else {
                printf("%2d ", grid[i][j].autour);
            }

        }
        printf("\n");
    }
}
