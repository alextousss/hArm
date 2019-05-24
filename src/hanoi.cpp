
/*
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct deplacement deplacement;
struct deplacement
{
    int from, to;
};
deplacement *tour(int, int, int, int);

deplacement *coup_a_jouer()
{
    int Nombre_de_disques;
    int Nombre_de_deplacement;
    int compteur;
    printf("entrez le nombre de disques\n");
    scanf("%d", &Nombre_de_disques); //On demande le nombre de disques en jeu
    deplacement *tableau = tour(Nombre_de_disques, 1, 2, 3);
    Nombre_de_deplacement = pow(2, Nombre_de_disques) - 1; //Nombre de déplacement mini (On peut l'enlever en cas de bug)
    printf("Il y a %d coup a jouer\n", Nombre_de_deplacement);
    for (compteur = 0; compteur < Nombre_de_deplacement; compteur++)
    {
        printf("Il faut faire le deplacement {%d , %d}\n", tableau[compteur], tableau[compteur]);
    }
    return tableau;
}

deplacement *tour(int Nombre_de_disques, int source, int zone_de_passage, int destination)
{

    static deplacement tableau[100];
    static int g_compteur;
    deplacement i;

    if (Nombre_de_disques == 1) //Valeur du déplacement à renvoyer pour n=1
    {

        if (source == 1 && destination == 2)
        {
            i.from = 0, i.to = 1;
        } //De la tour 1 à la tour 2
        if (source == 1 && destination == 3)
        {
            i.from = 0, i.to = 2;
        } //De la tour 1 à la tour 3
        if (source == 2 && destination == 1)
        {
            i.from = 1, i.to = 0;
        } //De la tour 2 à la tour 1
        if (source == 2 && destination == 3)
        {
            i.from = 1, i.to = 2;
        } //De la tour 2 à la tour 3
        if (source == 3 && destination == 1)
        {
            i.from = 2, i.to = 0;
        } //De la tour 3 à la tour 1
        if (source == 3 && destination == 2)
        {
            i.from = 2, i.to = 1;
        } //De la tour 3 à la tour 2

        tableau[g_compteur] = i;
        g_compteur++;
    }

    else if (Nombre_de_disques > 1) //Fonction recursive de base
    {
        tour(Nombre_de_disques - 1, source, destination, zone_de_passage);
        tour(1, source, zone_de_passage, destination);
        tour(Nombre_de_disques - 1, zone_de_passage, source, destination);
    }

    return tableau;
}

int main() //Juste un test
{
    *coup_a_jouer();
    return 0;
}*/