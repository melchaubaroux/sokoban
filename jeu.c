#include <stdlib.h>
#include <stdio.h>
#include <C:\Users\mamel\Desktop\sokoban\SDL\SDL.h>
//#include <SDL_image.h>

#include "constantes.h"
#include "jeu.h"

void jouer(SDL_Surface* screen)
{
    SDL_Surface *chara[4] = {NULL}; // 4 surfaces pour chacune des directions
    SDL_Surface *wall = NULL, *caisse = NULL, *caisseOK = NULL, *objectif = NULL, *charactual = NULL;
    SDL_Rect position, positionJoueur;
    SDL_Event event;

    int continuer = 1, objectifsRestants = 0, i = 0, j = 0;
    int map[NB_BLOCS_LARGEUR][NB_BLOCS_UPEUR] = {0};

    // Chargement des sprites (décors, personnage...)
    wall = IMG_Load("wall.jpg");
    caisse = IMG_Load("caisse.jpg");
    caisseOK = IMG_Load("caisse_ok.jpg");
    objectif = IMG_Load("objectif.png");
    chara[DOWN] = IMG_Load("perso_DOWN.gif");
    chara[LEFT] = IMG_Load("perso_LEFT.gif");
    chara[UP] = IMG_Load("perso_UP.gif");
    chara[RIGHT] = IMG_Load("perso_RIGHT.gif");

    charactual = chara[DOWN]; // Le perso sera dirigé vers le DOWN au départ

    // Chargement du niveau
    if (!loadlvl(map))
        exit(EXIT_FAILURE); // On arrête le jeu si on n'a pas pu charger le niveau

    // Recherche de la position de chara au départ
    for (i = 0 ; i < NB_BLOCS_LARGEUR ; i++)
    {
        for (j = 0 ; j < NB_BLOCS_UPEUR ; j++)
        {
            if (map[i][j] == CHARA) // Si on se trouve à cette position sur la map
            {
                positionJoueur.x = i;
                positionJoueur.y = j;
                map[i][j] = VIDE;
            }
        }
    }

    // Activation de la répétition des touches
    SDL_EnableKeyRepeat(100, 100);

    while (continuer)
    {
        SDL_WaitEvent(&event);
switch(event.type)
{
    case SDL_QUIT:
        continuer = 0;
        break;
    case SDL_KEYDOWN:
        switch(event.key.keysym.sym)
        {
            case SDLK_ESCAPE:
                continuer = 0;
                break;
            case SDLK_UP:
                charactual = chara[UP];
                deplacerJoueur(map, &positionJoueur, UP);
                break;
            case SDLK_DOWN:
                charactual = chara[DOWN];
                deplacerJoueur(map, &positionJoueur, DOWN);
                break;
            case SDLK_RIGHT:
                charactual = chara[RIGHT];
                deplacerJoueur(map, &positionJoueur, RIGHT);
                break;
            case SDLK_LEFT:
                charactual = chara[LEFT];
                deplacerJoueur(map, &positionJoueur, LEFT);
                break;
        }
        break;
}

        // Effacement de l'écran
        SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 255, 255, 255));

        // Placement des objets à l'écran
        objectifsRestants = 0;

        for (i = 0 ; i < NB_BLOCS_LARGEUR ; i++)
        {
            for (j = 0 ; j < NB_BLOCS_UPEUR ; j++)
            {
                position.x = i * TAILLE_BLOC;
                position.y = j * TAILLE_BLOC;

                switch(map[i][j])
                {
                    case WALL:
                        SDL_BlitSurface(wall, NULL, screen, &position);
                        break;
                    case CAISSE:
                        SDL_BlitSurface(caisse, NULL, screen, &position);
                        break;
                    case CAISSE_OK:
                        SDL_BlitSurface(caisseOK, NULL, screen, &position);
                        break;
                    case OBJECTIF:
                        SDL_BlitSurface(objectif, NULL, screen, &position);
                        objectifsRestants = 1;
                        break;
                }
            }
        }

        // Si on n'a trouvé aucun objectif sur la map, c'est qu'on a gagné
        if (!objectifsRestants)
            continuer = 0;

        // On place le joueur à la bonne position
        position.x = positionJoueur.x * TAILLE_BLOC;
        position.y = positionJoueur.y * TAILLE_BLOC;
        SDL_BlitSurface(charactual, NULL, screen, &position);



        SDL_Flip(screen);
    }

    // Désactivation de la répétition des touches (remise à 0)
    SDL_EnableKeyRepeat(0, 0);

    // Libération des surfaces chargées
    SDL_FreeSurface(wall);
    SDL_FreeSurface(caisse);
    SDL_FreeSurface(caisseOK);
    SDL_FreeSurface(objectif);
    for (i = 0 ; i < 4 ; i++)
        SDL_FreeSurface(chara[i]);
}

