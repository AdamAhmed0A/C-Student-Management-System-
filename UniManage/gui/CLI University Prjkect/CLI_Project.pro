QT += core sql

CONFIG += c++17 console
CONFIG -= app_bundle

TARGET = CLI_University_Project
TEMPLATE = app

SOURCES += main.cpp \
    ../../database/dbconnection.cpp \
    ../../controllers/usercontroller.cpp \
    ../../controllers/professorcontroller.cpp \
    ../../controllers/studentcontroller.cpp \
    ../../controllers/enrollmentcontroller.cpp \
    ../../controllers/coursecontroller.cpp \
    ../../controllers/schedulecontroller.cpp \
    ../../controllers/academic_level_controller.cpp \
    ../../controllers/roomcontroller.cpp \
    ../../controllers/departmentcontroller.cpp \
    ../../controllers/collegecontroller.cpp \
    ../../controllers/semestercontroller.cpp \
    ../../controllers/calendarcontroller.cpp \
    ../../controllers/sectioncontroller.cpp \
    ../../controllers/newscontroller.cpp \
    ../../controllers/paymentcontroller.cpp \
    ../../models/user.cpp \
    ../../models/professor.cpp \
    ../../models/student_data.cpp \
    ../../models/enrollment.cpp \
    ../../models/course.cpp \
    ../../models/schedule.cpp \
    ../../models/academic_level.cpp \
    ../../models/room.cpp \
    ../../models/department.cpp \
    ../../models/college.cpp \
    ../../models/semester.cpp \
    ../../models/calendar_event.cpp \
    ../../models/attendance_log.cpp \
    ../../models/section.cpp \
    ../../models/news.cpp \
    ../../models/payment.cpp

HEADERS += \
    ../../database/dbconnection.h \
    ../../database/queries.h \
    ../../controllers/usercontroller.h \
    ../../controllers/professorcontroller.h \
    ../../controllers/studentcontroller.h \
    ../../controllers/enrollmentcontroller.h \
    ../../controllers/coursecontroller.h \
    ../../controllers/schedulecontroller.h \
    ../../controllers/academic_level_controller.h \
    ../../controllers/roomcontroller.h \
    ../../controllers/departmentcontroller.h \
    ../../controllers/collegecontroller.h \
    ../../controllers/semestercontroller.h \
    ../../controllers/calendarcontroller.h \
    ../../controllers/sectioncontroller.h \
    ../../controllers/newscontroller.h \
    ../../controllers/paymentcontroller.h \
    ../../models/user.h \
    ../../models/professor.h \
    ../../models/student_data.h \
    ../../models/enrollment.h \
    ../../models/course.h \
    ../../models/schedule.h \
    ../../models/academic_level.h \
    ../../models/room.h \
    ../../models/department.h \
    ../../models/college.h \
    ../../models/semester.h \
    ../../models/calendar_event.h \
    ../../models/attendance_log.h \
    ../../models/section.h \
    ../../models/news.h \
    ../../models/payment.h

INCLUDEPATH += ../../controllers ../../models ../../database
