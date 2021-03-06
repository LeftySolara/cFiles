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

struct dir_entry {
    char *name;
    unsigned char type;
    int is_executable;
    int is_symlink;
    int is_hidden_file;

    struct dir_entry *prev;
    struct dir_entry *next;
};

struct dir_list {
    struct dir_entry *head;
    struct dir_entry *selected_entry;
    int num_entries;
    char *path;
};


struct dir_entry *dir_entry_init();
struct dir_list *dir_list_init();

void dir_entry_free(struct dir_entry *entry);
void dir_list_free(struct dir_list *list);

void dir_list_append(struct dir_list *list, char *name, unsigned char type, int executable, int symlink, int hidden);

void get_entries(struct dir_list *list, char *path, int select_current);
void clear_entries(struct dir_list *list);

unsigned char resolve_symlink_type(struct dirent *entry, char *path);

void select_prev(struct dir_list *list, int skip_hidden);
void select_next(struct dir_list *list, int skip_hidden);
void select_first_non_hidden(struct dir_list *list);

void open_selected_entry(struct dir_list *list, int select_current);

struct dir_entry *merge(struct dir_entry *first, struct dir_entry *second);
struct dir_entry *mergesort(struct dir_entry *head);
struct dir_entry *split(struct dir_entry *head);

#endif /* FILESYSTEM_H */
