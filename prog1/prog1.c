//to store n records in binary file
//use func to get mth record and display.
//Use fseek
//use func to delete a record.
// Program to store records in binary file
// Display mth record using fseek()
// Delete a record using temporary file

#include <stdio.h>
#include <stdlib.h>

struct Student
{
    int id;
    char name[50];
    float marks;
};

/* Function Prototypes */
void storeRecords();
void displayMthRecord(int m);
void deleteRecord(int deleteId);
void displayAll();

/* Store records in binary file */
void storeRecords()
{
    FILE *fp;
    struct Student s;
    int n, i;

    /* Append mode so old records are not deleted */
    fp = fopen("students.dat", "ab");

    if(fp == NULL)
    {
        printf("Error opening file.\n");
        return;
    }

    printf("Enter number of records: ");
    scanf("%d", &n);

    for(i = 0; i < n; i++)
    {
        printf("\n--- Record %d ---\n", i + 1);

        printf("Enter ID: ");
        scanf("%d", &s.id);

        printf("Enter Name: ");
        scanf(" %[^\n]", s.name);

        printf("Enter Marks: ");
        scanf("%f", &s.marks);

        fwrite(&s, sizeof(struct Student), 1, fp);
    }

    fclose(fp);

    printf("\nRecords stored successfully.\n");
}

/* Display mth record using fseek */
void displayMthRecord(int m)
{
    FILE *fp;
    struct Student s;
    int totalRecords;

    fp = fopen("students.dat", "rb");

    if(fp == NULL)
    {
        printf("File not found.\n");
        return;
    }

    /* Find total number of records */
    fseek(fp, 0, SEEK_END);

    totalRecords = ftell(fp) / sizeof(struct Student);

    if(m < 1 || m > totalRecords)
    {
        printf("Invalid record number.\n");
        fclose(fp);
        return;
    }

    /* Move to mth record */
    fseek(fp, (m - 1) * sizeof(struct Student), SEEK_SET);

    fread(&s, sizeof(struct Student), 1, fp);

    printf("\n===== Mth Record =====\n");
    printf("ID    : %d\n", s.id);
    printf("Name  : %s\n", s.name);
    printf("Marks : %.2f\n", s.marks);

    fclose(fp);
}

/* Delete record by ID */
void deleteRecord(int deleteId)
{
    FILE *fp, *temp;
    struct Student s;
    int found = 0;

    fp = fopen("students.dat", "rb");

    if(fp == NULL)
    {
        printf("File not found.\n");
        return;
    }

    temp = fopen("temp.dat", "wb");

    if(temp == NULL)
    {
        printf("Error creating temp file.\n");
        fclose(fp);
        return;
    }

    while(fread(&s, sizeof(struct Student), 1, fp))
    {
        if(s.id != deleteId)
        {
            fwrite(&s, sizeof(struct Student), 1, temp);
        }
        else
        {
            found = 1;
        }
    }

    fclose(fp);
    fclose(temp);

    remove("students.dat");
    rename("temp.dat", "students.dat");

    if(found)
    {
        printf("Record deleted successfully.\n");
    }
    else
    {
        printf("Record not found.\n");
    }
}

/* Display all records */
void displayAll()
{
    FILE *fp;
    struct Student s;

    fp = fopen("students.dat", "rb");

    if(fp == NULL)
    {
        printf("File not found.\n");
        return;
    }

    printf("\n===== All Records =====\n");

    while(fread(&s, sizeof(struct Student), 1, fp))
    {
        printf("\nID    : %d\n", s.id);
        printf("Name  : %s\n", s.name);
        printf("Marks : %.2f\n", s.marks);
    }

    fclose(fp);
}

/* Main Function */
int main()
{
    int choice, m, deleteId;

    while(1)
    {
        printf("\n========== MENU ==========\n");
        printf("1. Store Records\n");
        printf("2. Display Mth Record\n");
        printf("3. Delete Record\n");
        printf("4. Display All Records\n");
        printf("5. Exit\n");

        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch(choice)
        {
            case 1:
                storeRecords();
                break;

            case 2:
                printf("Enter m value: ");
                scanf("%d", &m);

                displayMthRecord(m);
                break;

            case 3:
                printf("Enter ID to delete: ");
                scanf("%d", &deleteId);

                deleteRecord(deleteId);
                break;

            case 4:
                displayAll();
                break;

            case 5:
                printf("Exiting program...\n");
                return 0;

            default:
                printf("Invalid choice.\n");
        }
    }

    return 0;
}