#include "sectioncontroller.h"
#include "../database/dbconnection.h"
#include "../database/queries.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

SectionController::SectionController() {}

//function to add section
bool SectionController::addSection(const Section& section)
{
	QSqlQuery query(DBConnection::instance().database());
    query.prepare(Queries::INSERT_SECTION);
    query.addBindValue(section.courseId());
    query.addBindValue(section.capacity());
    query.addBindValue(section.semesterId());
    if (!query.exec()) {
        qDebug() << "Error adding section:" << query.lastError().text();
        return false;
	}
	return true;
}

//function to update section
bool SectionController::updateSection(const Section& section)
{
	QSqlQuery query(DBConnection::instance().database());
    query.prepare(Queries::UPDATE_SECTION);
    query.addBindValue(section.courseId());
    query.addBindValue(section.capacity());
    query.addBindValue(section.semesterId());
    query.addBindValue(section.id());
    if (!query.exec()) {
        qDebug() << "Error updating section:" << query.lastError().text();
        return false;
	}
    return true;
}

//function to delete section
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
        section.setCourseId(query.value("course_id").toInt());
        section.setCapacity(query.value("capacity").toInt());
        section.setSemesterId(query.value("semester_id").toInt());
        section.setCreatedAt(query.value("created_at").toDateTime());
        section.setUpdatedAt(query.value("updated_at").toDateTime());
        // Additional course info
        section.setCourseName(query.value("course_name").toString());
        
        sections.append(section);
	}
	return sections;
}

//function to get sections by course id
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
        section.setCourseId(query.value("course_id").toInt());
        section.setCapacity(query.value("capacity").toInt());
        section.setSemesterId(query.value("semester_id").toInt());
        section.setCreatedAt(query.value("created_at").toDateTime());
        section.setUpdatedAt(query.value("updated_at").toDateTime());
        
        sections.append(section);
	}
	return sections;
}
