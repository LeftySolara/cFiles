/*******************************************************************************
 * main.c : entry point for the main program loop
 *******************************************************************************
 * cFiles - A basic ncurses file browser
 * Copyright (C) 2018 Jalen Adams
 *
 * Authors: Jalen Adams <leftysolara@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 ******************************************************************************/

#include <stdio.h>
#include <string.h>
#include <locale.h>
#include <ncurses.h>

void ncurses_init();

int main(int argc, char **argv)
{
    ncurses_init();

    int maxx;
    int maxy;
    getmaxyx(stdscr, maxy, maxx);

    const char *message = "Hello world!";
    wclear(stdscr);
    mvwaddstr(stdscr, maxy / 2, maxx / 2, message);
    wnoutrefresh(stdscr);
    doupdate();

    int ch;
    while (ch != 'q')
        ch = getch();

    endwin();

    return 0;
}

void ncurses_init()
{
    setlocale(LC_ALL, "");
    initscr();
    cbreak();
    noecho();
    curs_set(0);
    nodelay(stdscr, 1);
    keypad(stdscr, TRUE);
    refresh();
}
