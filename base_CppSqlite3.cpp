/*****************************************************
*                                                    *
*                 BASE_CPPSQLITE3.CPP                *
*                                                    *
*****************************************************/
#include <stdio.h>
#include <windows.h>
#include "console.h"
#include <string.h> // Penser à inclure string.h pour strchr()
#include "CppSQLite3/CppSQLite3.h"
#include <iostream>


//#include <stdlib.h>


using namespace std;

// Variables locales à la gestion de la base de données

static char *zErrMsg = 0;
static char sql[300];

const char* gszFile = "Motos.db";

CppSQLite3DB db;

/*******************************************
*             longueur_max_champ           *
*******************************************/
// Retourne la longueur maximum des champs
// d'une colonne afin de déterminer la largeur
// maximum pour l'affichage
int longueur_max_champ(CppSQLite3Table &t, int num_col)
{
   int lg;
   int longueur_max = 0;
   for (int row = 0; row < t.numRows(); row++)
   {
      t.setRow(row);
      if (!t.fieldIsNull(num_col))
      {
         lg = strlen(t.fieldValue(num_col));
         if (lg > longueur_max)
         {
            longueur_max = lg;
         }
      }
   }
   return longueur_max;
}

/*******************************************
*                viderBuffer               *
*******************************************/
// Vide le buffer de saisie de caractère
void viderBuffer()
{
   int c = 0;
   while (c != '\n' && c != EOF)
   {
      c = getchar();
   }
}


/*******************************************
*                   lire                   *
*******************************************/
// Lit un nombre maximum saisit au clavier
int lire(char *chaine, int longueur)
{
   char *positionEntree = NULL;

   if (fgets(chaine, longueur, stdin) != NULL)
   {
      positionEntree = strchr(chaine, '\n');
      if (positionEntree != NULL)
      {
         *positionEntree = '\0';
      }
      else
      {
         viderBuffer();
      }
      if( strlen(chaine)<=0 || chaine[0]== '\n' || chaine[0] == '\n')
         return 0;
      return 1;
   }
   else
   {
      viderBuffer();
      return 0;
   }
}


/*******************************************
*           creation_ouverture_bd          *
*******************************************/
int creation_ouverture_bd(void)
{
   try
   {
      int touche;

      clear_console();
      gotoxy (10,2);
      printf("Version de SQLite: %s\n\n", sqlite3_libversion());
      // Versions SQLite
      cout << "SQLite Header Version: " << CppSQLite3DB::SQLiteHeaderVersion() << endl;
      cout << "SQLite Library Version: " << CppSQLite3DB::SQLiteLibraryVersion() << endl;
      cout << "SQLite Library Version Number: " << CppSQLite3DB::SQLiteLibraryVersionNumber() << endl;

      // Test de l'existance de la base de données
      db.open(gszFile);

      // Création de la table
      db.execDML("CREATE TABLE IF NOT EXISTS MOTOS ("  \
                 "ID INTEGER PRIMARY KEY AUTOINCREMENT," \
                 "MARQUE      TEXT    NOT NULL," \
                 "MODELE      TEXT     NOT NULL," \
                 "TYPE        CHAR(50)," \
                 "CYLINDREE   INTEGER      NOT NULL );");

      cout << endl << "Existance table MOTOS=" << (db.tableExists("MOTOS") ? "TRUE":"FALSE") << endl;
      fprintf(stdout, "Appuyer sur une touche:\n");
      touche = _getch();
   }
   catch (CppSQLite3Exception& e)
   {
      cerr << e.errorCode() << ":" << e.errorMessage() << endl;
   }
   return 1;
}

/*******************************************
*             supprimer_element            *
*******************************************/
int supprimer_element(void)
{
   try
   {
      int touche, id, nRows;
      char tab[80];
      char commande[200];
      std::string sql_cde;

      clear_console();
      gotoxy (10,2);

      cout << db.execScalar("SELECT COUNT(*) FROM MOTOS;") << " enregistrements dans la table MOTOS " << endl;

      printf ("Entrez l'ID de l'élément à supprimer :");
      scanf("%d", &id);
      itoa(id,tab,10);
      strcpy(commande,"DELETE FROM MOTOS WHERE ID = ");
      strcat(commande, tab);
      strcpy(sql, commande);
      printf ("commande sql = %s\n", sql);

      nRows = db.execDML(sql);
      cout << nRows << " enregitrement(s) effacé(s)" << endl;

      fprintf(stdout, "Appuyer sur une touche:\n");
      touche = _getch();
   }
   catch (CppSQLite3Exception& e)
   {
      cerr << e.errorCode() << ":" << e.errorMessage() << endl;
   }
   return 1;
}

