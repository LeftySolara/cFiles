/*******************************************************************************
 * command.c : interface for executing user commands
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

#include "command.h"
#include <ncurses.h>

#define KEY_CTRL(x) ((x) & 0x1f)

static struct command cmds[] = {

    {CMD_NONE, {0, 0, 0}},
    {CMD_MENU_MOVE_UP, {'k', KEY_UP, 0}},
    {CMD_MENU_MOVE_DOWN, {'j', KEY_DOWN, 0}},
    {CMD_QUIT, {'q', KEY_CTRL('c'), 0}},
};

enum command_type find_command(int key)
{
    if (key == 0)
        return CMD_NONE;

    for (int i = 0; i < NUM_CMDS; ++i) {
        for (int j = 0; j < MAX_KEYS; ++j) {
            if (cmds[i].keys[j] == key)
                return cmds[i].type;
        }
    }

    return CMD_NONE;
}

int execute_command(enum command_type cmd_type, struct ui *ui)
{
    unsigned rc = 0;
    switch(cmd_type) {
    case CMD_NONE:
        break;
    case CMD_MENU_MOVE_UP:
        menu_move_up(ui->menu);
        break;
    case CMD_MENU_MOVE_DOWN:
        menu_move_down(ui->menu);
        break;
    default:
        break;
    }

    return rc;
}
