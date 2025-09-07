/***************************************************************
 * Name        : SUMIT KUMAR PRADHAN
 * Reg No      : 25017_150
 * Project     : Address Book
 * File Name   : contact.c
 * Start Date  : 12/08/2025
 * End Date    : 30/08/2025
 * Description : This file implements all core functionalities of 
 *               the address book, including adding, editing, deleting, 
 *               validating, and searching contacts.
 ***************************************************************/

#include <stdio.h>      // Standard I/O functions
#include <string.h>     // For string handling
#include <ctype.h>      // For character checks (isalpha, isdigit, etc.)
#include "contact.h"    // Header file with structures and declarations

// ===== ANSI escape codes for colored output =====
#define RED     "\033[1;31m"   // Red text
#define GREEN   "\033[1;32m"   // Green text
#define YELLOW  "\033[1;33m"   // Yellow text
#define CYAN    "\033[1;36m"   // Cyan text
#define RESET   "\033[0m"      // Reset color

//================ VALIDATION FUNCTIONS =================//
// Validate Name (must not be empty and contain only alphabets/spaces)
int validate_name(const char *name)
 {
    if (strlen(name) == 0) return 0; // Reject empty name
    for (int i = 0; name[i]!='\0'; i++) 
    {
        if (!isalpha(name[i]) && name[i] != ' ') return 0; // Invalid char
    }
    return 1; // Valid name
}

// Validate Phone (10 digits, not starting with 0, unique in addressbook)
int validate_phone(const char *phone, struct Address_book *addressbook) 
{
    if (strlen(phone) != 10) return 0; // Must be 10 digits
    for (int i = 0; phone[i]!='\0'; i++)
    {
        if (!isdigit(phone[i]) || phone[0]=='0') return 0; // Invalid phone
    }
    for (int i = 0; i < addressbook->contact_count; i++) 
    {
        if (strcmp(phone, addressbook->contact_details[i].Mobile_number) == 0)
            return -1; // Duplicate phone
    }
    return 1; // Valid phone
}

// Validate Email (check .com, only valid chars, one '@', unique)
int validate_email(const char *email, struct Address_book *addressbook) 
{
    int at_count = 0, len = strlen(email);
    if (len < 5 || strcmp(email + len - 4, ".com") != 0) return 0; // Must end with .com
    if (!isalpha(email[0])) return 0; // First char must be alphabet

    for (int i = 0; i < len; i++) {
        unsigned char ch = email[i];
        if (!(islower(ch) || isdigit(ch) || ch == '@' || ch == '.')) return 0; // Invalid char
        if (ch == '@') at_count++; // Count '@'
    }
    if (at_count != 1) return 0; // Must have one '@'

    for (int i = 0; i < addressbook->contact_count; i++) {
        if (strcmp(email, addressbook->contact_details[i].Mail_ID) == 0) return -1; // Duplicate email
    }
    return 1; // Valid email
}

