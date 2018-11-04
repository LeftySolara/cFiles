/*******************************************************************************
 * filesystem.h : functions for filesystem interaction
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

#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <dirent.h>

struct directory {
    char *path;
    int num_entries;
    struct dirent **entries;
};

struct directory *get_dir(char *path);
void free_dir(struct directory *directory);

char *get_cwd_path(char *buffer);

#endif /* FILESYSTEM_H */
