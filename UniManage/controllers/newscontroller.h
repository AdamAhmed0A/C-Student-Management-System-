#ifndef NEWSCONTROLLER_H
#define NEWSCONTROLLER_H

#include "../models/news.h"
#include <QList>

class NewsController
{
public:
    NewsController();
    
    bool addNews(const News& news);
    bool updateNews(const News& news);
    bool deleteNews(int id);
    
    QList<News> getAllNews();
};

#endif // NEWSCONTROLLER_H
