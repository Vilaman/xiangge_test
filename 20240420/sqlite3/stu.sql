CREATE TABLE students (
student_id INTEGER PRIMARY KEY,
student_name TEXT NOT NULL
);
CREATE TABLE courses (
course_id INTEGER PRIMARY KEY,
course_name TEXT NOT NULL,
credits INTEGER NOT NULL
);

CREATE TABLE enrollments (
enrollment_id INTEGER PRIMARY KEY,
student_id INTEGER,
course_id INTEGER,
semester TEXT NOT NULL,
FOREIGN KEY (student_id) REFERENCES
students(student_id),
FOREIGN KEY (course_id) REFERENCES courses(course_id)
);

-- 插入学生数据
INSERT INTO students (student_id, student_name) VALUES (1,'Alice');
INSERT INTO students (student_id, student_name) VALUES (2,'Bob');
INSERT INTO students (student_id, student_name) VALUES (3,'Charlie');
-- 插入课程数据
INSERT INTO courses (course_id, course_name, credits) VALUES (101,'Mathematics', 4);
INSERT INTO courses (course_id, course_name, credits) VALUES (102,'Physics', 4);
INSERT INTO courses (course_id, course_name, credits) VALUES (103,'Chemistry', 4);
-- 插入选课记录
INSERT INTO enrollments (enrollment_id, student_id,course_id, semester) VALUES (1, 1, 101, 'Fall 2023');
INSERT INTO enrollments (enrollment_id, student_id,course_id, semester) VALUES (2, 1, 102, 'Fall 2023');
INSERT INTO enrollments (enrollment_id, student_id,course_id, semester) VALUES (3, 2, 101, 'Fall 2023');
INSERT INTO enrollments (enrollment_id, student_id,course_id, semester) VALUES (4, 2, 102, 'Fall 2023');
INSERT INTO enrollments (enrollment_id, student_id,course_id, semester) VALUES (5, 3, 103, 'Fall 2023');
-- 更新
UPDATE enrollments SET course_id = 103 WHERE student_id = 1 AND course_id = 101;
-- 删除
DELETE FROM enrollments WHERE course_id = 102;
-- 查询
SELECT s.student_name, c.course_name, e.semester FROM enrollments e
JOIN students s ON e.student_id = s.student_id
JOIN courses c ON e.course_id = c.course_id
WHERE e.semester = 'Fall 2023';
