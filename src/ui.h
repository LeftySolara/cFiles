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

#include <ncurses.h>

struct ui {
    WINDOW *main_window;
    WINDOW *main_window_sub;

    char *cwd;
    const int color_enabled;
};

struct ui *setup_ui();
void setup_ncurses();
void setup_main_window(struct ui *ui);

void teardown_ui(struct ui *ui);
void teardown_main_window(struct ui *ui);

void refresh_ui(struct ui *ui);

void print_cwd(struct ui *ui);