//================ SEARCH FUNCTIONS =================//
// Search contact by Name
void search_by_name(struct Address_book *addressbook) 
{
    char name[32]; int found = 0;
    printf(CYAN "Enter name to search: " RESET);
    scanf(" %[^\n]", name); // Read input with spaces

    for (int i = 0; i < addressbook->contact_count; i++) 
    {
        if (strstr(addressbook->contact_details[i].Name, name) != NULL) { // Partial match
            if (found == 0) printf(GREEN "\n--- Contacts Found ---\n" RESET);
            printf(YELLOW "\nContact %d:\n" RESET, found + 1);
            printf("Name  : %s\n", addressbook->contact_details[i].Name);
            printf("Phone : %s\n", addressbook->contact_details[i].Mobile_number);
            printf("Email : %s\n", addressbook->contact_details[i].Mail_ID);
            found++; // Count match
        }
    }
    if (!found)
    {
     printf(RED "Contact not found!\n" RESET);
    }
    else
    {
         printf(CYAN "\nTotal %d contact(s) found containing \"%s\".\n" RESET, found, name);
    }
}
// Search contact by Phone
void search_by_phone(struct Address_book *addressbook) 
{
    char phone[11];
    printf(CYAN "Enter Phone Number to search: " RESET);
    scanf("%s", phone);

    for (int i = 0; i < addressbook->contact_count; i++)
     {
        if (strcmp(addressbook->contact_details[i].Mobile_number, phone) == 0) 
        { // Match found
            printf(GREEN "\nContact Found!\n" RESET);
            printf("Name: %s\n", addressbook->contact_details[i].Name);
            printf("Phone: %s\n", addressbook->contact_details[i].Mobile_number);
            printf("Email: %s\n", addressbook->contact_details[i].Mail_ID);
            return; // Stop after first match
        }
    }
    printf(RED "Contact not found.\n" RESET);
}
// Search contact by Email
void search_by_email(struct Address_book *addressbook) {
    char email[35];
    printf(CYAN "Enter Email to search: " RESET);
    scanf("%s", email);

    for (int i = 0; i < addressbook->contact_count; i++) 
    {
        if (strcmp(addressbook->contact_details[i].Mail_ID, email) == 0) 
        { // Match found
            printf(GREEN "\nContact Found!\n" RESET);
            printf("Name: %s\n", addressbook->contact_details[i].Name);
            printf("Phone: %s\n", addressbook->contact_details[i].Mobile_number);
            printf("Email: %s\n", addressbook->contact_details[i].Mail_ID);
            return;
        }
    }
    printf(RED "Contact not found.\n" RESET);
}
//================ EDIT FUNCTIONS =================//
// Edit by Name (ask phone/email if duplicates)
void edit_by_name(struct Address_book *addressbook) {
    char name[32]; int found = 0, matchIndexes[100];
    printf(CYAN "Enter name to edit: " RESET);
    scanf(" %[^\n]", name);

    for (int i = 0; i < addressbook->contact_count; i++) 
    {
        if (strstr(addressbook->contact_details[i].Name, name) != NULL) 
        { // Partial match
            if (found == 0) printf(GREEN "\n--- Matching Contacts ---\n" RESET);
            printf(YELLOW "\nContact %d:\n" RESET, found + 1);
            printf("Name  : %s\n", addressbook->contact_details[i].Name);
            printf("Phone : %s\n", addressbook->contact_details[i].Mobile_number);
            printf("Email : %s\n", addressbook->contact_details[i].Mail_ID);
            matchIndexes[found] = i; // Save index
            found++;
        }
    }

    if (!found) 
    { 
        printf(RED "No contact found with name: %s\n" RESET, name); return; 
    }

    char input[50];
    printf(CYAN "\nEnter phone or email of the contact you want to edit: " RESET);
    scanf(" %[^\n]", input);

    for (int j = 0; j < found; j++) 
    {
        int i = matchIndexes[j];
        if (strcmp(addressbook->contact_details[i].Mobile_number, input) == 0 ||
            strcmp(addressbook->contact_details[i].Mail_ID, input) == 0) 
            { // Confirm unique match
            
            printf(GREEN "\nEditing Contact:\n" RESET);
            printf("Current -> Name: %s, Phone: %s, Email: %s\n",
                   addressbook->contact_details[i].Name,
                   addressbook->contact_details[i].Mobile_number,
                   addressbook->contact_details[i].Mail_ID);

            // Edit Name
            char new_name[32];
            printf(CYAN "\nEnter new name (leave empty to keep unchanged): " RESET);
            scanf(" %[^\n]", new_name);
            if (strlen(new_name) > 0 && validate_name(new_name)) 
            {
                strcpy(addressbook->contact_details[i].Name, new_name); // Update name
            }

            // Ask if phone edit needed
            char choice;
            printf(CYAN "Do you want to edit phone? (y/n): " RESET);
            scanf(" %c", &choice);
            if (choice == 'y' || choice == 'Y')
             {
                char new_phone[11];
                do {
                    printf(CYAN "Enter new phone: " RESET);
                    scanf(" %[^\n]", new_phone);
                    int status = validate_phone(new_phone, addressbook);
                    if (status == 0) printf(RED "Invalid phone.\n" RESET);
                    else if (status == -1) 
                    {
                    printf(RED "Phone already exists.\n" RESET);
                    }
                    else 
                    { 
                        strcpy(addressbook->contact_details[i].Mobile_number, new_phone);
                         break; 
                    }
                } while (1);
            }

            // Ask if email edit needed
            printf(CYAN "Do you want to edit email? (y/n): " RESET);
            scanf(" %c", &choice);
            if (choice == 'y' || choice == 'Y') 
            {
                char new_email[35];
                do {
                    printf(CYAN "Enter new email: " RESET);
                    scanf(" %[^\n]", new_email);
                    int status = validate_email(new_email, addressbook);
                    if (status == 0) printf(RED "Invalid email.\n" RESET);
                    else if (status == -1) printf(RED "Email already exists.\n" RESET);
                    else { strcpy(addressbook->contact_details[i].Mail_ID, new_email); break; }
                } while (1);
            }

            printf(GREEN "\nContact updated successfully!\n" RESET);
            return;
        }
    }
    printf(RED "No contact matched under name %s.\n" RESET, name);
}

