#ifndef DIRHELPER
#define DIRHELPER
# include <QString>
# include <QDir>
namespace Util {
class Dirs
{
public:

    static std::string GetCurentWorkingFolder()
    {
        QString ret = QDir::currentPath();
        return ret.toStdString();
    }

    static void MkDirIfNotExist(std::string folderPath)
    {
        if (!QDir(folderPath.c_str()).exists()) {
            QDir().mkdir(folderPath.c_str());
        }
    }

    static void RmDir(std::string folderPath)
    {
        if(QDir(folderPath.c_str()).exists()) {
            QDir().rmdir(folderPath.c_str());
        }
    }

    static void RmDirIfEmpty(std::string folderPath)
    {
        QDir dir(folderPath.c_str());
        if (dir.entryInfoList(QDir::NoDotAndDotDot|QDir::AllEntries).count() == 0) {
            dir.cdUp();
            std::cout << dir.dirName().toStdString() << std::endl;
            dir.rmdir(QString(folderPath.c_str()));
        }
    }

    static std::string ToAbsolutePath(std::string path)
    {
        QDir dir(path.c_str());
        return dir.absolutePath().toStdString();
    }

    static void RmFilesRe(std::string folderPath, std::vector<std::string> filterStrList)
    {
        QDir dir(folderPath.c_str());
        QStringList list;
        foreach (std::string filterStr, filterStrList) {
           list << QString(filterStr.c_str());
        }
        dir.setNameFilters(list);
        dir.setFilter(QDir::Files);
        foreach (QString fileName, dir.entryList()) {
            dir.remove(fileName);
        }
    }

    static bool StringEndsWith(std::string str, std::string suffix)
    {
        return str.size() >= suffix.size() &&
               str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
    }
};
}

#endif // DIRHELPER