void deplacerJoueur(int map[][NB_BLOCS_UPEUR], SDL_Rect *pos, int direction)
{
    switch(direction)
    {
        case UP:
            if (pos->y - 1 < 0) // Si le joueur dépasse l'écran, on arrête
                break;
            if (map[pos->x][pos->y - 1] == WALL) // S'il y a un wall, on arrête
                break;
            // Si on veut pousser une caisse, il faut vérifier qu'il n'y a pas de wall derrière (ou une autre caisse, ou la limite du monde)
            if ((map[pos->x][pos->y - 1] == CAISSE || map[pos->x][pos->y - 1] == CAISSE_OK) &&
                (pos->y - 2 < 0 || map[pos->x][pos->y - 2] == WALL ||
                map[pos->x][pos->y - 2] == CAISSE || map[pos->x][pos->y - 2] == CAISSE_OK))
                break;

            // Si on arrive là, c'est qu'on peut déplacer le joueur !
            // On vérifie d'abord s'il y a une caisse à déplacer
            deplacerCaisse(&map[pos->x][pos->y - 1], &map[pos->x][pos->y - 2]);

            pos->y--; // On peut enfin faire monter le joueur (oufff !)
            break;


        case DOWN:
            if (pos->y + 1 >= NB_BLOCS_UPEUR)
                break;
            if (map[pos->x][pos->y + 1] == WALL)
                break;

            if ((map[pos->x][pos->y + 1] == CAISSE || map[pos->x][pos->y + 1] == CAISSE_OK) &&
                (pos->y + 2 >= NB_BLOCS_UPEUR || map[pos->x][pos->y + 2] == WALL ||
                map[pos->x][pos->y + 2] == CAISSE || map[pos->x][pos->y + 2] == CAISSE_OK))
                break;


            deplacerCaisse(&map[pos->x][pos->y + 1], &map[pos->x][pos->y + 2]);

            pos->y++;
            break;


        case LEFT:
            if (pos->x - 1 < 0)
                break;
            if (map[pos->x - 1][pos->y] == WALL)
                break;

            if ((map[pos->x - 1][pos->y] == CAISSE || map[pos->x - 1][pos->y] == CAISSE_OK) &&
                (pos->x - 2 < 0 || map[pos->x - 2][pos->y] == WALL ||
                map[pos->x - 2][pos->y] == CAISSE || map[pos->x - 2][pos->y] == CAISSE_OK))
                break;


            deplacerCaisse(&map[pos->x - 1][pos->y], &map[pos->x - 2][pos->y]);

            pos->x--;
            break;


        case RIGHT:
            if (pos->x + 1 >= NB_BLOCS_LARGEUR)
                break;
            if (map[pos->x + 1][pos->y] == WALL)
                break;

            if ((map[pos->x + 1][pos->y] == CAISSE || map[pos->x + 1][pos->y] == CAISSE_OK) &&
                (pos->x + 2 >= NB_BLOCS_LARGEUR || map[pos->x + 2][pos->y] == WALL ||
                map[pos->x + 2][pos->y] == CAISSE || map[pos->x + 2][pos->y] == CAISSE_OK))
                break;

            deplacerCaisse(&map[pos->x + 1][pos->y], &map[pos->x + 2][pos->y]);

            pos->x++;
            break;
    }
}

void deplacerCaisse(int *premiereCase, int *secondeCase)
{
    if (*premiereCase == CAISSE || *premiereCase == CAISSE_OK)
    {
        if (*secondeCase == OBJECTIF)
            *secondeCase = CAISSE_OK;
        else
            *secondeCase = CAISSE;

        if (*premiereCase == CAISSE_OK)
            *premiereCase = OBJECTIF;
        else
            *premiereCase = VIDE;
    }
}

