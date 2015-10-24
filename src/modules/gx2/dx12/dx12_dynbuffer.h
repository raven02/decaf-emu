#pragma once
#include <gsl.h>
#include "dx12.h"
#include "utils/align.h"

class DXDynBuffer {
public:
   class BaseAllocation {
      friend class DXDynBuffer;

   public:
      BaseAllocation()
      {
         mCpuAddr = nullptr;
         mGpuAddr = NULL;
      }

      BaseAllocation(uint8_t *cpuAddr, D3D12_GPU_VIRTUAL_ADDRESS gpuAddr)
      {
         mCpuAddr = cpuAddr;
         mGpuAddr = gpuAddr;
      }

      bool valid() const {
         return mGpuAddr != 0;
      }

      operator uint8_t*() {
         return mCpuAddr;
      }

      operator D3D12_GPU_VIRTUAL_ADDRESS() {
         return mGpuAddr;
      }

   protected:
      uint8_t *mCpuAddr;
      D3D12_GPU_VIRTUAL_ADDRESS mGpuAddr;

   };

   class VertexAllocation : public BaseAllocation
   {
   public:
      VertexAllocation() { }

      VertexAllocation(const BaseAllocation& alloc, UINT stride, UINT size)
         : BaseAllocation(alloc)
      {
         mView.BufferLocation = mGpuAddr;
         mView.StrideInBytes = stride;
         mView.SizeInBytes = size;
      }

      operator D3D12_VERTEX_BUFFER_VIEW*() {
         return &mView;
      }

   protected:
      D3D12_VERTEX_BUFFER_VIEW mView;

   };

   class IndexAllocation : public BaseAllocation
   {
   public:
      IndexAllocation() { }

      IndexAllocation(const BaseAllocation& alloc, DXGI_FORMAT format, UINT size)
         : BaseAllocation(alloc)
      {
         mView.BufferLocation = mGpuAddr;
         mView.Format = format;
         mView.SizeInBytes = size;
      }

      operator D3D12_INDEX_BUFFER_VIEW*() {
         return &mView;
      }

   protected:
      D3D12_INDEX_BUFFER_VIEW mView;

   };

   DXDynBuffer(ID3D12Device *device, size_t size)
      : mSize(size), mOffset(0)
   {
      ThrowIfFailed(device->CreateCommittedResource(
         &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
         D3D12_HEAP_FLAG_NONE,
         &CD3DX12_RESOURCE_DESC::Buffer(size),
         D3D12_RESOURCE_STATE_GENERIC_READ,
         nullptr,
         IID_PPV_ARGS(&mBuffer)));

      CD3DX12_RANGE readRange(0, 0);
      ThrowIfFailed(mBuffer->Map(0, &readRange, reinterpret_cast<void**>(&mCpuAddr)));
   }

   void reset() {
      mOffset = 0;
   }

   BaseAllocation get(UINT size, void *data, UINT alignment = 0) {
      // align the offset according to the alignment requirements
      if (alignment != 0) {
         mOffset = align_up(mOffset, alignment);
      }

      // Fail if we don't have enough room
      gsl::fail_fast_assert(mOffset + size < mSize);

      // Build this buffer
      auto thisGpuAddr = mBuffer->GetGPUVirtualAddress() + mOffset;
      auto thisCpuAddr = mCpuAddr + mOffset;
      mOffset += size;

      BaseAllocation alloc(thisCpuAddr, thisGpuAddr);
      if (data) {
         memcpy(thisCpuAddr, data, size);
      }
      return alloc;
   }

   VertexAllocation get(UINT stride, UINT size, void *data, UINT alignment = 0) {
      return VertexAllocation(get(size, data, alignment), stride, size);
   }

   IndexAllocation get(DXGI_FORMAT format, UINT size, void *data, UINT alignment = 0) {
      return IndexAllocation(get(size, data, alignment), format, size);
   }

   ComPtr<ID3D12Resource> mBuffer;
   uint8_t *mCpuAddr;
   size_t mSize;
   size_t mOffset;

};
