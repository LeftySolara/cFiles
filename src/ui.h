/*******************************************************************************
 * ui.h : functions for UI display and interaction
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

#ifndef UI_H
#define UI_H

#include "filesystem.h"
#include <ncurses.h>

struct directory;

enum color_pair {
    PAIR_NORMAL = 1,
    PAIR_CWD,
    PAIR_DIR,
    PAIR_EXECUTABLE,
    PAIR_SYMLINK,
};

struct menu_item {
    char *display_text;
    enum color_pair colors;
    int is_bold;
};

struct menu {
    struct menu_item **items;
    int idx_selected;
    unsigned int num_items;
    unsigned int max_items;
    int show_hidden;
    int changed;
};

struct ui {
    WINDOW *main_window;
    WINDOW *main_window_sub;
    struct menu *menu;

    int changed;
    const int color_enabled;
};

struct ui *setup_ui(struct dir_list *dir_list);
struct menu *setup_menu();
struct menu_item *setup_menu_item(char *text, enum color_pair colors, int bold);

void setup_ncurses();
void setup_main_window(struct ui *ui, struct dir_list *dir_list);

void teardown_ui(struct ui *ui);
void teardown_menu(struct menu *menu);
void teardown_menu_item(struct menu_item *item);
void teardown_main_window(struct ui *ui);

void refresh_ui(struct ui *ui, struct dir_list *dir_list);

void print_path(struct ui *ui, char *path);
void print_menu(struct ui *ui, struct dir_list *dir_list);

void menu_move_up(struct menu *menu);
void menu_move_down(struct menu *menu);
void menu_append(struct menu *menu, char *text, enum color_pair colors, int bold);

#endif /* UI_H */
