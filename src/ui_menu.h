/*******************************************************************************
 * ui_menu.h : functions for menu interaction
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

#ifndef UI_MENU_H
#define UI_MENU_H

struct menu_item {
    char *display_text;
    short color_back;
    short color_fore;
    int is_bold;
};

struct menu {
    struct menu_item **items;
    unsigned int idx_selected;
    unsigned int num_items;
    unsigned int max_items;
};

struct menu *setup_menu();
struct menu_item *setup_menu_item(const char *text, short background,
                                 short foreground, int bold);

void teardown_menu(struct menu *menu);
void teardown_menu_item(struct menu_item *item);

void menu_append(struct menu *menu,const char *text,
                 short background, short foreground, int bold);

#endif /* UI_MENU_H */
