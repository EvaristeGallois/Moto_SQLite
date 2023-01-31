#include <iostream>
#include "console.h"
#include <string>
#include  <iostream>
using namespace std;

int main()
{
   int touche;
   char *tab = new char[50];
   strcpy(tab, "abcdefghijklmn");
   if (edit_line(tab, 30, 1, 1) == 0)
   {
      printf ("\n\n%s\n", tab);
   }
   else
   {
      printf ("\n\nErreur dans la saisie\n");
   }
   fprintf(stdout, "Appuyer sur une touche:\n");
   touche = _getch();

  // char chaine[81]; // 80 caractères + '\0' terminal
  // printf("Donnez une phrase (pas plus de 80 car.) : ");
  // show_cursor(TRUE);
  // strcpy(chaine, "Bojour Monsieur Duguidon.");
  // fgets(chaine, 81, stdin);
  // chaine[strlen(chaine)-1]='\0'; //enlève le '\n'
  // printf ("\n\n%s\n", chaine);

   return 0;
}
