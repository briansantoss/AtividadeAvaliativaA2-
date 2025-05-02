#include <stdio.h>
#include "user.h"

typedef enum {
    REGISTER_USER = 1,
    REMOVE_USER,
    SEARCH_USER,
    ORDERED_LIST_USERS,
    EXIT
} MenuOption;

void display_menu(void);

int main() {
    Users* users = create_users_tree();
    if (!users) {
        perror("Error: internal management error.");
        return 1;
    }

    char name[100];
    char email[100];

    int choose;
    do {
        display_menu();
        scanf("%d", &choose);

        switch (choose) {
            case REGISTER_USER:
                printf("\nInser the user name: ");
                scanf("\n%99[^\n]", name);

                printf("Inser the user email: ");
                scanf("\n%99[^\n]", email);

                if(register_user(users, name, email)) {
                    printf("\nRegister done succesfully.");
                } else {
                    printf("\nError: register went wrong.");
                }

                break;
                
            case REMOVE_USER:
                // printf("\nProvide a name: ");
                // scanf("\n%99[^\n]", name);

                remove_user(users, name);
                break;

            case SEARCH_USER:
                printf("\nProvide a name to search for: ");
                scanf("\n%99[^\n]", name);

                search_user(users, name);
                break;

            case ORDERED_LIST_USERS:
                ordered_list_users(users);
                break;
        }

    } while (choose != EXIT);
    printf("\nThanks for the trust in our system. See you next time!");

    free_users_tree(users);

    return 0;
}

void display_menu(void) {
    printf("\n\n[%d] Register an user\n[%d] Remove an user by name\n[%d] Search for an user by name\n[%d] Ordered list all users\n[%d] Exit system", REGISTER_USER, REMOVE_USER, SEARCH_USER, ORDERED_LIST_USERS, EXIT);
    printf("\nProvide an option number: ");
}