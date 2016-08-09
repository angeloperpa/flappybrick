#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "data.h"
#include "bib_game.h"

#define DELAY   1000000
#define COLUNAS 70
#define LINHAS 39
#define SIZE_COL 8
#define SPACEMID_COL 8
#define SPACE_COL 30

void show_score(int score_global) {       //Ajustes do arquivo de pontuação.

	char buffer[30], *linha;
	int i;
	score_t *score;
	FILE *score_file;
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);

	linha = (char*) malloc (30 * sizeof(char));

	score = (score_t*) malloc (5 * sizeof(score_t));
	score_file = fopen("score.txt", "r");
	for(i = 0; i < 5; i++) {
		fgets(linha, 30, score_file);
		sscanf(linha, "%s %d %d %d %d",score[i].nome, &score[i].dia, &score[i].mes, &score[i].ano, &score[i].score);
	}
	fclose(score_file);
	clear();
	endwin();
	system("reset");
	score_global /= 2;
	if(score_global >= score[4].score) {
		i = 4;
		printf("NEW SCORE: %d\n", score_global);
		printf("DIGITE SEU NOME: ");
		fflush(stdin);
		scanf("%s", buffer);
		while(i >= 0) {
			if(score_global > score[i].score && i == 4) {
				strncpy(score[i].nome, buffer, strlen(buffer));
				score[i].nome[strlen(buffer)] = '\0';
				score[i].dia = tm.tm_mday;
				score[i].mes = tm.tm_mon + 1;
				score[i].ano = tm.tm_year + 1900;
				score[i].score = score_global;
			}
			else if(score_global > score[i].score && i != 4) {
				strncpy(score[i + 1].nome, score[i].nome, strlen(score[i].nome));
				score[i + 1].nome[strlen(score[i].nome)] = '\0';
				score[i + 1].dia = score[i].dia;
				score[i + 1].mes = score[i].mes;
				score[i + 1].ano = score[i].ano;
				score[i + 1].score = score[i].score;
				strncpy(score[i].nome, buffer, strlen(buffer));
				score[i].nome[strlen(buffer)] = '\0';
                score[i].dia = tm.tm_mday;
                score[i].mes = tm.tm_mon + 1;
	            score[i].ano = tm.tm_year + 1900;
	            score[i].score = score_global;
			}
			i--;
		}
	}
	score_file = fopen("score.txt", "w");
	for (i = 0; i < 5; i++)
	{
		fprintf(score_file, "%s %d %d %d %d\n", score[i].nome, score[i].dia, score[i].mes, score[i].ano, score[i].score);
	}
	fclose(score_file);
  free(linha);
}

void start_page() {           //Inicio da pagina do game.
    FILE *start_game;
    char *line;
    int i;

    char load_start_game[] = "start_game.sprite";
    line = (char*) malloc (COLUNAS * sizeof(char));
    start_game = fopen(load_start_game, "r");
    i = 0;
    init_pair(1, COLOR_RED, COLOR_YELLOW);
    bkgd(COLOR_PAIR(1));
    fgets(line, COLUNAS, start_game);
    while(!feof(start_game)) {
        mvwprintw(stdscr, i, 0, line);
        wrefresh(stdscr);
        i++;
        fgets(line, COLUNAS, start_game);
    }
    fclose(start_game);
    wgetch(stdscr);
}

void help() {             //Print da tela de ajuda ao jogador.
    FILE *help_file;
    char *line;
    int i;
    int op;

    line = (char*) malloc (COLUNAS * sizeof(char));
    help_file = fopen("help.sprite", "r");
    i = 0;
    clear();
    init_pair(1, COLOR_RED, COLOR_YELLOW);
    bkgd(COLOR_PAIR(1));
    fgets(line, COLUNAS, help_file);

    while(!feof(help_file)) {
        mvprintw(i, 0, line);
        refresh();
	i++;
	fgets(line, COLUNAS, help_file);
    }
    while(1) {
	op = getch();
        if(op == 104 || op == 112) {
	      break;
   	}
    }
}
void size_of_screen() {     //Controle de erro para ajuste da tela pelo usuário.
    int size_x;
    int size_y;

    getmaxyx(stdscr, size_y, size_x);

    if(size_x < COLUNAS || size_y < LINHAS) {
        endwin();
        fprintf(stdout,"AJUSTE O TAMANHO DA TELA PARA MAIOR OU IGUAL 70 X 40\nTAMANHO ATUAL: %d x %d\n",size_x, size_y);
        exit(0);
    }
}

