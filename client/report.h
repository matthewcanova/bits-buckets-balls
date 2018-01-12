#ifndef REPORT_H
#define REPORT_H

#include <QObject>
#include <vector>

class Report : public QObject
{
    Q_OBJECT

public:
    Report();

signals:

private slots:
    void displayReport(std::vector<std::vector<int> >);

private:
    void buildFile(std::vector<std::vector<int> >);
};

#endif // REPORT_H
