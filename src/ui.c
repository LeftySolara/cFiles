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
    ui->menu = setup_menu();

    *(int *)&ui->color_enabled = has_colors();

    setup_main_window(ui);

    return ui;
}

struct menu *setup_menu()
{
    struct menu *menu = malloc(sizeof(struct menu));
    menu->items = calloc(255, sizeof(struct menu_item));
    menu->idx_selected = -1;  /* no item selected */
    menu->num_items = 0;
    menu->max_items = 255;

    return menu;
}

struct menu_item *setup_menu_item(const char *text, short background,
                                  short foreground, int bold)
{
    struct menu_item *item = malloc(sizeof(struct menu_item));
    item->display_text = text;
    item->color_back = background;
    item->color_fore = foreground;
    item->is_bold = bold;

    return item;
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
    teardown_menu(ui->menu);

    free(ui->cwd);
    free(ui);
}

void teardown_menu(struct menu *menu)
{
    for (int i = 0; i < menu->num_items; ++i)
        teardown_menu_item(menu->items[i]);

    free(menu);
}

void teardown_menu_item(struct menu_item *item)
{
    free(item);
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

void print_menu(struct ui *ui)
{
    WINDOW *target_win = ui->main_window_sub;

    for (int i = 0; i < ui->menu->num_items; ++i)
        mvwaddstr(target_win, i, 0, ui->menu->items[i]->display_text);
}

void menu_append(struct menu *menu, const char *text,
                 short background, short foreground, int bold)
{
    int new_max_size;
    if (menu->num_items == menu->max_items) {
        new_max_size = menu->num_items * 2;
        menu->items = realloc(menu->items, sizeof(struct menu_item) * new_max_size);
        menu->max_items = new_max_size;
    }

    struct menu_item *item = setup_menu_item(text, background, foreground, bold);
    menu->items[menu->num_items++] = item;

    if (menu->idx_selected < 0)
        menu->idx_selected = 0;
}
