#include <buffer.hpp>
#include <types.h>

namespace engine::system {

struct Pause {
  char* mKey;
  u32 mHash;
  u8 mPauseMask[0x10];
  u32 mPauseCount;  // atomic but I don't feel like caring
};

struct PauseTargetArray {
  u32* mHashArray;
  u8* mIndexArray;
  s32 mCount;
  s32 mCapacity;
};

struct PauseContext {
  u8 mPauseMask[0x10];
  PauseTargetArray* mTargetArrayPtr;
};

struct PauseMgr {
  sead::Buffer<Pause> mRequestArray;
  PauseTargetArray mTargetArray;
  void* mPauseConfigDocument;
  PauseContext mContext;
  void** mUnk0x48;  // vtable with a bunch of stubbed functions
  u8 mUnk0x50[0x10];

  s32 getPauseRequestCount(u32 hash) const {
    if (mRequestArray.getSize() == 0) {
      return 0;
    }

    auto cmp = [](const Pause& lhs, const u32& rhs) {
      if (lhs.mHash == rhs)
        return 0;
      if (lhs.mHash < rhs)
        return -1;
      return 1;
    };

    const s32 index = mRequestArray.binarySearch<u32>(hash, cmp);

    return mRequestArray[index].mPauseCount;
  }

  // I wrote this but then I remembered there's already a function that does this so I guess this is useless lol
  bool isTargetPaused(u32 hash) const {
    if (mTargetArray.mCapacity == 0) {
      return false;
    }

    for (s32 i = hash % mTargetArray.mCapacity; i < mTargetArray.mCapacity; ++i) {
      if (mTargetArray.mHashArray[i] != hash) {
        continue;
      }
      const u8 index = mTargetArray.mIndexArray[i];

      const u32 mask = *reinterpret_cast<const u32*>(&mContext.mPauseMask[(index >> 3) & 0x1c]);

      return ((mask >> (index & 0x1f)) & 1) != 0;
    }

    return false;
  }
};

}  // namespace engine::system