#ifndef SAVEHANDLER_H
#define SAVEHANDLER_H

#include <vector>

class QSettings;

class saveHandler
{
public:
    saveHandler();
    void performSave(char *filename);
    void performOpen(char *filename);
    void writeArchive(const char *outname, std::vector<const char*> files, std::vector<const char*> filenames);
    void extract(const char *filename, const char* directory);
    int copyData(struct archive *ar, struct archive *aw);

    QSettings *handler;

    char *version;
};

#endif // SAVEHANDLER_H
