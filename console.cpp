/*******************************************************/
/*                                                     */
/*           GESTIONNAIRE DE CONSOLE                   */
/*                                                     */
/*                  console.cpp                        */
/*                                                     */
/*        Gestion de l'écran en mode console           */
/*                                                     */
/*                 Version : 1.01                      */
/*******************************************************/
// 28/03/2021 Version 1.01 : Ajout des fonctions "edit_line"
//                           et "edit_line_string"
//                           Qui permettent de modifier/saisir une
//                           ligne de texte de type 'char *' ou 'string'
//
// 20/03/2021 Version 1.00 : Version initiale
//
#include <string>
#include "console.h"
#include <conio.h>

using namespace std;

/*-----------------------------------------------------*/
/*                                                     */
/*                      STRUCTURE                      */
/*                                                     */
/*-----------------------------------------------------*/

//  structure COORD  (coordonnées pour les x et y console

COORD coord(int x, int y)
{
   COORD c;
   c.X=x;
   c.Y=y;
   return c;
}

/*******************************************************/
/*                                                     */
/*           MANIPULATION DES FENETRES                 */
/*                                                     */
/*******************************************************/
/*-----------------------------------------------------*/
/*                                                     */
/*                    GetStdout                        */
/*                                                     */
/*-----------------------------------------------------*/
// Récupération du "handle" pour les sorties dans la fenêtre

HANDLE GetStdout()
{
   static HANDLE h=NULL;
   if (h==NULL)
   {
      h=GetStdHandle(STD_OUTPUT_HANDLE);
      if (h == INVALID_HANDLE_VALUE)
      {
         char* data = "impossible d'obtenir std output handle";
         debug(Fatal,data);
      }
      return h;
   }
}

/*-----------------------------------------------------*/
/*                                                     */
/*                     GetStdin                        */
/*                                                     */
/*-----------------------------------------------------*/
// Récupération du "handle" pour les entrées sur la fenêtre

HANDLE GetStdin()
{
   static HANDLE h=NULL;
   if (h==NULL)
   {
      h=GetStdHandle(STD_INPUT_HANDLE);
      if (h == INVALID_HANDLE_VALUE)
         debug(Fatal,"impossible d'obtenir std input handle");
   }
   return h;
}

/*-----------------------------------------------------*/
/*                                                     */
/*                     GetConsoleW                     */
/*                                                     */
/*-----------------------------------------------------*/
// Récupération du "handle" pour la console

HANDLE GetConsoleW()
{
   static HANDLE h=NULL;
   if (h==NULL)
   {
      h=GetConsoleWindow();
      if (h == INVALID_HANDLE_VALUE)
         debug(Fatal,"impossible d'obtenir handle console");
   }
   return h;
}

/*-----------------------------------------------------*/
/*                                                     */
/*                         Debug                       */
/*                                                     */
/*-----------------------------------------------------*/
// Permet de suivre et de gérer les erreurs
// Utilisation de GetLastError():
// avec la fonction FormatMessage() qui donne les informations
// venant du System Error Codes.
// return GetLastError(); (type  : unsigned int

void debug(int errLevel, char*msg)
{
   gotoxy(0,0);
   switch(errLevel)
   {
   case Info    :
      printf("Info");
      break;
   case Warming :
      printf("Warming");
      break;
   case Error   :
      printf("Error");
      break;
   case Fatal   :
      printf("Fatal");
      break;
   }
   printf(" : %s\nligne %d\nfile %s\n",msg,__LINE__,__FILE__);
   system("PAUSE");
   if(errLevel==Fatal)
      exit(EXIT_FAILURE);
}


/*******************************************************/
/*                                                     */
/*           GESTIONNAIRE ECRAN/CONSOLE                */
/*                                                     */
/*******************************************************/
/*-----------------------------------------------------*/
/*                                                     */
/*                  resize_console                     */
/*                                                     */
/*-----------------------------------------------------*/
// Permet de redimensionner la console (buffer des données et fenêtre affichée.
// width : largeur voulue pour la fenêtre
// height : hauteur voulue pour la fenêtre
// la fenêtre affichée et le buffer des données ont la même taille
// Taille minimum : 80 caractères en largeur, 25 en hauteur
// Il y a une taille maximum déterminée automatiquement
// selon la résolution de l'écran et la taille des caractères
// sélectionnée directement par l'utilisateur avec une fenêtre console ouverte.
// Retourne  TRUE (1) en cas de réussite et FALSE (0) en cas d'erreur