// Edit by Phone(ask phone/email if duplicates)
void edit_by_phone(struct Address_book *addressbook) 
{
    char phone[11];
    printf(CYAN "Enter Phone to edit: " RESET);
    scanf("%s", phone);

    for (int i = 0; i < addressbook->contact_count; i++) {
        if (strcmp(addressbook->contact_details[i].Mobile_number, phone) == 0) 
        {
            printf(GREEN "\n--- Contact Found ---\n" RESET);
            printf("Name: %s\nPhone: %s\nEmail: %s\n",
                   addressbook->contact_details[i].Name,
                   addressbook->contact_details[i].Mobile_number,
                   addressbook->contact_details[i].Mail_ID);

            // Name edit
            char new_name[32];
            printf(CYAN "Enter new name (leave empty to keep unchanged): " RESET);
            scanf(" %[^\n]", new_name);
            if (strlen(new_name) > 0 && validate_name(new_name)) 
            {
                strcpy(addressbook->contact_details[i].Name, new_name);
            }

            // Phone edit
            char choice;
            printf(CYAN "Do you want to edit phone? (y/n): " RESET);
            scanf(" %c", &choice);
            if (choice == 'y' || choice == 'Y') 
            {
                char new_phone[11];
                do {
                    printf(CYAN "Enter new phone: " RESET);
                    scanf(" %[^\n]", new_phone);
                    int status = validate_phone(new_phone, addressbook);
                    if (status == 0) printf(RED "Invalid phone.\n" RESET);
                    else if (status == -1) printf(RED "Phone already exists.\n" RESET);
                    else { strcpy(addressbook->contact_details[i].Mobile_number, new_phone); break; }
                } while (1);
            }

            // Email edit
            printf(CYAN "Do you want to edit email? (y/n): " RESET);
            scanf(" %c", &choice);
            if (choice == 'y' || choice == 'Y') 
            {
                char new_email[35];
                do {
                    printf(CYAN "Enter new email: " RESET);
                    scanf(" %[^\n]", new_email);
                    int status = validate_email(new_email, addressbook);
                    if (status == 0) printf(RED "Invalid email.\n" RESET);
                    else if (status == -1) printf(RED "Email already exists.\n" RESET);
                    else { strcpy(addressbook->contact_details[i].Mail_ID, new_email); break; }
                } while (1);
            }

            printf(GREEN "Contact updated successfully!\n" RESET);
            return;
        }
    }
    printf(RED "No contact found with that Phone.\n" RESET);
}

