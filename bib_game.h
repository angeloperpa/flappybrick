#ifndef __GAME__
#define __GAME__

void show_score(int score_global);
void start_page();
void help();
void size_of_screen();
int rand_number(int min, int max);
char **init_screen(int col, int lin);
void write_screen(char **tela, int score_global);
void write_col(char **tela);
void gameover(char **tela, int score_global);
void collision(char **tela, int score_global);
void move_col(char **tela);
void gravity(char **tela, int score_global);
void create_per(char **tela);
void jump(char **tela, int score_global);
void controls(char **tela, int score_global);
int score_check(char **tela);
void game_init(char **tela);

#endif
