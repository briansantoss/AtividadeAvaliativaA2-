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
    return get_node_height(node->right) - get_node_height(node->left);
}

static void rotate_left(User** node_ptr) {
    User* next_root = (*node_ptr)->right;
    (*node_ptr)->right = next_root->left;
    next_root->left = *node_ptr;
    *node_ptr = next_root;

    update_node_height((*node_ptr)->left);
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

static void double_rotate_right(User** node_ptr) {
    rotate_left(&(*node_ptr)->left);
    rotate_right(node_ptr);
}

static void double_rotate_left(User** node_ptr) {
    rotate_right(&(*node_ptr)->right);
    rotate_left(node_ptr);
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

    *new_user = (User) {.name = user_name, .email = user_email, .id = actual_id++, .height = 1, .left = NULL, .right = NULL};
    return new_user;
}

static bool recursive_register_user(User** node_ptr, const char* name, const char* email) {
    if (!*node_ptr) {
        User* new_user = create_node(name, email);
        if (!new_user) return false;

        *node_ptr = new_user; 
        return true;
    } 

    bool inserted = false;
    int comparation_result = strcmp(name, (*node_ptr)->name);
    if (comparation_result > 0) {
        inserted = recursive_register_user(&(*node_ptr)->right, name, email);
    } else if (comparation_result < 0) {
        inserted = recursive_register_user(&(*node_ptr)->left, name, email);
    } else {
        return false;
    }

    if (inserted) {
        update_node_height(*node_ptr);

        int balance_factor = get_balance_factor(*node_ptr);

        if (balance_factor > 1) {
            if (strcmp(name, (*node_ptr)->right->name) > 0) {
                rotate_left(node_ptr);
            } else {
                double_rotate_left(node_ptr);
            }
        } else if (balance_factor < -1) {
            if (strcmp(name, (*node_ptr)->left->name) < 0) {
                rotate_right(node_ptr);
            } else {
                double_rotate_right(node_ptr);
            }
        }
    }
    return inserted;
}

bool register_user(Users* users, const char* name, const char* email) {
    return users && name && email ? recursive_register_user(&users->root, name, email) : false;
}

static User* get_min_node(User* root) {;
    while (root && root->left) {
        root = root->left;
    }
    return root;
}

static bool recursive_remove_user(User** node_ptr, const char* target_name) {
    if (!*node_ptr) {
        printf("\nNo user named %s was not found...\n", target_name);
        return false;
    }

    bool removed = false;
    int comparation_result = strcmp(target_name, (*node_ptr)->name);

    if (comparation_result > 0) {
        removed = recursive_remove_user(&(*node_ptr)->right, target_name);
    } else if (comparation_result < 0) {
        removed = recursive_remove_user(&(*node_ptr)->left, target_name);
    } else {
        User* right = (*node_ptr)->right;
        User* left = (*node_ptr)->left;

        if (right && left) {
            User* right_min = get_min_node(right);
            char* temp_name = strdup(right_min->name);
            char* temp_email = strdup(right_min->email);

            if (!temp_name || !temp_email) {
                free(temp_name);
                free(temp_email);
                return false; 
            }

            free((*node_ptr)->name);
            free((*node_ptr)->email);
            (*node_ptr)->name = temp_name;
            (*node_ptr)->email = temp_email;
            (*node_ptr)->id = right_min->id;
            (*node_ptr)->height = right_min->height;

            removed = recursive_remove_user(&(*node_ptr)->right, right_min->name);
        } else {
            User* temp = *node_ptr;
            *node_ptr = right ? right : left;
            free(temp->name);
            free(temp->email);
            free(temp);
            removed = true;
        }
    }

    if (removed && *node_ptr) {
        update_node_height(*node_ptr);
        int balance_factor = get_balance_factor(*node_ptr);

        if (balance_factor > 1) {
            if (get_balance_factor((*node_ptr)->right) >= 0) {
                rotate_left(node_ptr);
            } else {
                double_rotate_left(node_ptr);
            }
        } else if (balance_factor < -1) {
            if (get_balance_factor((*node_ptr)->left) <= 0) {
                rotate_right(node_ptr);
            } else {
                double_rotate_right(node_ptr);
            }
        }
    }
    return removed;
}

bool remove_user(Users* users, const char* target_name) {
    return users && target_name ? recursive_remove_user(&users->root, target_name) : false;
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

void get_head_name(Users* users) {
    printf("%s", users->root->name);
}