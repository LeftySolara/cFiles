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

#include "filesystem.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static const int max_path_length = 4096;

struct directory *get_dir(char *path)
{
    struct directory *directory = malloc(sizeof(struct directory));
    directory->path = malloc(sizeof(char) * max_path_length);

    directory->num_entries = scandir(path, &directory->entries, NULL, alphasort);
    getcwd(directory->path, max_path_length);

    return directory;
}

void free_dir(struct directory *directory)
{
    free(directory->entries);
    free(directory->path);
    free(directory);
}

void open_entry(struct directory *cwd, struct dirent *entry, struct ui *ui)
{
    unsigned entry_type = entry->d_type;

    if (entry_type == DT_DIR) {
        if (strcmp(entry->d_name, ".") == 0)
            return;
        else if (strcmp(entry->d_name, "..") == 0) {
            char *last_slash = strrchr(cwd->path, '/');
            *last_slash = '\0';
        }
        else {
            strcat(cwd->path, "/");
            strcat(cwd->path, entry->d_name);
        }

        if (strcmp(cwd->path, "") == 0)
            strcpy(cwd->path, "/");

        free(cwd->entries);
        cwd->num_entries = scandir(cwd->path, &cwd->entries, NULL, alphasort);
        ui->menu->idx_selected = 0;
    }
}
