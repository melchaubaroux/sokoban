//définir des constantes communes à tout le programme (taille de la fenêtre...)

#ifndef DEF_CONSTANTES
#define DEF_CONSTANTES

    #define TAILLE_BLOC         34 // Taille d'un bloc (carré) en pixels
    #define NB_BLOCS_LARGEUR    12
    #define NB_BLOCS_UPEUR    12
    #define LARGEUR_FENETRE     TAILLE_BLOC * NB_BLOCS_LARGEUR
    #define UPEUR_FENETRE     TAILLE_BLOC * NB_BLOCS_UPEUR


    enum {UP, DOWN, LEFT, RIGHT};
    enum {VIDE, WALL, CAISSE, OBJECTIF, CHARA, CAISSE_OK};

#endif
