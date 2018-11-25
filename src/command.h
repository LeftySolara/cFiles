/*******************************************************************************
 * command.h : interface for executing user commands
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

#ifndef COMMAND_H
#define COMMAND_H

#include "ui.h"
#include "filesystem.h"

#define MAX_KEYS 3 /* Number of keys that can be mapped to a command. */

enum command_type {
    CMD_NONE,
    CMD_MENU_SELECT,
    CMD_MENU_SELECT_PARENT,
    CMD_MENU_MOVE_UP,
    CMD_MENU_MOVE_DOWN,
    CMD_TOGGLE_HIDDEN,
    CMD_QUIT,
    NUM_CMDS
};

struct command {
    enum command_type type;
    int keys[MAX_KEYS];
};

enum command_type find_command(int key);
int execute_command(enum command_type cmd_type, struct dir_list *dir_list, struct ui *ui);

#endif /* COMMAND_H */
