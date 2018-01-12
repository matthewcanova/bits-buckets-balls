#include <iostream>
#include <fstream>
#include <QDesktopServices>
#include <QUrl>
#include <QCoreApplication>
#include <QString>
#include <QDir>
#include <vector>

#include "report.h"

Report::Report() {

}

void Report::buildFile(std::vector<std::vector<int> > data) {
    std::ofstream myfile;
    myfile.open ("report.html");
    myfile << "<!DOCTYPE html>"
              << "<html>"
              << "<head>"
              << "<style>"
              << "table, th, td {"
                  << "border: 1px solid black;"
                  << "border-collapse: collapse;"
                  << "text-align: center;"
              << "}"
              << "</style>"
              << "</head>"
              << "<body>"
              << "<h1 style=\"text-align:center\">Class Report</h1><br /><br />"
              << "<table style=\"width:100%\">"
                << "<tr>"
                  << "<th>Student ID</th>"
                  << "<th>Total Score</th> "
                  << "<th>Total Rounds Completed</th>"
                  << "<th>Level 1 Score</th>"
                  << "<th>Level 1 Rounds Completed</th>"
                  << "<th>Level 2 Score</th>"
                  << "<th>Level 2 Rounds Completed</th>"
                  << "<th>Level 3 Score</th>"
                  << "<th>Level 3 Rounds Completed</th>"
                << "</tr>";

                 std::vector<int> level1;
                 std::vector<int> level2;
                 std::vector<int> level3;
                 int level1Rounds = 0;
                 int level2Rounds = 0;
                 int level3Rounds = 0;
                 int level1Score = 0;
                 int level2Score = 0;
                 int level3Score = 0;
                 int totalScore = 0;
                 int totalRounds = 0;
                 int studentID;

                 // this vector will be input to the method
                 for (unsigned int i = 0; i < data.size(); i++) {
                     level1.clear();
                     level2.clear();
                     level3.clear();
                     unsigned int rowSize = data[i].size();
                     for (unsigned int j = 0; j < rowSize; j++) {
                         // first element will be id, next 45 will be scores
                         if (j == 0) {
                             studentID = data[i][j];
                         }
                         if (j > 0 && j <= 15) {
                             level1.push_back(data[i][j]);
                         } else if (j > 15 && j <= 30) {
                             level2.push_back(data[i][j]);
                         } else if (j > 30) {
                             level3.push_back(data[i][j]);
                         }
                     }

                     level1Rounds = 0;
                     level2Rounds = 0;
                     level3Rounds = 0;
                     level1Score = 0;
                     level2Score = 0;
                     level3Score = 0;
                     totalScore = 0;

                     // now it's divided into levels, apply calculations
                     // percent rn is off of nonzero scores, may change to stars earned
                     for (unsigned int i1 = 0; i1 < level1.size(); i1++) {
                         level1Score += level1[i1];
                         if (level1[i1] != 0) {
                             level1Rounds ++;
                         }
                     }

                     for (unsigned int i2 = 0; i2 < level2.size(); i2++) {
                         level2Score += level2[i2];
                         if (level2[i2] != 0) {
                             level2Rounds ++;
                         }
                     }

                     for (unsigned int i3 = 0; i3 < level3.size(); i3++) {
                         level3Score += level3[i3];
                         if (level3[i3] != 0) {
                             level3Rounds ++;
                         }
                     }
                     // percentage variables used to count scores of 0, then use themselves to actually calculate percentage
                     totalScore = level1Score + level2Score + level3Score;
                     totalRounds = level1Rounds + level2Rounds + level3Rounds;


                     // now output the row
                     myfile
                               << "<tr>"
                                 << "<td>" << studentID << "</td>"
                                 << "<td>" << totalScore << "</td>"
                                 << "<td>" << totalRounds << "/45" << "</td>"
                                 << "<td>" << level1Score << "</td>"
                                 << "<td>" << level1Rounds << "/15" << "</td>"
                                 << "<td>" << level2Score << "</td>"
                                 << "<td>" << level2Rounds << "/15" << "</td>"
                                 << "<td>" << level3Score << "</td>"
                                 << "<td>" << level3Rounds << "/15" << "</td>"
                               << "</tr>";

                 }
              myfile << "</table>"
              << "</body></html>";
    myfile.close();
}

void Report::displayReport(std::vector<std::vector<int> > data) {

    buildFile(data);

    QString path = QDir::currentPath();

    // /home/daniale/Dropbox/Documents/cs3505/homework/a8/edu-app-scrummy-bears/build-BitsBucketsAndBalls-Desktop-Debug
    QDesktopServices::openUrl(QUrl("file://" + path + "/report.html"));
}
