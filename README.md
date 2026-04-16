# customer-support-system
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structure for Ticket
struct Ticket {
    int id;
    char issue[100];
    char status[50];
    struct Ticket *left, *right;
};

// Create a new ticket
struct Ticket* createTicket(int id, char issue[], char status[]) {
    struct Ticket* newNode = (struct Ticket*)malloc(sizeof(struct Ticket));
    newNode->id = id;
    strcpy(newNode->issue, issue);
    strcpy(newNode->status, status);
    newNode->left = newNode->right = NULL;
    return newNode;
}

// Insert (CREATE)
struct Ticket* insert(struct Ticket* root, int id, char issue[], char status[]) {
    if (root == NULL)
        return createTicket(id, issue, status);

    if (id < root->id)
        root->left = insert(root->left, id, issue, status);
    else if (id > root->id)
        root->right = insert(root->right, id, issue, status);
    else
        printf("Ticket ID already exists!\n");

    return root;
}

// Display (READ)
void display(struct Ticket* root) {
    if (root != NULL) {
        display(root->left);
        printf("ID: %d | Issue: %s | Status: %s\n",
               root->id, root->issue, root->status);
        display(root->right);
    }
}

// Search ticket
struct Ticket* search(struct Ticket* root, int id) {
    if (root == NULL || root->id == id)
        return root;

    if (id < root->id)
        return search(root->left, id);
    else
        return search(root->right, id);
}

// Update (UPDATE)
void update(struct Ticket* root, int id) {
    struct Ticket* t = search(root, id);

    if (t != NULL) {
        printf("Enter new issue: ");
        scanf(" %[^\n]", t->issue);

        printf("Enter new status: ");
        scanf(" %[^\n]", t->status);

        printf("✅ Ticket updated successfully!\n");
    } else {
        printf("❌ Ticket not found!\n");
    }
}

// Find minimum (helper for delete)
struct Ticket* findMin(struct Ticket* root) {
    while (root && root->left != NULL)
        root = root->left;
    return root;
}

// Delete (DELETE)
struct Ticket* deleteTicket(struct Ticket* root, int id) {
    if (root == NULL)
        return root;

    if (id < root->id)
        root->left = deleteTicket(root->left, id);
    else if (id > root->id)
        root->right = deleteTicket(root->right, id);
    else {
        // Node with one or no child
        if (root->left == NULL) {
            struct Ticket* temp = root->right;
            free(root);
            return temp;
        } else if (root->right == NULL) {
            struct Ticket* temp = root->left;
            free(root);
            return temp;
        }

        // Node with two children
        struct Ticket* temp = findMin(root->right);
        root->id = temp->id;
        strcpy(root->issue, temp->issue);
        strcpy(root->status, temp->status);
        root->right = deleteTicket(root->right, temp->id);
    }
    return root;
}

// Main Function
int main() {
    struct Ticket* root = NULL;
    int choice, id;
    char issue[100], status[50];

    while (1) {
        printf("\n--- Customer Support System ---\n");
        printf("1. Create Ticket\n");
        printf("2. Display Tickets\n");
        printf("3. Update Ticket\n");
        printf("4. Delete Ticket\n");
        printf("5. Search Ticket\n");
        printf("6. Exit\n");

        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Enter Ticket ID: ");
                scanf("%d", &id);

                printf("Enter Issue: ");
                scanf(" %[^\n]", issue);

                printf("Enter Status: ");
                scanf(" %[^\n]", status);

                root = insert(root, id, issue, status);
                break;

            case 2:
                printf("\n--- Ticket List ---\n");
                display(root);
                break;

            case 3:
                printf("Enter Ticket ID to update: ");
                scanf("%d", &id);
                update(root, id);
                break;

            case 4:
                printf("Enter Ticket ID to delete: ");
                scanf("%d", &id);
                root = deleteTicket(root, id);
                printf("🗑 Ticket deleted!\n");
                break;

            case 5:
                printf("Enter Ticket ID to search: ");
                scanf("%d", &id);
                struct Ticket* t = search(root, id);

                if (t != NULL)
                    printf("Found → ID: %d | Issue: %s | Status: %s\n",
                           t->id, t->issue, t->status);
                else
                    printf("❌ Ticket not found!\n");
                break;

            case 6:
                printf("Exiting...\n");
                exit(0);

            default:
                printf("Invalid choice!\n");
        }
    }
}
