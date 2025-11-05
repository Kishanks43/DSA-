#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "attendance_utils.h"

TeacherNode college[MAX_TEACHERS];
int numTeachers = 0;

TeacherNode *findTeacherNode(const char *teacherName)
{
    int i;
    for (i = 0; i < numTeachers; i++)
    {
        if (strcasecmp(college[i].teacherName, teacherName) == 0)
        {
            return &college[i];
        }
    }
    return NULL;
}

ClassNode *findClassNode(TeacherNode *teacher, const char *sectionName)
{
    if (teacher == NULL)
        return NULL;

    ClassNode *current = teacher->firstChild;
    while (current != NULL)
    {
        if (strcasecmp(current->sectionName, sectionName) == 0)
        {
            return current;
        }
        current = current->nextSibling;
    }
    return NULL;
}


ClassNode *createClassNode(const char *sectionName)
{
    ClassNode *newNode = (ClassNode *)malloc(sizeof(ClassNode));
    if (newNode == NULL)
    {
        printf("Memory allocation failed!\n");
        return NULL;
    }

    strncpy(newNode->sectionName, sectionName, 49);
    newNode->sectionName[49] = '\0';
    newNode->numStudents = 0;
    newNode->capacity = INITIAL_STUDENT_CAPACITY;
    newNode->nextSibling = NULL;

    // Allocate initial student array
    newNode->studentArray = (Student *)malloc(sizeof(Student) * INITIAL_STUDENT_CAPACITY);
    if (newNode->studentArray == NULL)
    {
        printf("Memory allocation failed for student array!\n");
        free(newNode);
        return NULL;
    }

    return newNode;
}

void expandStudentArray(ClassNode *classNode)
{
    if (classNode == NULL)
        return;

    int newCapacity = classNode->capacity * 2;
    Student *newArray = (Student *)realloc(classNode->studentArray, sizeof(Student) * newCapacity);

    if (newArray == NULL)
    {
        printf("Failed to expand student array!\n");
        return;
    }

    classNode->studentArray = newArray;
    classNode->capacity = newCapacity;
}


void initializeCollege()
{
    strcpy(college[0].teacherName, "Anand MS");
    college[0].firstChild = NULL;

    ClassNode *classG = createClassNode("G");
    ClassNode *classA = createClassNode("A");
    classG->nextSibling = classA;
    classA->nextSibling = NULL;
    college[0].firstChild = classG;


    strcpy(college[1].teacherName, "DDCO");
    college[1].firstChild = NULL;
    
    ClassNode* classB1 = createClassNode("B1");
    ClassNode* classB2 = createClassNode("B2");
    classB1->nextSibling = classB2;
    classB2->nextSibling = NULL;
    college[1].firstChild = classB1;

    
    numTeachers=2;

}

void addStudent(const char *teacherName, const char *sectionName, const char *studentName, const char *srn)
{
    TeacherNode *teacher = findTeacherNode(teacherName);
    if (teacher == NULL) {
    printf("Teacher '%s' not found!\n", teacherName);
    return;
    }

    ClassNode *targetClass = findClassNode(teacher, sectionName);

    if (targetClass == NULL)
    {
        printf("Section '%s' not found for teacher '%s'!\n", sectionName, teacherName);
        printf("Would you like to create this class? (y/n): ");
        char response;
        scanf(" %c", &response);
        if (response == 'y' || response == 'Y')
        {
            targetClass = createClassNode(sectionName);
            if (targetClass == NULL)
            {
                printf("Failed to create class node!\n");
                return;
            }
            targetClass->nextSibling = teacher->firstChild;
            teacher->firstChild = targetClass;
        }
        else
        {
            return;
        }
    }

    if (targetClass->numStudents >= targetClass->capacity)
    {
        expandStudentArray(targetClass);
    }

    Student *newStudent = &targetClass->studentArray[targetClass->numStudents];
    strncpy(newStudent->name, studentName, 99);
    newStudent->name[99] = '\0';
    strncpy(newStudent->srn, srn, 13);
    newStudent->srn[13] = '\0';
    newStudent->presentDays = 0;
    newStudent->totalDays = 0;

    targetClass->numStudents++;
    printf("Student '%s' (SRN: %s) added successfully to %s - %s!\n",
           studentName, srn, teacherName, sectionName);
}