BOOL resize_console(int width, int height)
{
   COORD size, sizeMax;
// les valeurs minimums au départ
   SMALL_RECT rBuffer= {0,0,80,25}; //Left,Top,Right,Bottom
   SMALL_RECT rWindow= {0,0,1,1};
   if(!SetConsoleWindowInfo(GetStdout(),TRUE, &rWindow ))
   {
      debug(Error,"[resize_console]SetConsoleWindowInfo()");
      return FALSE;
   }
   sizeMax=GetLargestConsoleWindowSize(GetStdout());

   // contrôle taille min et max
   size.X = (width < rBuffer.Right) ? rBuffer.Right :
            ((width >sizeMax.X) ? sizeMax.X : width);
   size.Y = (height< rBuffer.Bottom)? rBuffer.Bottom:
            ((height>sizeMax.Y) ? sizeMax.Y : height);

   if (!SetConsoleScreenBufferSize(GetStdout(),size))
   {
      debug(Error,"[resize_console]SetConsoleScreenBufferSize");
      return FALSE;
   }

   // taille fenêtre identique taille buffer)
   rWindow.Right=size.X-1;
   rWindow.Bottom=size.Y-1;

   if(!SetConsoleWindowInfo(GetStdout(),TRUE, &rWindow ))
   {
      debug(Error,"[resize_console]SetConsoleWindowInfo");
      return FALSE;
   }
   return TRUE;
}

/*-----------------------------------------------------*/
/*                                                     */
/*                resize_console_max                   */
/*                                                     */
/*-----------------------------------------------------*/
// Permet d'avoir une fenêtre de taille maximum.
// La taille dépend de la police de caractères choisie.

BOOL resize_console_max()
{
   COORD sizeMax;
   sizeMax = GetLargestConsoleWindowSize(GetStdout());
   return resize_console(sizeMax.X, sizeMax.Y);
}

/*-----------------------------------------------------*/
/*                                                     */
/*                       screen_w                      */
/*                                                     */
/*-----------------------------------------------------*/
// Retourne la largeur maximum de la fenêtre en fonction de la taille du buffer.

int screen_w()
{
   CONSOLE_SCREEN_BUFFER_INFO info;
   GetConsoleScreenBufferInfo(GetStdout(),&info);
   return info.dwMaximumWindowSize.X;
}

/*-----------------------------------------------------*/
/*                                                     */
/*                       screen_h                      */
/*                                                     */
/*-----------------------------------------------------*/
// Retourne la hauteur maximum de la fenêtre en fonction de la taille du buffer.

int screen_h()
{
   CONSOLE_SCREEN_BUFFER_INFO info;
   GetConsoleScreenBufferInfo(GetStdout(),&info);
   return info.dwMaximumWindowSize.Y;
}

/*-----------------------------------------------------*/
/*                                                     */
/*                     screen_size                     */
/*                                                     */
/*-----------------------------------------------------*/
// Retourne les largeur et hauteur maxi sous forme d'une stucture COORD.

COORD screen_size()
{
   CONSOLE_SCREEN_BUFFER_INFO info;
   GetConsoleScreenBufferInfo(GetStdout(),&info);
   return info.dwMaximumWindowSize;
}

/*-----------------------------------------------------*/
/*                                                     */
/*                     fill-console                    */
/*                                                     */
/*-----------------------------------------------------*/
// Remplit la fenêtre avec le caractère et les couleurs
// du fond spécifiés.

