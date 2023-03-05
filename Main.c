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
void affichage(Tile** grid, int finish);
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
            affichage(grid, finish);
            printf("\n");
            ninvisible(grid);
            jouer(grid, &finish, &caseLibres, bombe, &drapeau, &firstMove, &nbrindice);
            if (finish == 1) {
                caseLibres = 0;
            }
        }
        system("cls");
        int redemarrer = askNumber("Voulez - vous rejouer au demineur ? 1 pour Oui, 0 pour Non ", 0, 1);
        if (redemarrer == 0)
        {
            break;
        }

    }


}

void nombreAutour(Tile** grid) {

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

void victoire(Tile** grid, int bombe, int* finish) {
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
            affichage(grid, *finish);
            Sleep(4000);
            *finish = 1;
        }
    }
}

void tristan(Tile** grid)
{
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (grid[i][j].isbomb == 0) {
                grid[i][j].see = 1;
            }
        }
    }
}

void remplir(Tile** grid) {
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

void modif(Tile** grid, int bombe) {
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

void first(Tile** grid, int i, int j, int* caseLibres)
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
        if ((i + 1) < size && grid[i + 1][j].check == 0)
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
        if ((j - 1) >= 0 && grid[i][j - 1].check == 0)
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
        if ((i + 1) < size && (j + 1) < size && grid[i + 1][j + 1].check == 0)
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
        if ((i - 1) >= 0 && (j - 1) >= 0 && grid[i - 1][j - 1].check == 0)
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
        if ((i - 1) >= 0 && (j + 1) < size && grid[i - 1][j + 1].check == 0)
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

int jouer(Tile** grid, int* finish, int* caseLibres, int bombe, int* drapeau, int* firstMove, int* nbrindice)
{
    printf("\n");
    printf("Action  1:CLIQUE  2:DRAPEAU  3:INDICE");
    printf("\n");


    int action = askNumber("action", 1, 4);
    if (action == 1 || action == 2) {
        int ligne = askNumber("ligne", 1, size) - 1;
        int col = askNumber("colonne", 1, size) - 1;
        if (action == 1)
        {
            if (grid[ligne][col].isbomb == 1) {
                system("cls");
                grid[ligne][col].see = 1;
                affichage(grid, finish);
                Sleep(4000);
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
                    if (*firstMove == 0)
                    {
                        first(grid, ligne, col, caseLibres);
                    }
                    victoire(grid, bombe, finish);
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

void exitError(const char* Message)
{
    SDL_Log("ERREUR : %s > %s\n ", Message, SDL_GetError);
    SDL_Quit();
    exit(EXIT_FAILURE);
}

void detruire(SDL_Renderer* renderer, SDL_Window* window)
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
}

void affichage(Tile** grid, int finish)
{

    
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;

    /*INITIALISATION*/


    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) != 0)
    {
        exitError("Initialisation SDL");
    }

    if (SDL_CreateWindowAndRenderer(size * 31, size * 31, 0, &window, &renderer) != 0)
    {
        exitError("Initialisation fenetre");
    }

    SDL_Surface* vide = NULL;
    SDL_Surface* bombe = NULL;
    SDL_Surface* drapeau = NULL;
    SDL_Texture* zero = NULL;
    SDL_Surface* un = NULL;
    SDL_Surface* deux = NULL;
    SDL_Surface* trois = NULL;
    SDL_Surface* quatre = NULL;
    SDL_Surface* cinq = NULL;
    SDL_Surface* six = NULL;
    SDL_Surface* sept = NULL;
    SDL_Surface* huit = NULL;

    SDL_Texture* texture = NULL;

    /*CHARGEMENT IMAGE*/

    vide = SDL_LoadBMP("ben10bmp/vide.bmp");
    if (vide == NULL)
    {
        detruire(renderer, window);
        exitError("Impossible de charger l'image : vide ");
    }

    bombe = SDL_LoadBMP("ben10bmp/bombe.bmp");
    if (bombe == NULL)
    {
        detruire(renderer, window);
        exitError("Impossible de charger l'image : bombe ");
    }

    drapeau = SDL_LoadBMP("ben10bmp/drapeau.bmp");
    if (drapeau == NULL)
    {
        detruire(renderer, window);
        exitError("Impossible de charger l'image : drapeau ");
    }

    zero = SDL_LoadBMP("ben10bmp/0.bmp");
    if (zero == NULL)
    {
        detruire(renderer, window);
        exitError("Impossible de charger l'image : zero ");
    }

    un = SDL_LoadBMP("ben10bmp/1.bmp");
    if (un == NULL)
    {
        detruire(renderer, window);
        exitError("Impossible de charger l'image : un ");
    }

    deux = SDL_LoadBMP("ben10bmp/2.bmp");
    if (deux == NULL)
    {
        detruire(renderer, window);
        exitError("Impossible de charger l'image : deux ");
    }

    trois = SDL_LoadBMP("ben10bmp/3.bmp");
    if (trois == NULL)
    {
        detruire(renderer, window);
        exitError("Impossible de charger l'image : trois ");
    }

    quatre = SDL_LoadBMP("ben10bmp/4.bmp");
    if (quatre == NULL)
    {
        detruire(renderer, window);
        exitError("Impossible de charger l'image : quatre ");
    }

    cinq = SDL_LoadBMP("ben10bmp/5.bmp");
    if (cinq == NULL)
    {
        detruire(renderer, window);
        exitError("Impossible de charger l'image : cinq ");
    }

    six = SDL_LoadBMP("ben10bmp/6.bmp");
    if (six == NULL)
    {
        detruire(renderer, window);
        exitError("Impossible de charger l'image : six ");
    }

    sept = SDL_LoadBMP("ben10bmp/7.bmp");
    if (sept == NULL)
    {
        detruire(renderer, window);
        exitError("Impossible de charger l'image : sept ");

    }

    huit = SDL_LoadBMP("ben10bmp/8.bmp");
    if (huit == NULL)
    {
        detruire(renderer, window);
        exitError("Impossible de charger l'image : huit ");
    }

    /*TEXTURE*/

    SDL_Texture* tvide = NULL;
    SDL_Texture* tbombe = NULL;
    SDL_Texture* tdrapeau = NULL;
    SDL_Texture* tzero = NULL;
    SDL_Texture* tun = NULL;
    SDL_Texture* tdeux = NULL;
    SDL_Texture* ttrois = NULL;
    SDL_Texture* tquatre = NULL;
    SDL_Texture* tcinq = NULL;
    SDL_Texture* tsix = NULL;
    SDL_Texture* tsept = NULL;
    SDL_Texture* thuit = NULL;

    tvide = SDL_CreateTextureFromSurface(renderer, vide);
    SDL_FreeSurface(vide);
    if (tvide == NULL)
    {
        detruire(renderer, window);
        exitError("Impossible de charger la texture : vide");
    }

    tbombe = SDL_CreateTextureFromSurface(renderer, bombe);
    SDL_FreeSurface(bombe);
    if (tbombe == NULL)
    {
        detruire(renderer, window);
        exitError("Impossible de charger la texture : bombe");
    }

    tdrapeau = SDL_CreateTextureFromSurface(renderer, drapeau);
    SDL_FreeSurface(drapeau);
    if (tdrapeau == NULL)
    {
        detruire(renderer, window);
        exitError("Impossible de charger la texture : drapeau");
    }

    tzero = SDL_CreateTextureFromSurface(renderer, zero);
    SDL_FreeSurface(zero);
    if (tzero == NULL)
    {
        detruire(renderer, window);
        exitError("Impossible de charger la texture : zero");
    }

    tun = SDL_CreateTextureFromSurface(renderer, un);
    SDL_FreeSurface(un);
    if (tun == NULL)
    {
        detruire(renderer, window);
        exitError("Impossible de charger la texture : un");
    }

    tdeux = SDL_CreateTextureFromSurface(renderer, deux);
    SDL_FreeSurface(deux);
    if (tdeux == NULL)
    {
        detruire(renderer, window);
        exitError("Impossible de charger la texture : deux");
    }

    ttrois = SDL_CreateTextureFromSurface(renderer, trois);
    SDL_FreeSurface(trois);
    if (ttrois == NULL)
    {
        detruire(renderer, window);
        exitError("Impossible de charger la texture : trois");
    }

    tquatre = SDL_CreateTextureFromSurface(renderer, quatre);
    SDL_FreeSurface(quatre);
    if (tquatre == NULL)
    {
        detruire(renderer, window);
        exitError("Impossible de charger la texture : quatre");
    }

    tcinq = SDL_CreateTextureFromSurface(renderer, cinq);
    SDL_FreeSurface(cinq);
    if (tcinq == NULL)
    {
        detruire(renderer, window);
        exitError("Impossible de charger la texture : cinq");
    }

    tsix = SDL_CreateTextureFromSurface(renderer, six);
    SDL_FreeSurface(six);
    if (tsix == NULL)
    {
        detruire(renderer, window);
        exitError("Impossible de charger la texture : six");
    }

    tsept = SDL_CreateTextureFromSurface(renderer, sept);
    SDL_FreeSurface(sept);
    if (tsept == NULL)
    {
        detruire(renderer, window);
        exitError("Impossible de charger la texture : sept");
    }

    thuit = SDL_CreateTextureFromSurface(renderer, huit);
    SDL_FreeSurface(huit);
    if (thuit == NULL)
    {
        detruire(renderer, window);
        exitError("Impossible de charger la texture : huit");
    }

    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            if (grid[i][j].see == 0)
            {
                texture = tvide;
            }
            else {
                if (grid[i][j].type == 3)
                {
                    texture = tdrapeau;
                }
                if (grid[i][j].isbomb == 1)
                {
                    texture = tbombe;
                }
                else {
                    if (grid[i][j].autour == 0)
                    {
                        texture = tzero;
                    }
                    if (grid[i][j].autour == 1)
                    {
                        texture = tun;
                    }
                    if (grid[i][j].autour == 2)
                    {
                        texture = tdeux;
                    }
                    if (grid[i][j].autour == 3)
                    {
                        texture = ttrois;
                    }
                    if (grid[i][j].autour == 4)
                    {
                        texture = tquatre;
                    }
                    if (grid[i][j].autour == 5)
                    {
                        texture = tcinq;
                    }
                    if (grid[i][j].autour == 6)
                    {
                        texture = tsix;
                    }
                    if (grid[i][j].autour == 7)
                    {
                        texture = tsept;
                    }
                    if (grid[i][j].autour == 8)
                    {
                        texture = thuit;
                    }
                }
            }

            SDL_Rect rectangle;
            if (SDL_QueryTexture(texture, NULL, NULL, &rectangle.w, &rectangle.h) != 0)
            {
                void detruire(renderer, window);
                exitError("Impossible de charger la texture : premier");
            }
            rectangle.x = j * 31;
            rectangle.y = i * 31;
            if (SDL_RenderCopy(renderer, texture, NULL, &rectangle) != 0)
            {
                void detruire(renderer, window);
                exitError("Impossible de charger la texture : second");
            }
        }

    }

    /*LANCEMENT JEU*/

    SDL_RenderPresent(renderer);
    if (finish == 1)
    {
        SDL_DestroyTexture(texture);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_QUIT;
    }

}

void ninvisible(Tile** grid)
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