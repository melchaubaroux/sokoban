#ifndef DEF_JEU
#define DEF_JEU

    void jouer(SDL_Surface* screen);
    void deplacerJoueur(int map[][NB_BLOCS_UPEUR], SDL_Rect *pos, int direction);
    void deplacerCaisse(int *premiereCase, int *secondeCase);


#endif
