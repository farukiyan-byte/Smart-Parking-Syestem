#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#define MAX_SLOTS 10

typedef struct Vehicle {
    int number;
    char type[10]; // "car" or "scooter"
    int priority; // For heap
} Vehicle;

// Queue for waiting vehicles
typedef struct Node {
    Vehicle vehicle;
    struct Node* next;
} Node;

typedef struct Queue {
    Node* front;
    Node* rear;
} Queue;

// BST for tracking parked vehicles
typedef struct TreeNode {
    Vehicle vehicle;
    struct TreeNode* left;
    struct TreeNode* right;
} TreeNode;

Vehicle* heap[MAX_SLOTS]; // Max heap (priority queue) for parking
int heapSize = 0;
Queue waitingQueue = {NULL, NULL};
TreeNode* root = NULL;

int totalVehicles = 0;
int carCount = 0;
int scooterCount = 0;

// Login system
void login() {
    char user[10], pass[10];
    int attempt = 0;
    while (attempt < 3) {
        
        printf("\n\t\t\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\n");
        printf("\t\t\xdb                                               \xdb\n");
        printf("\t\t\xdb       =============================           \xdb\n");
        printf("\t\t\xdb             Smart Parking Syestem             \xdb\n");
        printf("\t\t\xdb       =============================           \xdb\n");
        printf("\t\t\xdb                                               \xdb\n");
        printf("\t\t\xdb                                               \xdb\n");
        printf("\t\t\xdb                                               \xdb\n");
        printf("\t\t\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\n\n\n");
        
        printf("Press Any Key To Continue:\n");
        system("pause");  // Pauses and waits for user input
        
        printf("Username: ");
        scanf("%s", user);
        printf("Password: ");
        scanf("%s", pass);

        if (strcmp(user, "admin") == 0 && strcmp(pass, "admin") == 0) {
            printf("Login Successful!\n");
            system("pause");
            return;
        } else {
            printf("Invalid credentials.\n");
            attempt++;
        }
    }
    printf("Too many failed attempts. Exiting.\n");
    exit(0);
}

// Heap functions
void swap(Vehicle** a, Vehicle** b) {
    Vehicle* temp = *a;
    *a = *b;
    *b = temp;
}

void heapifyUp(int i) {
    while (i > 0 && heap[i]->priority > heap[(i - 1) / 2]->priority) {
        swap(&heap[i], &heap[(i - 1) / 2]);
        i = (i - 1) / 2;
    }
}

void heapifyDown(int i) {
    int left = 2 * i + 1;
    int right = 2 * i + 2;
    int largest = i;
    if (left < heapSize && heap[left]->priority > heap[largest]->priority)
        largest = left;
    if (right < heapSize && heap[right]->priority > heap[largest]->priority)
        largest = right;
    if (largest != i) {
        swap(&heap[i], &heap[largest]);
        heapifyDown(largest);
    }
}

void insertHeap(Vehicle* v) {
    if (heapSize == MAX_SLOTS) {
        printf("Parking Full! Adding to waiting queue.\n");
        Node* temp = (Node*)malloc(sizeof(Node));
        temp->vehicle = *v;
        temp->next = NULL;
        if (!waitingQueue.rear) {
            waitingQueue.front = waitingQueue.rear = temp;
        } else {
            waitingQueue.rear->next = temp;
            waitingQueue.rear = temp;
        }
        return;
    }
    heap[heapSize] = v;
    heapifyUp(heapSize++);
}

// BST functions
TreeNode* insertBST(TreeNode* root, Vehicle* v) {
    if (!root) {
        TreeNode* newNode = (TreeNode*)malloc(sizeof(TreeNode));
        newNode->vehicle = *v;
        newNode->left = newNode->right = NULL;
        return newNode;
    }
    if (v->number < root->vehicle.number)
        root->left = insertBST(root->left, v);
    else
        root->right = insertBST(root->right, v);
    return root;
}

TreeNode* searchBST(TreeNode* root, int number) {
    if (!root || root->vehicle.number == number) return root;
    if (number < root->vehicle.number)
        return searchBST(root->left, number);
    else
        return searchBST(root->right, number);
}

void inorderBST(TreeNode* root) {
    if (root) {
        inorderBST(root->left);
        printf("Vehicle No: %d, Type: %s, Priority: %d\n", root->vehicle.number, root->vehicle.type, root->vehicle.priority);
        inorderBST(root->right);
    }
}

// Add vehicle
void addVehicle() {
    Vehicle* v = (Vehicle*)malloc(sizeof(Vehicle));
    printf("Enter Vehicle Number: ");
    scanf("%d", &v->number);
    printf("Enter Vehicle Type (car/scooter): ");
    scanf("%s", v->type);
    printf("Enter Priority (1-10): ");
    scanf("%d", &v->priority);

    if (strcmp(v->type, "car") == 0) carCount++;
    else if (strcmp(v->type, "scooter") == 0) scooterCount++;
    totalVehicles++;

    insertHeap(v);
    root = insertBST(root, v);
    printf("Vehicle added successfully.\n");
    system("pause");
}

// Remove vehicle by number
void removeVehicle() {
    int num;
    printf("Enter vehicle number to remove: ");
    scanf("%d", &num);

    int found = 0;
    for (int i = 0; i < heapSize; i++) {
        if (heap[i]->number == num) {
            found = 1;
            printf(" Successfully Remove Vehicle No: %d\n", heap[i]->number);
            if (strcmp(heap[i]->type, "car") == 0) carCount--;
            else if (strcmp(heap[i]->type, "scooter") == 0) scooterCount--;
            totalVehicles--;

            free(heap[i]);
            heap[i] = heap[--heapSize];
            heapifyDown(i);

            if (waitingQueue.front) {
                Vehicle* fromQueue = (Vehicle*)malloc(sizeof(Vehicle));
                *fromQueue = waitingQueue.front->vehicle;
                insertHeap(fromQueue);

                Node* temp = waitingQueue.front;
                waitingQueue.front = waitingQueue.front->next;
                if (!waitingQueue.front) waitingQueue.rear = NULL;
                free(temp);
            }
            break;
        }
    }
    if (!found) printf("Vehicle not found in parking.\n");
    system("pause");
}

// Display all vehicles in parking (in BST)
void displayParking() {
    printf("Currently Parked Vehicles (In-order BST Traversal):\n");
    inorderBST(root);
    system("pause");
}

// Display all stats
void showStats() {
    printf("Total Vehicles: %d\n", totalVehicles);
    printf("Cars: %d\n", carCount);
    printf("Scooters: %d\n", scooterCount);
    printf("Waiting Queue: ");
    Node* temp = waitingQueue.front;
    while (temp) {
        printf("%d -> ", temp->vehicle.number);
        temp = temp->next;
    }
    printf("NULL\n");
    system("pause");
}

int main() {
    login();
    int choice;
    do {
        system("cls");
        printf("Smart Parking System\n");
        printf("1. Add Vehicle\n");
        printf("2. Remove Vehicle\n");
        printf("3. Show All Parked Vehicles\n");
        printf("4. Show Stats\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        switch (choice) {
            case 1: addVehicle(); break;
            case 2: removeVehicle(); break;
            case 3: displayParking(); break;
            case 4: showStats(); break;
            case 5: printf("Exiting...\n"); break;
            default: printf("Invalid choice!\n"); system("pause");
        }
    } while (choice != 5);
    return 0;
}
