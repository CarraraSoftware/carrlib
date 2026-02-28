#ifndef CARR_TERM_H_
#define CARR_TERM_H_


#include <stdio.h>

#define ESC "\x1B"

#ifndef TERM_COLS
#define TERM_COLS 96
#endif

#ifndef TERM_ROWS
#define TERM_ROWS 40
#endif

#ifndef VERTICAL_BORDER  
#define VERTICAL_BORDER   "║"
#endif

#ifndef HORIZONTAL_BORDER
#define HORIZONTAL_BORDER "═"
#endif

#ifndef TOP_LEFT_BORDER  
#define TOP_LEFT_BORDER   "╔"
#endif

#ifndef TOP_RIGHT_BORDER 
#define TOP_RIGHT_BORDER  "╗"
#endif

#ifndef BOT_LEFT_BORDER  
#define BOT_LEFT_BORDER   "╚"
#endif

#ifndef BOT_RIGHT_BORDER 
#define BOT_RIGHT_BORDER  "╝"
#endif

#ifndef CARR_TERM_FORCE_PREFIX
#define term_clear     carr_term_clear
#define term_init      carr_term_init
#define term_cleanup   carr_term_cleanup
#define term_rectangle carr_term_rectangle
#endif

void carr_term_clear();
void carr_term_init();
void carr_term_cleanup();
void carr_term_rectangle(int col, int row, int w, int h);


#ifdef CARR_TERM_IMPLEMENTATION

void carr_term_clear()
{
    printf(ESC"[2J");                  // erase entire screen
    printf(ESC"[H");                   // move cursor to Home = (0,0)
}

void carr_term_init()
{
    printf(ESC"[?47h");                // save screen
    printf(ESC"[?25l");                // make cursor invisible
    printf(ESC"[?1049h");              // enables the alternative buffer
}

void carr_term_cleanup() 
{
    printf(ESC"[?25h");                // make cursor visible
    printf(ESC"[?1049l");              // disables the alternative buffer
    printf(ESC"[?47l");                // restore screen
}

void carr_term_move(int col, int row)
{
    printf(ESC "[%d;%df", row, col);   // move cursor to (row, col)
}

void carr_term_rectangle(int col, int row, int w, int h)
{
    // (col, row)
    // v
    // +---- w ------+
    // |             |
    // h             |
    // |             |
    // +-------------+
    // Top and Bottom Borders
    for (int i = col; i < col + w; ++i) {
         carr_term_move(i, row);
         printf(HORIZONTAL_BORDER);
         carr_term_move(i, row + h);
         printf(HORIZONTAL_BORDER);
    }
    // Left and Right Borders
    for (int j = row; j < row + h; ++j) {
         carr_term_move(col, j);
         printf(VERTICAL_BORDER);
         carr_term_move(col + w, j);
         printf(VERTICAL_BORDER);
    }
    // Corners
    carr_term_move(col, row);
    printf(TOP_LEFT_BORDER);
    carr_term_move(col + w, row);
    printf(TOP_RIGHT_BORDER);
    carr_term_move(col, row + h);
    printf(BOT_LEFT_BORDER);
    carr_term_move(col + w, row + h);
    printf(BOT_RIGHT_BORDER);
}

#endif
#endif
