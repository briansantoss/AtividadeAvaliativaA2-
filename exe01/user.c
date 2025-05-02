#include "user.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int get_node_height(User* node) {
    return  node ? node->height : 0;
}

static int greater_node_height(User* node1, User* node2) {
    int node1_height = get_node_height(node1);
    int node2_height = get_node_height(node2);

    return node1_height > node2_height ? node1_height : node2_height;
}

static void update_node_height(User* node) {
    node->height =  1 + greater_node_height(node->left, node->right);
}

static int get_balance_factor(User* node) {
    if (!node) return 0;
    return get_node_height(node->left) - get_node_height(node->right);
}

static void rotate_left(User** node_ptr) {
    User* next_root = (*node_ptr)->right;
    (*node_ptr)->right = next_root->left;
    next_root->left = *node_ptr;
    *node_ptr = next_root;

    update_node_height((*node_ptr)->right);
    update_node_height(*node_ptr);
}

static void rotate_right(User** node_ptr) {
    User* next_root = (*node_ptr)->left;
    (*node_ptr)->left = next_root->right;
    next_root->right = *node_ptr;
    *node_ptr = next_root;

    update_node_height((*node_ptr)->right);
    update_node_height(*node_ptr);
}

static User* create_node(const char* name, const char* email) {
    static int actual_id = 0;

    User* new_user = (User*) malloc(sizeof (User));
    if (!new_user) return NULL;

    char* user_name = strdup(name);
    if (!user_name) {
        free(new_user);
        return NULL;
    } 

    char* user_email = strdup(email);
    if (!user_email) {
        free(new_user);
        free(user_name);
        return NULL;
    }

    new_user->name = user_name;
    new_user->email = user_email;
    new_user->id = actual_id++;
    new_user->height = 0;
    new_user->left = new_user->right = NULL;
    return new_user;
}

static bool recursive_register_user(User** node_ptr, const char* name, const char* email) {
    if (!*node_ptr) {
        User* new_user = create_node(name, email);
        if (!new_user) return false;

        *node_ptr = new_user; 
        return true;
    } else {
        int comparation_result = strcmp(name, (*node_ptr)->name);
        if (comparation_result > 0) {
            recursive_register_user(&(*node_ptr)->right, name, email);
        } else if (comparation_result < 0) {
            recursive_register_user(&(*node_ptr)->left, name, email);
        } else {
            return false;
        }
    }
}

bool register_user(Users* users, const char* name, const char* email) {
    return users && name && email ? recursive_register_user(&users->root, name, email) : false;
}

void remove_user(Users* users, const char* target_name) {
    printf("\nTODO");
}

static void recursive_search_user(User* node, const char* name) {
    if (!node) {
        printf("\nNo user named %s was found in the system.", name);
    } else {
        int comparation_result = strcmp(name, node->name);
        if (comparation_result > 0) {
            recursive_search_user(node->right, name);
        } else if (comparation_result < 0) {
            recursive_search_user(node->left, name);
        } else {
            printf("\nId: %d\nName: %s\nEmail: %s\n", node->id, node->name, node->email);
        }
    }
}

void search_user(Users* users, const char* target_name) {
    if (!users) {
        printf("\nNo user named %s was found in the system.", target_name);
    } else {
        recursive_search_user(users->root, target_name);
    }
}

static void recursive_ordered_list_users(User* node) {
    if (node) {
        recursive_ordered_list_users(node->left);
        printf("\n%s", node->name);
        recursive_ordered_list_users(node->right);
    }
}

void ordered_list_users(Users* users) {
    if (!users || !users->root) {
        puts("\nNo user registered.");
    } else {
        recursive_ordered_list_users(users->root);
    }
}

static void recursive_free_users_tree(User* node) {
    if (node) {
        recursive_free_users_tree(node->left);
        recursive_free_users_tree(node->right);

        free(node->name);
        free(node->email);
        free(node);
    }
}

void free_users_tree(Users* users) {
    if (users) {
        recursive_free_users_tree(users->root);
    }
}

Users* create_users_tree() {
    Users* new_users_tree = (Users*) malloc(sizeof (Users));
    new_users_tree->root = NULL;
    return new_users_tree;
}
