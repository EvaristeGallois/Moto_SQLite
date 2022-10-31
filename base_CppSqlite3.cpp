/*****************************************************
*                                                    *
*                 BASE_CPPSQLITE3.CPP                *
*                                                    *
*****************************************************/
#include <stdio.h>
#include <windows.h>
#include "console.h"

#include "CppSQLite3/CppSQLite3.h"
#include <iostream>

using namespace std;

// Variables locales à la gestion de la base de données

static char *zErrMsg = 0;
static char sql[300];
static char **table;

const char* gszFile = "Motos.db";

CppSQLite3DB db;

/*******************************************
*           creation_ouverture_bd          *
*******************************************/
int creation_ouverture_bd(void)
{
  try
  {
    int touche, rc, i, j;

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
}

/*******************************************
*             supprimer_element            *
*******************************************/
int supprimer_element(void)
{
  try
  {
    int rc, touche, id, fld, nRows;
    char tab[80];
    int compte = 0;
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
}

/*******************************************
*            renumeroter_ID_base           *
*******************************************/
int renumeroter_ID_base(void)
{
  try
  {

    int rc, touche, id;
    char tab[80];
    char commande[200];

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
}

/*******************************************
*              ajouter_element             *
*******************************************/
int ajouter_element(void)
{
   try
  {
    int rc, touche, cyl;
    char tab[80];
    char commande[300];

    clear_console();
    gotoxy (10,2);

    cout << db.execScalar("SELECT COUNT(*) FROM MOTOS;") << " enregistrements dans la table MOTOS " << endl;

    // Saisie de la marque
    printf ("Entrez la marque :");
    scanf("%s", &tab);
    //scanf("Marque : %80s", tab);
    strcpy(commande,"INSERT INTO MOTOS (MARQUE, MODELE, TYPE, CYLINDREE) VALUES ('");
    strcat(commande, tab);
    strcat(commande,"','");

    // Saisie du modèle
    printf ("\nEntrez le modèle :");
    scanf("%s", &tab);
    strcat(commande, tab);
    strcat(commande,"','");

    // Saisie du type
    printf ("\nEntrez le type :");
    scanf("%s", &tab);
    strcat(commande, tab);
    strcat(commande,"','");

    // Saisie de la cylindrée
    printf ("\nEntrez la cylindrée :");
    scanf("%s", &tab);
    strcat(commande, tab);
    strcat(commande,"')");
    strcpy(sql, commande);

    int nRows = db.execDML(sql);

    fprintf(stdout, "Appuyer sur une touche:\n");
    touche = _getch();
  }
    catch (CppSQLite3Exception& e)
    {
        cerr << e.errorCode() << ":" << e.errorMessage() << endl;
    }
}

/*******************************************
*                 fermer_bd                *
*******************************************/
int fermer_bd(void)
{
    db.close();
}

/*******************************************
*                  lister_bd               *
*******************************************/
int lister_bd(void)
{
    int touche, rc, i, j, k, l, m, mod, fld, row;
    int nrows, ncols,maxcol;
    char texte[200]="";
    char texte2[200]="";
    int lig = 2, col = 10;
    string tex, tex2, ch;

    clear_console();
    gotoxy (col,lig++);

    CppSQLite3Table t = db.getTable("SELECT ID, MARQUE, MODELE, TYPE, CYLINDREE FROM MOTOS;");

    tex2 = "+";
    for (i=0; i<t.numFields()-1; i++)
    {
       tex2 += "--------------+";
    }
    tex2 += "--------------+";
    gotoxy (col,lig++);
    cout << tex2 << endl;
    maxcol = 14;
    gotoxy (col,lig++);
    for (fld = 0; fld < t.numFields(); fld++)
    {
        tex = "|";
        j = strlen(t.fieldName(fld));
        k = maxcol - j;
        l = k / 2;
        mod = k % 2;
        for (m=0; m<(l+mod); m++)
            tex +=" ";
        tex += t.fieldName(fld);
        for (m=0; m<l; m++)
            tex += " ";
        cout << tex;

        //cout << t.fieldName(fld) << "|";
    }
    cout << "|" << endl;
    gotoxy (col,lig++);
    cout << tex2 << endl;

    for (int row = 0; row < t.numRows(); row++)
    {
        gotoxy (col,lig++);
        cout << "|";
        t.setRow(row);
        for (fld = 0; fld < t.numFields(); fld++)
        {
            if (!t.fieldIsNull(fld))
            {
              l = strlen(t.fieldValue(fld));
              cout << " " << t.fieldValue(fld);
              for (m=1; m<maxcol-l; m++)
                  cout << " ";
              cout << "|";
            }
            else
            {
                cout << "NULL" << "|";
            }
        }
        cout << endl;
    }
    gotoxy (col,lig++);
    cout << tex2 << endl;

    fprintf(stdout, "Appuyer sur une touche:\n");
    touche = _getch();

}

