//Gestion de l'éditeur

#include <stdlib.h>
#include <stdio.h>
#include <C:\Users\mamel\Desktop\sokoban\SDL\SDL.h>
//#include <C:\Users\mamel\Desktop\sokoban\SDL\SDL_image.h>
#include "constantes.h"
#include "editeur.h"
#include "fichiers.h"

void editeur(SDL_Surface* screen)
{
    SDL_Surface *wall = NULL, *caisse = NULL, *objectif = NULL, *chara = NULL;
    SDL_Rect position;
    SDL_Event event;

    int continuer = 1, clicLEFTEnCours = 0, clicRIGHTnCours = 0;
    int objetActuel = wall, i = 0, j = 0;
    int map[NB_BLOCS_LARGEUR][NB_BLOCS_UPEUR] = {0};

    // Chargement des objets et du niveau
    wall = IMG_Load("wall.jpg");
    caisse = IMG_Load("caisse.jpg");
    objectif = IMG_Load("objectif.png");
    chara = IMG_Load("chara.gif");

    if (!loadlvl(map))
        exit(EXIT_FAILURE);

    // Boucle infinie de l'éditeur
    while (continuer)
    {
        SDL_WaitEvent(&event);
        switch(event.type)
        {
            case SDL_QUIT:
                continuer = 0;
                break;
            case SDL_MOUSEBUTTONDOWN:
                if (event.button.button == SDL_BUTTON_LEFT)
                {
                    // On met l'objet actuallement choisi (wall, caisse...) à l'endroit du clic
                    map[event.button.x / TAILLE_BLOC][event.button.y / TAILLE_BLOC] = objetActuel;
                    clicLEFTEnCours = 1; // On active un booléen pour retenir qu'un bouton est enfoncé
                }
                else if (event.button.button == SDL_BUTTON_RIGHT) // Le clic droit sert à effacer
                {
                    map[event.button.x / TAILLE_BLOC][event.button.y /TAILLE_BLOC] = VIDE;
                    clicRIGHTnCours = 1;
                }
                break;
            case SDL_MOUSEBUTTONUP: // On désactive le booléen qui disait qu'un bouton était enfoncé
                if (event.button.button == SDL_BUTTON_LEFT)
                    clicLEFTEnCours = 0;
                else if (event.button.button == SDL_BUTTON_RIGHT)
                    clicRIGHTnCours = 0;
                break;
            case SDL_MOUSEMOTION:
                if (clicLEFTEnCours) // Si on déplace la souris et que le bouton LEFT de la souris est enfoncé
                {
                    map[event.motion.x / TAILLE_BLOC][event.motion.y / TAILLE_BLOC] = objetActuel;
                }
                else if (clicRIGHTnCours) // Pareil pour le bouton droit de la souris
                {
                    map[event.motion.x / TAILLE_BLOC][event.motion.y / TAILLE_BLOC] = VIDE;
                }
                break;
            case SDL_KEYDOWN:
                switch(event.key.keysym.sym)
                {
                    case SDLK_ESCAPE:
                        continuer = 0;
                        break;
                    case SDLK_s:
                        savelvl(map);
                        break;
                    case SDLK_c:
                        loadlvl(map);
                        break;
                    case SDLK_KP1:
                        objetActuel = WALL;
                        break;
                    case SDLK_KP2:
                        objetActuel = CAISSE;
                        break;
                    case SDLK_KP3:
                        objetActuel = OBJECTIF;
                        break;
                    case SDLK_KP4:
                        objetActuel = CHARA;
                        break;
                }
                break;
        }

        // Effacement de l'écran
        SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 255, 255, 255));

        // Placement des objets à l'écran
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
                    case OBJECTIF:
                        SDL_BlitSurface(objectif, NULL, screen, &position);
                        break;
                    case CHARA:
                        SDL_BlitSurface(chara, NULL, screen, &position);
                        break;
                }
            }
        }

        // Mise à jour de l'écran
        SDL_Flip(screen);
    }

    SDL_FreeSurface(wall);
    SDL_FreeSurface(caisse);
    SDL_FreeSurface(objectif);
    SDL_FreeSurface(chara);
}
