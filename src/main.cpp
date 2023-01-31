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
            case 0:
              // statements
              break;
            case 1:
              ret = creation_ouverture_bd();
              break;
            case 2:
              ret = lister_bd();
              break;
            case 3:
              ret = ajouter_element();
              break;
            case 4:
              ret = editer_element();
              break;
            case 5:
              ret = supprimer_element();
              break;
            case 6:
              ret = renumeroter_ID_base();
              //ret = reindexer_base();
              break;
            case 7:
              quitter=true;
              break;
            default:
                //
                break;
        }
    }
    // Fermeture de la base de donnée
    ret = fermer_bd();
    clear_console();
    //i = resize_console(120,50);
   return 0;
}
