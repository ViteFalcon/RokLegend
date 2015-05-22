////////////////////////////////////////////////////////////////////////////////
// This source code is part of RokLegend project.
//         (Ragnarök Online Client)
//     Copyright (c) RokLegend Team, 2013
////////////////////////////////////////////////////////////////////////////////
/*
#include <core/RoFileSystem.h>

#include "RoGrfArchive.h"
#include "RoGrfFile.h"
#include "RoGrfSqliteBackend.h"

#include <Utilities/RoSqliteUtils.h>
//------------------------------------------------------------------------------
const RoString RoGrfSqliteBackend::TypeName = "sqlite";
const RoString RoGrfSqliteBackend::MemorySession = "memory";
const RoString RoGrfSqliteBackend::FileSession = "file";
const RoString RoGrfSqliteBackend::DefaultSession = RoGrfSqliteBackend::MemorySession;
//------------------------------------------------------------------------------
#define roGRF_ID_TABLE "GrfIdTable"
#   define roGRF_ID_TABLE_ID_COL "grfId"
#   define roGRF_ID_TABLE_FILE_COL "grfName"

#define roGRF_FILES_TABLE "GrfFilesTable"
#   define roGRF_FILES_TABLE_FILE_ID "fileId"
#   define roGRF_FILES_TABLE_GRF_ID "grfId"
#   define roGRF_FILES_TABLE_NAME "fileName"
#   define roGRF_FILES_TABLE_ZSIZE "compressedSize"
#   define roGRF_FILES_TABLE_ESIZE "encryptedSize"
#   define roGRF_FILES_TABLE_OSIZE "originalSize"
#   define roGRF_FILES_TABLE_OFFSET "offset"
#   define roGRF_FILES_TABLE_FLAGS "flags"
//------------------------------------------------------------------------------
RoString RoGrfSqliteBackend::ResolveSessionType(const RoString& sessionType)
{
    if (MemorySession == sessionType || FileSession == sessionType)
    {
        return sessionType;
    }
    return DefaultSession;
}
//------------------------------------------------------------------------------
sqlitepp::session& RoGrfSqliteBackend::GetSession(const RoString& sessionType)
{
    if (FileSession == sessionType)
    {
        return RoGame::getSingleton().getRepository().getFileStore().getSession();
    }
    else if (MemorySession == sessionType)
    {
        return RoGame::getSingleton().getRepository().getMemoryStore().getSession();
    }
    else
    {
        roLOG_ERR <<
            "The requested session type '"<<sessionType<<"' isn't recognized. "
            "Defaulting to '"<<DefaultSession<<"' session type.";
    }
    return GetSession(DefaultSession);
}
//------------------------------------------------------------------------------
RoGrfSqliteBackend::RoGrfSqliteBackend(const RoString& sessionType)
    : RoGrfStorageBackend(TypeName)
    , mSessionType(ResolveSessionType(sessionType))
    , mSession(GetSession(mSessionType))
{
    if (!mSession.is_open())
    {
        roTHROW(RoException() << RoErrorInfoDetail("Database session doesn't seem to be open!"));
    }
    createTables();
}
//------------------------------------------------------------------------------
RoGrfSqliteBackend::~RoGrfSqliteBackend()
{
}
//------------------------------------------------------------------------------
roINLINE void RoGrfSqliteBackend::createTables()
{
    createGrfIdTable();
    createFilesTable();
}
//------------------------------------------------------------------------------
roINLINE void RoGrfSqliteBackend::createGrfIdTable()
{
    sqlitepp::statement createGrfTableStatement(mSession);
    createGrfTableStatement << "create table if not exists " roGRF_ID_TABLE " ("
        roGRF_ID_TABLE_ID_COL " integer primary key autoincrement,"
        roGRF_ID_TABLE_FILE_COL" text unique not null);";
    createGrfTableStatement.exec();
}
//------------------------------------------------------------------------------
roINLINE void RoGrfSqliteBackend::createFilesTable()
{
    sqlitepp::statement createFilesTableStatement(mSession);
    createFilesTableStatement << "create table if not exists "roGRF_FILES_TABLE " ("
        roGRF_FILES_TABLE_FILE_ID " integer primary key autoincrement,"
        roGRF_FILES_TABLE_GRF_ID " integer references "roGRF_ID_TABLE"("roGRF_ID_TABLE_ID_COL") not null,"
        roGRF_FILES_TABLE_NAME " text unique not null,"
        roGRF_FILES_TABLE_ZSIZE " integer not null,"
        roGRF_FILES_TABLE_ESIZE " integer not null,"
        roGRF_FILES_TABLE_OSIZE " integer not null,"
        roGRF_FILES_TABLE_OFFSET " integer not null,"
        roGRF_FILES_TABLE_FLAGS " integer not null);";
    createFilesTableStatement.exec();
}
//------------------------------------------------------------------------------
size_t RoGrfSqliteBackend::getGrfId(const RoString& grfFile)
{
    size_t grfId = 0;
    std::string grfFileUtf8 = grfFile.asUTF8();
    sqlitepp::statement getGrfIdStatement(mSession);
    getGrfIdStatement << "select "roGRF_ID_TABLE_ID_COL" from "roGRF_ID_TABLE" where "roGRF_ID_TABLE_FILE_COL" = :grfFile;",
        sqlitepp::use(grfFileUtf8), sqlitepp::into(grfId);
    if (getGrfIdStatement.exec())
    {
        return grfId;
    }
    sqlitepp::statement insertGrfStatement(mSession);
    insertGrfStatement << "insert into "roGRF_ID_TABLE" ("roGRF_ID_TABLE_FILE_COL") values (:grfFile);",
        sqlitepp::use(grfFileUtf8);
    insertGrfStatement.exec();
    return getGrfId(grfFile);
}
//------------------------------------------------------------------------------
size_t RoGrfSqliteBackend::getFileCountForGrf(size_t grfId)
{
    sqlitepp::statement grfFileCount(mSession);
    size_t fileCount = 0;
    grfFileCount << "select count("roGRF_FILES_TABLE_FILE_ID") from "roGRF_FILES_TABLE" where "roGRF_FILES_TABLE_GRF_ID"=:grfId;",
        sqlitepp::use(grfId), sqlitepp::into(fileCount);
    return fileCount;
}
//------------------------------------------------------------------------------
bool RoGrfSqliteBackend::hasFile(const RoString& file)
{
    sqlitepp::statement hasGrfStatement(mSession);
    size_t fileCount = 0;
    hasGrfStatement << "select count("roGRF_FILES_TABLE_FILE_ID") from "roGRF_FILES_TABLE" where "roGRF_FILES_TABLE_NAME"=:fileName;",
        sqlitepp::use(file), sqlitepp::into(fileCount);
    hasGrfStatement.exec();
    return 0 != fileCount;
}
//------------------------------------------------------------------------------
void RoGrfSqliteBackend::addFile(size_t grfId, const RoGrfFilePtr& file)
{
    if (hasFile(file->getName()))
    {
        roLOG_WARN << "The file named " << file->getName() << " already exists in the store!";
        return;
    }
    sqlitepp::statement addGrfFileStatement(mSession);
    addGrfFileStatement <<
        "insert into "roGRF_FILES_TABLE
        "("roGRF_FILES_TABLE_GRF_ID
        ","roGRF_FILES_TABLE_NAME
        ","roGRF_FILES_TABLE_ZSIZE
        ","roGRF_FILES_TABLE_ESIZE
        ","roGRF_FILES_TABLE_OSIZE
        ","roGRF_FILES_TABLE_OFFSET
        ","roGRF_FILES_TABLE_FLAGS") values"
        "(:grfId, :fileName, :fileZSize, :fileESize, :fileOSize, :fileOffset, :fileFlags);"
        ,sqlitepp::use(grfId)
        ,sqlitepp::use(file->getName().asUTF8())
        ,sqlitepp::use(file->getCompressedSize())
        ,sqlitepp::use(file->getEncryptedSize())
        ,sqlitepp::use(file->getOriginalSize())
        ,sqlitepp::use(file->getOffset())
        ,sqlitepp::use(file->getFlags());
    addGrfFileStatement.exec();
}
//------------------------------------------------------------------------------
RoGrfFilePtr RoGrfSqliteBackend::getFileInfo(const RoString& fileName)
{
    RoGrfFilePtr file;
    sqlitepp::statement getFileInfoStatement(mSession);
    RoGrfFileStats stats;
    getFileInfoStatement << "select "
        roGRF_FILES_TABLE_ZSIZE","
        roGRF_FILES_TABLE_ESIZE","
        roGRF_FILES_TABLE_OSIZE","
        roGRF_FILES_TABLE_OFFSET","
        roGRF_FILES_TABLE_FLAGS" from "roGRF_FILES_TABLE" where "roGRF_FILES_TABLE_NAME"=:fileName;",
        sqlitepp::use(fileName.asUTF8()),
        sqlitepp::into(stats.compressedSize),
        sqlitepp::into(stats.encryptedSize),
        sqlitepp::into(stats.originalSize),
        sqlitepp::into(stats.offset),
        sqlitepp::into(stats.flags);
    if (!getFileInfoStatement.exec())
    {
        roTHROW(RoItemNotFound() << RoErrorInfoDetail("Failed to find file in GRF storage.") << RoErrorInfoItemName(fileName));
    }
    file.reset(new RoGrfFile(fileName, stats));
    return file;
}
//------------------------------------------------------------------------------
void RoGrfSqliteBackend::getFileNames(size_t grfId, Ogre::StringVector& fileNames)
{
    fileNames.clear();
    size_t fileCount = getFileCountForGrf(grfId);
    fileNames.reserve(fileCount);
    std::string fileName;
    sqlitepp::statement selectFileNamesStatement(mSession);
    selectFileNamesStatement << "select "roGRF_FILES_TABLE_NAME" from "roGRF_FILES_TABLE" where "roGRF_FILES_TABLE_GRF_ID"=:grfId;",
        sqlitepp::use(grfId), sqlitepp::into(fileName);
    while (selectFileNamesStatement.exec())
    {
        fileNames.push_back(fileName);
    }
}
//------------------------------------------------------------------------------
void RoGrfSqliteBackend::getFileInfos(RoGrfArchive* archive, Ogre::FileInfoList& fileInfos)
{
    fileInfos.clear();
    size_t grfId = archive->getGrfId();
    size_t fileCount = getFileCountForGrf(grfId);
    fileInfos.reserve(fileCount);
    Ogre::FileInfo fileInfo;
    fileInfo.archive = archive;
    std::string filename;
    sqlitepp::statement selectFileInfosStatement(mSession);
    selectFileInfosStatement << "select "
        roGRF_FILES_TABLE_NAME","
        roGRF_FILES_TABLE_ZSIZE","
        roGRF_FILES_TABLE_OSIZE" from "roGRF_FILES_TABLE" where "roGRF_FILES_TABLE_GRF_ID"=:grfId;",
        sqlitepp::use(grfId),
        sqlitepp::into(filename),
        sqlitepp::into(fileInfo.compressedSize),
        sqlitepp::into(fileInfo.uncompressedSize);
    while (selectFileInfosStatement.exec())
    {
        fileInfo.filename = filename;
        fileInfo.basename = RoFileSystem::GetBaseName(fileInfo.filename);
        fileInfo.path = RoFileSystem::GetParentDirectory(fileInfo.filename);
        fileInfos.push_back(fileInfo);
    }
}
//------------------------------------------------------------------------------
void RoGrfSqliteBackend::findFileNames(size_t grfId, const RoString& pattern, Ogre::StringVector& fileNames)
{
    fileNames.clear();
    size_t fileCount = getFileCountForGrf(grfId);
    fileNames.reserve(fileCount);
    std::string fileName;
    std::string fileNamePattern = pattern.asUTF8();
    boost::algorithm::replace_all(fileNamePattern, "*", "%");
    sqlitepp::statement selectFileNamesStatement(mSession);
    selectFileNamesStatement << "select "
        roGRF_FILES_TABLE_NAME" from "
        roGRF_FILES_TABLE" where "
        roGRF_FILES_TABLE_GRF_ID"=:grfId and "
        roGRF_FILES_TABLE_NAME" like :fileNamePattern;",
        sqlitepp::use(grfId), sqlitepp::use(fileNamePattern),
        sqlitepp::into(fileName);
    while (selectFileNamesStatement.exec())
    {
        fileNames.push_back(fileName);
    }
}
//------------------------------------------------------------------------------
void RoGrfSqliteBackend::findFileInfos(const RoGrfArchive* archive, const RoString& pattern, Ogre::FileInfoList& fileInfos)
{
    fileInfos.clear();
    size_t grfId = archive->getGrfId();
    size_t fileCount = getFileCountForGrf(grfId);
    fileInfos.reserve(fileCount);
    Ogre::FileInfo fileInfo;
    fileInfo.archive = archive;
    sqlitepp::statement selectFileInfosStatement(mSession);
    std::string filename;
    std::string fileNamePattern = pattern.asUTF8();
    boost::algorithm::replace_all(fileNamePattern, "*", "%");
    selectFileInfosStatement << "select "
        roGRF_FILES_TABLE_NAME","
        roGRF_FILES_TABLE_ZSIZE","
        roGRF_FILES_TABLE_OSIZE" from "roGRF_FILES_TABLE" where "
        roGRF_FILES_TABLE_GRF_ID"=:grfId and "
        roGRF_FILES_TABLE_NAME" like :fileNamePattern;",
        sqlitepp::use(grfId),
        sqlitepp::use(fileNamePattern),
        sqlitepp::into(filename),
        sqlitepp::into(fileInfo.compressedSize),
        sqlitepp::into(fileInfo.uncompressedSize);
    while (selectFileInfosStatement.exec())
    {
        fileInfo.filename = filename;
        fileInfo.basename = RoFileSystem::GetBaseName(filename);
        fileInfo.path = RoFileSystem::GetParentDirectory(filename);
        fileInfos.push_back(fileInfo);
    }
}
//------------------------------------------------------------------------------
*/
