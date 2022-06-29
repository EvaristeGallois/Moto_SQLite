#include <stdio.h>
#include "sqlite3.h"
#include <windows.h>
#include "console.h"

// Variables locales à la gestion de la base de données
static sqlite3 *db;
static char *zErrMsg = 0;
static char sql[300];
static char **table;


static int callback(void *NotUsed, int argc, char **argv, char **azColName)
{
    int i;
    for(i=0; i<argc; i++)
    {
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
    return 0;
}

/*
 * Arguments:
 *
 *   unused - Ignored in this case, see the documentation for sqlite3_exec
 *    count - The number of columns in the result set
 *     data - The row's data
 *  columns - The column names
 */
static int my_special_callback(void *unused, int count, char **data, char **columns)
{
    int idx;

    printf("Il y a %d colonne(s)\n", count);
    for (idx = 0; idx < count; idx++)
    {
        printf("La donnée dans la colonne\"%s\" est: %s\n", columns[idx], data[idx]);
    }
    printf("\n");
    return 0;
}

int The_Callback(void *a_param, int argc, char **argv, char **column)
{
    for (int i=0; i< argc; i++)
        printf("%s,\t", argv[i]);
    printf("\n");
    return 0;
}
int creation_ouverture_bd(void)
{
    /*
    sqlite3 *db;
    char *zErrMsg = 0;
    char *sql;
    char **table;
    */
    int touche, rc, i, j;

    clear_console();
    gotoxy (10,2);
    printf("Version de SQLite: %s\n\n", sqlite3_libversion());

// Test de l'existance de la base de données
    rc = sqlite3_open_v2 ("Motos.db", &db, SQLITE_OPEN_READWRITE, NULL);
    if( rc )
    {
        //fprintf(stderr, "Impossible d'ouvrir la base de données : %s\n", sqlite3_errmsg(db));
        printf("Base de données inexistante.\n");
        printf("Appuyer sur une touche:\n");
        touche = _getch();
        printf("Création de la base\n");
    }

    /* Ouverture de la base de données */
    rc = sqlite3_open("Motos.db", &db);
    if( rc )
    {
        fprintf(stderr, "Impossible d'ouvrir la base de données : %s\n", sqlite3_errmsg(db));
        return(0);
    }
    else
    {
        fprintf(stdout, "Base de données 'Motos.db' ouverte\n");
    }

    /* Create SQL statement */
    strcpy(sql , "CREATE TABLE IF NOT EXISTS MOTOS ("  \
          "ID INTEGER PRIMARY KEY AUTOINCREMENT," \
          "MARQUE      TEXT    NOT NULL," \
          "MODELE      TEXT     NOT NULL," \
          "TYPE        CHAR(50)," \
          "CYLINDREE   INT      NOT NULL );");

    /* Execution de la commande SQL */
    rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
    if( rc != SQLITE_OK )
    {
        fprintf(stderr, "Erreur SQL: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    else
    {
        fprintf(stdout, "Table créée.\n");
    }

    fprintf(stdout, "Appuyer sur une touche:\n");
    touche = _getch();
}

int supprimer_element(void)
{
    int rc, touche, id;
    char tab[80];
    char commande[200];

    clear_console();
    gotoxy (10,2);

    /* Nombre de champs */
    strcpy(sql, "SELECT COUNT(ID) FROM MOTOS");
    /* Execute SQL statement */
    rc = sqlite3_exec(db, sql, callback, &tab, &zErrMsg);
    if( rc != SQLITE_OK )
    {
        fprintf(stderr, "Erreur SQL: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    else
    {
        fprintf(stdout, "Nombre d'enregistrements.\n");
        printf("---->  %s \n", tab);
    }

    printf ("Entrez l'ID de l'élément à supprimer :");
    scanf("%d", &id);
    itoa(id,tab,10);
    strcpy(commande,"DELETE FROM MOTOS WHERE ID = ");
    strcat(commande, tab);
    strcpy(sql, commande);
    printf ("commande sql = %s\n", sql);
    /* Execute SQL statement */
    rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
    if( rc != SQLITE_OK )
    {
        fprintf(stderr, "Erreur SQL: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    else
    {
        fprintf(stdout, "Elément supprimé. %s\n", tab);
    }

    fprintf(stdout, "Appuyer sur une touche:\n");
    touche = _getch();
}

int reindexer_base(void)
{
    int rc, touche, id;
    char tab[80];
    char commande[200];

    clear_console();
    gotoxy (10,2);

        // Mise à jour de l'index
    strcpy(sql, " UPDATE MOTOS SET ID = rowid");
    /* Execute SQL statement */
    rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
    if( rc != SQLITE_OK )
    {
        fprintf(stderr, "Erreur SQL: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    else
    {
        fprintf(stdout, "Index principal mis à jour.\n");
    }

    strcpy(sql, "OPTIMIZE TABLE MOTOS;");
    /* Execute SQL statement */
    rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
    if( rc != SQLITE_OK )
    {
        fprintf(stderr, "\n\nErreur SQL: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    else
    {
        fprintf(stdout, "Table optimisée %s\n");
    }


    fprintf(stdout, "Appuyer sur une touche:\n");
    touche = _getch();
}

int renumeroter_ID_base(void)
{
    int rc, touche, id;
    char tab[80];
    char commande[200];

    clear_console();
    gotoxy (10,2);

    // Renumérotation du champs ID
    strcpy(sql, "UPDATE MOTOS SET ID=(SELECT (COUNT(*) + 1) * 1 FROM MOTOS AS T1 WHERE T1.ID < MOTOS.ID);");
    /* Execute SQL statement */
    rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
    if( rc != SQLITE_OK )
    {
        fprintf(stderr, "Erreur SQL: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    else
    {
        fprintf(stdout, "ID renumérotés.\n");
    }

    fprintf(stdout, "Appuyer sur une touche:\n");
    touche = _getch();
}


int ajouter_element(void)
{
    int rc, touche, cyl;
    char tab[80];
    char commande[300];

    clear_console();
    gotoxy (10,2);

    /* Nombre d'enregistrements */
    strcpy(sql, "SELECT COUNT(ID) FROM MOTOS");
    /* Execute SQL statement */
    rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
    if( rc != SQLITE_OK )
    {
        fprintf(stderr, "Erreur SQL: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    else
    {
        fprintf(stdout, "Nombre d'enregistrements\n\n");
    }

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
    strcpy(sql, commande);
    //sql = "INSERT INTO MOTOS (MARQUE, MODELE, TYPE, CYLINDREE) VALUES (tab,'T350','R','340')";

    // Saisie du type
    printf ("\nEntrez le type :");
    scanf("%s", &tab);
    strcat(commande, tab);
    strcat(commande,"','");
    strcpy(sql, commande);
    //sql = "INSERT INTO MOTOS (MARQUE, MODELE, TYPE, CYLINDREE) VALUES (tab,'T350','R','340')";

    // Saisie de la cylindrée
    printf ("\nEntrez la cylindrée :");
    scanf("%s", &tab);
    strcat(commande, tab);
    strcat(commande,"')");
    strcpy(sql, commande);
    //sql = "INSERT INTO MOTOS (MARQUE, MODELE, TYPE, CYLINDREE) VALUES (tab,'T350','R','340')";

    /* Execute SQL statement */
    rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
    if( rc != SQLITE_OK )
    {
        fprintf(stderr, "Erreur SQL: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    else
    {
        fprintf(stdout, "Elément ajouté.\n");
    }

    fprintf(stdout, "Appuyer sur une touche:\n");
    touche = _getch();
}

int fermer_bd(void)
{
    /* Libération de la mémoire */
    sqlite3_free_table(table);

    sqlite3_close(db);
}

int lister_bd(void)
{
   /* sqlite3 *db;
    char *zErrMsg = 0;
    char *sql;
    char **table; */
    int touche, rc, i, j, k, l, m, mod;
    int nrows, ncols,maxcol;
    char texte[200]="";
    char texte2[200]="";

    clear_console();
    gotoxy (10,2);

    rc = sqlite3_open("Motos.db", &db);
    if( rc )
    {
        fprintf(stderr, "Impossible d'ouvrir la base de données : %s\n", sqlite3_errmsg(db));
        return(0);
    }
    else
    {
        fprintf(stdout, "Base de données 'Motos.db' ouverte\n");
    }
    strcpy (sql ,"SELECT ID, MARQUE, MODELE, TYPE, CYLINDREE FROM MOTOS");
    rc = sqlite3_get_table(db, sql, &table, &nrows, &ncols, &zErrMsg);
    if( rc != SQLITE_OK )
    {
        fprintf(stderr, "Erreur SQL: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
        return(-1);
    }

    /* Exploitation de la table reçue*/
    strcpy(texte2, "+");
    for (i=0; i<ncols-1; i++)
    {
        strcat(texte2, "------------+");
    }
    strcat(texte2, "------------");
    strcat(texte2, "+\n");
    printf("%s",texte2);

    // Affichage du nom  des colonnes de la base
    maxcol = 12;
    for (i=0; i<ncols; i++)
    {
        strcpy(texte, "|");
        j = strlen(table[i]);
        k = maxcol - j;
        l = k / 2;
        mod = k % 2;
        //printf("j=%d, k=%d, l=%d, mod=%d m=%d \n",j,k,l,mod,m);
        for (m=0; m<(l+mod); m++)
            strcat(texte," ");
        strcat(texte,table[i]);
        for (m=0; m<l; m++)
            strcat(texte," ");
        printf("%s",texte);
    }
    printf("|\n");
    printf("%s",texte2);

// Affichage du contenu de la base en colonnes
    for (i=1; i<nrows+1; i++)
    {
        strcpy(texte, "|");
        for (j=i*ncols; j<(i*ncols)+ncols; j++)
        {
            l = strlen(table[j]);
            strcat(texte," ");
            strcat(texte,table[j]);
            for (m=1; m<maxcol-l;m++)
                strcat(texte," ");
            strcat(texte,"|");
        }
        printf("%s\n",texte);
    }
    printf("%s",texte2);

    fprintf(stdout, "Appuyer sur une touche:\n");
    touche = _getch();

}
/*
int bd_exec(void)
{
    char texte1[]="abcdefghijkl";
    char texte2[]="abcdefghijkl";
    char texte3[]="abcdefghijkl";

    int nrows, ncols;

    int  rc, i, j;

    printf("Version sqlite: %s\n\n", sqlite3_libversion());
    // Ouverture de la base de données
    rc = sqlite3_open("Motos.db", &db);
    if( rc )
    {
        fprintf(stderr, "Impossible d'ouvrir la base de données : %s\n", sqlite3_errmsg(db));
        return(0);
    }
    else
    {
        fprintf(stdout, "Base de données ouverte\n");
    }

    //Create SQL statement
    strcpy(sql ,"CREATE TABLE IF NOT EXISTS MOTOS ("  \
          "ID INTEGER PRIMARY KEY AUTOINCREMENT," \
          "MARQUE         TEXT    NOT NULL," \
          "MODELE         TEXT     NOT NULL," \
          "TYPE           CHAR(50)," \
          "CYLINDREE      INT      NOT NULL );");

   //Execution de la commande SQL
    rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
    if( rc != SQLITE_OK )
    {
        fprintf(stderr, "Erreur SQL: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    else
    {
        fprintf(stdout, "Table créée.\n");
    }

    //ajout de champs sans la clé primaire qui est auto-incrémentée
    sql = "INSERT INTO MOTOS (MARQUE, MODELE, TYPE, CYLINDREE) VALUES ('Suzuki','T350','R','340')");

    //Execute SQL statement
    rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
    if( rc != SQLITE_OK )
    {
        fprintf(stderr, "Erreur SQL: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    else
    {
        fprintf(stdout, "Elément ajouté.\n");
    }
    //Nombre de champs
    //sql="ALTER TABLE MOTOS AUTO_INCREMENT=1";
    strcpy(sql,"SELECT COUNT(ID) FROM MOTOS");
    //Execute SQL statement
    rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
    if( rc != SQLITE_OK )
    {
        fprintf(stderr, "Erreur SQL: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    else
    {
        fprintf(stdout, "Nombre de champs.\n");
    }

    //Liste des valeurs d'une colonne
    strcpy(sql ,"SELECT MARQUE, MODELE FROM MOTOS");
    // Execute SQL statement
    rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
    if( rc != SQLITE_OK )
    {
        fprintf(stderr, "Erreur SQL: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    else
    {
        fprintf(stdout, "Liste des modèles\n");
    }

    system("PAUSE");


    strcpy(sql ="SELECT ID, MARQUE, MODELE, TYPE, CYLINDREE FROM MOTOS";
    rc = sqlite3_get_table(db, sql, &table, &nrows, &ncols, &zErrMsg);
    if( rc != SQLITE_OK )
    {
        fprintf(stderr, "Erreur SQL: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
        return(-1);
    }

    //Exploitation de la table reçue
    printf("|12\t\t|123\t\t|1234\t|12345\t|\n");
    printf("%s\t%.5s\t%.10s\n", texte1, texte2, texte3);
    printf("-------------------------------------------------\n");

    for (i=0; i<ncols; i++)
    {
        printf("|%.10s\t", table[i]);
    }
    printf("|\n");
    printf("-------------------------------------------------\n");


    for (i=1; i<nrows+1; i++)
    {
        for (j=i*ncols; j<(i*ncols)+ncols; j++)
        {
            printf("|%.10s\t",table[j]);
        }
        printf("|\n");
    }
    printf("-------------------------------------------------\n");

    printf("\n");

    //Libération de la mémoire
    sqlite3_free_table(table);

    sqlite3_close(db);
}

*/
