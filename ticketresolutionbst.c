#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Ticket structure (BST Node)
struct Ticket {
    int id;
    char name[50];
    char issue[100];
    struct Ticket *left, *right;
};

// Create new ticket node
struct Ticket* createTicket(int id, char name[], char issue[]) {
    struct Ticket* newNode = (struct Ticket*)malloc(sizeof(struct Ticket));
    newNode->id = id;
    strcpy(newNode->name, name);
    strcpy(newNode->issue, issue);
    newNode->left = newNode->right = NULL;
    return newNode;
}

// Insert into BST
struct Ticket* insert(struct Ticket* root, int id, char name[], char issue[]) {
    if (root == NULL)
        return createTicket(id, name, issue);

    if (id < root->id)
        root->left = insert(root->left, id, name, issue);
    else if (id > root->id)
        root->right = insert(root->right, id, name, issue);

    return root;
}

// Inorder traversal (Display sorted)
void display(struct Ticket* root) {
    if (root != NULL) {
        display(root->left);
        printf("\nID: %d\nName: %s\nIssue: %s\n", root->id, root->name, root->issue);
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

// Find minimum node
struct Ticket* findMin(struct Ticket* root) {
    while (root->left != NULL)
        root = root->left;
    return root;
}

// Delete ticket
struct Ticket* deleteTicket(struct Ticket* root, int id) {
    if (root == NULL) return root;

    if (id < root->id)
        root->left = deleteTicket(root->left, id);
    else if (id > root->id)
        root->right = deleteTicket(root->right, id);
    else {
        if (root->left == NULL) {
            struct Ticket* temp = root->right;
            free(root);
            return temp;
        } else if (root->right == NULL) {
            struct Ticket* temp = root->left;
            free(root);
            return temp;
        }

        struct Ticket* temp = findMin(root->right);
        root->id = temp->id;
        strcpy(root->name, temp->name);
        strcpy(root->issue, temp->issue);

        root->right = deleteTicket(root->right, temp->id);
    }
    return root;
}

// Update ticket
void update(struct Ticket* root, int id) {
    struct Ticket* t = search(root, id);
    if (t == NULL) {
        printf("Ticket not found!\n");
        return;
    }

    printf("Enter new name: ");
    scanf(" %[^\n]", t->name);
    printf("Enter new issue: ");
    scanf(" %[^\n]", t->issue);

    printf("Ticket updated successfully!\n");
}

// Save to file
void saveToFile(struct Ticket* root, FILE* file) {
    if (root != NULL) {
        saveToFile(root->left, file);
        fprintf(file, "%d|%s|%s\n", root->id, root->name, root->issue);
        saveToFile(root->right, file);
    }
}

// Load from file
struct Ticket* loadFromFile(struct Ticket* root) {
    FILE* file = fopen("tickets.txt", "r");
    if (!file) return root;

    int id;
    char name[50], issue[100];

    while (fscanf(file, "%d|%[^|]|%[^\n]\n", &id, name, issue) != EOF) {
        root = insert(root, id, name, issue);
    }

    fclose(file);
    return root;
}

// Main menu
int main() {
    struct Ticket* root = NULL;
    int choice, id;
    char name[50], issue[100];

    root = loadFromFile(root);

    do {
        printf("\n--- Customer Support System ---\n");
        printf("1. Create Ticket\n2. Display Tickets\n3. Search Ticket\n4. Update Ticket\n5. Delete Ticket\n6. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Enter ID: ");
                scanf("%d", &id);
                printf("Enter Name: ");
                scanf(" %[^\n]", name);
                printf("Enter Issue: ");
                scanf(" %[^\n]", issue);
                root = insert(root, id, name, issue);
                break;

            case 2:
                display(root);
                break;

            case 3:
                printf("Enter ID to search: ");
                scanf("%d", &id);
                struct Ticket* t = search(root, id);
                if (t)
                    printf("Found → Name: %s | Issue: %s\n", t->name, t->issue);
                else
                    printf("Ticket not found!\n");
                break;

            case 4:
                printf("Enter ID to update: ");
                scanf("%d", &id);
                update(root, id);
                break;

            case 5:
                printf("Enter ID to delete: ");
                scanf("%d", &id);
                root = deleteTicket(root, id);
                break;

            case 6: {
                FILE* file = fopen("tickets.txt", "w");
                saveToFile(root, file);
                fclose(file);
                printf("Data saved. Exiting...\n");
                break;
            }

            default:
                printf("Invalid choice!\n");
        }

    } while (choice != 6);

    return 0;
}