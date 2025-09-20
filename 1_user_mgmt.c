#include "common.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ADMIN_USERNAME "admin"
#define ADMIN_PASSWORD "admin123"
#define ADMIN_ID 9999

int nextUserID = 1000;

void clearScreen()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}
void saveUsersToFile();

void pauseScreen()
{

    printf("Press Enter to return...");
    getchar();
    getchar();
}

void registerUser()
{
    User *newuser = (User *)malloc(sizeof(User));
    newuser->user_id = nextUserID++;
    if (!newuser)
    {
        printf("Memory allocation failed!\n");
        printf("Press Enter to return to main menu...");
        getchar();
        getchar();
        return;
    }
    clearScreen();
    printf("===== Register New User =====\n");
    printf("Enter full name(maximum 50 character): ");
    getchar();
    fgets(newuser->name, sizeof(newuser->name), stdin);
    newuser->name[strcspn(newuser->name, "\n")] = 0;

    printf("Enter username: (maximum 20 character)");
    scanf("%19s", newuser->username);

    User *temp = userHead;
    while (temp)
    {
        if (strcmp(temp->username, newuser->username) == 0)
        {
            printf("Username already exists! Try another.\n");
            free(newuser);
            printf("Press Enter to return to main menu...");
            getchar();
            getchar();
            return;
        }
        temp = temp->next;
    }

    printf("Enter password(using alphabet and numbers,not more than 20): ");
    scanf("%19s", newuser->password);

    printf("Enter license(max size 20 character): ");
    scanf("%29s", newuser->license);

    newuser->next = userHead;
    userHead = newuser;

    FILE *fp = fopen("users.txt", "a");
    if (fp == NULL)
    {
        printf("Error opening file to save user data!\n");
    }
    else
    {
        fprintf(fp, "%d|%s|%s|%s|%s\n", newuser->user_id, newuser->name, newuser->username, newuser->password, newuser->license);
        fclose(fp);
    }

    printf("Registration successful!\n");
    printf("Press Enter to return to main menu...");
    getchar();
    getchar();
}

void userMenu(User *);
void adminMenu();