// Edit by Email(ask phone/email if duplicates)
void edit_by_email(struct Address_book *addressbook)
 {
    char email[35];
    printf(CYAN "Enter Email to edit: " RESET);
    scanf("%s", email);

    for (int i = 0; i < addressbook->contact_count; i++)
     {
        if (strcmp(addressbook->contact_details[i].Mail_ID, email) == 0) 
        {
            printf(GREEN "\n--- Contact Found ---\n" RESET);
            printf("Name: %s\nPhone: %s\nEmail: %s\n",
                   addressbook->contact_details[i].Name,
                   addressbook->contact_details[i].Mobile_number,
                   addressbook->contact_details[i].Mail_ID);

            // Name edit
            char new_name[32];
            printf(CYAN "Enter new name (leave empty to keep unchanged): " RESET);
            scanf(" %[^\n]", new_name);
            if (strlen(new_name) > 0 && validate_name(new_name))
             {
                strcpy(addressbook->contact_details[i].Name, new_name);
            }

            // Phone edit
            char choice;
            printf(CYAN "Do you want to edit phone? (y/n): " RESET);
            scanf(" %c", &choice);
            if (choice == 'y' || choice == 'Y') 
            {
                char new_phone[11];
                do {
                    printf(CYAN "Enter new phone: " RESET);
                    scanf(" %[^\n]", new_phone);
                    int status = validate_phone(new_phone, addressbook);
                    if (status == 0) printf(RED "Invalid phone.\n" RESET);
                    else if (status == -1)
                    { 
                    printf(RED "Phone already exists.\n" RESET);
                    }
                    else{
                         strcpy(addressbook->contact_details[i].Mobile_number, new_phone);
                          break; 
                        }
                } while (1);
            }

            // Email edit
            printf(CYAN "Do you want to edit email? (y/n): " RESET);
            scanf(" %c", &choice);
            if (choice == 'y' || choice == 'Y') {
                char new_email[35];
                do {
                    printf(CYAN "Enter new email: " RESET);
                    scanf(" %[^\n]", new_email);
                    int status = validate_email(new_email, addressbook);
                    if (status == 0) printf(RED "Invalid email.\n" RESET);
                    else if (status == -1)
                    {
                        printf(RED "Email already exists.\n" RESET);
                    }
                    else { 
                        strcpy(addressbook->contact_details[i].Mail_ID, new_email); 
                        break; 
                    }
                } while (1);
            }

            printf(GREEN "Contact updated successfully!\n" RESET);
            return;
        }
    }
    printf(RED "No contact found with that Email.\n" RESET);
}
//================ DELETE FUNCTIONS =================//
// Delete by Name
void delete_by_name(struct Address_book *addressbook)
 {
    char name[32];
    printf(CYAN "Enter Name to delete: " RESET);
    scanf("%s", name);

    for (int i = 0; i < addressbook->contact_count; i++)
    {
        if (strcmp(addressbook->contact_details[i].Name, name) == 0)
         {
            printf(GREEN "\n--- Contact Found ---\n" RESET);
            printf("Name: %s\nPhone: %s\nEmail: %s\n",
                   addressbook->contact_details[i].Name,
                   addressbook->contact_details[i].Mobile_number,
                   addressbook->contact_details[i].Mail_ID);

            for (int j = i; j < addressbook->contact_count - 1; j++) 
            {
                addressbook->contact_details[j] = addressbook->contact_details[j + 1];
            }
            addressbook->contact_count--;
            printf(GREEN "Contact deleted successfully!\n" RESET);
            return;
        }
    }
    printf(RED "No contact found with that Name.\n" RESET);
}

