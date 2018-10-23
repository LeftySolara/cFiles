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

#include "ui.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <ncurses.h>

#define MAX_PATH_LENGTH 4096

int main()
{
    struct ui *ui = setup_ui();
    getcwd(ui->cwd, MAX_PATH_LENGTH);

    menu_append(ui->menu, "test 1", COLOR_BLACK, COLOR_CYAN, 0);
    menu_append(ui->menu, "test 2", COLOR_BLACK, COLOR_CYAN, 0);
    menu_append(ui->menu, "test 3", COLOR_BLACK, COLOR_CYAN, 0);
    menu_append(ui->menu, "test 4", COLOR_BLACK, COLOR_CYAN, 0);

    print_cwd(ui);
    print_menu(ui);
    refresh_ui(ui);

    int ch = 0;
    while (ch != 'q')
        ch = getch();

    teardown_ui(ui);

    return 0;
}
