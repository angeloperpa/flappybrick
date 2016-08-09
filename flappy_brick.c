#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "data.h"
#include "bib_game.h"

#define COLUNAS 70
#define LINHAS 39

int main() {
    char **matriz_tela;
    int i;

    initscr();
    start_color();
    size_of_screen();
    keypad(stdscr, TRUE);
    noecho();
    cbreak();
    curs_set(0);
    start_page();
    matriz_tela = init_screen(COLUNAS, LINHAS);
    game_init(matriz_tela);
    endwin();
    for(i = 0; i < LINHAS; i++)
        free(matriz_tela[i]);
    free(matriz_tela);
    return 0;
}