int rand_number(int min, int max) {     //Geração do numeros randomicos para gerar o tamanho das colunas.
    int number;
    srand(time(NULL));
    number = min + rand() % (max - min);
    return number;
}

char **init_screen(int col, int lin) {      //Inicialização da tela.

    char **tela;
    int i, j;

    tela = (char**) malloc (lin * sizeof(char*));
    for(i = 0; i < col; i++)
        tela[i] = (char*) malloc (col * sizeof(char));

    for(i = 0; i < col; i++)
        tela[0][i] = '@';

    for(i = 0; i < col; i++)
        tela[lin - 1][i] = '@';

    for(j = 1; j < (lin - 1); j++)
        for(i = 0; i < col; i++)
            tela[j][i] = '.';



    return(tela);
}

void write_screen(char **tela, int score_global) {          //Printa a tela no terminal do usuario, interface gráfica.
    int i, j;
    clear();

    init_pair(11,COLOR_WHITE, COLOR_BLUE);
    init_pair(12,COLOR_WHITE, COLOR_MAGENTA);
    init_pair(13,COLOR_WHITE, COLOR_YELLOW);
    init_pair(14,COLOR_WHITE, COLOR_GREEN);

    init_pair(4,COLOR_BLACK, COLOR_BLACK);
    init_pair(5,COLOR_RED, COLOR_RED);
    for(i = 0; i < LINHAS; i++)
        for(j = 0; j < COLUNAS; j++) {
            if(tela[i][j] == '.')
		attron(COLOR_PAIR(rand_number(11, 14)));
            if(tela[i][j] == '#' || tela[i][j] == '@' || tela[i][j] == 'S')
                attron(COLOR_PAIR(4));
            if(tela[i][j] == 'X')
                attron(COLOR_PAIR(5));
            mvprintw(i, j,"%c", tela[i][j]);
        }
    attron(COLOR_PAIR(11));
    mvprintw(LINHAS, 0, "SCORE: %d                                                                       ", score_global/2);
    refresh();
}

void write_col(char **tela) {       //Printa as colunas na tela.
    int rand_col;
    int i, j;
    rand_col = rand_number(10, 35);
    for(i = rand_col; i <= LINHAS - 2; i++)
        for(j = 1; j <= SIZE_COL; j++) {
            if(j == 1 && i == rand_col)
                tela[i][COLUNAS - j] = 'S';
            else
                tela[i][COLUNAS - j] = '#';
        }
    for(i = 1; i <= rand_col - SPACEMID_COL; i++)
        for(j = 0; j<= SIZE_COL; j++) {
            tela[i][COLUNAS - j] = '#';
        }
}

void gameover(char **tela, int score_global) {        ///Finaliza o jogo mostrando o score.
    int op, i, count;
    FILE *score_file;
    char line[30];
    score_t score;

    write_screen(tela, score_global);
    clear();
    show_score(score_global);
    count = 0;
    init_pair(1, COLOR_RED, COLOR_YELLOW);
    bkgd(COLOR_PAIR(1));
    score_file = fopen("score.sprite", "r");
    while(!feof(score_file)) {
		fgets(line, COLUNAS, score_file);
		mvprintw(count, 0, "%s", line);
		count++;
	}
	fclose(score_file);
    score_file = fopen("score.txt", "r");

  for(i = 0; i < 5; i++)
  {
		fgets(line, 30, score_file);
    sscanf(line, "%s %d %d %d %d", score.nome, &score.dia, &score.mes, &score.ano, &score.score);
		mvprintw(i + (count + 2), 0, "%d. %s        %d/%d/%d     %d ", (i + 1), score.nome, score.dia, score.mes, score.ano, score.score);
	}
	fclose(score_file);
	refresh();

    while (1) {
        op = getch();
        if(op == KEY_UP) {
            score_global = 0;
            tela = init_screen(COLUNAS, LINHAS);
            game_init(tela);
        }
    }
}

