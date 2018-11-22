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
#include <unistd.h>

struct ui *setup_ui(struct dir_list *dir_list)
{
    setup_ncurses();

    struct ui *ui = malloc(sizeof(struct ui));
    *(int *)&ui->color_enabled = has_colors();
    ui->menu = setup_menu();

    setup_main_window(ui, dir_list);

    return ui;
}

struct menu *setup_menu()
{
    struct menu *menu = malloc(sizeof(struct menu));
    menu->items = calloc(255, sizeof(struct menu_item));
    menu->idx_selected = -1;  /* no item selected */
    menu->num_items = 0;
    menu->max_items = 255;
    menu->changed = 1;

    return menu;
}

struct menu_item *setup_menu_item(char *text, enum color_pair colors, int bold)
{
    struct menu_item *item = malloc(sizeof(struct menu_item));
    item->display_text = text;
    item->colors = colors;
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
    init_pair(PAIR_NORMAL, COLOR_WHITE, COLOR_BLACK);
    init_pair(PAIR_CWD, COLOR_CYAN, COLOR_BLACK);
    init_pair(PAIR_DIR, COLOR_BLUE, COLOR_BLACK);
    init_pair(PAIR_EXECUTABLE, COLOR_RED, COLOR_BLACK);
    init_pair(PAIR_SYMLINK, COLOR_CYAN, COLOR_BLACK);

    refresh();
}

/* Create a new fullscreen window, with a subwindow for displaying file properties. */
void setup_main_window(struct ui *ui, struct dir_list *dir_list)
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

    refresh_ui(ui, dir_list);
}

void teardown_ui(struct ui *ui)
{
    endwin();
    teardown_main_window(ui);
    teardown_menu(ui->menu);

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

void refresh_ui(struct ui *ui, struct dir_list *dir_list)
{
    if (ui->changed) {
        print_menu(ui, dir_list);
        ui->changed = 0;
    }
    
    wnoutrefresh(ui->main_window);
    touchwin(ui->main_window_sub);
    wnoutrefresh(ui->main_window_sub);
    doupdate();
}

void print_path(struct ui *ui, char *path)
{
    wmove(ui->main_window, 1, 2);
    wclrtoeol(ui->main_window);
    mvaddch(1, ui->main_window->_maxx, ACS_VLINE);

    if (ui->color_enabled)
        wattron(ui->main_window, COLOR_PAIR(PAIR_CWD));

    wattron(ui->main_window, A_BOLD);
    mvwaddstr(ui->main_window, 1, 2, path);
    wattroff(ui->main_window, A_BOLD);

    if (ui->color_enabled)
        wattroff(ui->main_window, COLOR_PAIR(PAIR_CWD));
}

void print_menu(struct ui *ui, struct dir_list *dir_list)
{
    WINDOW *target_win = ui->main_window_sub;
    wclear(target_win);

    int y = 0;
    int bold = 0;
    int selected = 0;
    enum color_pair colors = PAIR_NORMAL;
    struct dir_entry *entry = dir_list->head;

    while (entry) {
        selected = (entry == dir_list->selected_entry);

        switch (entry->type) {
        case DT_REG:
            bold = (entry->is_executable == 0);
            colors = (entry->is_executable) ? PAIR_NORMAL : PAIR_EXECUTABLE;
            break;
        case DT_DIR:
            bold = 1;
            colors = PAIR_DIR;
            break;
        case DT_LNK:  /* Symlinks to files that aren't directories */
            bold = 0;
            colors = PAIR_SYMLINK;
            break;
        default:
            bold = 0;
            colors = PAIR_NORMAL;
            break;
        }

        if (ui->color_enabled)
            wattron(target_win, COLOR_PAIR(colors));
        if (bold)
            wattr_on(target_win, A_BOLD, NULL);
        if (selected)
            wattr_on(target_win, A_STANDOUT, NULL);

        mvwaddstr(target_win, y++, 0, entry->name);

        if (ui->color_enabled)
            wattroff(target_win, COLOR_PAIR(colors));
        if (bold)
            wattr_off(target_win, A_BOLD, NULL);
        if (selected)
            wattr_off(target_win, A_STANDOUT, NULL);

        entry = entry->next;
    }
}

void menu_move_up(struct menu *menu)
{
    if (menu->idx_selected <= 0)
        return;

    menu->idx_selected -= 1;
    menu->changed = 1;
}

void menu_move_down(struct menu *menu)
{
    if (menu->idx_selected == (menu->num_items - 1))
        return;

    menu->idx_selected += 1;
    menu->changed = 1;
}

void menu_append(struct menu *menu, char *text, enum color_pair colors, int bold)
{
    int new_max_size;
    if (menu->num_items == menu->max_items) {
        new_max_size = menu->num_items * 2;
        menu->items = realloc(menu->items, sizeof(struct menu_item) * new_max_size);
        menu->max_items = new_max_size;
    }

    struct menu_item *item = setup_menu_item(text, colors, bold);
    menu->items[menu->num_items++] = item;

    if (menu->idx_selected < 0)
        menu->idx_selected = 0;

    menu->changed = 1;
}

void menu_update_entries(struct ui *ui, struct directory *cwd)
{
    for (int i = 0; i < ui->menu->num_items; ++i)
        teardown_menu_item(ui->menu->items[i]);
    ui->menu->num_items = 0;

    /*
    struct dirent *entry;
    int entry_type;
    enum color_pair colors = PAIR_NORMAL;
    int bold = 0;
    int executable;

    for (int i = 0; i < cwd->num_entries; ++i) {
        entry = cwd->entries[i];
        entry_type = entry->d_type;
        executable = access(entry->d_name, X_OK);

        switch (entry_type) {
        case DT_REG:
            bold = 0;
            colors = (executable != 0) ? PAIR_NORMAL : PAIR_EXECUTABLE;
            break;
        case DT_DIR:
            bold = 1;
            colors = PAIR_DIR;
            break;
        case DT_LNK:
            bold = 1;
            colors = PAIR_SYMLINK;
            break;
        default:
            bold = 0;
            colors = PAIR_NORMAL;
            break;
        }
        menu_append(ui->menu, entry->d_name, colors, bold);
    }
    */
}
