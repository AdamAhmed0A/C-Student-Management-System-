-- Complete Database Reset and Recreation Script
-- This will drop and recreate the entire database with correct schema

-- Drop existing database
DROP DATABASE IF EXISTS university;

-- Create fresh database
CREATE DATABASE university CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci;

USE university;

-- ============================================
-- CORE TABLES
-- ============================================

-- Users table (for all system users)
CREATE TABLE users (
    id INT AUTO_INCREMENT PRIMARY KEY,
    full_name VARCHAR(255) NOT NULL,
    username VARCHAR(100) NOT NULL UNIQUE,
    password VARCHAR(255) NOT NULL,
    role ENUM('admin', 'professor', 'student') NOT NULL,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    updated_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
    INDEX idx_username (username),
    INDEX idx_role (role)
) ENGINE=InnoDB;

-- Colleges/Faculties
CREATE TABLE colleges (
    id INT AUTO_INCREMENT PRIMARY KEY,
    name VARCHAR(255) NOT NULL,
    code VARCHAR(50) UNIQUE,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
) ENGINE=InnoDB;

-- Departments
CREATE TABLE departments (
    id INT AUTO_INCREMENT PRIMARY KEY,
    name VARCHAR(255) NOT NULL,
    code VARCHAR(50),
    college_id INT,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY (college_id) REFERENCES colleges(id) ON DELETE SET NULL,
    INDEX idx_college (college_id)
) ENGINE=InnoDB;

-- Academic Levels (First Year, Second Year, etc.)
CREATE TABLE academic_levels (
    id INT AUTO_INCREMENT PRIMARY KEY,
    name VARCHAR(100) NOT NULL,
    level_number INT,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
) ENGINE=InnoDB;

-- Semesters
CREATE TABLE semester (
    id INT AUTO_INCREMENT PRIMARY KEY,
    year DATE NOT NULL,
    semester INT NOT NULL,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
) ENGINE=InnoDB;

-- ============================================
-- STUDENT TABLES
-- ============================================

-- Student Data (profiles)
CREATE TABLE students_data (
    id INT AUTO_INCREMENT PRIMARY KEY,
    user_id INT NOT NULL UNIQUE,
    student_number VARCHAR(50) NOT NULL UNIQUE,
    id_number VARCHAR(14),
    dob DATE,
    department VARCHAR(255),
    department_id INT,
    academic_level_id INT,
    section_id INT,
    seat_number VARCHAR(50),
    status VARCHAR(50) DEFAULT 'active',
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    updated_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
    FOREIGN KEY (user_id) REFERENCES users(id) ON DELETE CASCADE,
    FOREIGN KEY (department_id) REFERENCES departments(id) ON DELETE SET NULL,
    FOREIGN KEY (academic_level_id) REFERENCES academic_levels(id) ON DELETE SET NULL,
    INDEX idx_user (user_id),
    INDEX idx_student_number (student_number),
    INDEX idx_department (department_id),
    INDEX idx_level (academic_level_id)
) ENGINE=InnoDB;

-- ============================================
-- PROFESSOR TABLES
-- ============================================

-- Professor Data
CREATE TABLE professors (
    id INT AUTO_INCREMENT PRIMARY KEY,
    user_id INT NOT NULL UNIQUE,
    specialization VARCHAR(255),
    title VARCHAR(100),
    personal_info TEXT,
    id_number VARCHAR(14),
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    updated_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
    FOREIGN KEY (user_id) REFERENCES users(id) ON DELETE CASCADE,
    INDEX idx_user (user_id),
    INDEX idx_id_number (id_number)
) ENGINE=InnoDB;

-- ============================================
-- COURSE TABLES
-- ============================================

-- Courses
CREATE TABLE courses (
    id INT AUTO_INCREMENT PRIMARY KEY,
    name VARCHAR(255) NOT NULL,
    course_code VARCHAR(50),
    course_type VARCHAR(50),
    max_grade INT DEFAULT 100,
    credit_hours INT,
    year_level INT,
    semester_id INT,
    description TEXT,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY (semester_id) REFERENCES semester(id) ON DELETE SET NULL,
    INDEX idx_semester (semester_id)
) ENGINE=InnoDB;

