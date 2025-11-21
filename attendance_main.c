#include <stdio.h>
#include <string.h>
#include "attendance_utils.h"

int main()
{
    int choice;
    char teacherName[100], sectionName[50], studentName[100], srn[14];
    float threshold;

    printf("Initializing Student Attendance Management System...\n");
    initializeCollege();
    printf("System initialized with %d teachers.\n", numTeachers);

    while (1)
    {
        printf("\n============================================================\n");
        printf("     Student Attendance Management System\n");
        printf("============================================================\n");
        printf("1. Add a new student to a class\n");
        printf("2. Display roster for a class\n");
        printf("3. Mark attendance for a class (one day)\n");
        printf("4. Generate low attendance report (college-wide)\n");
        printf("5. Display entire college structure (teachers -> classes -> students)\n");
        printf("6. Exit\n");
        printf("============================================================\n");
        printf("Enter your choice: ");

        if (scanf("%d", &choice) != 1)
        {
            printf("Invalid input! Please enter a number.\n");
            while (getchar() != '\n')
                ;
            continue;
        }

        switch (choice)
        {
        case 1:
        {
            // Add Student
            printf("\n--- Add New Student ---\n");
            printf("Enter Teacher Name: ");
            scanf(" %[^\n]", teacherName);
            printf("Enter Section Name: ");
            scanf("%s", sectionName);
            printf("Enter Student Name: ");
            scanf(" %[^\n]", studentName);
            printf("Enter Student SRN: ");
            scanf("%s", srn);

            addStudent(teacherName, sectionName, studentName, srn);
            break;
        }

        case 2:
        {
            // Display Roster
            printf("\n--- Display Class Roster ---\n");
            printf("Enter Teacher Name: ");
            scanf(" %[^\n]", teacherName);
            printf("Enter Section Name: ");
            scanf("%s", sectionName);

            displayRoster(teacherName, sectionName);
            break;
        }

        case 3:
        {
            // Mark Attendance
            printf("\n--- Mark Attendance ---\n");
            printf("Enter Teacher Name: ");
            scanf(" %[^\n]", teacherName);
            printf("Enter Section Name: ");
            scanf("%s", sectionName);

            markAttendance(teacherName, sectionName);
            break;
        }

        case 4:
        {
            // Low Attendance Report
            printf("\n--- Generate Low Attendance Report ---\n");
            printf("Enter attendance percentage threshold: ");
            if ((scanf("%f", &threshold) != 1) || threshold>100 || threshold<0)
            {
                printf("Invalid input! Please enter a number(valid).\n");
                while (getchar() != '\n')
                    ;
                break;
            }

            generateLowAttendanceReport(threshold);
            break;
        }
        case 5:
        {
            displayCollege();
            break;
        }
        case 6:
        {
            printf("\nExiting program. Goodbye!\n");
            destroyCollege();
            return 0;
        }

        default:
        {
            printf("Invalid choice. Please enter a number between 1-6.\n");
            break;
        }
        }
    }
    
    return 0;
}
