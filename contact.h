/***************************************************************
 * Name        : SUMIT KUMAR PRADHAN
 * Reg No      : 25017_150
 * Project     : Address Book
 * File Name   : contact.h
 * Start Date  : 12/08/2025
 * End Date    : 30/08/2025
 * Description : Header file that contains structure definitions 
 *               and function declarations for managing contacts.
 ***************************************************************/
#ifndef CONTACT_H
#define CONTACT_H

// Structure to store a single contact's details
struct Contact_data
{
    char Name[32];           // Contact's name (max 31 chars + '\0')
    char Mobile_number[11];  // Contact's phone number (10 digits + '\0')
    char Mail_ID[35];        // Contact's email ID (max 34 chars + '\0')
};

// Structure to store multiple contacts in an address book
struct Address_book
{
    struct Contact_data contact_details[100];  // Array of 100 contacts
    int contact_count;                          // Number of contacts stored
};

/* ===== Function Declarations ===== */

// Initialization
void init_intitalization(struct Address_book *addressbook);

// Core operations
int create_contact(struct Address_book *);          // Add a new contact
void list_contacts(struct Address_book *);          // Display all contacts
int search_contacts(struct Address_book *addressbook); // Search for a contact
int edit_contact(struct Address_book *addressbook); // Edit existing contact
int delete_contact(struct Address_book *addressbook); // Delete a contact

// File operations
int save_contacts(struct Address_book *addressbook); // Save contacts to file
int load_contacts(struct Address_book *addressbook); // Load contacts from file

// Validation functions
int validate_name(const char *name);                                      ///validate name
int validate_phone(const char *phone, struct Address_book *addressbook);  //validate phone
int validate_email(const char *email, struct Address_book *addressbook); //validate email

// Search sub-functions
void search_by_name(struct Address_book *addressbook);   // Search by name
void search_by_phone(struct Address_book *addressbook);  // Search by phone
void search_by_email(struct Address_book *addressbook);  // Search by email

// Edit sub-functions
void edit_by_name(struct Address_book *addressbook);     // Edit by name
void edit_by_phone(struct Address_book *addressbook);    // Edit by phone
void edit_by_email(struct Address_book *addressbook);    // Edit by email

// Delete sub-functions
void delete_by_name(struct Address_book *addressbook);   // Delete by name
void delete_by_phone(struct Address_book *addressbook);  // Delete by phone
void delete_by_email(struct Address_book *addressbook);  // Delete by email

#endif // CONTACT_H
