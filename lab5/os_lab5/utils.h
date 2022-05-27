//
// Created by даша on 27.05.2022.
//

#ifndef OS_LAB5_UTILS_H
#define OS_LAB5_UTILS_H

#endif //OS_LAB5_UTILS_H
#ifndef LAB5_UTILS_H
#define LAB5_UTILS_H

#endif //LAB5_UTILS_H
#pragma once
#include <string>
#include <vector>
#include <Windows.h>

std::wstring convertToWideString(const std::string& str);

struct ThreadArgs {
    size_t number_of_clients;
    std::string filename;

    const std::vector<HANDLE>& read_events;
    const std::vector<HANDLE>& modify_events;

    std::vector<size_t> read_section_counter;

    const std::vector<HANDLE>& mutexes;

    ThreadArgs(size_t num_of_clients, std::string file,const std::vector<HANDLE>& r_events, const std::vector<HANDLE>& m_events, const std::vector<HANDLE>& muts);
};