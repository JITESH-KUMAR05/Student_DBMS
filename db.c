#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_KEY_LENGTH 100
#define MAX_VALUE_LENGTH 100
#define MAX_STUDENTS 100
#define NUM_SUBJECTS 4

void saveData();
void loadData();

typedef struct {
    char name[MAX_KEY_LENGTH];
    char roll[MAX_VALUE_LENGTH];
    char branch[MAX_VALUE_LENGTH];
    char year[MAX_VALUE_LENGTH];
    int marks[NUM_SUBJECTS]; // Maths, Physics, BEE, PPS
    int totalMarks;
} Student;

Student students[MAX_STUDENTS];
int numStudents = 0;
int isAdmin = 0; // 0 for regular user, 1 for admin

void saveData() {
    FILE *file = fopen("student_data.txt", "w");
    if (file == NULL) {
        printf("Error opening file for writing.\n");
        return;
    }

    for (int i = 0; i < numStudents; i++) {
        fprintf(file, "%s %s %s %s ", students[i].name, students[i].roll, students[i].branch, students[i].year);
        for (int j = 0; j < NUM_SUBJECTS; j++) {
            fprintf(file, "%d ", students[i].marks[j]);
        }
        fprintf(file, "%d\n", students[i].totalMarks); // Save total marks
    }

    fclose(file);
    printf("Data saved successfully.\n");
}


void loadData() {
    FILE *file = fopen("student_data.txt", "r");
    if (file == NULL) {
        printf("No existing data found. Starting fresh.\n");
        return;
    }

    while (fscanf(file, "%s %s %s %s", students[numStudents].name, students[numStudents].roll, students[numStudents].branch, students[numStudents].year) == 4) {
        for (int j = 0; j < NUM_SUBJECTS; j++) {
            fscanf(file, "%d", &students[numStudents].marks[j]);
        }
        fscanf(file, "%d", &students[numStudents].totalMarks); // Load total marks
        numStudents++;
        if (numStudents >= MAX_STUDENTS) break; // Prevent overflow
    }

    fclose(file);
    printf("Data loaded successfully.\n");
}

void calculateTotalMarks(int index) {
    students[index].totalMarks = 0;
    for (int i = 0; i < NUM_SUBJECTS; i++) {
        students[index].totalMarks += students[index].marks[i];
    }
}

void insertStudentDetails() {
    if (numStudents >= MAX_STUDENTS) {
        printf("Error: Database is full, cannot insert more students.\n");
        return;
    }

    printf("Enter name: ");
    scanf("%s", students[numStudents].name);
    printf("Enter roll: ");
    scanf("%s", students[numStudents].roll);
    for (int i = 0; i < numStudents; i++) {
        if (strcmp(students[i].roll, students[numStudents].roll) == 0) {
            printf("Error: Roll number '%s' already exists in the database.\n", students[numStudents].roll);
            return;
        }
    }
    printf("Enter branch: ");
    scanf("%s", students[numStudents].branch);
    printf("Enter year: ");
    scanf("%s", students[numStudents].year);

    char subjects[NUM_SUBJECTS][20] = {"Maths", "Physics", "BEE", "PPS"};
    for (int i = 0; i < NUM_SUBJECTS; i++) {
        printf("Enter marks for %s: ", subjects[i]);
        scanf("%d", &students[numStudents].marks[i]);
    }

    calculateTotalMarks(numStudents);
    numStudents++;
    saveData();
    printf("Student details inserted successfully.\n");
}

void displayStudentDetails() {
    char roll[MAX_VALUE_LENGTH];
    printf("Enter roll number to display details: ");
    scanf("%s", roll);

    int found = 0;
    for (int i = 0; i < numStudents; i++) {
        if (strcmp(students[i].roll, roll) == 0) {
            found = 1;
            printf("\nStudent Details:\n");
            printf("Name: %s | Roll: %s | Branch: %s | Year: %s\n", students[i].name, students[i].roll, students[i].branch, students[i].year);
            printf("Subject\t\tMarks\n");
            printf("-----------------------------\n");
            printf("Maths\t\t%d\n", students[i].marks[0]);
            printf("Physics\t\t%d\n", students[i].marks[1]);
            printf("BEE\t\t%d\n", students[i].marks[2]);
            printf("PPS\t\t%d\n", students[i].marks[3]);
            printf("Total Marks: %d\n", students[i].totalMarks);
            break;
        }
    }

    if (!found) {
        printf("Roll number '%s' not found in the database.\n", roll);
    }
}

void displayAllStudents() {
    if (!isAdmin) {
        printf("Access Denied: Only admins can display all students.\n");
        return;
    }

    if (numStudents == 0) {
        printf("Database is empty.\n");
        return;
    }

    printf("\nAll Student Details:\n");
    for (int i = 0; i < numStudents; i++) {
        printf("\n%d. Name: %s, Roll: %s, Branch: %s, Year: %s, Total Marks: %d\n", i + 1, students[i].name, students[i].roll, students[i].branch, students[i].year, students[i].totalMarks);
        printf("Subject\t\tMarks\n");
        printf("-----------------------------\n");
        printf("Maths\t\t%d\n", students[i].marks[0]);
        printf("Physics\t\t%d\n", students[i].marks[1]);
        printf("BEE\t\t%d\n", students[i].marks[2]);
        printf("PPS\t\t%d\n", students[i].marks[3]);
    }
}

void deleteStudent() {
    if (!isAdmin) {
        printf("Access Denied: Only admins can delete a student.\n");
        return;
    }

    char roll[MAX_VALUE_LENGTH];
    printf("Enter roll number to delete: ");
    scanf("%s", roll);

    int found = 0;
    for (int i = 0; i < numStudents; i++) {
        if (strcmp(students[i].roll, roll) == 0) {
            found = 1;
            for (int j = i; j < numStudents - 1; j++) {
                students[j] = students[j + 1];
            }
            numStudents--;
            saveData();
            printf("Student with roll number '%s' deleted successfully.\n", roll);
            break;
        }
    }

    if (!found) {
        printf("Roll number '%s' not found in the database.\n", roll);
    }
}

void toggleAdminAccess() {
    char password[20];
    printf("Enter admin password to toggle access: ");
    scanf("%s", password);

    if (strcmp(password, "adminpass") == 0) {
        isAdmin = !isAdmin;
        printf("Admin access %s.\n", isAdmin ? "granted" : "revoked");
    } else {
        printf("Incorrect password.\n");
    }
}

int main() {
    loadData();
    int choice;
    do {
        printf("\nOptions:\n");
        printf("1. Add student details\n");
        printf("2. Display student details by roll number\n");
        printf("3. Display all student details\n");
        printf("4. Delete a student\n");
        printf("5. Toggle admin access\n");
        printf("6. Quit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                insertStudentDetails();
                break;
            case 2:
                displayStudentDetails();
                break;
            case 3:
                displayAllStudents();
                break;
            case 4:
                deleteStudent();
                break;
            case 5:
                toggleAdminAccess();
                break;
            case 6:
                printf("Exiting program.\n");
                break;
            default:
                printf("Invalid choice. Please enter a valid option.\n");
        }
    } while (choice != 6);

    return 0;
}
