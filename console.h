/**********************************************/
/*                                            */
/*                  console.h                 */
/*                                            */
/*     Gestion de l'écran en mode console     */
/*                                            */
/*                Version : 1.01              */
/**********************************************/
// 28/03/2021 Version 1.01 : Ajout des fonctions "edit_line"
//                           et "edit_line_string"
//                           Qui permettent de modifier/saisir une
//                           ligne de texte de type 'char *' ou 'string'
//
// 20/03/2021 Version 1.00 : Version initiale
//
#ifndef CONSOLE_H_INCLUDED
#define CONSOLE_H_INCLUDED

//#define _WIN32_WINNT 0x0501
//#define _WIN32_WINDOWS 0x600

// Permet de compiler des fonctions qui fonctionnent sous Visual C
// mais pas sous mingw (particuliérement les fontes)
//#define _VISUALCPP

#include <windows.h>
#include <string>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>

using namespace std;

/*****************************************/
/*                                       */
/*           YPES ET STRUCTURES          */
/*                                       */
/*****************************************/

typedef enum
{
   Info,
   Warming,
   Error,
   Fatal
}ErrLevel;

// scan du clavier
typedef struct
{
   BOOL keyState[256];
   int controlKeyState;
}KEYBOARD_DATA;

// CHBITMAP (matrice de lettres)
typedef struct
{
   int h;         // height, hauteur
   int w;         // width, largeur
   CHAR_INFO*dat; // Les données
}CHBITMAP;

// Couleurs des lettres sur la cosole
typedef enum {NOIR, BLEU_FONCE, VERT, CYAN, ROUGE, VIOLET, JAUNE, BLANC, GRIS_CLAIR, BLEU, VERT_CLAIR, BLEU_CLAIR, ROUGE_CLAIR, MAGENTA, JAUNE_CLAIR, BLANC2} T_COLOR_CHAR;
//0=noir, 1=bleu foncé, 2=vert, 3=cyan, 4=rouge; 5=violet, 6=jaune, 7=blanc, 8=gris clair, 9=bleu, 10=vert clair, 11=bleu clair, 12=rouge clair, 13=magenta, 14=jaune clair,15=blanc2, 16=noir sur fond bleu

#ifdef __cplusplus
extern "C"{
#endif

/*****************************************/
/*                                       */
/*               FONCTIONS               */
/*                                       */
/*****************************************/
/*---------------------------------------*/
/*    Fonctions Handles des fenêtres     */
/*---------------------------------------*/
COORD    coord(int x, int y);

HANDLE	 GetStdout(void);
HANDLE	 GetStdin(void);
HANDLE	 GetConsoleW(void);
void	   debug(int errLevel,char*msg);

/*---------------------------------------*/
/*            Fonctions console          */
/*---------------------------------------*/
BOOL	resize_console(int width, int height);
BOOL	resize_console_max(void);
int		screen_w(void);
int		screen_h(void);
COORD	screen_size(void);
void	fill_console(char asciiChar, int color);
void	clear_console(void);
void	test_screen_console_info(int x,int y,int color);

#ifdef _VISUALCPP
COORD	get_font_size(void);
DWORD	get_font_type(void);
void	title_console(LPCTSTR title);
#else
void	title_console(char*title);
#endif

/*---------------------------------------*/
/*     Gestion des couleurs du fond      */
/*            et des lettres             */
/*---------------------------------------*/
void	set_color(int color);
void	set_bf_color(int backcolor, int forecolor);
int		get_back_color(void);
int		get_fore_color(void);
void	set_back_color(int color);
void	set_fore_color(int color);

/*---------------------------------------*/
/*           Gestion du curseur          */
/*---------------------------------------*/
void	gotoxy(int x, int y);
int		wherex(void);
int		wherey(void);
void	show_cursor(BOOL visible);
void	size_cursor(unsigned int size);

/*---------------------------------------*/
/*         Gestion des évènements        */
/*            souris et clavier          */
/*---------------------------------------*/
void  poll_event(void);
BOOL  key_pressed(int keyCode);
COORD get_mouse_pos(void);
int		mouse_x(void);
int		mouse_y(void);
int		mouse_w(void);
BOOL  click_pressed(int button);

/*---------------------------------------*/
/*             Saisie de texte           */
/*---------------------------------------*/
int edit_line(char *line, int long_max, int pos_x, int pos_y);
int edit_line_string(string &line, int long_max, int pos_x, int pos_y);

/*---------------------------------------*/
/*           Gestion d'un BitMap         */
/*---------------------------------------*/
CHBITMAP*	create_chb(int width,int height);
CHBITMAP*	free_chb(CHBITMAP*b);
void		  fill_part_chb(CHBITMAP*b,int x,int y,int w,int h,
								 int asciiChar, int color);
void		  fill_chb(CHBITMAP*b, int asciiChar, int color);
void		  clear_chb(CHBITMAP*b);
void		  copy_chb(CHBITMAP*src,CHBITMAP*dst,int srcX, int srcY,
								 int dstX,int dstY, int w, int h );
void		  draw_chb(CHBITMAP*dst,CHBITMAP*src, int x, int y);
void		  show_to_console(CHBITMAP*b,int x, int y);
CHBITMAP*	MemScreen(void);
void		  show_MemScreen(void);
void		  clear_MemScreen(void);


/*---------------------------------------*/
/*   Gestion de l'écriture avec BitMap   */
/*---------------------------------------*/
void draw_char(CHBITMAP*b, int x, int y, int color, int asciiChar);


#if __cplusplus
}
#endif

#endif // CONSOLE_H_INCLUDED
