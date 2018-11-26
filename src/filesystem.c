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
#include <limits.h>
#include <ctype.h>
#include <dirent.h>
#include <unistd.h>
// #include <unistd.h>
#include <sys/stat.h>

/* static const int max_path_length = 4096; */

struct dir_entry *dir_entry_init()
{
    struct dir_entry *entry = malloc(sizeof(struct dir_entry));

    entry->name = calloc(256, sizeof(char));
    entry->name[0] = '\0';
    entry->type = DT_UNKNOWN;

    entry->is_executable = 0;
    entry->is_symlink = 0;
    entry->is_hidden_file = 0;

    entry->prev = NULL;
    entry->next = NULL;

    return entry;
}

struct dir_list *dir_list_init()
{
    struct dir_list *list = malloc(sizeof(struct dir_list));
    list->head = NULL;
    list->selected_entry = NULL;
    list->num_entries = 0;
    list->path = calloc(PATH_MAX, sizeof(char));
    list->path[0] = '\0';

    return list;
}

void dir_entry_free(struct dir_entry *entry)
{
    entry->prev = NULL;
    entry->next = NULL;
    free(entry->name);
    free(entry);
}

void dir_list_free(struct dir_list *list)
{
    list->selected_entry = NULL;

    struct dir_entry *current = list->head;
    while (list->head) {
        list->head = current->next;
        dir_entry_free(current);
        current = current->next;
    }

    free(list->path);
    free(list);
}

void dir_list_append(struct dir_list *list, char *name, unsigned char type,
                     int executable, int symlink, int hidden)
{
    struct dir_entry *entry = dir_entry_init();

    strcpy(entry->name, name);
    entry->type = type;
    entry->is_executable = executable;
    entry->is_symlink = symlink;
    entry->is_hidden_file = hidden;

    if (!list->head) {
        list->head = entry;
        list->selected_entry = list->head;
    }
    else {
        struct dir_entry *current = list->head;
        while (current->next)
            current = current->next;
        current->next = entry;
        entry->prev = current;
    }

    ++list->num_entries;
}

void get_entries(struct dir_list *list, char *path, int select_current)
{
    DIR *dp;
    struct dirent *ent;
    /* enum color_pair colors = PAIR_NORMAL; */

    int is_executable;
    int is_symlink;
    int is_hidden_file;
    if ((dp = opendir(path)) != NULL) {
        strcpy(list->path, path);
        while ((ent = readdir(dp)) != NULL) {
            is_executable = access(ent->d_name, X_OK);
            is_symlink = (ent->d_type == DT_LNK);
            is_hidden_file = (ent->d_name[0] == '.');
            if (is_symlink)
                ent->d_type = resolve_symlink_type(ent, path);

            dir_list_append(list, ent->d_name, ent->d_type, is_executable, is_symlink, is_hidden_file);
        }
        closedir(dp);
        list->head = mergesort(list->head);

        if (select_current)
            list->selected_entry = list->head;
        else
            select_first_non_hidden(list);
    }

    /* TODO: Handle directory not opening */
}

void clear_entries(struct dir_list *list)
{
    struct dir_entry *current = list->head;

    while (current) {
        list->head = current ->next;
        dir_entry_free(current);
        current = list->head;
    }

    list->head = NULL;
    list->selected_entry = NULL;
    list->num_entries = 0;
}

/* Determine the type of file that a symlink points to. */
unsigned char resolve_symlink_type(struct dirent *entry, char *path)
{
    if (entry->d_type != DT_LNK)
        return entry->d_type;

    char full_path[PATH_MAX + 1];

    strcpy(full_path, path);
    strcat(full_path, "/");
    strcat(full_path, entry->d_name);

    struct stat sb;
    if (stat(full_path, &sb) == 0 && S_ISDIR(sb.st_mode))
        return DT_DIR;

    return entry->d_type;
}

void select_prev(struct dir_list *list, int skip_hidden)
{
    struct dir_entry *current = list->selected_entry;

    if (!current->prev)
        return;

    if (skip_hidden) {

        struct dir_entry *last_non_hidden = current;
        while (current != list->head->next && current->prev) {
            current = current->prev;
            if (!current->is_hidden_file) {
                last_non_hidden = current;
                break;
            }
        }
        list->selected_entry = last_non_hidden;
    }
    else
        list->selected_entry = current->prev;

    /*
    do {
        if (skip_hidden && !strcmp(current->prev->name, ".."))
            break;
        current = current->prev;
    } while (skip_hidden && current->is_hidden_file && current->prev);

    list->selected_entry = current;
    */

}

void select_next(struct dir_list *list, int skip_hidden)
{
    struct dir_entry *current = list->selected_entry;

    if (!current->next)
        return;

    do {
        current = current->next;
    } while (skip_hidden && current->is_hidden_file && current->next);

    list->selected_entry = current;
}

void select_first_non_hidden(struct dir_list *list)
{
    struct dir_entry *current = list->head;

    while (current) {
        if (current->is_hidden_file == 0) {
            list->selected_entry = current;
            return;
        }
        current = current->next;
    }
}

/*
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

    if (entry_type == DT_LNK) {
        char full_path[PATH_MAX + 1];

        strcpy(full_path, cwd->path);
        strcat(full_path, "/");
        strcat(full_path, entry->d_name);

        struct stat sb;
        if (stat(full_path, &sb) == 0 && S_ISDIR(sb.st_mode))
            entry_type = DT_DIR;
    }
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

*/

void open_selected_entry(struct dir_list *list, int select_current)
{
    struct dir_entry *target = list->selected_entry;
    char new_path[PATH_MAX];

    strcpy(new_path, list->path);
    if (target->type == DT_DIR) {
        if (strcmp(target->name, ".") == 0)
            return;
        else if (strcmp(target->name, "..") == 0) {
            char *last_slash = strrchr(new_path, '/');
            *last_slash = '\0';
        }
        else {
            strcat(new_path, "/");
            strcat(new_path, target->name);
        }

        /* Prevent attempting to go higher than the system root directory. */
        if (strcmp(new_path, "") == 0)
            strcpy(new_path, "/");

        clear_entries(list);
        get_entries(list, new_path, select_current);
    }
}

struct dir_entry *merge(struct dir_entry *first, struct dir_entry *second)
{
    if (!first)
        return second;
    if (!second)
        return first;

    int i = 0;
    char first_char = tolower(first->name[i]);
    char second_char = tolower(second->name[i]);

    while (first_char == second_char) {
        ++i;
        first_char = tolower(first->name[i]);
        second_char = tolower(second->name[i]);
    }

    if (first_char < second_char) {
        first->next = merge(first->next, second);
        first->next->prev = first;
        first->prev = NULL;
        return first;
    }
    else {
        second->next = merge(first, second->next);
        second->next->prev = second;
        second->prev = NULL;
        return second;
    }
}

struct dir_entry *mergesort(struct dir_entry *head)
{
    if (!head || !head->next)
        return head;
    struct dir_entry *second = split(head);

    head = mergesort(head);
    second = mergesort(second);

    return merge(head, second);
}

/* Split a list of entries into two halves. */
struct dir_entry *split(struct dir_entry *head)
{
    struct dir_entry *fast = head;
    struct dir_entry *slow = head;

    while (fast->next && fast->next->next) {
        fast = fast->next->next;
        slow = slow->next;
    }
    struct dir_entry *temp = slow->next;
    slow->next = NULL;

    return temp;
}
