-- Database Diagnostic and Test Data Script
-- Run this in your MySQL database to check structure and add test data

USE unimanage;

-- 1. Check if tables exist
SHOW TABLES;

-- 2. Check users table structure
DESCRIBE users;

-- 3. Check students_data table structure
DESCRIBE students_data;

-- 4. Count records in each table
SELECT 'Users Table' as TableName, COUNT(*) as RecordCount FROM users
UNION ALL
SELECT 'Students Data Table', COUNT(*) FROM students_data
UNION ALL
SELECT 'Users with role=student', COUNT(*) FROM users WHERE role='student';

-- 5. Show all users with role='student'
SELECT id, full_name, username, role, created_at 
FROM users 
WHERE role='student'
ORDER BY id;

-- 6. Show all student profiles
SELECT sd.id, sd.user_id, sd.student_number, sd.id_number, u.full_name, u.role
FROM students_data sd
LEFT JOIN users u ON sd.user_id = u.id
ORDER BY sd.id;

-- 7. Find orphaned student users (users with role='student' but no profile)
SELECT u.id, u.full_name, u.username, u.role
FROM users u
LEFT JOIN students_data sd ON u.id = sd.user_id
WHERE u.role='student' AND sd.id IS NULL;

-- 8. Test the exact query used by the application
SELECT u.id as user_id, u.full_name, u.username, u.role, sd.id, 
COALESCE(sd.student_number, u.username) as student_number, 
sd.id_number, sd.phone, sd.dob, sd.department, sd.department_id, sd.academic_level_id, 
sd.section_id, sd.seat_number, sd.status, sd.created_at, sd.updated_at, 
d.name as dept_name, al.name as level_name 
FROM users u 
LEFT JOIN students_data sd ON u.id = sd.user_id 
LEFT JOIN departments d ON sd.department_id = d.id 
LEFT JOIN academic_levels al ON sd.academic_level_id = al.id 
WHERE u.role = 'student' 
ORDER BY student_number;

-- 9. INSERT TEST DATA (only if no students exist)
-- First, check if we need test data
SET @student_count = (SELECT COUNT(*) FROM users WHERE role='student');

-- If no students exist, create a test student
-- Note: You may need to run these INSERT statements manually if the conditional doesn't work

-- Test Student 1 (Complete profile)
INSERT INTO users (full_name, username, password, role) 
VALUES ('Test Student One', 'TEST001', SHA2('TEST001', 256), 'student');

SET @test_user_id = LAST_INSERT_ID();

INSERT INTO students_data (user_id, student_number, id_number, phone, status) 
VALUES (@test_user_id, 'TEST001', '12345678901234', '01234567890', 'active');

-- Test Student 2 (Orphaned - user only, no profile)
INSERT INTO users (full_name, username, password, role) 
VALUES ('Test Student Two (Orphaned)', 'TEST002', SHA2('TEST002', 256), 'student');

-- Verify test data was inserted
SELECT 'Test data inserted. Verify:' as Message;
SELECT u.id, u.full_name, u.username, u.role, sd.id as profile_id
FROM users u
LEFT JOIN students_data sd ON u.id = sd.user_id
WHERE u.username LIKE 'TEST%';
