#include "common.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define BOOKINGS_FILE "bookings.txt"
#define VEHICLE_FILE "vehicles.txt"
#define FINE_PER_DAY 50

void loadVehicles()
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
        }
        else
        {
            free(newNode);
        }
    }
    fclose(fp);
}

void saveVehicles(Vehicle *head)
{
    FILE *fp = fopen(VEHICLE_FILE, "w");
    if (!fp)
    {
        printf("Error: couldn't open %s\n", VEHICLE_FILE);
        return;
    }
    while (head)
    {
        fprintf(fp, "%d %s %s %.2f %d\n", head->v_id, head->name, head->type, head->price, head->available);
        head = head->next;
    }
    fclose(fp);
}

void bookVehicle(int user_id)
{
    FILE *bf = fopen("bookings.txt", "a");
    if (!bf)
    {
        printf("Error opening bookings file.\n");
        return;
    }

    int id;
    printf("\n--- Available Vehicles ---\n");
    Vehicle *temp = vehicleHead;
    while (temp)
    {
        if (temp->available == 1)
        {
            printf("ID: %d | Name: %s | Type: %s | Price: %.2f\n",
                   temp->v_id, temp->name, temp->type, temp->price);
        }
        temp = temp->next;
    }

    printf("Enter Vehicle ID to book: ");
    scanf("%d", &id);
    temp = vehicleHead;
    while (temp)
    {
        if (temp->v_id == id && temp->available == 1)
        {
            char expectedReturnDate[20];
            printf("Enter expected return date (YYYY-MM-DD): ");
            scanf("%s", expectedReturnDate);

            fprintf(bf, "%d %s %s %.2f %d Rented %s\n", temp->v_id, temp->name, temp->type, temp->price, user_id, expectedReturnDate);

            temp->available = 0;
            saveVehicles(vehicleHead);
            fclose(bf);
            printf("Vehicle booked successfully by user ID %d.\n", user_id);
            return;
        }
        temp = temp->next;
    }

    printf("Invalid vehicle ID or not available.\n");
    fclose(bf);
}

int daysLate(const char *expectedDateStr) {
    struct tm expectedDate = {0};
    time_t now = time(NULL);
    sscanf(expectedDateStr, "%d-%d-%d", &expectedDate.tm_year, &expectedDate.tm_mon, &expectedDate.tm_mday);
    expectedDate.tm_year -= 1900;
    expectedDate.tm_mon -= 1;
    time_t expected = mktime(&expectedDate);
    double seconds = difftime(now, expected);
    return seconds > 0 ? (int)(seconds / (60 * 60 * 24)) : 0;
}

void checkOverdueBookings() {
    FILE *fp = fopen("bookings.txt", "r");
    if (!fp) return;

    int v_id, user_id;
    char name[50], type[20], status[10], date[20];
    float price;

    printf("\n--- Overdue Bookings ---\n");

    while (fscanf(fp, "%d %s %s %f %d %s %s", &v_id, name, type, &price, &user_id, status, date) == 7) {
        if (strcmp(status, "Rented") == 0) {
            int lateDays = daysLate(date);
            if (lateDays > 0) {
                int fine = lateDays * FINE_PER_DAY;
                printf("Vehicle ID: %d | Name: %s | User ID: %d | Expected: %s | Late: %d days | Fine: Rs %d\n",
                       v_id, name, user_id, date, lateDays, fine);
            }
        }
    }

    fclose(fp);
}

void bookingMenu(int user_id) {
    int choice;
    loadVehicles();

    do {
        printf("\n--- Booking System Menu ---\n");
        printf("1. Book a Vehicle\n");
        printf("2. Display All Vehicles\n");
        printf("3. Check Overdue Bookings\n");
        printf("4. Exit to Main Menu\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
        case 1:
            bookVehicle(user_id);
            break;
        case 2:
            displayVehicles();
            break;
        case 3:
            checkOverdueBookings();
            break;
        case 4:
            printf("Returning to main menu...\n");
            break;
        default:
            printf("Invalid choice.\n");
        }
    } while (choice != 4);
}