#ifndef ATTENDANCE_UTILS_H
#define ATTENDANCE_UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TEACHERS 50
#define INITIAL_STUDENT_CAPACITY 10  

typedef struct {
    char name[100];
    char srn[20];
    int totalDays;      
    int presentDays;    
} Student;

typedef struct ClassNode {
    char sectionName[50];
    int numStudents;
    int capacity;                  
    Student* studentArray;         
    struct ClassNode* nextSibling; 
} ClassNode;


typedef struct {
    char teacherName[100];
    ClassNode* firstChild; 
} TeacherNode;

extern TeacherNode college[MAX_TEACHERS];
extern int numTeachers;

TeacherNode* findTeacherNode(const char* teacherName);
ClassNode* findClassNode(TeacherNode* teacher, const char* sectionName);
ClassNode* createClassNode(const char* sectionName);
void expandStudentArray(ClassNode* classNode);
void destroyCollege();
int addTeacher(const char* teacherName);
void addStudent(const char* teacherName, const char* sectionName, const char* studentName, const char* srn);
void displayRoster(const char* teacherName, const char* sectionName);
void markAttendance(const char* teacherName, const char* sectionName);
void generateLowAttendanceReport(float threshold);
void displayCollege();  
void initializeCollege();
int srnValidation(TeacherNode *teacher, const char *sectionName, const char *srn);

#endif 