void loginUser()
{
    int choice;
    char username[20], password[20];

    clearScreen();
    printf("====== Login ======\n");
    printf("1. User Login\n");
    printf("2. Admin Login\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);

    if (choice == 1)
    {
        printf("Enter username: ");
        scanf("%19s", username);
        printf("Enter password: ");
        scanf("%19s", password);

        User *temp = userHead;
        while (temp)
        {
            if (strcmp(temp->username, username) == 0 && strcmp(temp->password, password) == 0)
            {
                printf("User login successful! Welcome, %s\n", temp->name);
                printf("press enter to continue");
                getchar();
                getchar();
                userMenu(temp);
                return;
            }
            temp = temp->next;
        }
        printf("Invalid username or password.\n");
    }

    else if (choice == 2)
    {
        printf("Enter username: ");
        scanf("%19s", username);
        printf("Enter password: ");
        scanf("%19s", password);

        if (strcmp(username, ADMIN_USERNAME) == 0 && strcmp(password, ADMIN_PASSWORD) == 0)
        {
            printf("Admin login successful!\n");
            printf("**Welcome Admin\n");
            printf("press enter to continue....");
            getchar();
            getchar();
            adminMenu();
            return;
        }
        else
        {
            printf("Invalid admin credentials.\n");
        }
    }
    else
    {
        printf("Invalid choice.\n");
    }
    printf("Press Enter to return to main menu...");
    getchar();
    getchar();
}

void editUserProfile(User *);
void bookingMenu(int);
void returnMenu();

void userMenu(User *user)
{
    int choice;
    while (1)
    {
        clearScreen();
        printf("Welcome, %s!\n", user->name);
        printf("1. Edit Your Profile\n");
        printf("2. Book a Vehicle\n");
        printf("3. Return a Vehicle\n");
        printf("4. Logout\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            editUserProfile(user);
            break;
        case 2:
            bookingMenu(user->user_id);
            break;
        case 3:
            returnMenu(user->user_id);
            break;
        case 4:
            printf("Logging out....\n");
            return;
        default:
            printf("Invalid choice.\n");
        }
        printf("Press Enter to continue...");
        getchar();
        getchar();
    }
}

void viewUsers();
void vehicleMenu();

void adminMenu()
{
    int choice;
    while (1)
    {
        clearScreen();

        printf("===Admin Panel===\n");
        printf("Welcome, Admin!\n");
        printf("1. View Users\n");
        printf("2. Vehicle Management\n");
        printf("3. Booking System\n");
        printf("4. Return System\n");
        printf("5. Logout\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            viewUsers();
            break;
        case 2:
            vehicleMenu();
            break;
        case 3:
            bookingMenu(ADMIN_ID);
            break;
        case 4:
            returnMenu();
            break;
        case 5:
            printf("Logging out....");
            return;
        default:
            printf("Invalid choice.\n");
        }
        printf("Press Enter to continue...");
        getchar();
    }
}

void viewUsers()
{
    clearScreen();
    printf("==== Registered Users ====\n");

    if (!userHead)
    {
        printf("No users found.\n");
        pauseScreen();
        return;
    }

    User *temp = userHead;
    int index = 1;

    while (temp)
    {
        printf("%d. %s => |UserId: %d| |Username: %s| |License: %s|\n", index++, temp->name, temp->user_id, temp->username, temp->license);
        temp = temp->next;
    }

    int subchoice;
    printf("\n1. Delete User\n2. Edit User\n3. Back\n");
    printf("Enter your choice: ");
    scanf("%d", &subchoice);

    if (subchoice == 1)
    {
        int uid;
        printf("Enter the User ID to delete: ");
        scanf("%d", &uid);

        User *prev = NULL;
        temp = userHead;

        while (temp)
        {
            if (temp->user_id == uid)
            {
                if (prev)
                    prev->next = temp->next;
                else
                    userHead = temp->next;

                free(temp);
                saveUsersToFile();
                printf("User deleted successfully.\n");
                pauseScreen();
                return;
            }
            prev = temp;
            temp = temp->next;
        }
        printf("User ID not found.\n");
    }

    else if (subchoice == 2)
{
    int uid;
    printf("Enter the User ID to edit: ");
    scanf("%d", &uid);
    getchar(); // consume newline

    temp = userHead;
    int found = 0;

    while (temp)
    {
        if (temp->user_id == uid)
        {
            found = 1;
            char buffer[100];
            int opt;

            printf("Editing user: %s (ID: %d)\n", temp->name, temp->user_id);
            printf("1. Change Full Name\n");
            printf("2. Change Password\n");
            printf("3. Change License\n");
            printf("Enter your choice: ");
            scanf("%d", &opt);
            getchar(); // consume newline

            switch (opt)
            {
            case 1:
                printf("Enter new full name: ");
                fgets(buffer, sizeof(buffer), stdin);
                buffer[strcspn(buffer, "\n")] = 0;
                strcpy(temp->name, buffer);
                printf("Name updated.\n");
                break;
            case 2:
                printf("Enter new password: ");
                fgets(buffer, sizeof(buffer), stdin);
                buffer[strcspn(buffer, "\n")] = 0;
                strcpy(temp->password, buffer);
                printf("Password updated.\n");
                break;
            case 3:
                printf("Enter new license: ");
                fgets(buffer, sizeof(buffer), stdin);
                buffer[strcspn(buffer, "\n")] = 0;
                strcpy(temp->license, buffer);
                printf("License updated.\n");
                break;
            default:
                printf("Invalid option.\n");
            }

            saveUsersToFile();
            pauseScreen();
            return;  // IMPORTANT
        }

        temp = temp->next;
    }

    if (!found)
    {
        printf("User ID not found.\n");
        pauseScreen();
    }
}

    else if (subchoice == 3)
    {
        return;
    }
    else
    {
        printf("Invalid choice.\n");
    }

    pauseScreen();
}

void saveUsersToFile()
{
    FILE *fp = fopen("users.txt", "w");
    if (!fp)
    {
        printf("Error saving user data!\n");
        return;
    }

    User *temp = userHead;
    while (temp)
    {
        fprintf(fp, "%d|%s|%s|%s|%s\n", temp->user_id, temp->name, temp->username, temp->password, temp->license);

        temp = temp->next;
    }
    fclose(fp);
}

void loadUsersFromFile()
{
    // ✅ Step 1: Free previously loaded users to prevent duplicates
    User *current = userHead;
    while (current != NULL)
    {
        User *temp = current;
        current = current->next;
        free(temp);
    }
    userHead = NULL;

    // ✅ Step 2: Load from file
    FILE *fp = fopen("users.txt", "r");
    if (!fp)
    {
        return;
    }

    char line[200];
    while (fgets(line, sizeof(line), fp))
    {
        User *newuser = (User *)malloc(sizeof(User));
        if (!newuser)
        {
            printf("Memory allocation failed!\n");
            fclose(fp);
            return;
        }

        line[strcspn(line, "\n")] = 0;  // remove newline
        int id;
        sscanf(line, "%d|%49[^|]|%19[^|]|%19[^|]|%29[^\n]",
               &id, newuser->name, newuser->username, newuser->password, newuser->license);
        newuser->user_id = id;

        // ✅ Update the nextUserID tracker
        if (id >= nextUserID)
            nextUserID = id + 1;

        // ✅ Insert into the linked list
        newuser->next = userHead;
        userHead = newuser;
    }

    fclose(fp);
}



void editUserProfile(User *user)
{
    int choice;
    char buffer[100];

    while (1)
    {
        clearScreen();

        // Show current user details
        printf("=== Your Current Profile ===\n");
        printf("Full Name     : %s\n", user->name);
        printf("Username      : %s\n", user->username);
        printf("Password      : %s\n", user->password);  // (Optional: hide in real apps)
        printf("License No.   : %s\n", user->license);
        printf("============================\n\n");

        // Now show edit menu
        printf("=== Edit Profile ===\n");
        printf("1. Change Full Name\n");
        printf("2. Change Password\n");
        printf("3. Change License Number\n");
        printf("4. Back\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar();  // consume newline

        switch (choice)
        {
        case 1:
            printf("Enter new full name: ");
            fgets(buffer, sizeof(buffer), stdin);
            buffer[strcspn(buffer, "\n")] = 0;
            strcpy(user->name, buffer);
            printf("Name updated successfully.\n");
            break;
        case 2:
            printf("Enter new password: ");
            fgets(buffer, sizeof(buffer), stdin);
            buffer[strcspn(buffer, "\n")] = 0;
            strcpy(user->password, buffer);
            printf("Password updated successfully.\n");
            break;
        case 3:
            printf("Enter new license number: ");
            fgets(buffer, sizeof(buffer), stdin);
            buffer[strcspn(buffer, "\n")] = 0;
            strcpy(user->license, buffer);
            printf("License number updated successfully.\n");
            break;
        case 4:
            return;
        default:
            printf("Invalid choice.\n");
        }

        saveUsersToFile();

        printf("Press Enter to continue...");
        getchar();
    }
}

void viewFacilities()
{
    clearScreen();
    printf("=============== AVAILABLE FACILITIES ===============\n");
    printf("1.  Browse available two-wheelers (bikes, scooters)\n");
    printf("2.  Easy registration and secure login\n");
    printf("3.  License verification and data privacy\n");
    printf("4.  Book vehicles for flexible durations\n");
    printf("5.  Transparent hourly rental rates\n");
    printf("6.  Admin panel for managing bikes and bookings\n");
    printf("====================================================\n");
    printf("Press Enter to return to main menu...");
    getchar();
    getchar();
}