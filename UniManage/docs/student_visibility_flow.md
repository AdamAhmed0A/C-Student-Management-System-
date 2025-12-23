# Student Visibility Flow Guide

This document explains the correct workflow to ensure students appear correctly in the Professor Dashboard.

## 1. Setup Academic Foundation (Admin)
Before adding students or professors, ensure the underlying structure exists:
1.  **Create Academic Levels**: Go to `Admin Panel > Administration > Academic Years` and define years (e.g., Year 1, Year 2).
2.  **Create Courses**: Go to `Courses`. When creating a course:
    *   **Assign to an Academic Year** (Level). This determines which students take this course.
    *   **Assign a Professor**. Only this professor will see students enrolled in this course.

## 2. Register Students (Admin)
When you add a new student (`Students > Register Student`):
1.  **Select the Academic Year** (Level) (e.g., Year 1).
2.  **Auto-Enrollment**: The system now **automatically enrolls** the student in ALL courses assigned to that Academic Year.
    *   *Example*: If "Math 101" is a "Year 1" course, any new student created in "Year 1" will be instantly enrolled.

## 3. Professor Dashboard Visibility
For a student to appear in the Professor's "My Students" list:
1.  The **Professor** must be assigned to teach a **Course** (e.g., Math 101).
2.  The **Student** must be enrolled in that **Course**.
    *   *Note*: This happens automatically if the Student and Course share the same **Academic Year** (Level).

## Troubleshooting: Why is a student not showing?
*   **Mismatched Year**: Did you create the Course in "Year 1" but the Student in "Year 2"?
*   **No Professor Assigned**: Is the course linked to **You** (the logged-in professor)?
*   **Legacy Data**: Students created *before* the auto-enrollment update (Active as of Dec 2025 updates) might not be enrolled. 
    *   *Fix*: Edit the student's profile (save again) or manually re-add them to trigger enrollment logic (if implemented in Edit), or simply rely on new registrations.

## Summary Checklist
- [x] Course is created.
- [x] Course has a Professor assigned.
- [x] Course is linked to a Year Level.
- [x] Student is created and linked to the SAME Year Level.