// Delete by Phone
void delete_by_phone(struct Address_book *addressbook) 
{
    char phone[11];
    printf(CYAN "Enter Phone to delete: " RESET);
    scanf("%s", phone);

    for (int i = 0; i < addressbook->contact_count; i++) {
        if (strcmp(addressbook->contact_details[i].Mobile_number, phone) == 0) 
        {
            printf(GREEN "\n--- Contact Found ---\n" RESET);
            printf("Name: %s\nPhone: %s\nEmail: %s\n",
                   addressbook->contact_details[i].Name,
                   addressbook->contact_details[i].Mobile_number,
                   addressbook->contact_details[i].Mail_ID);

            for (int j = i; j < addressbook->contact_count - 1; j++) 
            {
                addressbook->contact_details[j] = addressbook->contact_details[j + 1];
            }
            addressbook->contact_count--;
            printf(GREEN "Contact deleted successfully!\n" RESET);
            return;
        }
    }
    printf(RED "No contact found with that Phone.\n" RESET);
}

// Delete by Email
void delete_by_email(struct Address_book *addressbook) 
{
    char email[35];
    printf(CYAN "Enter Email to delete: " RESET);
    scanf("%s", email);

    for (int i = 0; i < addressbook->contact_count; i++) 
    {
        if (strcmp(addressbook->contact_details[i].Mail_ID, email) == 0) {
            printf(GREEN "\n--- Contact Found ---\n" RESET);
            printf("Name: %s\nPhone: %s\nEmail: %s\n",
                   addressbook->contact_details[i].Name,
                   addressbook->contact_details[i].Mobile_number,
                   addressbook->contact_details[i].Mail_ID);

            for (int j = i; j < addressbook->contact_count - 1; j++) 
            {
                addressbook->contact_details[j] = addressbook->contact_details[j + 1];
            }
            addressbook->contact_count--;
            printf(GREEN "Contact deleted successfully!\n" RESET);
            return;
        }
    }
    printf(RED "No contact found with that Email.\n" RESET);
}
//================ CREATE CONTACT =================//
// Create a new contact
int create_contact(struct Address_book *addressbook)
 {
    char name[20], phone[11], email[30];

    // Validate Name
    do {
        printf(CYAN "Enter the name: " RESET);
        scanf(" %[^\n]", name);
        if (!validate_name(name)) {
            printf(RED "Invalid name. Only alphabets allowed.\n" RESET);
        }
        else { 
            printf(GREEN "Name accepted!\n" RESET); 
            break;
         }
    } while (1);

    // Validate Phone
    do {
        printf(CYAN "Enter phone number: " RESET);
        scanf(" %[^\n]", phone);
        int status = validate_phone(phone, addressbook);
        if (status == 0) printf(RED "Invalid phone. Must be 10 digits & not start with 0.\n" RESET);
        else if (status == -1) {
            printf(RED "Phone number already exists!\n" RESET);
        }
        else { 
            printf(GREEN "Phone accepted!\n" RESET); 
            break;
             }
    } while (1);

    // Validate Email
    do {
        printf(CYAN "Enter email id: " RESET);
        scanf(" %[^\n]", email);
        int status = validate_email(email, addressbook);
        if (status == 0){
             printf(RED "Invalid email format.\n" RESET);
        }
        else if (status == -1) {
            printf(RED "Email already exists!\n" RESET);
        }
        else {
             printf(GREEN "Email accepted!\n" RESET);
              break;
             }
    } while (1);

    // Save contact
    strcpy(addressbook->contact_details[addressbook->contact_count].Name, name);
    strcpy(addressbook->contact_details[addressbook->contact_count].Mobile_number, phone);
    strcpy(addressbook->contact_details[addressbook->contact_count].Mail_ID, email);
    addressbook->contact_count++;

    printf(CYAN "Contact created successfully!\n" RESET);
    return 0;
}
//================ LIST CONTACTS =================//
// Display all contacts
void list_contacts(struct Address_book *addressbook)
 {
    if (addressbook->contact_count == 0)
     {
        printf(RED "No contacts found.\n" RESET);
        return;
    }
    for (int i = 0; i < addressbook->contact_count; i++)
     {
        printf(YELLOW "\nContact %d:\n" RESET, i + 1);
        printf(" Name        : %s\n", addressbook->contact_details[i].Name);
        printf(" Phone Number: %s\n", addressbook->contact_details[i].Mobile_number);
        printf(" Email ID    : %s\n", addressbook->contact_details[i].Mail_ID);
    }
}

