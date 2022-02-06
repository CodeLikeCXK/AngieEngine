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

#include "SkinnedComponent.h"
#include "AnchorComponent.h"

/*

ASoftMeshComponent

Mesh with softbody physics simulation

*/
class ASoftMeshComponent : public ASkinnedComponent
{
    HK_COMPONENT(ASoftMeshComponent, ASkinnedComponent)

public:
    /** Velocities correction factor (Baumgarte) */
    float VelocitiesCorrection = 1;

    /** Damping coefficient [0,1] */
    float DampingCoefficient = 0;

    /** Drag coefficient [0,+inf] */
    float DragCoefficient = 0;

    /** Lift coefficient [0,+inf] */
    float LiftCoefficient = 0;

    /** Pressure coefficient [-inf,+inf] */
    float Pressure = 0;

    /** Volume conversation coefficient [0,+inf] */
    float VolumeConversation = 0;

    /** Dynamic friction coefficient [0,1] */
    float DynamicFriction = 0.2f;

    /** Pose matching coefficient [0,1] */
    float PoseMatching = 0;

    /** Linear stiffness coefficient [0,1] */
    float LinearStiffness = 1;

    /** Area/Angular stiffness coefficient [0,1] */
    float AngularStiffness = 1;

    /** Volume stiffness coefficient [0,1] */
    float VolumeStiffness = 1;

    //Float3x4 BaseTransform;

    /** Attach vertex to anchor point */
    void AttachVertex(int _VertexIndex, AAnchorComponent* _Anchor);

    /** Detach vertex from anchor point */
    void DetachVertex(int _VertexIndex);

    /** Detach all vertices */
    void DetachAllVertices();

    /** Get vertex attachment */
    AAnchorComponent* GetVertexAnchor(int _VertexIndex) const;

    /** Set a wind velocity for interaction with the air */
    void SetWindVelocity(Float3 const& _Velocity);

    Float3 const& GetWindVelocity() const;

    /** Add force (or gravity) to the entire soft body */
    void AddForceSoftBody(Float3 const& _Force);

    /** Add force (or gravity) to a vertex of the soft body */
    void AddForceToVertex(Float3 const& _Force, int _VertexIndex);

    Float3 GetVertexPosition(int _VertexIndex) const;

    Float3 GetVertexNormal(int _VertexIndex) const;

    Float3 GetVertexVelocity(int _VertexIndex) const;

protected:
    ASoftMeshComponent();

    void InitializeComponent() override;
    void DeinitializeComponent() override;

    void OnMeshChanged() override;

    void TickComponent(float _TimeStep) override;

    void DrawDebug(ADebugRenderer* InRenderer) override;

private:
    void RecreateSoftBody();
    void UpdateAnchorPoints();
    void UpdateSoftbodyTransform();
    void UpdateSoftbodyBoundingBox();

    Float3 WindVelocity = Float3(0.0f);

    //Float3x3 PrevTransformBasis;
    //Float3 PrevTransformOrigin;
    //btRigidBody * Anchor;

    struct SAnchorBinding
    {
        AAnchorComponent* Anchor;
        int               VertexIndex;
    };
    TPodVector<SAnchorBinding> Anchors;
    bool                       bUpdateAnchors = false;
};
