#pragma once
#include "../RoGrf.h"

extern "C" {
#   include <grf.h>
}

using RoGrfHandle = grf_handle;
using RoGrfFileHandle = grf_node_handle;

class RoGrf2Impl : public RoGrf2
{
public:
    RoGrf2Impl(const RoString& name, const RoGrfHandle handle)
        : RoGrf2(name)
        , mHandle(handle)
    {
    }
    ~RoGrf2Impl()
    {
        grf_free(static_cast<grf_handle>(mHandle));
    }

    RoDataStreamPtr getFileContentsOf(const RoString& fileName) const override;

    static RoGrf2Ptr FromFile(const RoString& name);

protected: // methods
    void findFiles(RoStringArray& result, const RoString& pattern) const override;
    void filterFiles(RoStringArray& result, const RoFileNameFilter& filter) const override;

private: // fields
    RoGrfFileHandle getFile(const RoString& name) const;

    RoGrfHandle mHandle;
};
