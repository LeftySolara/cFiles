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
#include "command.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <ncurses.h>

#define MAX_PATH_LENGTH 4096

int main()
{
    struct ui *ui = setup_ui();
    getcwd(ui->cwd, MAX_PATH_LENGTH);

    struct dirent **dir_entries;
    int dir_count = scandir(ui->cwd, &dir_entries, NULL, alphasort);

    for (int i = 0; i < dir_count; ++i)
        menu_append(ui->menu, dir_entries[i]->d_name, COLOR_BLACK, COLOR_WHITE, FALSE);

    print_cwd(ui);
    print_menu(ui);
    refresh_ui(ui);

    int ch = 0;
    enum command_type cmd = CMD_NONE;
    while (cmd != CMD_QUIT) {
        ch = getch();
        cmd = find_command(ch);
        execute_command(cmd, ui);

        refresh_ui(ui);
    }

    teardown_ui(ui);

    return 0;
}