void displayRoster(const char *teacherName, const char *sectionName)
{
    TeacherNode *teacher = findTeacherNode(teacherName);
    if (teacher == NULL)
    {
        printf("Teacher '%s' not found!\n", teacherName);
        return;
    }

    ClassNode *targetClass = findClassNode(teacher, sectionName);
    if (targetClass == NULL)
    {
        printf("Section '%s' not found for teacher '%s'!\n", sectionName, teacherName);
        return;
    }

    printf("\n=== Class Roster for %s - %s ===\n", teacherName, sectionName);
    printf("%-25s %-15s %-15s %-15s %-12s\n", "Name", "SRN", "Present Days", "Total Days","Percentage");
    printf("-------------------------------------------------------------------\n");

    if (targetClass->numStudents == 0)
    {
        printf("No students in this class.\n");
    }
    else
    {
        int i;
        for (i = 0; i < targetClass->numStudents; i++)
        {
            Student *stu = &targetClass->studentArray[i];
            if (stu->totalDays > 0)
            {
                printf("%-25s %-15s %-15d %-15d %-12.2f\n",
                       stu->name, stu->srn, stu->presentDays, stu->totalDays, (stu->presentDays * 100.0f) / stu->totalDays);
            }
            else
            {
                printf("%-25s %-15s %-15d %-15d %-12s\n",
                       stu->name, stu->srn, stu->presentDays, stu->totalDays, "N/A");
            }
        }
        printf("Total students: %d\n", targetClass->numStudents);
    }
}

void markAttendance(const char *teacherName, const char *sectionName)
{
    TeacherNode *teacher = findTeacherNode(teacherName);
    if (teacher == NULL)
    {
        printf("Teacher '%s' not found!\n", teacherName);
        return;
    }

    ClassNode *targetClass = findClassNode(teacher, sectionName);
    if (targetClass == NULL)
    {
        printf("Section '%s' not found for teacher '%s'!\n", sectionName, teacherName);
        return;
    }

    if (targetClass->numStudents == 0)
    {
        printf("No students in this class to mark attendance!\n");
        return;
    }

    printf("\n=== Marking Attendance for %s - %s ===\n", teacherName, sectionName);
    printf("Enter 'P' or 'Present' for present, 'A' or 'Absent' for absent\n");
    printf("----------------------------------------------------------\n");

    int i;
    for (i = 0; i < targetClass->numStudents; i++)
    {
        Student *stu = &targetClass->studentArray[i];
        char status[10];

        printf("%s (%s): Present/Absent? ", stu->name, stu->srn);
        scanf("%s", status);

        // Normalize input - check various formats
        if (strcmp(status, "Present") == 0 || strcmp(status, "present") == 0 ||
            strcmp(status, "P") == 0 || strcmp(status, "p") == 0)
        {
            stu->presentDays++;
            printf("  -> Marked as Present\n");
        }
        else if (strcmp(status, "Absent") == 0 || strcmp(status, "absent") == 0 ||
                 strcmp(status, "A") == 0 || strcmp(status, "a") == 0)
        {
            printf("  -> Marked as Absent\n");
        }
        else
        {
            printf("  -> Invalid input. Marking as Absent by default.\n");
        }

        stu->totalDays++;
    }

    printf("\nAttendance marked successfully for all students!\n");
}


void generateLowAttendanceReport(float threshold)
{
    printf("\n=== Low Attendance Report (Threshold: %.2f%%) ===\n", threshold);
    printf("%-20s %-15s %-20s %-20s %-12s\n",
           "Teacher", "Section", "Name", "SRN", "Attendance %");
    printf("-----------------------------------------------------------------------------------\n");

    int found = 0;
    int teacherIdx;

    for (teacherIdx = 0; teacherIdx < numTeachers; teacherIdx++)
    {
        TeacherNode *teacher = &college[teacherIdx];
        ClassNode *currentClass = teacher->firstChild;

        while (currentClass != NULL)
        {
            int i;
            for (i = 0; i < currentClass->numStudents; i++)
            {
                Student *stu = &currentClass->studentArray[i];

                if (stu->totalDays == 0)
                {
                    continue;
                }

                float attendancePercent = (stu->presentDays * 100.0f) / stu->totalDays;

                if (attendancePercent < threshold)
                {
                    printf("%-20s %-15s %-20s %-20s %-12.2f\n",
                           teacher->teacherName,
                           currentClass->sectionName,
                           stu->name,
                           stu->srn,
                           attendancePercent);
                    found = 1;
                }
            }

            currentClass = currentClass->nextSibling;
        }
    }

    if (!found)
    {
        printf("No students found with attendance below %.2f%%.\n", threshold);
    }
    else
    {
        printf("\nReport generated successfully!\n");
    }
}

void destroyCollege()
{
    for (int t = 0; t < numTeachers; ++t)
    {
        ClassNode *cur = college[t].firstChild;
        while (cur)
        {
            ClassNode *next = cur->nextSibling;
            free(cur->studentArray);
            free(cur);
            cur = next;
        }
        college[t].firstChild = NULL;
    }
    numTeachers = 0;
}

