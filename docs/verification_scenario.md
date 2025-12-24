# Verification Scenario: Auto-Enrollment & Dashboard Visibility

This scenario verifies that the new "Auto-Enrollment" feature works correctly and that students appear in the Professor Dashboard as expected.

## Prerequisites

1.  **Admin Account**: Logged in as an Administrator.
2.  **Professor Account**: A registered professor exists (e.g., "Dr. Smith").
3.  **Academic Year**: "Year 1" exists in the system.

## Step-by-Step Scenario

### Phase 1: Setup (Admin Panel)

1.  **Create/Verify Course**:
    - Navigate to **Courses** tab.
    - Click **Create New Course**.
    - **Name**: `Introduction to CS`
    - **Year Level**: Select `Year 1` (Important!). (Previously this might have been an ID, now it should show the Name).
    - **Assign Professor**: Select `Dr. Smith`.
    - Save the course.
    - _Expected Result_: Course appears in the table. "Level" column shows "Year 1".

2.  **Register New Student**:
    - Navigate to **Students** tab.
    - Click **Register Student**.
    - **Name**: `Alice Test`
    - **Student Number**: `ST-2024-001`
    - **Academic Year**: Select `Year 1` (This matches the course).
    - Fill in other required fields (ID, etc.).
    - Click **Register**.
    - _Expected Result_: Student is registered successfully.
    - _Behind the Scenes_: The system should have automatically enrolled Alice in "Introduction to CS".

### Phase 2: Verification (Professor Dashboard)

3.  **Login as Professor**:
    - Logout or open a new instance.
    - Login as `Dr. Smith` (using their credentials, usually Code/Code).

4.  **Check Enrollments**:
    - Navigate to **Dashboard** (Grades) tab.
    - **Year Selector**: Select `Year 1`.
    - **Course Selector**: Select `Introduction to CS`.
    - _Expected Result_: **Alice Test** should appear in the student list.

5.  **Check Attendance Tab**:
    - Navigate to **Attendance** tab.
    - Select `Year 1` and `Introduction to CS`.
    - _Expected Result_: **Alice Test** should appear in the attendance list.

### Phase 3: Negative Test (Admin Panel)

6.  **Register Out-of-Level Student**:
    - Login as Admin.
    - Register **Bob Test**.
    - **Academic Year**: Select `Year 2`.
    - Save.

7.  **Verify Non-Enrollment**:
    - Login as Dr. Smith.
    - Select `Year 1` -> `Introduction to CS`.
    - _Expected Result_: **Bob Test** should **NOT** appear (because he is Year 2, and the course is Year 1).

## Success Criteria

- [ ] Alice (Year 1) is visible in Dr. Smith's Year 1 Course.
- [ ] Bob (Year 2) is NOT visible in Dr. Smith's Year 1 Course.
- [ ] No manual enrollment steps were required by the Admin.
