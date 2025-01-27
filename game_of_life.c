#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define w 80
#define h 25

int input(char **a);
int count_life_sosed(char **a, int x, int y);
int get_i(int i, int now, int len);
void update(char **a, char **b);
int create_a(char ***a);
void del_memory(char ***a);
void copy_a(char **a, char **b);
int zero_a(char **a);
int equal_a_b(char **a, char **b);
void output(char **a, int k, int k_speed);
void print_borders();
void speed_fun(int *speed, int input, int *end_program, int *k_speed);
void Game_of_Life();
void del_all_memory(char ***a, char ***b, char ***c);

int main() {
    Game_of_Life();
    return 0;
}

void Game_of_Life() {
    initscr();
    start_color();
    init_pair(1, COLOR_WHITE, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    noecho();
    curs_set(FALSE);
    timeout(0);
    char **a = NULL;
    char **b = NULL;
    char **c = NULL;
    int flag = 1;
    if (create_a(&a) && create_a(&b) && create_a(&c) && input(a)) {
        int speed = 200000, gen = 1, end_program = 0, k_speed = 0;
        output(a, gen, k_speed);
        copy_a(a, c);
        while (!zero_a(a) && !equal_a_b(a, b) && !end_program && flag) {
            gen++;
            copy_a(a, b);
            update(a, b);
            if (gen > 1 && equal_a_b(a, c))
                flag = 0;
            else if (gen > 1 && !equal_a_b(a, c))
                copy_a(b, c);
            int ch = getch();
            speed_fun(&speed, ch, &end_program, &k_speed);
            usleep(speed);
            output(a, gen, k_speed);
        }
        timeout(-1);
        mvprintw(h + 2, w - 5, "THE END");
        getch();
        endwin();
    } else {
        endwin();
        printf("Error!");
    }
    del_all_memory(&a, &b, &c);
}

void del_all_memory(char ***a, char ***b, char ***c) {
    del_memory(a);
    del_memory(b);
    del_memory(c);
}

void speed_fun(int *speed, int input, int *end_program, int *k_speed) {
    int flag = 1;
    if (input == ERR) return;
    if (input == '+') {
        if (*speed >= 20000) {
            *speed -= 20000;
            refresh();
            (*k_speed)++;
        }
    } else if (input == '-') {
        *speed += 20000;
        refresh();
        (*k_speed)--;
    } else if (input == 'p') {
        refresh();
        while (flag) {
            int ch = getch();
            if (ch == 'p') {
                refresh();
                flag = 0;
            } else if (ch == 'o') {
                *end_program = 1;
                flag = 0;
            }
        }
    } else if (input == 'o')
        *end_program = 1;
}

void del_memory(char ***a) {
    for (int i = 0; i < h; i++) free((*a)[i]);
    free(*a);
}

void output(char **a, int k, int k_speed) {
    clear();
    print_borders();
    for (int i = 0; i < h; i++) {
        mvprintw(i + 1, 0, "|");
        for (int j = 0; j < w; j++) {
            if (a[i][j] == '*') {
                attron(COLOR_PAIR(2));
                mvprintw(i + 1, j + 1, "%c", a[i][j]);
                attron(COLOR_PAIR(1));
            } else
                mvprintw(i + 1, j + 1, "%c", a[i][j]);
        }
        mvprintw(i + 1, w + 1, "|");
    }
    print_borders();
    mvprintw(h + 2, 0, "Generation: %d", k);
    mvprintw(h + 3, 0, "Speed: %d", k_speed);
    refresh();
}

void print_borders() {
    mvprintw(0, 0, "+");
    for (int i = 1; i <= w; i++) {
        mvprintw(0, i, "-");
        mvprintw(h + 1, i, "-");
    }
    mvprintw(0, w + 1, "+");
    mvprintw(h + 1, 0, "+");
    mvprintw(h + 1, w + 1, "+");
}

void copy_a(char **a, char **b) {
    for (int i = 0; i < h; i++)
        for (int j = 0; j < w; j++) b[i][j] = a[i][j];
}

int zero_a(char **a) {
    int flag = 1;
    for (int i = 0; i < h && flag; i++)
        for (int j = 0; j < w && flag; j++)
            if (a[i][j] == '*') flag = 0;
    return flag;
}

int equal_a_b(char **a, char **b) {
    int flag = 1;
    for (int i = 0; i < h && flag; i++)
        for (int j = 0; j < w && flag; j++)
            if (a[i][j] != b[i][j]) flag = 0;
    return flag;
}

int input(char **a) {
    int flag = 1;
    for (int i = 0; i < h && flag; i++) {
        for (int j = 0; j < w && flag; j++)
            if (scanf("%c", &a[i][j]) == EOF || (a[i][j] != '~' && a[i][j] != '*')) flag = 0;
        getchar();
    }
    freopen("/dev/tty", "r", stdin);
    return flag;
}

int count_life_sosed(char **a, int x, int y) {
    int k = 0;
    k += (a[get_i(-1, x, h)][get_i(0, y, w)] == '*') ? 1 : 0;
    k += (a[get_i(1, x, h)][get_i(0, y, w)] == '*') ? 1 : 0;
    k += (a[get_i(0, x, h)][get_i(-1, y, w)] == '*') ? 1 : 0;
    k += (a[get_i(0, x, h)][get_i(1, y, w)] == '*') ? 1 : 0;
    k += (a[get_i(-1, x, h)][get_i(-1, y, w)] == '*') ? 1 : 0;
    k += (a[get_i(-1, x, h)][get_i(1, y, w)] == '*') ? 1 : 0;
    k += (a[get_i(1, x, h)][get_i(-1, y, w)] == '*') ? 1 : 0;
    k += (a[get_i(1, x, h)][get_i(1, y, w)] == '*') ? 1 : 0;
    return k;
}

int get_i(int i, int now, int len) { return (now + i + len) % len; }

void update(char **a, char **b) {
    for (int i = 0; i < h; i++)
        for (int j = 0; j < w; j++) {
            int k = count_life_sosed(b, i, j);
            if (b[i][j] == '*')
                a[i][j] = (k == 2 || k == 3) ? '*' : '~';
            else
                a[i][j] = (k == 3) ? '*' : '~';
        }
}

int create_a(char ***a) {
    int flag = 0;
    *a = malloc(h * sizeof(char *));
    if (*a != NULL) {
        flag = 1;
        for (int i = 0; i < h && flag; i++) {
            (*a)[i] = malloc(w * sizeof(char));
            if ((*a)[i] == NULL) {
                for (int j = 0; j < i; j++) free((*a)[j]);
                free(*a);
                flag = 0;
            }
        }
    }
    return flag;
}
