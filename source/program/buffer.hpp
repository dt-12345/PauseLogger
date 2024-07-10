#include <types.h>

namespace sead {

template <typename T>
struct Buffer {
  s32 mSize;
  T* mBuffer;

  T& operator[](s32 idx) {
    if (u32(mSize) <= u32(idx)) {
      return mBuffer[0];
    }
    return mBuffer[idx];
  }
  const T& operator[](s32 idx) const {
    if (u32(mSize) <= u32(idx)) {
      return mBuffer[0];
    }
    return mBuffer[idx];
  }

  s32 getSize() const { return mSize; }

  template <typename Key>
  s32 binarySearch(const Key& key, s32 (*cmp)(const T& item, const Key& key)) const {
    if (mSize == 0)
      return -1;

    s32 a = 0;
    s32 b = mSize - 1;
    while (a < b) {
      const s32 m = (a + b) / 2;
      const s32 c = cmp(mBuffer[m], key);
      if (c == 0)
        return m;
      if (c < 0)
        a = m + 1;
      else
        b = m;
    }

    if (cmp(mBuffer[a], key) == 0)
      return a;

    return -1;
  }
};

}  // namespace sead