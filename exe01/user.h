#ifndef USER_H
#define USER_H

#include <stdbool.h>

typedef struct AVLTreeNode {
    char* name;
    char* email;
    int id;
    int height;
    struct AVLTreeNode* left;
    struct AVLTreeNode* right;
} User;

typedef struct {
    User* root;
} Users;

Users* create_users_tree(void);
void free_users_tree(Users* users);
bool register_user(Users* users, const char* name, const char* email);
bool remove_user(Users* users, const char* target_name);
void search_user(Users* users, const char* target_name);
void ordered_list_users(Users* users);

#endif 