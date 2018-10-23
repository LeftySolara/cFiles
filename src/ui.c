/*******************************************************************************
 * ui.c : functions for UI display and interaction
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

#include "ui.h"

#include <stdlib.h>
#include <locale.h>

#define MAX_PATH_LENGTH 4096

struct ui *setup_ui()
{
    setup_ncurses();

    struct ui *ui = malloc(sizeof(struct ui));
    ui->cwd = malloc(sizeof(char) * MAX_PATH_LENGTH);

    *(int *)&ui->color_enabled = has_colors();

    setup_main_window(ui);

    return ui;
}

void setup_ncurses()
{
    setlocale(LC_ALL, "");
    initscr();
    cbreak();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);

    start_color();
    init_pair(1, COLOR_CYAN, COLOR_BLACK);

    refresh();
}

/* Create a new fullscreen window, with a subwindow for displaying file properties. */
void setup_main_window(struct ui *ui)
{
    int maxy, maxx;
    ui->main_window = newwin(0, 0, 0, 0);
    getmaxyx(ui->main_window, maxy, maxx);
    ui->main_window_sub = subwin(ui->main_window, maxy - 4, maxx - 4, 3, 2);

    /* Add borders for whole window and add box for file path display. */
    box(ui->main_window, 0, 0);
    mvwaddch(ui->main_window, 2, 0, ACS_LTEE);
    mvwhline(ui->main_window, 2, 1, ACS_HLINE, maxx - 2);
    mvwaddch(ui->main_window, 2, maxx - 1, ACS_RTEE);

    refresh_ui(ui);
}

void teardown_ui(struct ui *ui)
{
    endwin();
    teardown_main_window(ui);

    free(ui->cwd);
    free(ui);
}

void teardown_main_window(struct ui *ui)
{
    delwin(ui->main_window_sub);
    delwin(ui->main_window);
}

void refresh_ui(struct ui *ui)
{
    wnoutrefresh(ui->main_window);
    touchwin(ui->main_window_sub);
    wnoutrefresh(ui->main_window_sub);
    doupdate();
}

void print_cwd(struct ui *ui)
{
    if (ui->color_enabled)
        wattron(ui->main_window, COLOR_PAIR(1));

    wattron(ui->main_window, A_BOLD);
    mvwaddstr(ui->main_window, 1, 2, ui->cwd);
    wattroff(ui->main_window, A_BOLD);

    if (ui->color_enabled)
        wattroff(ui->main_window, COLOR_PAIR(1));

}