/*******************************************
*            renumeroter_ID_base           *
*******************************************/
int renumeroter_ID_base(void)
{
   try
   {
      int touche;

      clear_console();
      gotoxy (10,2);
      // Création de la table
      db.execDML("UPDATE MOTOS SET ID=(SELECT (COUNT(*) + 1) * 1 FROM MOTOS AS T1 WHERE T1.ID < MOTOS.ID);");

      fprintf(stdout, "Appuyer sur une touche:\n");
      touche = _getch();
   }
   catch (CppSQLite3Exception& e)
   {
      cerr << e.errorCode() << ":" << e.errorMessage() << endl;
   }
   return 1;
}

/*******************************************
*              ajouter_element             *
*******************************************/
int ajouter_element(void)
{
   try
   {
      int touche;
      //char tab[80];
      char *tab = new char[50];
      char *liste_saisie[4] = {"Entrez la marque :","Entrez le modèle :","Entrez le type :","Entrez la cylindrée :"};
      int nb_champs = 4;
      char commande[300];

      clear_console();
      gotoxy (10,2);
      cout << db.execScalar("SELECT COUNT(*) FROM MOTOS;") << " enregistrements dans la table MOTOS " << endl;
      sprintf (commande, "%s", "INSERT INTO MOTOS (MARQUE,MODELE,TYPE,CYLINDREE) VALUES ('");
      for (int i = 0; i<nb_champs; i++)
      {
         printf ("\n%s",liste_saisie[i]);
         if (lire(tab, 15) == 1)
         {
            sprintf (commande, "%s%s%s", commande, tab, "','");
         }
         else
         {
            printf ("\nErreur dans la saisie\n");
            fprintf(stdout, "Appuyer sur une touche:\n");
            touche = _getch();
            return 0;
         }
      }
      int l = strlen(commande);
      commande[l-2]=')';
      commande[l-1]=';';

      int nRows = db.execDML(commande);

      fprintf(stdout, "Appuyer sur une touche:\n");
      touche = _getch();
   }
   catch (CppSQLite3Exception& e)
   {
      cerr << e.errorCode() << ":" << e.errorMessage() << endl;
   }
   return 1;
}

/*******************************************
*                 fermer_bd                *
*******************************************/
int fermer_bd(void)
{
   db.close();
   return 1;
}

/*******************************************
*                 lister_bd                *
*******************************************/
int lister_bd(void)
{
   int touche, i, k, lg, m, mod, fld, row;
   int lig = 2, col = 10;
   string tex, tex2, tex3, ch;
   int espace[20];
   int lgmax;

   clear_console();
   CppSQLite3Table t = db.getTable("SELECT ID, MARQUE, MODELE, TYPE, CYLINDREE FROM MOTOS;");

   // Calcul des espaces max pour les différentes colonnes
   // et stockage dans le tableau espace[]
   // et construction de la ligne séparatrice
   tex3.assign("+");
   for (fld = 0; fld < t.numFields(); fld++)
   {
      lg = strlen(t.fieldName(fld));
      for (row = 0; row < t.numRows(); row++)
      {
         lgmax = longueur_max_champ(t, fld);
      }
      if (lg > lgmax)
      {
         lgmax = lg;
      }
      espace[fld] = lgmax;
      for (i=0; i < lgmax + 2; i++)
         tex3.append("-");
      tex3.append("+");
   }
   gotoxy (col,lig++);
   cout << tex3 ;
   gotoxy (col,lig++);
   tex = "|";
   for (fld = 0; fld < t.numFields(); fld++)
   {
      lg = strlen(t.fieldName(fld));
      k =espace[fld] + 2 - lg;
      lg = k / 2;
      mod = k % 2;
      for (m = 0; m < (lg + mod); m++)
         tex += " ";
      tex += t.fieldName(fld);
      for (m = 0; m < lg; m++)
         tex += " ";
      tex += "|";
   }
   cout << tex;
   gotoxy (col,lig++);
   cout << tex3 ;

   for (row = 0; row < t.numRows(); row++)
   {
      gotoxy (col,lig++);
      cout << "|";
      t.setRow(row);
      for (fld = 0; fld < t.numFields(); fld++)
      {
         if (!t.fieldIsNull(fld))
         {
            lg = strlen(t.fieldValue(fld));
            cout << " " << t.fieldValue(fld);
         }
         else
         {
            lg = strlen("NULL");
            cout << " " << "NULL";
         }
         for (m=0; m < espace[fld] - lg + 1; m++)
            cout << " ";
         cout << "|";
      }
      cout << endl;
   }
   gotoxy (col,lig++);
   cout << tex3 << endl;

   fprintf(stdout, "Appuyer sur une touche:\n");
   touche = _getch();

   return 1;
}


