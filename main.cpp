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
//#include <iostream>
#include <windows.h>
#include <conio.h>
#include <locale> // Pour utilisation des caractères accentués

#include "console.h"
#include "menu.h"
#include "base.h"

int main(int argc, char* argv[])
{
    int menu_choisi, ret, i, quitter=0;

    setlocale(LC_CTYPE, "fra"); // Initialisation du jeu de caractères accentués Français
    // redimensionnement de la console
    i = resize_console(140,50);
    //gestion du menu
    while(quitter==0)
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
              ret = supprimer_element();
              break;
            case 5:
              ret = reindexer_base();
              break;

            case 6:
              quitter=1;
              break;
            default:
                // default statements
                break;
        }
    }
    // Fermeture de la base de donnée
    ret = fermer_bd();
    i = resize_console(120,50);

   return 0;
}