void fill_console(char asciiChar, int color)
{
   CONSOLE_SCREEN_BUFFER_INFO info;
   static CHAR_INFO*dat=NULL;
   static int w=0;
   static int h=0;
   static SMALL_RECT bRect;
   int x,y;

   if (!GetConsoleScreenBufferInfo(GetStdout(),&info))
   {
      debug(Error,"[fill_console]GetConsoleScreenBufferInfo");
      return;
   }

   // n'est refait qu'à chaque modification
   if (info.dwSize.X!=w || info.dwSize.Y!=h)
   {
      w=info.dwSize.X;
      h=info.dwSize.Y;
      dat=(CHAR_INFO*)malloc(sizeof(CHAR_INFO)*w*h);

      bRect.Left = 0;
      bRect.Top = 0;
      bRect.Right = w-1;
      bRect.Bottom = h-1;
   }

   if ( color == 0 )
      memset(dat,0,sizeof(CHAR_INFO)*w*h);
   else
   {
      for (y=0; y<h; y++)
         for (x=0; x<w ; x++)
         {
            dat[y*w+x].Attributes=color;
            dat[y*w+x].Char.AsciiChar = asciiChar;
            dat[y*w+x].Char.UnicodeChar = asciiChar;
         }
   }

   if (! WriteConsoleOutput(GetStdout(),// console active cible
                            dat,			 // datas src à afficher
                            coord(w,h),	 // taille
                            coord(0,0),	 // coin src départ
                            &bRect) )		 // rect de destination
      debug(Error,"[fill_console]WriteConsoleOutput");

}

/*-----------------------------------------------------*/
/*                                                     */
/*                   clear_console                     */
/*                                                     */
/*-----------------------------------------------------*/
//  Efface la console en noir

void clear_console()
{
   fill_console(0,0);
}

//////// Si utilisation de Visual C++ (au lieu de GNU) /////
#ifdef _VISUALCPP
/* ***********************************************
retourne la taille de la fonte courante
*/
COORD get_font_size()
{
   CONSOLE_FONT_INFO font;
   COORD size= {0,0};

   if (!GetCurrentConsoleFont(GetStdout(),FALSE,&font))
      debug(Warming,"[get_font_size]error GetCurrentConsoleFont\n");
   else
      size=font.dwFontSize;

   return size;
}

/* ********************************************************
get_font_type fonction / réservée Visual C++
retourne le type de la fontes courante
*/
DWORD get_font_type()
{
   CONSOLE_FONT_INFO font;
   DWORD type; // unsigned long

   if (!GetCurrentConsoleFont(GetStdout(),FALSE,&font))
      debug(Warming,"[get_font_size]error GetCurrentConsoleFont\n");
   else
      type=font.nFont;

   return type;
}
#endif
/////////////////////////////////////////////////////////

/*-----------------------------------------------------*/
/*                                                     */
/*              test_screen_console_info               */
/*                                                     */
/*-----------------------------------------------------*/
// Affiche les information de la fenetre console

void test_screen_console_info(int x,int y,int color)
{
   CONSOLE_SCREEN_BUFFER_INFO info;

   gotoxy(x,y);
   set_color(color);
   printf("SCREEN BUFFER INFO :\n");
   if(!GetConsoleScreenBufferInfo(GetStdout(),&info))
      debug(Error,"[test_screen_console_info]GetConsoleScreenBufferInfo");
   else
   {
      gotoxy(x,wherey());
      printf("dwSize              : %d-%d		\n",info.dwSize.X,info.dwSize.Y);
      gotoxy(x,wherey());
      printf("dwCursorPosition    : %d-%d		\n",info.dwCursorPosition.X,
             info.dwCursorPosition.Y);
      gotoxy(x,wherey());
      printf("wAttributes         : %d			\n",info.wAttributes);
      gotoxy(x,wherey());
      printf("srWindow            : t%d-L%d-R%d-B%d		\n",
             info.srWindow.Top,
             info.srWindow.Left,
             info.srWindow.Right,
             info.srWindow.Bottom);
      gotoxy(x,wherey());
      printf("dwMaximumWindowSize : %d-%d		\n",info.dwMaximumWindowSize.X,
             info.dwMaximumWindowSize.Y);
   }


//////// Si utilisation de Visual C++ (au lieu de GNU) /////
#ifdef _VISUALCPP
   {
      CONSOLE_FONT_INFO font;
      // fontes (pas possibilité de modification)
      gotoxy(x,wherey());
      printf("FONT :\n");
      if (!GetCurrentConsoleFont(GetStdout(),FALSE,&font))
         debug(Warming,"[test_screen_console_info]error GetCurrentConsoleFont\n");
      else
      {
         gotoxy(x,wherey());
         printf("dwFontSize : %d, %d\n",	font.dwFontSize.X,font.dwFontSize.Y);
         gotoxy(x,wherey());
         printf("nFont : %d\n",font.nFont);
      }
   }
#endif
}
/////////////////////////////////////////////////////////

