-- Fix for Orphaned Student Users
-- Run this in your MySQL database to create profiles for the 3 orphaned students

USE university;

-- Check current state
SELECT 'BEFORE FIX - Orphaned Students:' as Status;
SELECT u.id, u.full_name, u.username, u.role
FROM users u
LEFT JOIN students_data sd ON u.id = sd.user_id
WHERE u.role='student' AND sd.id IS NULL;

-- Create student profiles for orphaned users
-- Student 1: Adam (ID: 2, Username: 651)
INSERT INTO students_data (user_id, student_number, id_number, status)
VALUES (2, '651', '', 'active')
ON DUPLICATE KEY UPDATE student_number = '651';

-- Student 2: Adam (ID: 3, Username: 516)
INSERT INTO students_data (user_id, student_number, id_number, status)
VALUES (3, '516', '', 'active')
ON DUPLICATE KEY UPDATE student_number = '516';

-- Student 3: Adam (ID: 9, Username: 20240594)
INSERT INTO students_data (user_id, student_number, id_number, status)
VALUES (9, '20240594', '', 'active')
ON DUPLICATE KEY UPDATE student_number = '20240594';

-- Verify the fix
SELECT 'AFTER FIX - All Students:' as Status;
SELECT u.id as user_id, u.full_name, u.username, u.role, sd.id as profile_id, sd.student_number, sd.status
FROM users u
LEFT JOIN students_data sd ON u.id = sd.user_id
WHERE u.role='student'
ORDER BY u.id;

-- Check if they now appear in the application query
SELECT 'APPLICATION QUERY RESULT:' as Status;
SELECT u.id as user_id, u.full_name, u.username, u.role, sd.id, 
COALESCE(sd.student_number, u.username) as student_number, 
sd.id_number, sd.dob, sd.department, sd.department_id, sd.academic_level_id, 
sd.section_id, sd.seat_number, sd.status, sd.created_at, sd.updated_at, 
d.name as dept_name, al.name as level_name 
FROM users u 
LEFT JOIN students_data sd ON u.id = sd.user_id 
LEFT JOIN departments d ON sd.department_id = d.id 
LEFT JOIN academic_levels al ON sd.academic_level_id = al.id 
WHERE u.role = 'student' 
ORDER BY student_number;
