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

#include "PunctualLightComponent.h"

class AIBLComponent : public APunctualLightComponent
{
    HK_COMPONENT(AIBLComponent, APunctualLightComponent)

public:
    void  SetRadius(float _Radius);
    float GetRadius() const { return Radius; }

    void SetIrradianceMap(int _Index);
    int  GetIrradianceMap() const { return IrradianceMap; }

    void SetReflectionMap(int _Index);
    int  GetReflectionMap() const { return ReflectionMap; }

    BvSphere const& GetSphereWorldBounds() const { return SphereWorldBounds; }

    void PackProbe(Float4x4 const& InViewMatrix, struct SProbeParameters& Probe);

protected:
    AIBLComponent();

    void OnTransformDirty() override;
    void DrawDebug(ADebugRenderer* InRenderer) override;

private:
    void UpdateWorldBounds();

    BvSphere      SphereWorldBounds;
    BvOrientedBox OBBWorldBounds;

    float Radius;

    int IrradianceMap = 0;
    int ReflectionMap = 0;
};
