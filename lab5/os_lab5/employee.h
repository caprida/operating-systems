//
// Created by даша on 27.05.2022.
//

#ifndef OS_LAB5_EMPLOYEE_H
#define OS_LAB5_EMPLOYEE_H

#endif //OS_LAB5_EMPLOYEE_H
#pragma once

#include <iostream>

struct employee {
    int num;
    char name[20];
    double hours;

    friend std::istream& operator>>(std::istream& in, employee& emp);
    friend std::ostream& operator<<(std::ostream& out, const employee& emp);
};