/*-----------------------------------------------------*/
/*                                                     */
/*                     title_console                   */
/*                                                     */
/*-----------------------------------------------------*/
// Donne un titre à la fenetre console.

// Version visual C++
#ifdef _VISUALCPP
void title_console(LPCTSTR title)
{
   SetConsoleTitle(title);
}
#else
// version mingw
void title_console(char*title)
{
   TCHAR t[MAX_PATH];
   strcpy(t,TEXT(title));
   SetConsoleTitle(t);
}
#endif


/*******************************************************/
/*                                                     */
/*                GESTION DES COULEURS                 */
/*                                                     */
/*******************************************************/

/*-----------------------------------------------------*/
/*                                                     */
/*                      set_color                      */
/*                                                     */
/*-----------------------------------------------------*/
// Fixe la couleur de fond et couleur des lettres

void set_color(int color)
{
   SetConsoleTextAttribute(GetStdout(),color);
}

/*-----------------------------------------------------*/
/*                                                     */
/*                      set_color                      */
/*                                                     */
/*-----------------------------------------------------*/
// Fixe séparément la couleur de fond et couleur des lettres

void set_bf_color(int backcolor, int forecolor)
{
   SetConsoleTextAttribute(GetStdout(),backcolor*16+forecolor);
}

/*-----------------------------------------------------*/
/*                                                     */
/*                  get_back_color                     */
/*                                                     */
/*-----------------------------------------------------*/
// Récupère la couleur actuelle du fond

int get_back_color()
{
   CONSOLE_SCREEN_BUFFER_INFO info;
   GetConsoleScreenBufferInfo(GetStdout(),&info);
   return info.wAttributes>>4;
}

/*-----------------------------------------------------*/
/*                                                     */
/*                  get_fore_color                     */
/*                                                     */
/*-----------------------------------------------------*/
// Récupère la couleur actuelle de lettres (premier plan)

int get_fore_color()
{
   CONSOLE_SCREEN_BUFFER_INFO info;
   GetConsoleScreenBufferInfo(GetStdout(),&info);
   return info.wAttributes & 15;
}

/*-----------------------------------------------------*/
/*                                                     */
/*                  set_back_color                     */
/*                                                     */
/*-----------------------------------------------------*/
// Définit une couleur pour le fond

void set_back_color(int color)
{
   int fcolor = get_fore_color();
   SetConsoleTextAttribute(GetStdout(), (color<<4) + fcolor);
}

/*-----------------------------------------------------*/
/*                                                     */
/*                  set_fore_color                     */
/*                                                     */
/*-----------------------------------------------------*/
// Définit une couleur pour les caractères (Foreground)

void set_fore_color(int color)
{
   int bcolor = get_back_color();
   SetConsoleTextAttribute(GetStdout(), (bcolor<<4) + color);
}

/*******************************************************/
/*                                                     */
/*                  GESTION DU CURSEUR                 */
/*                                                     */
/*******************************************************/
/*-----------------------------------------------------*/
/*                                                     */
/*                       gotoxy                        */
/*                                                     */
/*-----------------------------------------------------*/
// Positionne le curseur dans la fenêtre

void gotoxy(int x, int y)
{
   if (x >= 0 && y >= 0 && x < screen_w() && y < screen_h())
      SetConsoleCursorPosition(GetStdout(),coord(x,y));
}

/*-----------------------------------------------------*/
/*                                                     */
/*                       wherex                        */
/*                                                     */
/*-----------------------------------------------------*/
// Retourne la position horizontale du curseur

int wherex()
{
   CONSOLE_SCREEN_BUFFER_INFO info;
   GetConsoleScreenBufferInfo(GetStdout(),&info);
   return info.dwCursorPosition.X;
}

/*-----------------------------------------------------*/
/*                                                     */
/*                       wherey                        */
/*                                                     */
/*-----------------------------------------------------*/
// Retourne la position verticale du curseur

int wherey()
{
   CONSOLE_SCREEN_BUFFER_INFO info;
   GetConsoleScreenBufferInfo(GetStdout(),&info);
   return info.dwCursorPosition.Y;
}

/*-----------------------------------------------------*/
/*                                                     */
/*                    show_cursor                      */
/*                                                     */
/*-----------------------------------------------------*/
// Montre (TRUE, 1) ou cache (FALSE, 0) le curseur

