/*****************************************************************//**
 * \file   Log.cpp
 * \brief  Log Source Code
 * 
 * \author Kevin Pruvost (pruvostkevin0@gmail.com)
 * \date   April, 03 2022
 *********************************************************************/
#include "Log.hpp"

// C++ includes
#include <unordered_map>
#include <ctime>
#include <memory>

static std::unordered_map<std::string, std::unique_ptr<Log::File>> files;

void Log::ForceCloseFile(const std::string & fileName)
{
    if (files.contains(fileName)) files.erase(fileName);
}

Log::File & Log::GetFile(const std::string & fileName)
{
    return *files.at(fileName);
}

bool Log::CreateFile(const std::string & fileName)
{
    if (files.contains(fileName)) return true;

    files.insert({ fileName, std::make_unique<File>(fileName) });
    if (!files.at(fileName)->file)
    {
        files.erase(fileName);
        return false;
    }
    return true;
}

Log::File::File(const std::string & fileName)
    : file{ nullptr }
{
    file = fopen(fileName.c_str(), "a");
    if (file)
    {
        time_t timer;
        char buffer[26];
        struct tm * tm_info;

        timer = time(&timer);
        tm_info = localtime(&timer);

        strftime(buffer, 26, "%Y-%m-%d %H:%M:%S", tm_info);

        fprintf(file,
            "\nSTARTING LOG SESSION: %s"
            "---------------------------------------------------------------\n",
            buffer);
    }
}

Log::File::~File()
{
    if (file)
    {
        time_t timer;
        char buffer[26];
        struct tm * tm_info;

        timer = time(&timer);
        tm_info = localtime(&timer);

        strftime(buffer, 26, "%Y-%m-%d %H:%M:%S", tm_info);

        fprintf(file,
            "\nENDING LOG SESSION:   %s"
            "---------------------------------------------------------------\n",
            buffer);
        fclose(file);
    }
}
