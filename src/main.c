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
#include "filesystem.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <ncurses.h>

int main()
{
    struct dir_list *directory_list = dir_list_init();
    getcwd(directory_list->path, PATH_MAX);
    get_entries(directory_list, directory_list->path, 0);

    struct ui *ui = setup_ui(directory_list);
    /* struct directory *cwd = get_dir("."); */

    /* menu_update_entries(ui, cwd); */
    print_path(ui, directory_list->path);
    print_menu(ui, directory_list);
    refresh_ui(ui, directory_list);

    int ch = 0;
    enum command_type cmd = CMD_NONE;
    halfdelay(1);
    while (cmd != CMD_QUIT) {
        ch = getch();
        cmd = find_command(ch);
        execute_command(cmd, directory_list, ui);
        refresh_ui(ui, directory_list);
    }

    teardown_ui(ui);

    return 0;
}