void show_cursor(BOOL visible)
{
   CONSOLE_CURSOR_INFO info;
   if (!GetConsoleCursorInfo(GetStdout(),&info))
      debug(Error,"[show_cursor]GetConsoleCursorInfo");
   else
   {
      info.bVisible=visible;
      SetConsoleCursorInfo(GetStdout(),&info);
   }
}

/*-----------------------------------------------------*/
/*                                                     */
/*                     size_cursor                     */
/*                                                     */
/*-----------------------------------------------------*/
// Modifie la taille du curseur
// Valeyur entre 0 et 100 = pourcentage de recouvrement
// de la cellule où se trouve le curseur

void size_cursor(unsigned int size)
{
   CONSOLE_CURSOR_INFO info;
   if (!GetConsoleCursorInfo(GetStdout(),&info))
      debug(Error,"[size_cursor]GetConsoleCursorInfo");
   else
   {
      info.dwSize=size;
      SetConsoleCursorInfo(GetStdout(),&info);
   }
}

/*******************************************************/
/*                                                     */
/*                 GESTION DES EVENEMENTS              */
/*                                                     */
/*******************************************************/
// Entrées
static KEYBOARD_DATA _keyboardData = {{0}};
static MOUSE_EVENT_RECORD _mouseData = {{0}};
static int _mouseX = 0;  // pos horizontale
static int _mouseY = 0;  // verticale
static int _mouseB = 0;  // etat des clics (2 ou 3 boutons)
static int _mouseW = 0;   // valeur bouton weel

/*-----------------------------------------------------*/
/*                                                     */
/*                     poll_event                      */
/*                                                     */
/*-----------------------------------------------------*/
// Récupération des entrées

void poll_event()
{
   DWORD nbEvents = 0;
   DWORD readEvents = 0;
   INPUT_RECORD* eventBuffer = NULL;
   unsigned int i,key;

   GetNumberOfConsoleInputEvents(GetStdin(),&nbEvents);

   if (nbEvents==0)
      return;

   eventBuffer=(INPUT_RECORD*)malloc(nbEvents*sizeof(INPUT_RECORD));
   ReadConsoleInput(GetStdin(), eventBuffer, nbEvents, &readEvents);

   for (i=0; i < readEvents; i++)
   {
      // CLAVIER
      if (eventBuffer[i].EventType == KEY_EVENT)
      {
         //if(eventBuffer[i].Event.KeyEvent.wRepeatCount==1){
         _keyboardData.controlKeyState = eventBuffer[i].Event.KeyEvent.dwControlKeyState;
         key = eventBuffer[i].Event.KeyEvent.wVirtualKeyCode;
         _keyboardData.keyState[key] = eventBuffer[i].Event.KeyEvent.bKeyDown;
         //}
      }
      // SOURIS
      if (eventBuffer[i].EventType == MOUSE_EVENT)
      {
         _mouseData = eventBuffer[i].Event.MouseEvent;
         // si mouvement
         if (eventBuffer[i].Event.MouseEvent.dwEventFlags == MOUSE_MOVED) //1
         {
            _mouseX = eventBuffer[i].Event.MouseEvent.dwMousePosition.X;
            _mouseY = eventBuffer[i].Event.MouseEvent.dwMousePosition.Y;
         }
         // si clic
         else if (eventBuffer[i].Event.MouseEvent.dwEventFlags == 0)//clic simple
            _mouseB = eventBuffer[i].Event.MouseEvent.dwButtonState;
         else if (eventBuffer[i].Event.MouseEvent.dwEventFlags == DOUBLE_CLICK)//2
            ;
         // si wheel
         else if (eventBuffer[i].Event.MouseEvent.dwEventFlags == MOUSE_WHEELED) //4
         {
            if (eventBuffer[i].Event.MouseEvent.dwButtonState & 0xFF000000)
               _mouseW++;
            else
               _mouseW--;
         }
      }
   }
   free(eventBuffer);
}

/*-----------------------------------------------------*/
/*                                                     */
/*                     key_pressed                     */
/*                                                     */
/*-----------------------------------------------------*/
// Récupération des touches pressées


BOOL key_pressed(int keyCode)
{
   BOOL res;

   res = _keyboardData.keyState[keyCode];
   if (res)
      _keyboardData.keyState[keyCode]^=1;
   return res;

   //return _keyBoardData.keyState[keyCode];
}

