/*

Hork Engine Source Code

MIT License

Copyright (C) 2017-2022 Alexander Samusev.

This file is part of the Hork Engine Source Code.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/

#pragma once

#include <Core/ConsoleVar.h>

#include "GpuMaterial.h"
#include "RenderBackend.h"
#include "CircularBuffer.h"
#include "SphereMesh.h"
#include "ShaderLoader.h"
#include "ShaderFactory.h"

struct SViewConstantBuffer
{
    Float4x4 ViewProjection;
    Float4x4 ProjectionMatrix;
    Float4x4 InverseProjectionMatrix;
    Float4x4 InverseViewMatrix;

    // Reprojection from viewspace to previous frame projected coordinates:
    // ReprojectionMatrix = ProjectionMatrixPrevFrame * WorldspaceToViewspacePrevFrame * ViewspaceToWorldspace
    Float4x4 ReprojectionMatrix;

    // Reprojection from viewspace to previous frame viewspace coordinates:
    // ViewspaceReprojection = WorldspaceToViewspacePrevFrame * ViewspaceToWorldspace
    Float4x4 ViewspaceReprojection;

    Float3x4 WorldNormalToViewSpace;

    // ViewportParams
    Float2 InvViewportSize;
    float  ZNear;
    float  ZFar;

    Float4 ProjectionInfo;

    // Timers
    float GameRunningTimeSeconds;
    float GameplayTimeSeconds;

    Float2 Pad;

    float DynamicResolutionRatioX;
    float DynamicResolutionRatioY;

    float DynamicResolutionRatioPX;
    float DynamicResolutionRatioPY;

    Float2 FeedbackBufferResolutionRatio;
    Float2 VTPageCacheCapacity;
    Float4 VTPageTranslationOffsetAndScale;

    Float3 ViewPosition;
    float  TimeDelta;

    //float ViewHeight;
    //float Pad[3];

    Float4 PostprocessBloomMix;

    // Postprocess attribs
    float BloomEnabled;
    float ToneMappingExposure;
    float ColorGrading;
    float FXAA;

    Float4 VignetteColorIntensity; // rgb, intensity
    float  VignetteOuterRadiusSqr;
    float  VignetteInnerRadiusSqr;
    float  ViewBrightness;
    float  ColorGradingAdaptationSpeed;

    float SSLRSampleOffset;
    float SSLRMaxDist;
    float IsPerspective;
    float TessellationLevel;

    uint64_t GlobalIrradianceMap;
    uint64_t GlobalReflectionMap;

    int32_t NumDirectionalLights;
    int32_t Pad3;
    int32_t Pad4;
    int32_t DebugMode;

    Float4   LightDirs[MAX_DIRECTIONAL_LIGHTS];          // Direction, W-channel is not used
    Float4   LightColors[MAX_DIRECTIONAL_LIGHTS];        // RGB, alpha - ambient intensity
    uint32_t LightParameters[MAX_DIRECTIONAL_LIGHTS][4]; // RenderMask, FirstCascade, NumCascades, W-channel is not used
};

static_assert(sizeof(SViewConstantBuffer) <= (16 << 10), "sizeof SViewConstantBuffer > 16 kB");

struct SInstanceConstantBuffer
{
    Float4x4 TransformMatrix;
    Float4x4 TransformMatrixP;
    Float3x4 ModelNormalToViewSpace;
    Float4   LightmapOffset;
    Float4   uaddr_0;
    Float4   uaddr_1;
    Float4   uaddr_2;
    Float4   uaddr_3;
    Float2   VTOffset;
    Float2   VTScale;
    uint32_t VTUnit;
    uint32_t Pad0;
    uint32_t Pad1;
    uint32_t Pad2;
};

struct SFeedbackConstantBuffer
{
    Float4x4 TransformMatrix; // Instance MVP
    Float2   VTOffset;
    Float2   VTScale;
    uint32_t VTUnit;
    uint32_t Pad[3];
};

struct SShadowInstanceConstantBuffer
{
    Float4x4 TransformMatrix; // TODO: 3x4
    //Float4 ModelNormalToViewSpace0;
    //Float4 ModelNormalToViewSpace1;
    //Float4 ModelNormalToViewSpace2;
    Float4   uaddr_0;
    Float4   uaddr_1;
    Float4   uaddr_2;
    Float4   uaddr_3;
    uint32_t CascadeMask;
    uint32_t Pad[3];
};

struct STerrainInstanceConstantBuffer
{
    Float4x4 LocalViewProjection;
    Float3x4 ModelNormalToViewSpace;
    Float4   ViewPositionAndHeight;
    Int2     TerrainClipMin;
    Int2     TerrainClipMax;
};


//
// Common variables
//

extern AConsoleVar r_RenderSnapshot;
extern AConsoleVar r_SSLR;
extern AConsoleVar r_HBAO;


//
// Globals
//

/** Render device */
extern RenderCore::IDevice* GDevice;

/** Render context */
extern RenderCore::IImmediateContext* rcmd;

/** Render resource table */
extern RenderCore::IResourceTable* rtbl;

/** Render frame data */
extern SRenderFrame* GFrameData;

