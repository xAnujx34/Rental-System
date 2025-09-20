#include "common.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define VEHICLE_FILE "vehicles.txt"

int nextVehicleID = 100;

void saveVehiclesToFile()
{
    FILE *fp = fopen(VEHICLE_FILE, "w");
    if (!fp)
        return;

    Vehicle *temp = vehicleHead;
    while (temp)
    {
        fprintf(fp, "%d %s %s %.2f %d\n",
                temp->v_id, temp->name, temp->type, temp->price, temp->available);
        temp = temp->next;
    }
    fclose(fp);
}

void loadVehiclesFromFile()
{
    FILE *fp = fopen(VEHICLE_FILE, "r");
    if (!fp)
        return;
    Vehicle *newNode;
    while (!feof(fp))
    {
        newNode = (Vehicle *)malloc(sizeof(Vehicle));
        if (fscanf(fp, "%d %s %s %f %d", &newNode->v_id, newNode->name, newNode->type, &newNode->price, &newNode->available) == 5)
        {
            newNode->next = vehicleHead;
            vehicleHead = newNode;
            if (newNode->v_id >= nextVehicleID)
                nextVehicleID = newNode->v_id + 1;
        }
        else
        {
            free(newNode);
        }
    }
    fclose(fp);
}

void addVehicle()
{
    Vehicle *newVehicle = (Vehicle *)malloc(sizeof(Vehicle));
    newVehicle->v_id = nextVehicleID++;
    printf("Enter vehicle name: ");
    scanf("%s", newVehicle->name);
    printf("Enter vehicle type: ");
    scanf("%s", newVehicle->type);
    printf("Enter vehicle price: ");
    scanf("%f", &newVehicle->price);
    newVehicle->available = 1;
    newVehicle->next = vehicleHead;
    vehicleHead = newVehicle;
    saveVehiclesToFile();
    printf("Vehicle added successfully with ID: %d\n", newVehicle->v_id);
}

void removeVehicle()
{
    int id;
    printf("Enter vehicle ID to remove: ");
    scanf("%d", &id);
    Vehicle *temp = vehicleHead, *prev = NULL;
    while (temp && temp->v_id != id)
    {
        prev = temp;
        temp = temp->next;
    }
    if (!temp)
    {
        printf("Vehicle ID not found.\n");
        return;
    }
    if (!prev)
        vehicleHead = temp->next;
    else
        prev->next = temp->next;
    free(temp);
    saveVehiclesToFile();
    printf("Vehicle removed successfully.\n");
}

void displayVehicles()
{
    Vehicle *current = vehicleHead;

    if (!current)
    {
        printf("No vehicles available.\n");
        return;
    }

    printf("\n--- Vehicle List ---\n");
    while (current)
    {
        printf("ID: %d | Name: %s | Type: %s | Price: %.2f | Status: %s\n",
               current->v_id, current->name, current->type, current->price,
               current->available == 1 ? "Available" : current->available == 0 ? "Rented"
                                                                               : "Maintenance");
        current = current->next;
    }
}

void updateAvailability()
{
    int id, status;
    Vehicle *current = vehicleHead;

    printf("Enter vehicle ID: ");
    scanf("%d", &id);
    printf("Enter new status (1 = Available, 0 = Rented, 2 = Maintenance): ");
    scanf("%d", &status);

    while (current)
    {
        if (current->v_id == id)
        {
            current->available = status;
            saveVehiclesToFile();
            printf("Availability updated.\n");
            return;
        }
        current = current->next;
    }
    printf("Vehicle not found.\n");
}

void vehicleMenu()
{
    int choice;
    Vehicle *temp;
    while (vehicleHead)
    {
        temp = vehicleHead;
        vehicleHead = vehicleHead->next;
        free(temp);
    }
    loadVehiclesFromFile();
    do
    {
        printf("\n--- Vehicle Management ---\n");
        printf("1. Add Vehicle\n");
        printf("2. Remove Vehicle\n");
        printf("3. Update Availability\n");
        printf("4. Display Vehicles\n");
        printf("5. Exit to Main Menu\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            addVehicle();
            break;
        case 2:
            removeVehicle();
            break;
        case 3:
            updateAvailability();
            break;
        case 4:
            displayVehicles();
            break;
        case 5:
            printf("Returning to main menu...\n");
            break;
        default:
            printf("Invalid choice.\n");
            break;
        }
    } while (choice != 5);
}
