/*

Angie Engine Source Code

MIT License

Copyright (C) 2017-2022 Alexander Samusev.

This file is part of the Angie Engine Source Code.

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

#include <RenderCore/ImmediateContext.h>

#include "RenderDefs.h"

void InitMaterialSamplers();

void CreateDepthPassPipeline( TRef< RenderCore::IPipeline > * ppPipeline, const char * _SourceCode, bool _AlphaMasking, RenderCore::POLYGON_CULL _CullMode, bool _Skinned, bool _Tessellation, STextureSampler const * Samplers, int NumSamplers );

void CreateDepthVelocityPassPipeline( TRef< RenderCore::IPipeline > * ppPipeline, const char * _SourceCode, RenderCore::POLYGON_CULL _CullMode, bool _Skinned, bool _Tessellation, STextureSampler const * Samplers, int NumSamplers );

void CreateWireframePassPipeline( TRef< RenderCore::IPipeline > * ppPipeline, const char * _SourceCode, RenderCore::POLYGON_CULL _CullMode, bool _Skinned, bool _Tessellation, STextureSampler const * Samplers, int NumSamplers );

void CreateNormalsPassPipeline( TRef< RenderCore::IPipeline > * ppPipeline, const char * _SourceCode, bool _Skinned, STextureSampler const * Samplers, int NumSamplers );

void CreateHUDPipeline( TRef< RenderCore::IPipeline > * ppPipeline, const char * _SourceCode, STextureSampler const * Samplers, int NumSamplers );

void CreateLightPassPipeline( TRef< RenderCore::IPipeline > * ppPipeline, const char * _SourceCode, RenderCore::POLYGON_CULL _CullMode, bool _Skinned, bool _DepthTest, bool _Translucent, EColorBlending _Blending, bool _Tessellation, STextureSampler const * Samplers, int NumSamplers );

void CreateLightPassLightmapPipeline( TRef< RenderCore::IPipeline > * ppPipeline, const char * _SourceCode, RenderCore::POLYGON_CULL _CullMode, bool _DepthTest, bool _Translucent, EColorBlending _Blending, bool _Tessellation, STextureSampler const * Samplers, int NumSamplers );

void CreateLightPassVertexLightPipeline( TRef< RenderCore::IPipeline > * ppPipeline, const char * _SourceCode, RenderCore::POLYGON_CULL _CullMode, bool _DepthTest, bool _Translucent, EColorBlending _Blending, bool _Tessellation, STextureSampler const * Samplers, int NumSamplers );

void CreateShadowMapPassPipeline( TRef< RenderCore::IPipeline > * ppPipeline, const char * _SourceCode, bool _ShadowMasking, bool _TwoSided, bool _Skinned, bool _Tessellation, STextureSampler const * Samplers, int NumSamplers );

void CreateFeedbackPassPipeline( TRef< RenderCore::IPipeline > * ppPipeline, const char * _SourceCode, RenderCore::POLYGON_CULL _CullMode, bool _Skinned, STextureSampler const * Samplers, int NumSamplers );

void CreateOutlinePassPipeline( TRef< RenderCore::IPipeline > * ppPipeline, const char * _SourceCode, RenderCore::POLYGON_CULL _CullMode, bool _Skinned, bool _Tessellation, STextureSampler const * Samplers, int NumSamplers );

void CreateTerrainMaterialDepth( TRef< RenderCore::IPipeline > * ppPipeline );
void CreateTerrainMaterialLight( TRef< RenderCore::IPipeline > * ppPipeline );
void CreateTerrainMaterialWireframe( TRef< RenderCore::IPipeline > * ppPipeline );
