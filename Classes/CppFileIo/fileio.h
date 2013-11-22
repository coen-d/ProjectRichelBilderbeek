#ifndef FILEIO_H
#define FILEIO_H

#include <string>
#include <vector>

namespace ribi {

namespace fileio {

enum class CopyMode   { allow_overwrite, prevent_overwrite };
enum class RenameMode { allow_overwrite, prevent_overwrite };

///Creates a folder
///Will abort if it already exists
void CreateFolder(const std::string& folder);

///Copy a file. If prevent_overwrite is set to true, a file
///cannot be copied over an existing target. If prevent_overwrite
///is set to false, a file can be copied over an existing target,
///which will overwrite the target.
//From http://www.richelbilderbeek.nl/CppCopyFile.htm
void CopyFile(
  const std::string& fileNameFrom,
  const std::string& fileNameTo,
  const CopyMode copy_mode = CopyMode::prevent_overwrite);

///Delete a file
//From http://www.richelbilderbeek.nl/CppDeleteFile.htm
void DeleteFile(const std::string& filename);

///Deletes a folder
///Will throw an exception if this fails
void DeleteFolder(const std::string& folder);

///Checks if two files are identical
///Will throw std::bad_alloc if at least one of the files is
///too big for a std::vector.
//From http://www.richelbilderbeek.nl/CppFilesAreIdentical.htm
bool FilesAreIdentical(
  const std::string& filename_a,
  const std::string& filename_b);

///FileToVector reads a file and converts it to a std::vector<std::string>
///Throws std::bad_alloc if the file is too big to fit in std::vector
///Throws std::logic_error if the file does not exist
//From http://www.richelbilderbeek.nl/CppFileToVector.htm
const std::vector<std::string> FileToVector(const std::string& filename);

///Returns the extension of a filename
//From http://www.richelbilderbeek.nl/CppGetExtension.htm
const std::string GetExtension(const std::string& filename);

///Returns the extension of a filename, without a trailing dot
const std::string GetExtensionNoDot(const std::string& filename);

///Returns the extension of a filename, with a trailing dot
const std::string GetExtensionWithDot(const std::string& filename);

///Obtain the basename of a file or folder
///MyFolder/MyFile.txt - > MyFile
///MyFolder/MySubFolder -> MySubFolder
///GetFileBasename is a replacement of boost::filesystem::basename
///From http://www.richelbilderbeek.nl/CppGetFileBasename.htm
const std::string GetFileBasename(const std::string& filename);

//From http://www.richelbilderbeek.nl/CppGetFilesInFolder.htm
const std::vector<std::string> GetFilesInFolder(const std::string& folder);

const std::vector<std::string> GetFilesInFolderByRegex(
  const std::string& folder,
  const std::string& regex_str);

///Returns the path of a filename
///From http://www.richelbilderbeek.nl/CppGetPath.htm
const std::string GetPath(const std::string& filename);

///Returns the path seperator depending on the operating system
///For Linux, this is a slash
///For Windows, this is a backslash
///From http://www.richelbilderbeek.nl/CppGetPathSeperator.htm
const std::string GetPathSeperator() noexcept;

///Returns the superfolder from a path
///For example: '/A/B/C' its superfolder is '/A/B'
const std::string GetSuperFolder(const std::string& folder);

///Obtain the name of a file that does not exist
///Will throw a std::runtime_error in the unlikely
///case it fails after one thousand times
const std::string GetTempFileName();

///Obtain the name of a folder that does not exist
///Will throw a std::runtime_error in the unlikely
///case it fails after one thousand times
const std::string GetTempFolderName();

///Obtain the version
const std::string GetVersion() noexcept;

///Obtain the version history
const std::vector<std::string> GetVersionHistory() noexcept;

///Returns if the name is a folder name
///From http://www.richelbilderbeek.nl/CppIsFolder.htm
bool IsFolder(const std::string& filename) noexcept;

///Determines if a filename is a regular file
///From http://www.richelbilderbeek.nl/CppIsRegularFile.htm
bool IsRegularFile(const std::string& filename) noexcept;


///Removes the path of a filename
///From http://www.richelbilderbeek.nl/CppRemovePath.htm
const std::string RemovePath(const std::string& filename);

///Renames a file
//From http://www.richelbilderbeek.nl/CppRenameFile.htm
void RenameFile(
  const std::string& from,
  const std::string& to,
  const RenameMode rename_mode = RenameMode::prevent_overwrite);

#ifndef NDEBUG
void Test() noexcept;
#endif

} //~namespace fileio

} //~namespace ribi

#endif // FILEIO_H
