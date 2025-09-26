/***************************************************/
/*                                                 */
/*            Programme : Moto_SQLite              */
/*                                                 */
/*                    main.cpp                     */
/*                                                 */
/***************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <conio.h>
#include <locale> // Pour utilisation des caractères accentués

#include "console.h"
#include "menu.h"
#include "base_CppSqlite3.h"

enum MenuOption {
    MENU_INVALID = -1,
    MENU_OPTION_0 = 0,
    MENU_CREER_OUVRIR = 1,
    MENU_LISTER = 2,
    MENU_AJOUTER = 3,
    MENU_EDITER = 4,
    MENU_SUPPRIMER = 5,
    MENU_RENUMEROTER = 6,
    MENU_QUITTER = 7
};

int main(int argc, char* argv[])
{
    int menu_choisi, ret, i;
    bool quitter = false;

    setlocale(LC_CTYPE, "fra"); // Initialisation du jeu de caractères accentués Français
    // redimensionnement de la console
    i = resize_console(140,50);
    //gestion du menu
    while(quitter==false)
    {
        menu_choisi = Choix_menu(10,10);
        switch (menu_choisi)
        {
            case MENU_OPTION_0:
              // statements
              break;
            case MENU_CREER_OUVRIR:
              ret = creation_ouverture_bd();
              break;
            case MENU_LISTER:
              ret = lister_bd();
              break;
            case MENU_AJOUTER:
              ret = ajouter_element();
              break;
            case MENU_EDITER:
              ret = editer_element(); // Dans version CppSqlite
              break;
            case MENU_SUPPRIMER:
              ret = supprimer_element();
              break;
            case MENU_RENUMEROTER:
              ret = renumeroter_ID_base();
              //ret = reindexer_base();
              break;
            case MENU_QUITTER:
              quitter=true;
              break;
            default:
                std::cerr << "Option invalide : " << menu_choisi << std::endl;
                break;
        }
    }
    // Fermeture de la base de donnée
    ret = fermer_bd();
    clear_console();
    //i = resize_console(120,50);
   return 0;
}