//================  SEARCH CONTACTS =================//
int search_contacts(struct Address_book *addressbook) {
    int opt;
    while (1) 
    {
        printf(CYAN "\n---- Search Contact ----\n" RESET);
        printf(YELLOW "1. Search by Name\n2. Search by Phone Number\n3. Search by Email Id\n4. Exit\n" RESET);
        printf("Enter choice: ");
        scanf("%d", &opt);
        switch (opt) 
        {
        case 1: search_by_name(addressbook);
                break;
        case 2: search_by_phone(addressbook);
                break;
        case 3: search_by_email(addressbook); 
                break;
        case 4: return 0;
        default: printf(RED "Invalid choice!\n" RESET);
        }
    }
}
//================  EDIT CONTACTS =================//
int edit_contact(struct Address_book *addressbook) {
    int choice;
    printf(CYAN "\nEdit Contact By:\n" RESET);
    printf(YELLOW "1. Name\n2. Phone Number\n3. Email Id\n" RESET);
    printf("Enter choice: ");
    scanf("%d", &choice);
    switch (choice) 
    {
    case 1: edit_by_name(addressbook);
            break;
    case 2: edit_by_phone(addressbook); 
            break;
    case 3: edit_by_email(addressbook); 
            break;
    default: printf(RED "Invalid choice!\n" RESET);
    }
    return 0;
}
//================  DELETE CONTACTS =================//
int delete_contact(struct Address_book *addressbook) 
{
    int choice;
    printf(CYAN "\nDelete Contact By:\n" RESET);
    printf(YELLOW "1. Name\n2. Phone Number\n3. Email Id\n" RESET);
    printf("Enter choice: ");
    scanf("%d", &choice);
    switch (choice)
     {
    case 1: delete_by_name(addressbook); 
            break;
    case 2: delete_by_phone(addressbook); 
            break;
    case 3: delete_by_email(addressbook); 
            break;
    default: printf(RED "Invalid choice!\n" RESET);
    }
    return 0;
}
//================ SAVE / LOAD =================//
int save_contacts(struct Address_book *addressbook) 
{
    // Open the file in write mode
    FILE *fp = fopen("database.txt", "w");
if (fp == NULL) {
    printf(RED "Error: Could not open file for writing.\n" RESET);
    return -1;
}
 fprintf(fp,"#%d\n",addressbook->contact_count);
    for (int i = 0; i < addressbook->contact_count; i++) 
    {
        fprintf(fp, "%s,%s,%s\n",
                addressbook->contact_details[i].Name,
                addressbook->contact_details[i].Mobile_number,
                addressbook->contact_details[i].Mail_ID);
    }
    fclose(fp);
    printf(GREEN "Contacts saved successfully!\n" RESET);
    return 0;
}
//================  LOAD CONTACTS FROM FILE =================//
int load_contacts(struct Address_book *addressbook)
 {
    // Open the file in read mode
    FILE* fp = fopen("database.txt", "r");
    if (fp == NULL) 
    {
        puts(RED "File does not exist.\n" RESET);
        addressbook->contact_count = 0;
        return 0;
    }
    if (fscanf(fp, "#%d\n", &addressbook->contact_count) != 1) 
    {
        printf(RED "Error: Invalid file format\n" RESET);
        fclose(fp);
        addressbook->contact_count = 0;
        return 0;
    }
    for (int i = 0; i < addressbook->contact_count; i++)
     {
        if (fscanf(fp, "%[^,],%[^,],%[^\n]\n",
                   addressbook->contact_details[i].Name,
                   addressbook->contact_details[i].Mobile_number,
                   addressbook->contact_details[i].Mail_ID) != 3) {
            printf(RED "Error: Corrupt contact at line %d\n" RESET, i+2);
            addressbook->contact_count = i;
            break;
        }
    }
    fclose(fp);
    return 1;
}
