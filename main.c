/***************************************************************
 * Author       : SUMIT KUMAR PRADHAN
 * Reg No       : 25017_150
 * Project Name : Address Book
 * File Name    : main.c
 * Start Date   : 12/08/2025
 * End Date     : 30/08/2025
 * Description  : Entry point of the Address Book application. 
 *                Displays menu and handles user interaction.
 ***************************************************************/
#include <stdio.h>
#include "contact.h"

// ANSI escape codes for colored text
#define RED     "\033[1;31m"
#define GREEN   "\033[1;32m"
#define YELLOW  "\033[1;33m"
#define CYAN    "\033[1;36m"
#define RESET   "\033[0m"

int main()
{
    int option;  // variable to store menu choice
    struct Address_book addressbook;  
    addressbook.contact_count = 0;  // initialize contact count

    // Load contacts from database.txt
    if(!load_contacts(&addressbook)) {
        printf(RED "Error: Failed to load existing contact\n" RESET);
        return 0;  // exit if loading failed
    } else {
        printf(GREEN "Contacts loaded successfully\n" RESET);
    }

    // Main menu loop
    while (1) {
        // Display menu
        printf(CYAN "\n======= Address Book Menu =======\n" RESET);
        printf(YELLOW "1. Add contact\n" RESET);
        printf(YELLOW "2. Delete contact\n" RESET);
        printf(YELLOW "3. Edit contact\n" RESET);
        printf(YELLOW "4. Search contact\n" RESET);
        printf(YELLOW "5. Display contact\n" RESET);
        printf(YELLOW "6. Save contact\n" RESET);
        printf(YELLOW "7. Exit\n" RESET);
        printf(CYAN "Enter your option: " RESET);
        scanf("%d", &option);  // read user input

        // Perform action based on user choice
        switch (option) {
        case 1:
            create_contact(&addressbook);  // add a new contact
            break;
        case 2:
            printf(CYAN "\nDelete Contact Menu:\n" RESET);
            delete_contact(&addressbook);  // delete contact
            break;
        case 3:
            printf(CYAN "\nEdit Contact Menu:\n" RESET);
            edit_contact(&addressbook);  // edit existing contact
            break;
        case 4:
            printf(CYAN "\nSearch Contact Menu:\n" RESET);
            search_contacts(&addressbook);  // search contact
            break;
        case 5:
            printf(CYAN "\n--- Contact List ---\n" RESET);
            list_contacts(&addressbook);  // list all contacts
            break;
        case 6:
            printf(GREEN "Saving contacts...\n" RESET);
            save_contacts(&addressbook);  // save contacts to file
            break;
        case 7:
            save_contacts(&addressbook);  // save before exit
            printf(GREEN "INFO: Save and Exit...\n" RESET);
            return 0;  // exit program
        default:
            printf(RED "Invalid option! Please try again.\n" RESET);
        }
    }
}
