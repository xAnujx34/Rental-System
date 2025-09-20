#include "common.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

ReturnStack *top = NULL;

void push(int v_id, char name[], char date[])
{
    ReturnStack *new = (ReturnStack *)malloc(sizeof(ReturnStack));
    new->v_id = v_id;
    strcpy(new->name, name);
    strcpy(new->returnDate, date);
    new->next = top;
    top = new;
}

void displayRecentReturns()
{
    if (top == NULL)
    {
        printf("No recent returns.\n");
        return;
    }
    printf("\nRecently Returned Vehicles:\n");
    ReturnStack *temp = top;
    while (temp != NULL)
    {
        printf("ID: %d, Name: %s, Date: %s\n", temp->v_id, temp->name, temp->returnDate);
        temp = temp->next;
    }
}

void displayRentedVehicles(int currentUserId)
{
    FILE *fp = fopen("bookings.txt", "r");
    if (fp == NULL)
    {
        printf("Error: Could not open bookings.txt\n");
        return;
    }
    int vehicleID, userID;
    char name[50], type[20], status[10];
    float price;
    int found = 0;
    printf("\n--- Rented Vehicles ---\n");
    char expectedReturnDate[20]; 

    while (fscanf(fp, "%d %s %s %f %d %s %s",
                  &vehicleID, name, type, &price, &userID, status, expectedReturnDate) == 7)

    {
        if (strcmp(status, "Rented") == 0 && userID == currentUserId)
        {
            printf("Vehicle ID: %d, Name: %s, Type: %s, Price: %.2f, Rented By User %d\n",
                   vehicleID, name, type, price, userID);
            found = 1;
        }
    }
    if (!found)
    {
        printf("No vehicles are currently rented.\n");
    }
    fclose(fp);
}

void returnVehicle(int currentUserId)
{
    int id;
    char condition, returnDate[20];

    displayRentedVehicles(currentUserId);

    printf("Enter Vehicle ID to return: ");
    scanf("%d", &id);

    FILE *inFile = fopen("bookings.txt", "r");
    FILE *tempFile = fopen("temp.txt", "w");

    if (inFile == NULL || tempFile == NULL)
    {
        printf("Error: Unable to open bookings file.\n");
        return;
    }

    int vehicleID, userID, found = 0;
    char name[50], type[20], status[10], expectedReturnDate[20];
    float price;

    while (fscanf(inFile, "%d %s %s %f %d %s %s", &vehicleID, name, type, &price, &userID, status, expectedReturnDate) == 7)
    {
        if (vehicleID == id && userID == currentUserId && strcmp(status, "Rented") == 0)
        {
            found = 1;

            printf("Enter actual return date (YYYY-MM-DD): ");
            scanf("%s", returnDate);

            printf("Is the vehicle in good condition? (Y/N): ");
            scanf(" %c", &condition);

            FILE *retFile = fopen("returns.txt", "a");
            if (retFile != NULL)
            {
                fprintf(retFile, "Vehicle ID: %d, Name: %s, Actual Date: %s, Expected: %s, Condition: %s\n",
                        vehicleID, name, returnDate, expectedReturnDate,
                        (condition == 'N' || condition == 'n') ? "Bad" : "Good");
                fclose(retFile);
            }

            push(vehicleID, name, returnDate);

            if (condition == 'N' || condition == 'n')
            {
                FILE *mFile = fopen("maintenance.txt", "a");
                if (mFile != NULL)
                {
                    fprintf(mFile, "Vehicle ID: %d, Name: %s, Return Date: %s\n",
                            vehicleID, name, returnDate);
                    fclose(mFile);
                }
            }

            // Update line with Returned status and actual return date
            fprintf(tempFile, "%d %s %s %.2f %d Returned %s\n",
                    vehicleID, name, type, price, userID, returnDate);

            printf("Vehicle ID %d returned by user ID %d on %s.\n", vehicleID, userID, returnDate);
        }
        else
        {
            // Preserve unchanged booking entry
            fprintf(tempFile, "%d %s %s %.2f %d %s %s\n",
                    vehicleID, name, type, price, userID, status, expectedReturnDate);
        }
    }

    fclose(inFile);
    fclose(tempFile);

    remove("bookings.txt");
    rename("temp.txt", "bookings.txt");

    if (!found)
    {
        printf("Vehicle not found or not currently rented.\n");
    }
}


void returnMenu(int currentUserId)
{
    int opt;
    do
    {
        printf("\n--- Return System Menu ---\n");
        printf("1. Return Vehicle\n");
        printf("2. View Recently Returned Vehicles\n");
        printf("3. Exit\n");
        printf("Enter an option: ");
        scanf("%d", &opt);

        switch (opt)
        {
        case 1:
            returnVehicle(currentUserId);
            break;
        case 2:
            displayRecentReturns(currentUserId);
            break;
        case 3:
            printf("Exiting.....\n");
            break;
        default:
            printf("Invalid choice. Try again.\n");
            break;
        }
    } while (opt != 3);
    return;
}