#include "savehandler.h"

#include <common/global.h>
#include <common/plugins.h>

#include <window/windowmain.h>

#include <QSettings>
#include <QFile>
#include <QFileInfo>
#include <QApplication>
#include <QDir>

#include <QDebug>
#include <pugixml/pugixml.hpp>

#include <fstream>

#include <archive.h>
#include <archive_entry.h>

#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>

saveHandler::saveHandler()
{
    version = "1.0.0";
}

void saveHandler::performSave(char *filename) {
    QDir *dir = new QDir();

    QFile::remove(filename);
    QFileInfo fileInfo(filename);

    QString tmpDir = QString("").sprintf("tmp%d", rand() % 30000 + 198).append(fileInfo.fileName().split(".")[0]);

    // Create some vars for dir
    QFileInfo pathInfo(QApplication::applicationFilePath());
    QString filePath = QString(pathInfo.path()).append("/").append(tmpDir);
    QString fileXMLPath = QString(filePath).append("/file.xml");
    QString fileNlTrack = QString(filePath).append("/track.nltrack");
    QString fileTarArchive = QString(filename);

    dir->mkdir(filePath);

    // Create XML Doc
    pugi::xml_document doc;
    pugi::xml_node docRootNode = doc.append_child("NoLimitsStudio");

    pugi::xml_node docFileInfo = docRootNode.append_child("FileInfo");
    docFileInfo.append_child("Version").append_attribute("value").set_value("1.0.0");

    for(int i = 0; i < NoLimitsRenderer::Plugins->loadedPlugins.size(); i++) {
        pugi::xml_node docPlugin = docRootNode.append_child(NoLimitsRenderer::Plugins->loadedPlugins[i]->PluginIdentifier);
        NoLimitsRenderer::Plugins->loadedPlugins[i]->handleFileSave(docPlugin);
    }
    doc.save_file(NoLimitsRenderer::toChar(fileXMLPath));

    // Save NLTrack File
    NlSave(NoLimitsRenderer::toChar(fileNlTrack), &NoLimitsRenderer::Track);

    // Create tar
    std::vector<const char*> archiveFiles;
    std::vector<const char*> archiveFilesNames;

    archiveFiles.push_back(NoLimitsRenderer::toChar(fileXMLPath));
    archiveFiles.push_back(NoLimitsRenderer::toChar(fileNlTrack));

    archiveFilesNames.push_back("NoLimitsStudio/file.xml");
    archiveFilesNames.push_back("NoLimitsStudio/track.nltrack");

    writeArchive(NoLimitsRenderer::toChar(fileTarArchive), archiveFiles, archiveFilesNames);

    NoLimitsRenderer::removeDir(filePath);
}

void saveHandler::performOpen(char *filename) {
    QDir *dir = new QDir();

    QFileInfo fileInfo(filename);

    QString tmpDir = QString("").sprintf("tmp%d", rand() % 30000 + 198).append(fileInfo.fileName().split(".")[0]);

    // Create some vars for dir
    QFileInfo pathInfo(QApplication::applicationFilePath());
    QString filePath = QString(pathInfo.path()).append("/").append(tmpDir);
    QString fileXMLPath = QString(filePath).append("/file.xml");
    QString fileNlTrack = QString(filePath).append("/track.nltrack");

    dir->mkdir(filePath);

    // Extract tar
    extract(filename, NoLimitsRenderer::toChar(filePath));

    // loading XML document
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(NoLimitsRenderer::toChar(fileXMLPath));
    pugi::xml_node docRootNode = doc.child("NoLimitsStudio");

    for(int i = 0; i < NoLimitsRenderer::Plugins->loadedPlugins.size(); i++) {
        pugi::xml_node docPlugin = docRootNode.child(NoLimitsRenderer::Plugins->loadedPlugins[i]->PluginIdentifier);
        NoLimitsRenderer::Plugins->loadedPlugins[i]->handleFileLoad(docPlugin);
    }

    // Open NlTrack
    NoLimitsRenderer::mainWindow->openNLTrack(NoLimitsRenderer::toChar(fileNlTrack));

    NoLimitsRenderer::removeDir(filePath);
}

