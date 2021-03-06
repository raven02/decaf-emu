#include "filesystem_host_folderhandle.h"
#include "platform/platform.h"

#ifdef PLATFORM_POSIX
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>

namespace fs
{

struct PosixData
{
   DIR *handle;
};

bool HostFolderHandle::open()
{
   auto data = reinterpret_cast<PosixData *>(mFindData);

   if (!data) {
      data = new PosixData();
      mFindData = data;
   }

   data->handle = opendir(mPath.path().c_str());

   if (!data->handle) {
      delete data;
      mFindData = nullptr;
      return false;
   }

   if (mVirtualHandle) {
      mVirtualHandle->open();
   }

   return true;
}

void HostFolderHandle::close()
{
   auto data = reinterpret_cast<PosixData *>(mFindData);

   if (data) {
      closedir(data->handle);
      delete data;
      mFindData = nullptr;
   }

   if (mVirtualHandle) {
      mVirtualHandle->close();
   }
}

bool HostFolderHandle::read(FolderEntry &entry)
{
   // Read virtual entries first
   if (mVirtualHandle->read(entry)) {
      return true;
   }

   if (!mFindData) {
      return false;
   }

   auto data = reinterpret_cast<PosixData *>(mFindData);
   auto item = readdir(data->handle);

   if (!item) {
      return false;
   }

   entry.name = item->d_name;
   entry.size = 0; // TODO: Get size

   if (item->d_type == DT_DIR) {
      entry.type = FolderEntry::Folder;
   } else {
      struct stat data;

      if (stat(mPath.join(entry.name).path().c_str(), &data) == 0) {
         entry.size = data.st_size;
      }

      entry.type = FolderEntry::File;
   }

   if (entry.name.compare(".") == 0 || entry.name.compare("..") == 0) {
      // Skip over . and .. entries
      return read(entry);
   }

   return true;
}

bool HostFolderHandle::rewind()
{
   auto data = reinterpret_cast<PosixData *>(mFindData);

   if (!data) {
      return false;
   }

   rewinddir(data->handle);

   if (mVirtualHandle) {
      mVirtualHandle->rewind();
   }

   return true;
}

} // namespace fs

#endif
