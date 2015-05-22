////////////////////////////////////////////////////////////////////////////////
// This source code is part of RokLegend project.
//         (Ragnarök Online Client)
//     Copyright (c) RokLegend Team, 2013
////////////////////////////////////////////////////////////////////////////////

#include "RoGrfHashMapBackend.h"
#include "RoGrfFile.h"
#include <core/RoGlobPattern.h>
#include <core/RoLog.h>
//------------------------------------------------------------------------------
const RoString RoGrfHashMapBackend::TypeName = "hashmap";
//------------------------------------------------------------------------------
RoGrfHashMapBackend::RoGrfHashMapBackend()
    : RoGrfStorageBackend(TypeName)
{
}
//------------------------------------------------------------------------------
RoGrfHashMapBackend::~RoGrfHashMapBackend()
{
}
//------------------------------------------------------------------------------
size_t RoGrfHashMapBackend::getFileCountForGrf(size_t grfId)
{
    if (mGrfs.size() <= grfId)
    {
        roLOG_ERR << "Failed to find GRF with ID " << grfId;
        return 0;
    }
    return mGrfs.at(grfId).size();
}
//------------------------------------------------------------------------------
size_t RoGrfHashMapBackend::getGrfId(const RoString& grfFile)
{
    size_t grfId = 0;
    GrfNameIndexMap::const_iterator nameIndexIterator = mNameToIndex.find(grfFile);
    if (mNameToIndex.end() == nameIndexIterator)
    {
        grfId = mGrfs.size();
        mNameToIndex[grfFile] = mGrfs.size();
        mGrfs.push_back(GrfHashMap());
    }
    else
    {
        grfId = nameIndexIterator->second;
    }
    return grfId;
}
//------------------------------------------------------------------------------
void RoGrfHashMapBackend::addFile(size_t grfId, const RoGrfFilePtr& grfFile)
{
    if (grfId >= mGrfs.size())
    {
        roLOG_ERR << "Invalid GRF ID ("<< grfId << ") specified to add file " << grfFile->getName();
        return;
    }
    GrfHashMap& grfMap = mGrfs[grfId];
    grfMap[grfFile->getName()] = grfFile;
}
//------------------------------------------------------------------------------
bool RoGrfHashMapBackend::hasFile(const RoString& grfFile)
{
    for (size_t grfId = 0; grfId < mGrfs.size(); ++grfId)
    {
        if (mGrfs[grfId].count(grfFile))
        {
            return true;
        }
    }
    return false;
}
//------------------------------------------------------------------------------
RoGrfFilePtr RoGrfHashMapBackend::getFileInfo(const RoString& grfFile)
{
    RoGrfFilePtr result;
    for (size_t grfId = 0; grfId < mGrfs.size(); ++grfId)
    {
        GrfHashMap& grfInfoMap = mGrfs[grfId];
        GrfHashMap::const_iterator grfFileInfoIterator = grfInfoMap.find(grfFile);
        if (grfInfoMap.end() != grfFileInfoIterator)
        {
            result = grfFileInfoIterator->second;
            break;
        }
    }
    return result;
}
//------------------------------------------------------------------------------
void RoGrfHashMapBackend::getFileNames(size_t grfId, RoStringArray& fileNames)
{
    findFileNames(grfId, fileNames, NoFilter());
}
//------------------------------------------------------------------------------
void RoGrfHashMapBackend::findFileNames(size_t grfId, const RoString& pattern, RoStringArray& fileNames)
{
    roLOG_INFO << "Search pattern: " << pattern;
    findFileNames(grfId, fileNames, GlobFilter(pattern.asUTF8()));
}
//------------------------------------------------------------------------------
void RoGrfHashMapBackend::findFileNames(size_t grfId, RoStringArray& fileNames, const FileInfoFilter& filter)
{
    fileNames.clear();
    if (mGrfs.size() <= grfId)
    {
        roLOG_ERR << "Invalid GRF ID specified when trying to retrieve all file names";
        return;
    }
    const GrfHashMap& grfInfoMap = mGrfs[grfId];
    RoStringArray result{};
    fileNames.reserve(grfInfoMap.size());
    for (auto entry : grfInfoMap)
    {
        const RoGrfFile& grfFileInfo = *entry.second;
        if (!filter(grfFileInfo))
        {
            continue;
        }
        result.push_back(grfFileInfo.getName());
    }
    fileNames.swap(result);
}
//------------------------------------------------------------------------------
RoGrfHashMapBackend::GlobFilter::GlobFilter(const std::string& pattern)
{
    matcher = RoGlobPattern::New(pattern);
    roLOG_INFO << "Searching for regex pattern: " << pattern;
}
//------------------------------------------------------------------------------
bool RoGrfHashMapBackend::GlobFilter::operator()(const RoGrfFile& fileInfo) const
{
    return matcher->matches(fileInfo.getName().asUTF8());
}