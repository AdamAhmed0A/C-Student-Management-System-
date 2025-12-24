#include "sectioncontroller.h"
#include "../database/dbconnection.h"
#include "../database/queries.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

/**
 * Constructor for the SectionController class
 */
SectionController::SectionController() {}

//function to add section
/**
 * Adds a new student section/group to the database
 * Handles optional bindings for course, semester, and academic level
 * @param section - The Section object containing details
 * @return True if successful, otherwise false
 */
bool SectionController::addSection(const Section& section)
{
	QSqlQuery query(DBConnection::instance().database());
    query.prepare(Queries::INSERT_SECTION);
    query.addBindValue(section.name());
    
    // Bind courseId: if 0, bind NULL
    if (section.courseId() > 0) query.addBindValue(section.courseId());
    else query.addBindValue(QVariant());

    query.addBindValue(section.capacity());
    
    // Bind semesterId: if 0, bind NULL
    if (section.semesterId() > 0) query.addBindValue(section.semesterId());
    else query.addBindValue(QVariant());
    
    // Bind academicLevelId
    if (section.academicLevelId() > 0) query.addBindValue(section.academicLevelId());
    else query.addBindValue(QVariant());

    if (!query.exec()) {
        qDebug() << "Error adding section:" << query.lastError().text();
        return false;
	}
	return true;
}

//function to update section
/**
 * Updates an existing section in the database
 * @param section - The Section object with updated details
 * @return True if successful, otherwise false
 */
bool SectionController::updateSection(const Section& section)
{
	QSqlQuery query(DBConnection::instance().database());
    query.prepare(Queries::UPDATE_SECTION);
    query.addBindValue(section.name());
    
    if (section.courseId() > 0) query.addBindValue(section.courseId());
    else query.addBindValue(QVariant());
    
    query.addBindValue(section.capacity());
    
    if (section.semesterId() > 0) query.addBindValue(section.semesterId());
    else query.addBindValue(QVariant());
    
    if (section.academicLevelId() > 0) query.addBindValue(section.academicLevelId());
    else query.addBindValue(QVariant());

    query.addBindValue(section.id());
    if (!query.exec()) { 
        qDebug() << "Error updating section:" << query.lastError().text();
        return false;
	}
    return true;
}

//function to delete section
/**
 * Deletes a section from the database
 * @param id - The ID of the section to delete
 * @return True if successful, otherwise false
 */
bool SectionController::deleteSection(int id)
{
	QSqlQuery query(DBConnection::instance().database());
    query.prepare(Queries::DELETE_SECTION);
    query.addBindValue(id);
	if (!query.exec()) {
        qDebug() << "Error deleting section:" << query.lastError().text();
        return false;
	}
	return true;
}

//function to get all sections
/**
 * Retrieves all sections from the database
 * @return A list of Section objects
 */
QList<Section> SectionController::getAllSections()
{
	QList<Section> sections;
    QSqlQuery query(DBConnection::instance().database());
    if (!query.exec(Queries::SELECT_ALL_SECTIONS)) {
        qDebug() << "Error retrieving sections:" << query.lastError().text();
        return sections;
    }
    
    while (query.next()) {
        Section section;
        section.setId(query.value("id").toInt());
        section.setName(query.value("name").toString());
        section.setCourseId(query.value("course_id").toInt());
        section.setCapacity(query.value("capacity").toInt());
        section.setSemesterId(query.value("semester_id").toInt());
        section.setAcademicLevelId(query.value("academic_level_id").toInt()); // Read new field
        section.setCreatedAt(query.value("created_at").toDateTime());
        section.setUpdatedAt(query.value("updated_at").toDateTime());
        // Additional course info
        section.setCourseName(query.value("course_name").toString());
        
        sections.append(section);
	}
	return sections;
}

//function to get sections by course id
/**
 * Retrieves sections associated with a specific course
 * @param courseId - The ID of the course
 * @return A list of Section objects
 */
QList<Section> SectionController::getSectionsByCourse(int courseId)
{
	QList<Section> sections;
    QSqlQuery query(DBConnection::instance().database());
    query.prepare(Queries::SELECT_SECTIONS_BY_COURSE);
    query.addBindValue(courseId);
    if (!query.exec()) {
        qDebug() << "Error retrieving sections by course:" << query.lastError().text();
        return sections;
    }
    
    while (query.next()) {
        Section section;
        section.setId(query.value("id").toInt());
        section.setName(query.value("name").toString());
        section.setCourseId(query.value("course_id").toInt());
        section.setCapacity(query.value("capacity").toInt());
        section.setSemesterId(query.value("semester_id").toInt());
        section.setCreatedAt(query.value("created_at").toDateTime());
        section.setUpdatedAt(query.value("updated_at").toDateTime());
        
        sections.append(section);
	}
	return sections;
}

//function to get section by id
/**
 * Retrieves a section by its ID
 * @param id - The ID of the section
 * @return The Section object if found, otherwise an empty object
 */
Section SectionController::getSectionById(int id)
{
    Section section;
    QSqlQuery query(DBConnection::instance().database());
    query.prepare(Queries::SELECT_SECTION_BY_ID);
    query.addBindValue(id);
    
    if (!query.exec()) {
        qDebug() << "Error retrieving section by ID:" << query.lastError().text();
        return section;
    }
    
    if (query.next()) {
        section.setId(query.value("id").toInt());
        section.setName(query.value("name").toString());
        section.setCourseId(query.value("course_id").toInt());
        section.setCapacity(query.value("capacity").toInt());
        section.setSemesterId(query.value("semester_id").toInt());
        section.setAcademicLevelId(query.value("academic_level_id").toInt());
        section.setCreatedAt(query.value("created_at").toDateTime());
        section.setUpdatedAt(query.value("updated_at").toDateTime());
        section.setCourseName(query.value("course_name").toString());
    }
    
    return section;
}
