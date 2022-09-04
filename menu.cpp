/***************************************************/
/*                                                 */
/*            Programme : Moto_SQLite              */
/*                                                 */
/*                    menu.c                       */
/*                                                 */
/***************************************************/

#include "console.h"

static const char* Menu[] = {"Cr�er/Ouvrir la Base","Lister la base","Ajouter un �l�ment","Supprimer un �l�ment","Renum�roter ID","Quitter"};

int Choix_menu(int pos_x, int pos_y)
{
    int nb_elements = 6;
    int i,curseur = 0;
    int touche; // Touche enfonc�e : Fl�che haute, Fl�che basse
    show_cursor(0);
    clear_console();
    for (i = 0; i < nb_elements; i++)
    {
        gotoxy(pos_x,pos_y+i);
        printf("  %s\n", Menu[i]);
    }
    gotoxy(pos_x,pos_y);
    printf(">");
    while (1)
    {
        touche = _getch();
        if (touche == 0x50 && curseur < nb_elements-1) // Test si fl�che basse
            curseur++;
        if (touche == 0x48 && curseur > 0) // Test si fl�che haute
            curseur--;
        if (touche == 0x0D) // Test si Retour Chariot
            return curseur + 1;
        for (i = 0; i < nb_elements; i++)
        {
            gotoxy(pos_x,pos_y+i);
            printf("%c\n", (i == curseur) ? '>':' ');
        }
    }
    show_cursor(1);
    return 0;
}