/*-----------------------------------------------------*/
/*                                                     */
/*                   get_mouse_pos                     */
/*                                                     */
/*-----------------------------------------------------*/
// Récupération des positions horizontales
// et verticales de la souris

COORD get_mouse_pos()
{
   return _mouseData.dwMousePosition;
}

/*-----------------------------------------------------*/
/*                                                     */
/*                      mouse_x                        */
/*                                                     */
/*-----------------------------------------------------*/
// Récupération de la position verticale de la souris

int mouse_x()
{
   return _mouseX;
}

/*-----------------------------------------------------*/
/*                                                     */
/*                      mouse_y                        */
/*                                                     */
/*-----------------------------------------------------*/
// Récupération de la position horizontale de la souris

int mouse_y()
{
   return _mouseY;
}

/*-----------------------------------------------------*/
/*                                                     */
/*                      mouse_w                        */
/*                                                     */
/*-----------------------------------------------------*/
// Récupération de la roulette de la souris

int mouse_w()
{
   return _mouseW;
}

/*-----------------------------------------------------*/
/*                                                     */
/*                    clic_pressed                      */
/*                                                     */
/*-----------------------------------------------------*/
// Récupération des clicks de la souris


BOOL click_pressed(int button)
{
   int res=0;

   if ((_mouseB & button) == button)
   {
      res=1;
      //_mouseB-=button;
   }
   return res;
}

/*******************************************************/
/*                                                     */
/*                    SAISIE DE TEXTE                  */
/*                                                     */
/*******************************************************/
/*-----------------------------------------------------*/
/*                                                     */
/*                     edit_line                       */
/*                                                     */
/*-----------------------------------------------------*/
// *line : pointeur sur la ligne à éditer
// long_max : longeur maximale de la ligne à éditer
// pos_x, pos_y : position de la ligne en colonne et ligne

int edit_line(char *line, int long_max, int pos_x, int pos_y)
{
   int lg, cur_pos_x;
   char c='a';
   char *text = (char*)malloc(long_max + 1);

   if(text==NULL) // Erreur malloc
      exit(1);
   cur_pos_x = pos_x;
   strcpy(text, line);
   gotoxy(cur_pos_x, pos_y);
   printf("%s", text);

   lg=strlen(text);
   cur_pos_x = cur_pos_x + lg;
   gotoxy(cur_pos_x, pos_y);
   show_cursor(TRUE);

   while (c!=13)
   {
      c=getch();
      if (c >= ' ' && c <= 'z' && lg <= long_max )
      {
         putch(c);
         text[lg++] = c;
         gotoxy(cur_pos_x++, pos_y);
      }
      else
      {
         if(c==(char)0x08 && lg >= 1)  //Backspace
         {
            gotoxy(--cur_pos_x,pos_y);
            putch(' ');
            text[--lg]='\0';
         }
      }
      gotoxy(cur_pos_x, pos_y);
   }
   text[lg]='\0';
   strcpy(line, text);
   free(text);
   show_cursor(FALSE);
   return 0;
}
/*-----------------------------------------------------*/
/*                                                     */
/*                     edit_line                       */
/*                                                     */
/*-----------------------------------------------------*/
// &line : adresse de la ligne à éditer (type : string
// long_max : longeur maximale de la ligne à éditer
// pos_x, pos_y : position de la ligne en colonne et ligne

int edit_line_string(string &line, int long_max, int pos_x, int pos_y)
{
   int lg, cur_pos_x;
   char c='a';
   char * text = new char [long_max+1];

   cur_pos_x = pos_x;
   strcpy (text, line.c_str());

   gotoxy(cur_pos_x, pos_y);
   printf("%s",text);

   lg = strlen(text);
   cur_pos_x = cur_pos_x + lg;
   gotoxy(cur_pos_x, pos_y);
   show_cursor(TRUE);

   while (c!=13)
   {
      c=getch();
      if (c >= ' ' && c <= 'z' && lg <= long_max )
      {
         putch(c);
         text[lg++] = c;
         gotoxy(cur_pos_x++, pos_y);
      }
      else
      {
         if(c==(char)0x08 && lg >= 1)  //Backspace
         {
            gotoxy(--cur_pos_x,pos_y);
            putch(' ');
            text[--lg]='\0';
         }
      }
      gotoxy(cur_pos_x, pos_y);
   }
   text[lg]='\0';
   line.assign(text);
   delete[] text;
   show_cursor(FALSE);
   return 0;
}

