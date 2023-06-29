#ifndef SEARCHER_H
#define SEARCHER_H

#include <QObject>

class Searcher : public QObject
{
    Q_OBJECT
public:
    explicit Searcher(QObject *parent = nullptr);

    QString searchKeywordInside(QString keyword);
    QString searchKeywordOutside(QString keyword);

signals:
private:
    QString *m_keyward;
    QString *m_filePath;
};

#endif  // SEARCHER_H
