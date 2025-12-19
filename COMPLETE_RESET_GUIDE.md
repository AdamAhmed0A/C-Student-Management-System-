# Complete Database Reset and Application Fix Guide

## 🎯 This Will Fix Everything

This guide will:
1. ✅ Drop and recreate the database with correct schema
2. ✅ Fix all student display issues
3. ✅ Fix add/edit/delete functionality for students
4. ✅ Fix add/edit/delete functionality for professors
5. ✅ Create default admin user and test data

## 📋 Step-by-Step Instructions

### Step 1: Reset the Database

**Run the SQL script:**
```batch
mysql -u root -p < C:\Users\Adam\C-Student-Management-System-\reset_database.sql
```

**Or in MySQL Workbench:**
1. Open MySQL Workbench
2. Connect to your MySQL server
3. File → Open SQL Script
4. Select `reset_database.sql`
5. Execute (⚡ button or Ctrl+Shift+Enter)

**What this does:**
- Drops the old `university` database
- Creates a fresh `university` database
- Creates all tables with correct schema (NO phone column)
- Sets up proper foreign keys and indexes
- Creates default admin user (username: `admin`, password: `admin123`)
- Creates academic levels (First Year, Second Year, etc.)
- Creates sample colleges and departments

### Step 2: Rebuild the Application

```batch
cd C:\Users\Adam\C-Student-Management-System-\UniManage
qmake
mingw32-make
```

Or use Qt Creator:
1. Open the project
2. Build → Rebuild All
3. Run

### Step 3: Test the Application

1. **Login**
   - Username: `admin`
   - Password: `admin123`

2. **Click "Test Database" Button**
   - Should show: ✓ Database is CONNECTED
   - Should show: ✓ Total users: 1 (the admin)
   - Should show: ✓ Users with role='student': 0
   - Should show: ✓ Query executed successfully!

3. **Go to Students Tab**
   - Should be empty (no students yet)
   - This is CORRECT - we start fresh

### Step 4: Test Adding a Student

1. **Click "Add Student" button**
2. **Fill in the form:**
   - Full Name: `Test Student`
   - Student Code: `TEST001`
   - National ID: `12345678901234` (14 digits)
   - Select a Department
   - Select an Academic Level
3. **Click "Register Student"**
4. **Expected Result:**
   - ✅ Success message
   - ✅ Student appears in the table
   - ✅ Can see: ID, Student#, Name, National ID, Department, Level, Status

### Step 5: Test Editing a Student

1. **Select the student** in the table (click on the row)
2. **Click "Edit Student" button**
3. **Modify some fields**
4. **Click "Save Changes"**
5. **Expected Result:**
   - ✅ Success message
   - ✅ Changes reflected in the table

### Step 6: Test Adding a Professor

1. **Go to "Professor Data" tab**
2. **Click "Add Professor" button**
3. **Fill in the form:**
   - Full Name: `Test Professor`
   - National ID: `98765432109876` (14 digits)
   - Specialization: `Computer Science`
   - Title: `Professor`
4. **Click "Register Professor"**
5. **Expected Result:**
   - ✅ Success message
   - ✅ Professor appears in the table

### Step 7: Test Editing a Professor

1. **Select the professor** in the table
2. **Click "Edit Professor" button**
3. **Modify fields**
4. **Click "Save Changes"**
5. **Expected Result:**
   - ✅ Success message
   - ✅ Changes reflected in the table

### Step 8: Test Deleting

1. **Select a student or professor**
2. **Click "Delete" button**
3. **Confirm deletion**
4. **Expected Result:**
   - ✅ Success message
   - ✅ Record removed from table

## 🔧 What Was Fixed

### Database Schema Issues:
- ❌ **Old**: Had `phone` column that didn't exist
- ✅ **New**: Correct schema without phone column
- ✅ **New**: Proper foreign keys and cascading deletes
- ✅ **New**: Indexes for better performance

### Query Issues:
- ❌ **Old**: Query tried to SELECT `sd.phone`
- ✅ **New**: Query matches actual schema
- ✅ **New**: Uses LEFT JOIN to show all students
- ✅ **New**: Handles NULL values properly

### Functionality Issues:
- ✅ **Student Add**: Creates user + profile in transaction
- ✅ **Student Edit**: Updates profile data
- ✅ **Student Delete**: Cascades to remove user
- ✅ **Professor Add**: Creates user + profile
- ✅ **Professor Edit**: Updates profile data
- ✅ **Professor Delete**: Cascades to remove user

## 📊 Database Structure

### Tables Created:
1. `users` - All system users (admin, professors, students)
2. `students_data` - Student profiles
3. `professors` - Professor profiles
4. `colleges` - Faculties
5. `departments` - Academic departments
6. `academic_levels` - Year levels
7. `semester` - Academic semesters
8. `courses` - Course catalog
9. `sections` - Course sections
10. `enrollments` - Student enrollments
11. `rooms` - Classrooms and labs
12. `room_specs` - Room equipment
13. `schedules` - Class schedules
14. `payments` - Student payments

### Default Data:
- ✅ 1 Admin user (admin/admin123)
- ✅ 4 Academic levels (First-Fourth Year)
- ✅ 1 Current semester
- ✅ 3 Sample colleges
- ✅ 4 Sample departments

## 🎯 Expected Behavior After Reset

### Students Tab:
- **Empty initially** ✓
- **Add button works** ✓
- **Students appear after adding** ✓
- **Edit button works** ✓
- **Delete button works** ✓
- **Refresh updates the list** ✓

### Professors Tab:
- **Empty initially** ✓
- **Add button works** ✓
- **Professors appear after adding** ✓
- **Edit button works** ✓
- **Delete button works** ✓

### Test Database Button:
- **Shows connection status** ✓
- **Shows accurate counts** ✓
- **Query executes successfully** ✓
- **No orphaned records** ✓

## 🐛 Troubleshooting

### If students still don't show:
1. Click "Test Database" button
2. Check the query result
3. Share the output with me

### If add fails:
1. Check console for error messages
2. Verify department and level are selected
3. Ensure National ID is 14 digits
4. Check that Student Code is unique

### If database connection fails:
1. Verify MySQL is running
2. Check .env file has correct credentials
3. Ensure database name is `university`

## ✅ Success Criteria

After following these steps, you should have:
- ✅ Fresh database with correct schema
- ✅ Working student add/edit/delete
- ✅ Working professor add/edit/delete
- ✅ No query errors
- ✅ All students visible in the table
- ✅ Test Database button shows accurate info

## 🎉 You're Done!

The application should now work perfectly. You can:
- Add students and professors
- Edit their information
- Delete records
- View all data in tables
- Use the Test Database button to verify everything

If you encounter any issues, run "Test Database" and share the results!
