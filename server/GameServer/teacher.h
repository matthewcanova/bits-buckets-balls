#ifndef TEACHER_H
#define TEACHER_H

#include <vector>
#include <string>

class Teacher {
public:
    Teacher();
    void load_template();
    void run_template(std::vector<std::vector<int>>);

private:
    FILE f;
};



#endif // TEACHER_H
