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
    const QString INSERT_STUDENT_DATA = "INSERT INTO students_data (user_id, student_number, id_number, dob, department, department_id, academic_level_id, section_id, college_id, tuition_fees, seat_number, status) "
                                        "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)";
    const QString UPDATE_STUDENT_DATA = "UPDATE students_data SET id_number = ?, dob = ?, department = ?, department_id = ?, academic_level_id = ?, section_id = ?, college_id = ?, tuition_fees = ?, seat_number = ?, status = ? "
                                        "WHERE id = ?";
    const QString DELETE_STUDENT_DATA = "DELETE FROM students_data WHERE id = ?";
    const QString SOFT_DELETE_STUDENT_DATA = "UPDATE students_data SET status = 'deleted' WHERE id = ?";
    const QString RESTORE_STUDENT_DATA = "UPDATE students_data SET status = 'active' WHERE id = ?";
    const QString SELECT_ALL_STUDENTS_DATA = "SELECT u.id AS user_id, u.full_name, u.username, u.role, sd.id, "
                                             "COALESCE(sd.student_number, u.username) AS student_number, "
                                             "sd.id_number, sd.dob, sd.department, sd.department_id, sd.academic_level_id, "
                                             "sd.section_id, sd.college_id, sd.tuition_fees, sd.seat_number, sd.status, sd.created_at, sd.updated_at, "
                                             "d.name AS dept_name, al.name AS level_name, col.name AS college_name, "
                                             "sec.name AS section_name, "
                                             "c_sec.name AS section_course_name "
                                             "FROM users u "
                                             "LEFT JOIN students_data sd ON u.id = sd.user_id "
                                             "LEFT JOIN departments d ON sd.department_id = d.id "
                                             "LEFT JOIN academic_levels al ON sd.academic_level_id = al.id "
                                             "LEFT JOIN colleges col ON sd.college_id = col.id "
                                             "LEFT JOIN sections sec ON sd.section_id = sec.id "
                                             "LEFT JOIN courses c_sec ON sec.course_id = c_sec.id "
                                             "WHERE u.role = 'student' AND sd.id IS NOT NULL AND sd.status != 'deleted' "
                                             "ORDER BY u.id";
    const QString SELECT_DELETED_STUDENTS_DATA = "SELECT u.id AS user_id, u.full_name, u.username, u.role, sd.id, "
                                                 "COALESCE(sd.student_number, u.username) AS student_number, "
                                                 "sd.id_number, sd.dob, sd.department, sd.department_id, sd.academic_level_id, "
                                                 "sd.section_id, sd.college_id, sd.tuition_fees, sd.seat_number, sd.status, sd.created_at, sd.updated_at, "
                                                 "d.name AS dept_name, al.name AS level_name, col.name AS college_name, "
                                                 "sec.name AS section_name, "
                                                 "c_sec.name AS section_course_name "
                                                 "FROM users u "
                                                 "JOIN students_data sd ON u.id = sd.user_id "
                                                 "LEFT JOIN departments d ON sd.department_id = d.id "
                                                 "LEFT JOIN academic_levels al ON sd.academic_level_id = al.id "
                                                 "LEFT JOIN colleges col ON sd.college_id = col.id "
                                                 "LEFT JOIN sections sec ON sd.section_id = sec.id "
                                                 "LEFT JOIN courses c_sec ON sec.course_id = c_sec.id "
                                                 "WHERE u.role = 'student' AND sd.status = 'deleted' "
                                                 "ORDER BY sd.updated_at DESC";
    const QString SELECT_STUDENT_COMMON_JOIN = "FROM users u "
                                               "LEFT JOIN students_data sd ON u.id = sd.user_id "
                                               "LEFT JOIN departments d ON sd.department_id = d.id "
                                               "LEFT JOIN academic_levels al ON sd.academic_level_id = al.id "
                                               "LEFT JOIN colleges col ON sd.college_id = col.id "
                                               "LEFT JOIN sections sec ON sd.section_id = sec.id ";

    const QString SELECT_STUDENT_DATA_BY_ID = "SELECT u.id AS user_id, u.full_name, u.username, u.role, sd.id, "
                                              "COALESCE(sd.student_number, u.username) AS student_number, "
                                              "sd.id_number, sd.dob, sd.department, sd.department_id, sd.academic_level_id, "
                                              "sd.section_id, sd.college_id, sd.tuition_fees, sd.seat_number, sd.status, sd.created_at, sd.updated_at, "
                                              "d.name AS dept_name, al.name AS level_name, col.name AS college_name, sec.name AS section_name "
                                              + SELECT_STUDENT_COMMON_JOIN + " WHERE sd.id = ?";

    const QString SELECT_STUDENT_DATA_BY_USER_ID = "SELECT u.id AS user_id, u.full_name, u.username, u.role, sd.id, "
                                                   "COALESCE(sd.student_number, u.username) AS student_number, "
                                                   "sd.id_number, sd.dob, sd.department, sd.department_id, sd.academic_level_id, "
                                                   "sd.section_id, sd.college_id, sd.tuition_fees, sd.seat_number, sd.status, sd.created_at, sd.updated_at, "
                                                   "d.name AS dept_name, al.name AS level_name, col.name AS college_name, sec.name AS section_name "
                                                   + SELECT_STUDENT_COMMON_JOIN + " WHERE u.id = ?";

    const QString SELECT_STUDENT_DATA_BY_STUDENT_NUMBER = "SELECT u.id AS user_id, u.full_name, u.username, u.role, sd.id, "
                                                          "COALESCE(sd.student_number, u.username) AS student_number, "
                                                          "sd.id_number, sd.dob, sd.department, sd.department_id, sd.academic_level_id, "
                                                          "sd.section_id, sd.college_id, sd.tuition_fees, sd.seat_number, sd.status, sd.created_at, sd.updated_at, "
                                                          "d.name AS dept_name, al.name AS level_name, col.name AS college_name, sec.name AS section_name "
                                                          + SELECT_STUDENT_COMMON_JOIN + " WHERE sd.student_number = ?";

    const QString SELECT_STUDENT_BY_ID_NUMBER = "SELECT u.id AS user_id, u.full_name, u.username, u.role, sd.id, "
                                                "COALESCE(sd.student_number, u.username) AS student_number, "
                                                "sd.id_number, sd.dob, sd.department, sd.department_id, sd.academic_level_id, "
                                                "sd.section_id, sd.college_id, sd.tuition_fees, sd.seat_number, sd.status, sd.created_at, sd.updated_at, "
                                                "d.name AS dept_name, al.name AS level_name, col.name AS college_name, sec.name AS section_name "
                                                + SELECT_STUDENT_COMMON_JOIN + " WHERE sd.id_number = ?";

    // Semester queries
    const QString INSERT_SEMESTER = "INSERT INTO semester (year, semester) VALUES (?, ?)";
    const QString UPDATE_SEMESTER = "UPDATE semester SET year = ?, semester = ? WHERE id = ?";
    const QString DELETE_SEMESTER = "DELETE FROM semester WHERE id = ?";
    const QString SELECT_ALL_SEMESTERS = "SELECT * FROM semester ORDER BY year DESC, semester DESC";

    // Course queries
    const QString INSERT_COURSE = "INSERT INTO courses (name, description, year_level, credit_hours, semester_id, department_id, max_grade, course_type) VALUES (?, ?, ?, ?, ?, ?, ?, ?)";
    const QString UPDATE_COURSE = "UPDATE courses SET name = ?, description = ?, year_level = ?, credit_hours = ?, semester_id = ?, department_id = ?, max_grade = ?, course_type = ? WHERE id = ?";
    const QString DELETE_COURSE = "DELETE FROM courses WHERE id = ?";
    const QString SELECT_ALL_COURSES = "SELECT c.*, s.year AS semester_year, s.semester AS semester_number, d.name AS department_name, "
                                       "(SELECT GROUP_CONCAT(DISTINCT u.full_name SEPARATOR ', ') "
                                       " FROM schedules sch "
                                       " JOIN professors p ON sch.professor_id = p.id "
                                       " JOIN users u ON p.user_id = u.id "
                                       " WHERE sch.course_id = c.id) AS assigned_professors "
                                       "FROM courses c "
                                       "LEFT JOIN semester s ON c.semester_id = s.id "
                                       "LEFT JOIN departments d ON c.department_id = d.id "
                                       "ORDER BY c.name";
    const QString SELECT_COURSES_BY_SEMESTER = "SELECT * FROM courses WHERE semester_id = ?";
    const QString SELECT_COURSE_BY_ID = "SELECT c.*, s.year as semester_year, s.semester as semester_number FROM courses c "
                                        "LEFT JOIN semester s ON c.semester_id = s.id WHERE c.id = ?";
    const QString SELECT_COURSES_BY_PROFESSOR = "SELECT DISTINCT c.*, s.year as semester_year, s.semester as semester_number "
                                                "FROM courses c "
                                                "JOIN schedules sch ON c.id = sch.course_id "
                                                "LEFT JOIN semester s ON c.semester_id = s.id "
                                                "WHERE sch.professor_id = ? ORDER BY c.name";

    // Section queries
    const QString INSERT_SECTION = "INSERT INTO sections (name, course_id, capacity, semester_id, academic_level_id) VALUES (?, ?, ?, ?, ?)";
    const QString UPDATE_SECTION = "UPDATE sections SET name = ?, course_id = ?, capacity = ?, semester_id = ?, academic_level_id = ? WHERE id = ?";
    const QString DELETE_SECTION = "DELETE FROM sections WHERE id = ?";
    const QString SELECT_ALL_SECTIONS = "SELECT sec.*, c.name as course_name FROM sections sec "
                                        "LEFT JOIN courses c ON sec.course_id = c.id ORDER BY sec.name"; // LEFT JOIN because course_id can be NULL/0
    const QString SELECT_SECTIONS_BY_COURSE = "SELECT * FROM sections WHERE course_id = ?";
    const QString SELECT_SECTION_BY_ID = "SELECT sec.*, c.name as course_name FROM sections sec "
                                         "LEFT JOIN courses c ON sec.course_id = c.id WHERE sec.id = ?";

    // Enrollment queries
    const QString INSERT_ENROLLMENT = "INSERT INTO enrollments (student_id, course_id, status, attendance_count, absence_count, assignment_1_grade, assignment_2_grade, coursework_grade, final_exam_grade, experience_grade, total_grade, letter_grade) "
                                      "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)";
    const QString UPDATE_ENROLLMENT = "UPDATE enrollments SET status = ?, attendance_count = ?, absence_count = ?, assignment_1_grade = ?, assignment_2_grade = ?, coursework_grade = ?, final_exam_grade = ?, experience_grade = ?, total_grade = ?, letter_grade = ? "
                                      "WHERE id = ?";
    const QString DELETE_ENROLLMENT = "DELETE FROM enrollments WHERE id = ?";
    const QString SELECT_ENROLLMENTS_BY_STUDENT = "SELECT e.*, c.name as course_name, c.course_type, c.max_grade FROM enrollments e "
                                                 "JOIN courses c ON e.course_id = c.id WHERE e.student_id = ?";
    const QString SELECT_ENROLLMENTS_BY_COURSE = "SELECT e.*, sd.student_number, u.full_name, s.name as section_name, al.name as level_name, sem.year as academic_year FROM enrollments e "
                                                "JOIN students_data sd ON e.student_id = sd.id "
                                                "JOIN users u ON sd.user_id = u.id "
                                                "LEFT JOIN sections s ON sd.section_id = s.id "
                                                "LEFT JOIN academic_levels al ON sd.academic_level_id = al.id "
                                                "JOIN courses c ON e.course_id = c.id "
                                                "LEFT JOIN semester sem ON c.semester_id = sem.id "
                                                "WHERE e.course_id = ?";
    const QString INSERT_ENROLLMENT_BATCH_BY_LEVEL = "INSERT INTO enrollments (student_id, course_id, status, attendance_count, absence_count, assignment_1_grade, assignment_2_grade, coursework_grade, final_exam_grade, experience_grade, total_grade, letter_grade, enrolled_at) "
                                                     "SELECT ?, id, 'Active', 0, 0, 0, 0, 0, 0, 0, 0, 'N/A', NOW() FROM courses WHERE year_level = ?";

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

    // College queries
    const QString INSERT_COLLEGE = "INSERT INTO colleges (name, code) VALUES (?, ?)";
    const QString UPDATE_COLLEGE = "UPDATE colleges SET name = ?, code = ? WHERE id = ?";
    const QString DELETE_COLLEGE = "DELETE FROM colleges WHERE id = ?";
    const QString SELECT_ALL_COLLEGES = "SELECT * FROM colleges ORDER BY name";

    // Department queries
    const QString INSERT_DEPARTMENT = "INSERT INTO departments (name, college_id, code) VALUES (?, ?, ?)";
    const QString UPDATE_DEPARTMENT = "UPDATE departments SET name = ?, college_id = ?, code = ? WHERE id = ?";
    const QString DELETE_DEPARTMENT = "DELETE FROM departments WHERE id = ?";
    const QString SELECT_ALL_DEPARTMENTS = "SELECT d.*, c.name as college_name FROM departments d JOIN colleges c ON d.college_id = c.id ORDER BY d.name";
    const QString SELECT_DEPARTMENT_BY_ID = "SELECT * FROM departments WHERE id = ?";
    const QString SELECT_DEPARTMENTS_BY_COLLEGE = "SELECT * FROM departments WHERE college_id = ?";

    // Academic Level queries
    const QString INSERT_ACADEMIC_LEVEL = "INSERT INTO academic_levels (name, level_number) VALUES (?, ?)";
    const QString UPDATE_ACADEMIC_LEVEL = "UPDATE academic_levels SET name = ?, level_number = ? WHERE id = ?";
    const QString DELETE_ACADEMIC_LEVEL = "DELETE FROM academic_levels WHERE id = ?";
    const QString SELECT_ALL_ACADEMIC_LEVELS = "SELECT * FROM academic_levels ORDER BY level_number";
    const QString SELECT_ACADEMIC_LEVEL_BY_ID = "SELECT * FROM academic_levels WHERE id = ?";

    // Room queries
    const QString INSERT_ROOM = "INSERT INTO rooms (name, type, capacity, ac_units, fans_count, lighting_points, computers_count, seating_description, code) "
                                "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?)";
    const QString UPDATE_ROOM = "UPDATE rooms SET name = ?, type = ?, capacity = ?, ac_units = ?, fans_count = ?, lighting_points = ?, computers_count = ?, seating_description = ?, code = ? "
                                "WHERE id = ?";
    const QString DELETE_ROOM = "DELETE FROM rooms WHERE id = ?";
    const QString SELECT_ALL_ROOMS = "SELECT * FROM rooms ORDER BY name";
    const QString SELECT_ROOM_BY_ID = "SELECT * FROM rooms WHERE id = ?";

    // Room Specs (Equipment)
    const QString INSERT_ROOM_SPEC = "INSERT INTO room_specs (room_id, product_id, product_name, product_description) VALUES (?, ?, ?, ?)";
    const QString DELETE_ROOM_SPECS = "DELETE FROM room_specs WHERE room_id = ?";
    const QString SELECT_ROOM_SPECS = "SELECT * FROM room_specs WHERE room_id = ?";

    // Professor queries
    const QString INSERT_PROFESSOR = "INSERT INTO professors (user_id, specialization, title, personal_info, id_number) VALUES (?, ?, ?, ?, ?)";
    const QString UPDATE_PROFESSOR = "UPDATE professors SET specialization = ?, title = ?, personal_info = ?, id_number = ? WHERE id = ?";
    const QString DELETE_PROFESSOR = "DELETE FROM professors WHERE id = ?";
    const QString SELECT_ALL_PROFESSORS = "SELECT p.*, u.full_name FROM professors p JOIN users u ON p.user_id = u.id ORDER BY u.full_name";
    const QString SELECT_PROFESSOR_BY_USER_ID = "SELECT p.*, u.full_name FROM professors p JOIN users u ON p.user_id = u.id WHERE p.user_id = ?";
    const QString SELECT_PROFESSOR_BY_ID_NUMBER = "SELECT p.*, u.full_name, u.role, u.password FROM professors p JOIN users u ON p.user_id = u.id WHERE p.id_number = ?";

    // Schedule queries
    const QString INSERT_SCHEDULE = "INSERT INTO schedules (course_id, room_id, professor_id, day_of_week, start_time, end_time) VALUES (?, ?, ?, ?, ?, ?)";
    const QString UPDATE_SCHEDULE = "UPDATE schedules SET course_id = ?, room_id = ?, professor_id = ?, day_of_week = ?, start_time = ?, end_time = ? WHERE id = ?";
    const QString DELETE_SCHEDULE = "DELETE FROM schedules WHERE id = ?";
    const QString SELECT_ALL_SCHEDULES = "SELECT s.*, c.name as course_name, r.name as room_name, u.full_name as professor_name "
                                         "FROM schedules s JOIN courses c ON s.course_id = c.id "
                                         "JOIN rooms r ON s.room_id = r.id "
                                         "JOIN professors p ON s.professor_id = p.id "
                                         "JOIN users u ON p.user_id = u.id";
    const QString SELECT_SCHEDULE_BY_PROFESSOR = "SELECT s.*, c.name as course_name, r.name as room_name "
                                                 "FROM schedules s JOIN courses c ON s.course_id = c.id "
                                                 "JOIN rooms r ON s.room_id = r.id "
                                                 "WHERE s.professor_id = ? ORDER BY s.day_of_week, s.start_time";
    const QString SELECT_SCHEDULE_BY_COURSE = "SELECT s.*, c.name as course_name, r.name as room_name "
                                              "FROM schedules s JOIN courses c ON s.course_id = c.id "
                                              "JOIN rooms r ON s.room_id = r.id "
                                              "WHERE s.course_id = ? ORDER BY s.day_of_week, s.start_time";
    const QString SELECT_SCHEDULE_BY_LEVEL = "SELECT s.*, c.name as course_name, r.name as room_name "
                                             "FROM schedules s JOIN courses c ON s.course_id = c.id "
                                             "JOIN rooms r ON s.room_id = r.id "
                                             "WHERE c.year_level = ? ORDER BY s.day_of_week, s.start_time";

    // Attendance Log Queries
    const QString INSERT_ATTENDANCE_LOG = "INSERT INTO attendance_logs (enrollment_id, date, status, notes) VALUES (?, ?, ?, ?)";
    const QString SELECT_ATTENDANCE_LOGS_BY_ENROLLMENT = "SELECT * FROM attendance_logs WHERE enrollment_id = ? ORDER BY date DESC";
    const QString SELECT_ATTENDANCE_LOGS_BY_COURSE_DATE = "SELECT al.* FROM attendance_logs al "
                                                          "JOIN enrollments e ON al.enrollment_id = e.id "
                                                          "WHERE e.course_id = ? AND al.date = ?";

    // Calendar Event queries
    const QString INSERT_CALENDAR_EVENT = "INSERT INTO calendar_events (title, description, start_date, end_date, event_type) VALUES (?, ?, ?, ?, ?)";
    const QString UPDATE_CALENDAR_EVENT = "UPDATE calendar_events SET title = ?, description = ?, start_date = ?, end_date = ?, event_type = ? WHERE id = ?";
    const QString DELETE_CALENDAR_EVENT = "DELETE FROM calendar_events WHERE id = ?";
    const QString SELECT_ALL_CALENDAR_EVENTS = "SELECT * FROM calendar_events ORDER BY start_date";
    const QString SELECT_CALENDAR_EVENTS_BY_RANGE = "SELECT * FROM calendar_events WHERE start_date >= ? AND end_date <= ? ORDER BY start_date";
}

#endif // QUERIES_H
