#pragma once

#include <fstream>

/**
 * @brief A class for reading lines from a file.
 *
 * This class provides functionality for reading lines from a file.
 * It is designed to be non-copyable and non-assignable, but copy constructor and assignment operator
 * are included for the sake of assignment requirements.
 */
class FileLineReader
{
private:
    std::string *fileName;
    std::ifstream *fileStream;

public:
    FileLineReader() = delete;
    /**
     * @brief Constructs a FileLineReader object with the specified file name.
     *
     * @param _fileName The name of the file to read.
     */
    FileLineReader(const std::string &_fileName);
    /**
     * @brief A class for reading lines from a file.
     * @attention We can't simply copy the filestream, so we attempt to open it again
     * this is undefined behaviour in most implementations, so we should avoid it
     * practically, a copy constructor for this class is not needed (and probably shouldn't exist),
     * but it is included for the sake of assignment requirements.
     */
    FileLineReader(const FileLineReader &_other);
    /**
     * @brief Destructor for the FileLineReader class.
     *
     * This destructor frees any resources allocated by the FileLineReader object.
     *
     */
    ~FileLineReader();

    // we can't simply copy the filestream, so we attempt to open it again
    // this is undefined behaviour in most implementations, so we should avoid it
    // practically, an assignment operator for this class is not needed (and probably shouldn't exist),
    // but it is included for the sake of assignment requirements
    FileLineReader &operator=(const FileLineReader &_other);

    /**
     * @brief Reads a line from a file and stores it in the provided string pointer.
     * @param _newLine Pointer to a string where the read line will be stored.
     * @return true if a line was successfully read, false otherwise.
     */
    bool getLine(std::string *_newLine);
    /**
     * @brief Checks if there is a line available to read from the file.
     *
     * @return true if there is a line available to read, false otherwise.
     */
    bool hasLine();
    /**
     * @brief Closes the file stream.
     *
     */
    void close();
};