void saveHandler::writeArchive(const char *outname, std::vector<const char*> files, std::vector<const char*> filenames) {
  archive *a;
  archive_entry *entry;
  struct stat st;
  int len;

  int lSize = 8192;
  char *buffer;

  a = archive_write_new();
  archive_write_set_bytes_per_block( a, 8192 );
  archive_write_set_format_pax_restricted(a); // Note 1
  archive_write_open_filename(a, outname);

  for (int i=0; i < files.size(); i++) {
    const char* filename = files[i];
    stat(filename, &st);
    entry = archive_entry_new(); // Note 2
    archive_entry_set_pathname(entry, filenames[i]);
    archive_entry_set_size(entry, st.st_size); // Note 3
    archive_entry_set_filetype(entry, AE_IFREG);
    archive_entry_set_perm(entry, 0644);
    archive_write_header(a, entry);

    FILE * pFile;
    pFile = fopen (filename, "rb");
    if(pFile == NULL) continue;

    buffer = (char*) malloc (sizeof(char)*lSize);
    len = fread(buffer, 1, lSize, pFile);

    while(len > 0) {
        archive_write_data(a, buffer, len);
        len = fread(buffer, 1, lSize, pFile);
    }

    fclose(pFile);

    archive_entry_free(entry);
  }
  archive_write_close(a); // Note 4
}


void saveHandler::extract(const char *filename, const char* directory) {
    struct archive *a;
    struct archive *ext;
    struct archive_entry *entry;
    int r;

    int flags;
    flags = ARCHIVE_EXTRACT_TIME;
    flags |= ARCHIVE_EXTRACT_PERM;
    flags |= ARCHIVE_EXTRACT_ACL;
    flags |= ARCHIVE_EXTRACT_FFLAGS;

    a = archive_read_new();
    ext = archive_write_disk_new();
    archive_write_disk_set_options(ext, flags);
    archive_read_support_format_tar(a);
    if ((r = archive_read_open_filename(a, filename, 10240))) qDebug() << "archive_read_open_filename()" << archive_error_string(a);
    for (;;) {
        r = archive_read_next_header(a, &entry);
        if (r == ARCHIVE_EOF) break;
        if (r != ARCHIVE_OK) qDebug() << "archive_read_next_header()" << archive_error_string(a);

        const char* path = archive_entry_pathname( entry );
        char newPath[PATH_MAX + 1];
        char *npath = NoLimitsRenderer::toChar(QString(path).replace("NoLimitsStudio/", ""));

        snprintf( newPath, PATH_MAX, "%s/%s", directory, npath );
        archive_entry_set_pathname( entry, newPath );

        r = archive_write_header(ext, entry);
        if (r != ARCHIVE_OK) qDebug() << "archive_write_header()" << archive_error_string(ext);
        else {
            copyData(a, ext);
            r = archive_write_finish_entry(ext);
            if (r != ARCHIVE_OK) qDebug() << "archive_write_finish_entry()" << archive_error_string(ext);
        }
    }
    archive_read_close(a);
}

int saveHandler::copyData(struct archive *ar, struct archive *aw) {
    int r;
    const void *buff;
    size_t size;
#if 1
    int64_t offset;
#else
    off_t offset;
#endif

    for (;;) {
        r = archive_read_data_block(ar, &buff, &size, &offset);
        if (r == ARCHIVE_EOF) return (ARCHIVE_OK);
        if (r != ARCHIVE_OK)  return (r);
        r = archive_write_data_block(aw, buff, size, offset);
        if (r != ARCHIVE_OK) {
            qDebug() << "archive_write_data_block()" << archive_error_string(aw);
            return (r);
        }
    }
}
