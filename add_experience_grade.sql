-- Add missing experience_grade column to enrollments table
ALTER TABLE enrollments ADD COLUMN experience_grade DOUBLE DEFAULT 0 AFTER final_exam_grade;