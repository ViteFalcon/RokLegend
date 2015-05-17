////////////////////////////////////////////////////////////////////////////////
// This source code is part of RokLegend project.
//         (Ragnarok Online Client)
//     Copyright (c) RokLegend Team, 2013
////////////////////////////////////////////////////////////////////////////////

#include <core/RoFileSystem.h>

#include "RoIrrKlangAudioManager.h"
#include "RoIrrKlangAudio.h"

#if roDEBUG_MODE
#   define roIRRKLANG_PLUGIN_DIR roIRRKLANG_PLUGIN_DIR_DBG
#else
#   define roIRRKLANG_PLUGIN_DIR roIRRKLANG_PLUGIN_DIR_REL
#endif // roDEBUG_MODE

/*namespace irrklang
{
    class OgreDataStreamReader : public IFileReader
    {
    public:
        OgreDataStreamReader(Ogre::String fileName, Ogre::DataStreamPtr stream)
            : mFileName(fileName)
            , mStream(stream)
        {
        }

        //! reads data, returns how much was read
        ik_s32 read(void* buffer, ik_u32 sizeToRead)
        {
            return mStream->read(buffer, sizeToRead);
        }

        //! changes position in file, returns true if successful
        bool seek(ik_s32 finalPos, bool relativeMovement)
        {
            size_t oldPos = mStream->tell();
            mStream->seek(finalPos);
            return mStream->tell() != oldPos;
        }

        //! returns size of file
        ik_s32 getSize()
        {
            return mStream->size();
        }

        //! returns where in the file we are.
        ik_s32 getPos()
        {
            return mStream->tell();
        }

        //! returns name of file
        const ik_c8* getFileName()
        {
            return mFileName.c_str();
        }
    private:
        const Ogre::String mFileName;
        Ogre::DataStreamPtr mStream;
    };
    //--------------------------------------------------------------------------
    class OgreFileFactory : public IFileFactory
    {
    public:
        virtual IFileReader* createFileReader(const ik_c8* filename)
        {
            if (RoFileSystem::FileExists(filename))
            {
                std::ifstream* stdFileStream = OGRE_NEW_T(std::ifstream, Ogre::MEMCATEGORY_GENERAL)(filename, std::ios::in|std::ios::binary);
                Ogre::DataStreamPtr fileStream(new Ogre::FileStreamDataStream(stdFileStream, true));
                return new OgreDataStreamReader(filename, fileStream);
            }

            static Ogre::ResourceGroupManager& resGroupMgr = Ogre::ResourceGroupManager::getSingleton();
            if (resGroupMgr.resourceExistsInAnyGroup(filename))
            {
                return new OgreDataStreamReader(filename, resGroupMgr.openResource(filename));
            }
            else
            {
                roLOG_ERR<<"Cannot find resource '"<<filename<<"'!";
            }
            return 0;
        }
    };
}*/
//------------------------------------------------------------------------------
RoIrrKlangAudioManager::RoIrrKlangAudioManager(const RoConfig& config)
    : RoAudioManager(config)
    , mEngine(irrklang::createIrrKlangDevice())
{
    mEngine->loadPlugins(roIRRKLANG_PLUGIN_DIR);
    //irrklang::OgreFileFactory* irrklangFileFactory = new irrklang::OgreFileFactory();
    //mEngine->addFileFactory(irrklangFileFactory);
    //irrklangFileFactory->drop();
}
//------------------------------------------------------------------------------
RoIrrKlangAudioManager::~RoIrrKlangAudioManager()
{
    mEngine->drop();
    mEngine = 0;
}
//------------------------------------------------------------------------------
RoAudioPtr RoIrrKlangAudioManager::getSound2D(const RoString& filename)
{
    static const bool IS_2D = false;
    return std::make_shared<RoIrrKlangAudio>(
        IS_2D,
        filename,
        *mEngine,
        mEngine->addSoundSourceFromFile(RoString::ToCurrentEncoding(filename).c_str()),
        RoVec3f::ZERO);
}
//------------------------------------------------------------------------------
RoAudioPtr RoIrrKlangAudioManager::getSound3D(const RoString& filename)
{
    static const bool IS_3D = true;
    return std::make_shared<RoIrrKlangAudio>(
        IS_3D,
        filename,
        *mEngine,
        mEngine->addSoundSourceFromFile(RoString::ToCurrentEncoding(filename).c_str()),
        RoVec3f::ZERO);
}
//------------------------------------------------------------------------------
void RoIrrKlangAudioManager::playSound2D(const RoString& filename, bool loop)
{
    mEngine->play2D(RoString::ToCurrentEncoding(filename).c_str(), loop);
}
//------------------------------------------------------------------------------
void RoIrrKlangAudioManager::playSound3D(const RoString& filename, bool loop, const RoVector3f& position)
{
    mEngine->play3D(
        RoString::ToCurrentEncoding(filename).c_str(),
        irrklang::vec3df(position.x(), position.y(), position.z()),
        loop);
}