/*******************************************************/
/*                                                     */
/*                        BITMAP                       */
/*                                                     */
/*******************************************************/
/*-----------------------------------------------------*/
/*                                                     */
/*                     create_chb                      */
/*                                                     */
/*-----------------------------------------------------*/
//  CHBITMAP : char bitmap (chb)
//	buffer de données pour affichage
//  Permet d'allouer une CHBITMAP selon une largeur et
// une hauteur données en paramètre.

CHBITMAP* create_chb (int width, int height)
{
   CHBITMAP*b=NULL;
   if (width > 0 && height > 0)
   {
      b=(CHBITMAP*)malloc(sizeof(CHBITMAP));
      b->w = width; //(width < 1) ? 1 : width;
      b->h = height; //(height < 1) ? 1 : height;
      b->dat = (CHAR_INFO*)malloc(sizeof(CHAR_INFO)*b->w*b->h);
      // initialiseé à 0 au départ
      if (b->dat != NULL)
         memset(b->dat,0,sizeof(CHAR_INFO)*b->w*b->h);
      else
      {
         free(b);
         b=NULL;
      }
   }
   return b;
}

/*-----------------------------------------------------*/
/*                                                     */
/*                       free_chb                      */
/*                                                     */
/*-----------------------------------------------------*/
//Libérer la mémoire précédement allouée pour une chbitmap

CHBITMAP* free_chb(CHBITMAP*b)
{
   if (b!=NULL)
   {
      free(b->dat);
      free(b);
      b=NULL;
   }
   return b;
}

/*-----------------------------------------------------*/
/*                                                     */
/*                     fill_part_chb                   */
/*                                                     */
/*-----------------------------------------------------*/
// Remplit la chbitmap en entier ou en partie avec
// une lettre et couleur (lettre, fond) choisies

void fill_part_chb(	CHBITMAP*b,		// la chbitmap
                     int x,int y,	// position de départ
                     int w,int h,	// taille à partir de la
                     // position de départ
                     int asciiChar,  // lettre de remplissage
                     int color)		// couleur de remplissage
{
   int i,j;

   if(b != NULL)
   {
      x = (x < 0) ? 0 : ((x > b->w) ? b->w : x);
      y = (y < 0) ? 0 : ((y > b->h) ? b->h : y);
      w = (x+w > b->w) ? b->w : x+w;
      h = (y+h > b->h) ? b->h : y+h;
      for(j=y; j < h; j++ )
         for (i=x; i < w; i++)
         {
            b->dat[ j*b->w + i ].Char.AsciiChar=asciiChar;
            b->dat[ j*b->w + i ].Attributes=color;
         }
   }
   else
      debug(Warming,"[fill_part_chb]chbitmap non initialisee");
}

/*-----------------------------------------------------*/
/*                                                     */
/*                        fill_chb                     */
/*                                                     */
/*-----------------------------------------------------*/
// Remplit le buffer avec une lettre et une couleur

void fill_chb (CHBITMAP*b, int asciiChar, int color)
{
   fill_part_chb(b, 0,0,b->w,b->h,asciiChar,color);
}

/*-----------------------------------------------------*/
/*                                                     */
/*                       clear_chb                     */
/*                                                     */
/*-----------------------------------------------------*/
// Initialise à 0 une chbitmap

void clear_chb(CHBITMAP*b)
{
   if (b)
      //fill_chbitmap(b,0,0);
      memset(b->dat,0,sizeof(CHAR_INFO)*b->w*b->h);
   else
      debug(Warming,"[clear_chbitmap]chbitmap non initialisee");
}

/*-----------------------------------------------------*/
/*                                                     */
/*                     fill_part_chb                   */
/*                                                     */
/*-----------------------------------------------------*/
// Copie une chbitmap dans une autre entièrement ou en partie

void copy_chb (	CHBITMAP*src,		// source
                  CHBITMAP*dst,		// destination
                  int srcX, int srcY,	// coin haut gauche source
                  int dstX,int dstY,	// coin haut gauche destination
                  int width, int height )		// taille copie à partir coin
