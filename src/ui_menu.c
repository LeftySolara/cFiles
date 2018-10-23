/*******************************************************************************
 * ui_menu.c : functions for menu interaction
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

#include <stdlib.h>
#include "ui_menu.h"

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