/** Render frame view */
extern SRenderView* GRenderView;

/** Render view area */
extern RenderCore::SRect2D GRenderViewArea;

/** Stream buffer */
extern RenderCore::IBuffer* GStreamBuffer;
extern AStreamedMemoryGPU*  GStreamedMemory;

/** Circular buffer. Contains constant data for single draw call.
Don't use to store long-live data. */
extern TRef<ACircularBuffer> GCircularBuffer;

/** Sphere mesh */
extern TRef<ASphereMesh> GSphereMesh;

/** Screen aligned quad mesh */
extern TRef<RenderCore::IBuffer> GSaq;

/** Simple white texture */
extern TRef<RenderCore::ITexture> GWhiteTexture;

/** BRDF */
extern TRef<RenderCore::ITexture> GLookupBRDF;

/** Cluster lookcup 3D texture */
extern TRef<RenderCore::ITexture> GClusterLookup;

/** Cluster item references */
extern TRef<RenderCore::IBuffer> GClusterItemBuffer;

/** Cluster item references view */
extern TRef<RenderCore::IBufferView> GClusterItemTBO;

struct SRenderViewContext
{
    /** View constant binding */
    size_t ViewConstantBufferBindingBindingOffset;
    size_t ViewConstantBufferBindingBindingSize;
};
extern TVector<SRenderViewContext> GRenderViewContext;


extern AVirtualTextureFeedbackAnalyzer* GFeedbackAnalyzerVT;
extern AVirtualTextureCache*            GPhysCacheVT;

extern RenderCore::IPipeline* GTerrainDepthPipeline;
extern RenderCore::IPipeline* GTerrainLightPipeline;
extern RenderCore::IPipeline* GTerrainWireframePipeline;

//
// Common functions
//

RenderCore::STextureResolution2D GetFrameResoultion();

void DrawSAQ(RenderCore::IImmediateContext* immediateCtx, RenderCore::IPipeline* Pipeline, unsigned int InstanceCount = 1);

void DrawSphere(RenderCore::IImmediateContext* immediateCtx, RenderCore::IPipeline* Pipeline, unsigned int InstanceCount = 1);

void BindVertexAndIndexBuffers(RenderCore::IImmediateContext* immediateCtx, SRenderInstance const* Instance);

void BindVertexAndIndexBuffers(RenderCore::IImmediateContext* immediateCtx, SShadowRenderInstance const* Instance);

void BindVertexAndIndexBuffers(RenderCore::IImmediateContext* immediateCtx, SLightPortalRenderInstance const* Instance);

void BindSkeleton(size_t _Offset, size_t _Size);
void BindSkeletonMotionBlur(size_t _Offset, size_t _Size);

void BindTextures(RenderCore::IResourceTable* Rtbl, SMaterialFrameData* Instance, int MaxTextures);
void BindTextures(SMaterialFrameData* Instance, int MaxTextures);

void BindInstanceConstants(SRenderInstance const* Instance);

void BindInstanceConstantsFB(SRenderInstance const* Instance);

void BindShadowInstanceConstants(SShadowRenderInstance const* Instance);
void BindShadowInstanceConstants(SShadowRenderInstance const* Instance, int FaceIndex, Float3 const& LightPosition);

void* MapDrawCallConstants(size_t SizeInBytes);

template <typename T>
T* MapDrawCallConstants()
{
    return (T*)MapDrawCallConstants(sizeof(T));
}

void BindShadowMatrix();
void BindShadowCascades(size_t StreamHandle);
void BindOmniShadowProjection(int FaceIndex);

void SaveSnapshot(RenderCore::ITexture& _Texture);

HK_FORCEINLINE void StoreFloat3x3AsFloat3x4Transposed(Float3x3 const& _In, Float3x4& _Out)
{
    _Out[0][0] = _In[0][0];
    _Out[0][1] = _In[1][0];
    _Out[0][2] = _In[2][0];
    _Out[0][3] = 0;

    _Out[1][0] = _In[0][1];
    _Out[1][1] = _In[1][1];
    _Out[1][2] = _In[2][1];
    _Out[1][3] = 0;

    _Out[2][0] = _In[0][2];
    _Out[2][1] = _In[1][2];
    _Out[2][2] = _In[2][2];
    _Out[2][3] = 0;
}

HK_FORCEINLINE void StoreFloat3x4AsFloat4x4Transposed(Float3x4 const& _In, Float4x4& _Out)
{
    _Out[0][0] = _In[0][0];
    _Out[0][1] = _In[1][0];
    _Out[0][2] = _In[2][0];
    _Out[0][3] = 0;

    _Out[1][0] = _In[0][1];
    _Out[1][1] = _In[1][1];
    _Out[1][2] = _In[2][1];
    _Out[1][3] = 0;

    _Out[2][0] = _In[0][2];
    _Out[2][1] = _In[1][2];
    _Out[2][2] = _In[2][2];
    _Out[2][3] = 0;

    _Out[3][0] = _In[0][3];
    _Out[3][1] = _In[1][3];
    _Out[3][2] = _In[2][3];
    _Out[3][3] = 1;
}
