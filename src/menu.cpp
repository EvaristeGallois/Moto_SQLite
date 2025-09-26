/***************************************************/
/*                                                 */
/*            Programme : Moto_SQLite              */
/*                                                 */
/*                    menu.c                       */
/*                                                 */
/***************************************************/
// Permet la g�n�ration et le traitement d'un menu vertical
// sur la console
#include "console.h"

// Définition du menu
static const std::array<std::string, 7> Menu = {
    "Créer/Ouvrir la Base",
    "Lister la base",
    "Ajouter un élément",
    "Editer un élément",
    "Supprimer un élément",
    "Renuméroter ID",
    "Quitter"
};

// Codes des touches (Windows)
constexpr int KEY_UP = 0x48;
constexpr int KEY_DOWN = 0x50;
constexpr int KEY_ENTER = 0x0D;

/// OLD  static const char* Menu[] = {"Cr�er/Ouvrir la Base","Lister la base","Ajouter un �l�ment","Editer un �l�ment","Supprimer un �l�ment","Renum�roter ID","Quitter"};

int Choix_menu(int pos_x, int pos_y)
{
    int nb_elements = 7;
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
    while (true)
    {
        /*
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
        */
         touche = _getch();

        if (touche == KEY_DOWN && curseur < nb_elements - 1)
        {
            ++curseur;
        }
        else if (touche == KEY_UP && curseur > 0)
        {
            --curseur;
        }
        else if (touche == KEY_ENTER)
        {
            show_cursor(1); // Réafficher le curseur avant de quitter
            return curseur + 1; // +1 si tu veux l'index à partir de 1
        }

        // Mise à jour de l'affichage
        for (int i = 0; i < nb_elements; ++i)
        {
            gotoxy(pos_x, pos_y + i);
            std::cout << ((i == curseur) ? ">" : " ") << " " << Menu[i];
        }
    }
    show_cursor(1);
    return 0;
}
