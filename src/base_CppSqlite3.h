/*****************************************************
*                                                    *
*                  BASE_CPPSQLITE3.H                 *
*                                                    *
*****************************************************/
#ifndef BASE_CPPSQLITE3_H
#define BASE_CPPSQLITE3_H

int bd_exec(void);
int creation_ouverture_bd(void);
int ajouter_element(void);
int supprimer_element(void);
int editer_element(void);
int reindexer_base(void);
int renumeroter_ID_base();
int lister_bd(void);
int fermer_bd(void);

#endif /* BASE_CPPSQLITE3_H */
