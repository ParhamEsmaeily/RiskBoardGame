#include "FileLineReader.h"

FileLineReader::FileLineReader(const std::string &_fileName)
{
    fileName = new std::string(_fileName);
    fileStream = new std::ifstream(*fileName);
}

FileLineReader::FileLineReader(const FileLineReader &_other)
{
    fileName = _other.fileName;
    fileStream = new std::ifstream(*fileName);
}

FileLineReader::~FileLineReader()
{
    close();

    delete fileName;
    delete fileStream;
}

FileLineReader &FileLineReader::operator=(const FileLineReader &_other)
{
    fileName = _other.fileName;
    fileStream = new std::ifstream(*fileName);

    return *this;
}

bool FileLineReader::getLine(std::string *_newLine)
{
    if (fileStream->is_open())
    {
        std::getline(*fileStream, *_newLine);

        return true;
    }

    return false;
}

bool FileLineReader::hasLine()
{
    if (fileStream->is_open())
    {
        return !fileStream->eof();
    }

    return false;
}

void FileLineReader::close()
{
    if (fileStream->is_open())
    {
        fileStream->close();
    }
}