// haut gauche source
{
   int x,y,sx,sy;
   if (src != NULL && dst != NULL)
   {

      for (y = dstY; y < dstY + height; y++)

         for (x = dstX; x < dstX + width; x++)
         {

            if (x < 0 || x >= dst->w || y < 0 || y >= dst->h)
               continue;

            sy = y - dstY + srcY;
            sx = x - dstX + srcX;

            if (sx < 0 || sx >= src->w || sy < 0 || sy >= src->h)
               continue;

            dst->dat[ y * dst->w + x ] = src->dat[ sy * src->w + sx];

         }
   }
   else
      debug(Warming,"[copy_chbitmap]source ou destination non allouee");
}

/*-----------------------------------------------------*/
/*                                                     */
/*                       draw_chb                      */
/*                                                     */
/*-----------------------------------------------------*/
// Copie un buffer dans un autre à partir d'une position donnée

void draw_chb(CHBITMAP*dst,CHBITMAP*src, int x, int y)
{
   if (dst != NULL && src != NULL)
      copy_chb (src,dst,0,0,x,y,src->w,src->h);
   else
      debug(Warming,"[draw_buffer]chbitmap source ou destination non allouee");
}


/*-----------------------------------------------------*/
/*                                                     */
/*                   show_to_console                   */
/*                                                     */
/*-----------------------------------------------------*/
// Permet d'afficher une chbitmap dans la fenêtre console.

void show_to_console(CHBITMAP*b, int x, int y)
{
   SMALL_RECT bRect;

   if ( b!=NULL  )
   {
      bRect.Left = x;
      bRect.Top = y;
      bRect.Right = x + b->w - 1;
      bRect.Bottom = y+ b->h - 1;

      if (! WriteConsoleOutput(GetStdout(),     // console active cible
                               b->dat,		  // les datas src à afficher
                               coord(b->w,b->h),// sa taille
                               coord(0,0),	  // coin src départ
                               &bRect) )		  // rect de destination
         debug(Error,"[show_to_console]erreur WriteConsoleOutput");
   }
   else
      debug(Warming,"[show_to_console] chbitmap non allouee");
}

/*-----------------------------------------------------*/
/*                                                     */
/*                      MemScreen                      */
/*                                                     */
/*-----------------------------------------------------*/
// Permet d'obtenir l'adresse de la chbitmap _screen.
// Cette chbitmap correspond à l'espace de la fenêtre
// console accessible en écriture. Elle sert de double buffer.
// Tous les affichages ont lieu d'abord dans _screen et
// ensuite _screen est affichée dans la fenêtre console.

CHBITMAP* MemScreen()
{
   static CHBITMAP*_screen = NULL;
   COORD size;
   size = screen_size();
   if (_screen == NULL )
      _screen = create_chb(size.X,size.Y);
   else if (_screen->w != size.X || _screen->h != size.Y)
   {
      _screen->w = size.X;
      _screen->h = size.Y;
      _screen->dat=(CHAR_INFO*)realloc(_screen->dat,sizeof(CHAR_INFO)*_screen->w*_screen->h);
   }
   return _screen;
}

/*-----------------------------------------------------*/
/*                                                     */
/*                      MemScreen                      */
/*                                                     */
/*-----------------------------------------------------*/
// Affiche la chbitmap _screen dans la fenêtre console

void show_MemScreen()
{
   show_to_console(MemScreen(),0,0);
}

/*-----------------------------------------------------*/
/*                                                     */
/*                      MemScreen                      */
/*                                                     */
/*-----------------------------------------------------*/
// Efface (mise à 0) la chbitmap _screen sans toucher
// à la fenêtre console

void clear_MemScreen()
{
   clear_chb(MemScreen());
}

/*******************************************************/
/*                                                     */
/*          GESTION DU TEXTE EN MODE BITMAP            */
/*                                                     */
/*******************************************************/
/*-----------------------------------------------------*/
/*                                                     */
/*                     draw_char                       */
/*                                                     */
/*-----------------------------------------------------*/
// Affiche le caractère spécifié en ascii (Char) avec la
// couleur (color) à la position x, y dans la CHBITMAP (b)

void draw_char(CHBITMAP*b, int x, int y, int color, int asciiChar)
{
   // impossible si
   if (b==NULL || x<0 || x>=b->w || y<0 || y>=b->h)
      return;

   b->dat[b->w*y + x].Char.AsciiChar=asciiChar;
   b->dat[b->w*y + x].Attributes = color;
}

