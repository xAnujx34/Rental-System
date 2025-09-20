#ifndef COMMON_H
#define COMMON_H

void displayVehicles();
void loadUsersFromFile();
void registerUser();
void loginUser();
void viewFacilities();
void clearScreen();
void pauseScreen();

typedef struct Vehicle {
    int v_id;
    char name[50];
    char type[20];
    float price;
    int available; // 1 = Available, 0 = Rented, 2 = Maintenance
    struct Vehicle* next;
} Vehicle;

typedef struct User {
    int user_id;
    char name[50];
    char username[20];
    char password[20];
    char license[30];
    struct User* next;
} User;

typedef struct ReturnStack {
    int v_id;
    char name[50];
    char returnDate[20];
    struct ReturnStack* next;
} ReturnStack;

extern User* userHead;
extern Vehicle* vehicleHead;

#endif