-- Sections
CREATE TABLE sections (
    id INT AUTO_INCREMENT PRIMARY KEY,
    course_id INT NOT NULL,
    capacity INT,
    semester_id INT,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY (course_id) REFERENCES courses(id) ON DELETE CASCADE,
    FOREIGN KEY (semester_id) REFERENCES semester(id) ON DELETE SET NULL
) ENGINE=InnoDB;

-- Enrollments
CREATE TABLE enrollments (
    id INT AUTO_INCREMENT PRIMARY KEY,
    student_id INT NOT NULL,
    section_id INT NOT NULL,
    grade DECIMAL(5,2),
    status VARCHAR(50) DEFAULT 'enrolled',
    enrolled_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY (student_id) REFERENCES students_data(id) ON DELETE CASCADE,
    FOREIGN KEY (section_id) REFERENCES sections(id) ON DELETE CASCADE,
    UNIQUE KEY unique_enrollment (student_id, section_id)
) ENGINE=InnoDB;

-- ============================================
-- FACILITY TABLES
-- ============================================

-- Rooms
CREATE TABLE rooms (
    id INT AUTO_INCREMENT PRIMARY KEY,
    name VARCHAR(255) NOT NULL,
    capacity INT,
    room_type VARCHAR(50),
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
) ENGINE=InnoDB;

-- Room Specifications/Equipment
CREATE TABLE room_specs (
    id INT AUTO_INCREMENT PRIMARY KEY,
    room_id INT NOT NULL,
    product_id INT,
    product_name VARCHAR(255),
    product_description TEXT,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY (room_id) REFERENCES rooms(id) ON DELETE CASCADE
) ENGINE=InnoDB;

-- ============================================
-- SCHEDULE TABLES
-- ============================================

-- Schedules
CREATE TABLE schedules (
    id INT AUTO_INCREMENT PRIMARY KEY,
    course_id INT NOT NULL,
    room_id INT,
    professor_id INT,
    day_of_week VARCHAR(20),
    start_time TIME,
    end_time TIME,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY (course_id) REFERENCES courses(id) ON DELETE CASCADE,
    FOREIGN KEY (room_id) REFERENCES rooms(id) ON DELETE SET NULL,
    FOREIGN KEY (professor_id) REFERENCES professors(id) ON DELETE SET NULL
) ENGINE=InnoDB;

-- ============================================
-- PAYMENT TABLES
-- ============================================

-- Payments
CREATE TABLE payments (
    id INT AUTO_INCREMENT PRIMARY KEY,
    student_id INT NOT NULL,
    amount DECIMAL(10,2) NOT NULL,
    payment_date DATE,
    payment_type VARCHAR(50),
    status VARCHAR(50) DEFAULT 'pending',
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY (student_id) REFERENCES students_data(id) ON DELETE CASCADE
) ENGINE=InnoDB;

-- ============================================
-- INSERT DEFAULT DATA
-- ============================================

-- Create default admin user (password: admin123)
INSERT INTO users (full_name, username, password, role) 
VALUES ('System Administrator', 'admin', SHA2('admin123', 256), 'admin');

-- Create academic levels
INSERT INTO academic_levels (name, level_number) VALUES
('First Year', 1),
('Second Year', 2),
('Third Year', 3),
('Fourth Year', 4);

-- Create default semester
INSERT INTO semester (year, semester) VALUES
(CURDATE(), 1);

-- Create sample college
INSERT INTO colleges (name, code) VALUES
('Faculty of Engineering', 'ENG'),
('Faculty of Science', 'SCI'),
('Faculty of Medicine', 'MED');

-- Create sample departments
INSERT INTO departments (name, code, college_id) VALUES
('Computer Science', 'CS', 1),
('Electrical Engineering', 'EE', 1),
('Physics', 'PHY', 2),
('Chemistry', 'CHEM', 2);

-- ============================================
-- VERIFICATION QUERIES
-- ============================================

SELECT 'Database created successfully!' as Status;
SELECT 'Tables created:' as Info;
SHOW TABLES;

SELECT 'Default admin user created:' as Info;
SELECT id, full_name, username, role FROM users WHERE role='admin';

SELECT 'Academic levels created:' as Info;
SELECT * FROM academic_levels;

SELECT 'Colleges created:' as Info;
SELECT * FROM colleges;

SELECT 'Departments created:' as Info;
SELECT * FROM departments;
