#ifndef QUERIES_H
#define QUERIES_H

#include <QString>

namespace Queries {
    // User queries
    const QString INSERT_USER = "INSERT INTO users (full_name, username, password, role) VALUES (?, ?, ?, ?)";
    const QString SELECT_USER_BY_USERNAME = "SELECT * FROM users WHERE username = ?";
    const QString SELECT_USER_BY_ID = "SELECT * FROM users WHERE id = ?";
    const QString SELECT_ALL_USERS = "SELECT * FROM users ORDER BY created_at DESC";
    const QString UPDATE_USER = "UPDATE users SET full_name = ?, password = ?, role = ? WHERE id = ?";
    const QString DELETE_USER = "DELETE FROM users WHERE id = ?";
	const QString Update_User_Password = "UPDATE users SET password = ? WHERE id = ?";

    // Student Data queries
    const QString INSERT_STUDENT_DATA = "INSERT INTO students_data (user_id, student_number, id_number, dob, year, department, section_id, seat_number, status) "
                                        "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?)";
    const QString UPDATE_STUDENT_DATA = "UPDATE students_data SET id_number = ?, dob = ?, year = ?, department = ?, section_id = ?, seat_number = ?, status = ? "
                                        "WHERE id = ?";
    const QString DELETE_STUDENT_DATA = "DELETE FROM students_data WHERE id = ?";
    const QString SELECT_ALL_STUDENTS_DATA = "SELECT sd.*, u.full_name, u.username FROM students_data sd "
                                             "JOIN users u ON sd.user_id = u.id ORDER BY sd.student_number";
    const QString SELECT_STUDENT_DATA_BY_ID = "SELECT sd.*, u.full_name, u.username FROM students_data sd "
                                              "JOIN users u ON sd.user_id = u.id WHERE sd.id = ?";
    const QString SELECT_STUDENT_DATA_BY_USER_ID = "SELECT * FROM students_data WHERE user_id = ?";
    const QString SELECT_STUDENT_DATA_BY_STUDENT_NUMBER = "SELECT sd.*, u.full_name, u.username FROM students_data sd "
		"JOIN users u ON sd.user_id = u.id WHERE sd.student_number = ?";

    // Semester queries
    const QString INSERT_SEMESTER = "INSERT INTO semester (year, semester) VALUES (?, ?)";
    const QString UPDATE_SEMESTER = "UPDATE semester SET year = ?, semester = ? WHERE id = ?";
    const QString DELETE_SEMESTER = "DELETE FROM semester WHERE id = ?";
    const QString SELECT_ALL_SEMESTERS = "SELECT * FROM semester ORDER BY year DESC, semester DESC";

    // Course queries
    const QString INSERT_COURSE = "INSERT INTO courses (name, description, year_level, credit_hours, semester_id) VALUES (?, ?, ?, ?, ?)";
    const QString UPDATE_COURSE = "UPDATE courses SET name = ?, description = ?, year_level = ?, credit_hours = ?, semester_id = ? WHERE id = ?";
    const QString DELETE_COURSE = "DELETE FROM courses WHERE id = ?";
    const QString SELECT_ALL_COURSES = "SELECT c.*, s.year as semester_year, s.semester as semester_number FROM courses c "
                                       "LEFT JOIN semester s ON c.semester_id = s.id ORDER BY c.name";
    const QString SELECT_COURSES_BY_SEMESTER = "SELECT * FROM courses WHERE semester_id = ?";
    const QString SELECT_COURSE_BY_ID = "SELECT c.*, s.year as semester_year, s.semester as semester_number FROM courses c "
                                        "LEFT JOIN semester s ON c.semester_id = s.id WHERE c.id = ?";

    // Section queries
    const QString INSERT_SECTION = "INSERT INTO sections (course_id, capacity, semester_id) VALUES (?, ?, ?)";
    const QString UPDATE_SECTION = "UPDATE sections SET course_id = ?, capacity = ?, semester_id = ? WHERE id = ?";
    const QString DELETE_SECTION = "DELETE FROM sections WHERE id = ?";
    const QString SELECT_ALL_SECTIONS = "SELECT sec.*, c.name as course_name FROM sections sec "
                                        "JOIN courses c ON sec.course_id = c.id ORDER BY c.name";
    const QString SELECT_SECTIONS_BY_COURSE = "SELECT * FROM sections WHERE course_id = ?";

    // Enrollment queries
    const QString INSERT_ENROLLMENT = "INSERT INTO enrollments (student_id, status, assessments_grades, Assignment_1, Assignment_2, final, total_marks, is_rafaa_applied) "
                                      "VALUES (?, ?, ?, ?, ?, ?, ?, ?)";
    const QString UPDATE_ENROLLMENT = "UPDATE enrollments SET status = ?, assessments_grades = ?, Assignment_1 = ?, Assignment_2 = ?, final = ?, total_marks = ?, is_rafaa_applied = ? "
                                      "WHERE id = ?";
    const QString DELETE_ENROLLMENT = "DELETE FROM enrollments WHERE id = ?";
    const QString SELECT_ENROLLMENTS_BY_STUDENT = "SELECT * FROM enrollments WHERE student_id = ?";

    // Payment queries
    const QString INSERT_PAYMENT = "INSERT INTO payments (student_id, amount, date, year, method, status, notes) VALUES (?, ?, ?, ?, ?, ?, ?)";
    const QString UPDATE_PAYMENT = "UPDATE payments SET amount = ?, method = ?, status = ?, notes = ? WHERE id = ?";
    const QString DELETE_PAYMENT = "DELETE FROM payments WHERE id = ?";
    const QString SELECT_PAYMENTS_BY_STUDENT = "SELECT * FROM payments WHERE student_id = ? ORDER BY date DESC";
    const QString SELECT_ALL_PAYMENTS = "SELECT p.*, s.student_number FROM payments p JOIN students_data s ON p.student_id = s.id ORDER BY p.date DESC";

    // News queries
    const QString INSERT_NEWS = "INSERT INTO news (title, body) VALUES (?, ?)";
    const QString UPDATE_NEWS = "UPDATE news SET title = ?, body = ? WHERE id = ?";
    const QString DELETE_NEWS = "DELETE FROM news WHERE id = ?";
    const QString SELECT_ALL_NEWS = "SELECT * FROM news ORDER BY created_at DESC";

    // Helper / Advanced Queries
    const QString SELECT_ENROLLMENTS_BY_SECTION = "SELECT e.* FROM enrollments e "
                                                  "JOIN students_data s ON e.student_id = s.id "
                                                  "WHERE s.section_id = ?";
    const QString SELECT_USERS_BY_ROLE = "SELECT * FROM users WHERE role = ?";
    const QString SELECT_STUDENTS_BY_SEARCH = "SELECT sd.*, u.full_name, u.username FROM students_data sd "
                                              "JOIN users u ON sd.user_id = u.id "
                                              "WHERE u.full_name LIKE ? OR sd.student_number LIKE ?";
}

#endif // QUERIES_H