void collision(char **tela, int score_global) {     //Teste para colisões, para finalizar o jogo.
  int i, j, aux, aux_two;
  aux = 0;
	aux_two = 0;

  for(i = 0; i < LINHAS; i++)
    for(j = 0; j < COLUNAS; j++) {
      if(tela[i][j] == 'X') {
        aux++;
      }
      if(tela[i][j] == '@') {
			  aux_two++;
			}
    }
  if(aux < 8) {
  	gameover(tela, score_global);
  }

	if(aux_two < (COLUNAS + COLUNAS) ) {
		gameover(tela, score_global);
	}
}

void move_col(char **tela) {        //Movimentação das colunas na tela.
    int i, j;

    for(i = 0; i < LINHAS; i++)
        for(j = 0; j < COLUNAS; j++) {
            if(tela[i][j] == '#') {
                tela[i][j - 1] = '#';
                tela[i][j] = '.';
            }
            if(tela[i][j] == 'S') {
                tela[i][j - 1] = 'S';
                tela[i][j] = '.';
            }
        }
}

void gravity(char **tela, int score_global) {   //Movimentação do "tijolo" para baixo, ou seja, a gravidade.
    int i, j;

    i = LINHAS - 1;

    while(i > 0) {
        j = COLUNAS - 1;
        while(j > 0) {
            if(tela[i][j] == 'X') {
                    if(tela[i + 1][j] == '#' || tela[i + 1][j] == 'S') {
                      gameover(tela, score_global);
		                }
		                else {
			                tela[i + 1][j] = 'X';
                    	tela[i][j] = '.';
		                }

            }
            j--;
        }
        i--;
    }
    collision(tela, score_global);
}

void create_per(char **tela) {    //Criação do "tijolo"
        int size_h, size_w, i, j;
        size_h = 2;
        size_w = 4;
        for(i = 0; i < size_h; i++)
                for(j = 0;j < size_w; j++) {
                        tela[i + 10][j + 15] = 'X';
                }
}

void jump(char **tela, int score_global) {  //Função que cuida da movimentação do "tijolo" para cima.
    int i, j;

    for(i = 0; i < LINHAS; i++)
            for(j = 0; j < COLUNAS; j++) {
                    if(tela[i][j] == 'X') {
			                     if(tela[i - 1][j] == '#') {
				                        gameover(tela, score_global);
			                      }
			                      else {
                                tela[i - 1][j] = 'X';
                                tela[i][j] = '.';
			                      }
                    }
            }
    collision(tela, score_global);
}
void controls(char **tela, int score_global) {  //Função que cuida da parte de controle do "tijolo" no jogo.
    int key;

    timeout(100);
    key = getch();

    switch(key) {
        case KEY_UP:
            jump(tela, score_global);
            jump(tela, score_global);
            jump(tela, score_global);
            jump(tela, score_global);
            break;
        case KEY_DOWN:
            gravity(tela, score_global);
            gravity(tela, score_global);
            gravity(tela, score_global);
            gravity(tela, score_global);
            break;
        case 104:
            help();
            break;
        case 112:
            help();
	    break;
        default:
            break;
    }
}

int score_check(char **tela) {     //Função que cuida da pontuação do jogador.
    int i, j, k, one_time;
    int score;

    score = 0;
    one_time = 1;

    for(i = 0; i < LINHAS; i++)
        for(j = 0; j < COLUNAS; j++) {
            if(tela[i][j] == 'X' && one_time) {
                for(k = 0; k < LINHAS; k++) {
                    if(tela[k][j] == 'S') {
                        score ++;
                        one_time = 0;
                    }
                }
            }
        }
    return(score);
}

void game_init(char **tela) { //Função para iniciar o jogo.
    int i;
    int game_over;
    int score_global;

    score_global = 0;
    game_over = 0;
    create_per(tela);             //Inicialização de variaveis.

    while(!game_over) {
        write_col(tela);
        for(i = 0; i < SPACE_COL; i++) {
            move_col(tela);
            score_global = score_check(tela);
            controls(tela, score_global);       //Loop principal do jogo.
            gravity(tela, score_global);
            move_col(tela);
            write_screen(tela, score_global);
            collision(tela, score_global);
        }
    }
}
