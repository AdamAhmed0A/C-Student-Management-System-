#ifndef SECTIONCONTROLLER_H
#define SECTIONCONTROLLER_H

#include "../models/section.h"
#include <QList>

class SectionController
{
public:
    SectionController();
    
    bool addSection(const Section& section);
    bool updateSection(const Section& section);
    bool deleteSection(int id);
    
    QList<Section> getAllSections();
    QList<Section> getSectionsByCourse(int courseId);
    Section getSectionById(int id);
};

#endif // SECTIONCONTROLLER_H
