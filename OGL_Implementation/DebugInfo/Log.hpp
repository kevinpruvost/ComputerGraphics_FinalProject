/*****************************************************************//**
 * \file   Log.hpp
 * \brief  Log class for debug purposes
 * 
 * \author Kevin Pruvost (pruvostkevin0@gmail.com)
 * \date   April, 03 2022
 *********************************************************************/
#pragma once

#include <iostream>
#include <format>

#ifdef NDEBUG
    #define LOG_PRINT(fileName, fmt, ...)
#else
    #define LOG_PRINT(fileName, fmt, ...) Log::Print(fileName, "[%s, Line %d]: " ## fmt, __FUNCTION__, __LINE__, __VA_ARGS__)
#endif

/**
 * @brief Prints in files or stdout/err/... in a centralized manner.
*/
class Log
{
public:
    /**
     * @brief Prints like fprintf
     * @tparam ...Args 
     * @param file 
     * @param fmt 
     * @param ...args 
    */
    template<typename... Args>
    static void Print(FILE * file, const char * fmt, Args... args);
    /**
     * @brief Prints in a file given by filename, log files are kept open
     * until a ForceCloseFile is called
     * @tparam ...Args 
     * @param fileName 
     * @param fmt 
     * @param ...args
     * @return false if error happened otherwise true 
    */
    template<typename... Args>
    static bool Print(const std::string & fileName, const char * fmt, Args... args);
    /**
     * @brief Closes specified fileName
     * @param fileName 
    */
    static void ForceCloseFile(const std::string & fileName);

    static constexpr const char * LogMainFileName = "OGL_Log.txt";

    /**
     * @brief Private Class used for Log file management
    */
    class File
    {
    public:
        File(const std::string & fileName);
        ~File();

        FILE * file;
    };
private:
    static File & GetFile(const std::string & fileName);
    static bool CreateFile(const std::string & fileName);
};

template<typename ...Args>
inline void Log::Print(FILE * file, const char * fmt, Args ...args)
{
    fprintf(file, fmt, args...);
}

template<typename ...Args>
inline bool Log::Print(const std::string & fileName, const char * fmt, Args ...args)
{
    if (!CreateFile(fileName))
        return false;

    File & file = GetFile(fileName);
    fprintf(file.file, fmt, args...);
    return true;
}
