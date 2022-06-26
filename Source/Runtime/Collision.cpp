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

#include "Collision.h"
#include "World.h"
#include "BulletCompatibility.h"

HK_CLASS_META(AHitProxy)

AHitProxy::AHitProxy()
{
}

AHitProxy::~AHitProxy()
{
    for (AActor* actor : CollisionIgnoreActors)
    {
        actor->RemoveRef();
    }
}

void AHitProxy::Initialize(ASceneComponent* _OwnerComponent, btCollisionObject* _CollisionObject)
{
    HK_ASSERT(!OwnerComponent);

    OwnerComponent  = _OwnerComponent;
    CollisionObject = _CollisionObject;

    AWorld* world = OwnerComponent->GetWorld();
    world->PhysicsSystem.AddHitProxy(this);
}

void AHitProxy::Deinitialize()
{
    if (OwnerComponent)
    {
        AWorld* world = OwnerComponent->GetWorld();
        world->PhysicsSystem.RemoveHitProxy(this);

        OwnerComponent  = nullptr;
        CollisionObject = nullptr;
    }
}

void AHitProxy::UpdateBroadphase()
{
    // Re-add collision object to physics world
    if (bInWorld)
    {
        GetWorld()->PhysicsSystem.AddHitProxy(this);
    }
}

void AHitProxy::SetCollisionGroup(COLLISION_MASK _CollisionGroup)
{
    if (CollisionGroup == _CollisionGroup)
    {
        return;
    }

    CollisionGroup = _CollisionGroup;

    UpdateBroadphase();
}

void AHitProxy::SetCollisionMask(COLLISION_MASK _CollisionMask)
{
    if (CollisionMask == _CollisionMask)
    {
        return;
    }

    CollisionMask = _CollisionMask;

    UpdateBroadphase();
}

void AHitProxy::SetCollisionFilter(COLLISION_MASK _CollisionGroup, COLLISION_MASK _CollisionMask)
{
    if (CollisionGroup == _CollisionGroup && CollisionMask == _CollisionMask)
    {
        return;
    }

    CollisionGroup = _CollisionGroup;
    CollisionMask  = _CollisionMask;

    UpdateBroadphase();
}

void AHitProxy::AddCollisionIgnoreActor(AActor* _Actor)
{
    if (!_Actor)
    {
        return;
    }
    if (!CollisionIgnoreActors.Contains(_Actor))
    {
        CollisionIgnoreActors.Add(_Actor);
        _Actor->AddRef();

        UpdateBroadphase();
    }
}

void AHitProxy::RemoveCollisionIgnoreActor(AActor* _Actor)
{
    if (!_Actor)
    {
        return;
    }
    auto index = CollisionIgnoreActors.IndexOf(_Actor);
    if (index != Core::NPOS)
    {
        _Actor->RemoveRef();

        CollisionIgnoreActors.RemoveUnsorted(index);

        UpdateBroadphase();
    }
}

struct SContactQueryCallback : public btCollisionWorld::ContactResultCallback
{
    SContactQueryCallback(TPodVector<AHitProxy*>& _Result, int _CollisionGroup, int _CollisionMask, AHitProxy const* _Self) :
        Result(_Result), Self(_Self)
    {
        m_collisionFilterGroup = _CollisionGroup;
        m_collisionFilterMask  = _CollisionMask;

        Result.Clear();
    }

    btScalar addSingleResult(btManifoldPoint& cp, const btCollisionObjectWrapper* colObj0Wrap, int partId0, int index0, const btCollisionObjectWrapper* colObj1Wrap, int partId1, int index1) override
    {
        AHitProxy* hitProxy;

        hitProxy = static_cast<AHitProxy*>(colObj0Wrap->getCollisionObject()->getUserPointer());
        if (hitProxy && hitProxy != Self /*&& (hitProxy->GetCollisionGroup() & CollisionMask)*/)
        {
            AddUnique(hitProxy);
        }

        hitProxy = static_cast<AHitProxy*>(colObj1Wrap->getCollisionObject()->getUserPointer());
        if (hitProxy && hitProxy != Self /*&& (hitProxy->GetCollisionGroup() & CollisionMask)*/)
        {
            AddUnique(hitProxy);
        }

        return 0.0f;
    }

    void AddUnique(AHitProxy* HitProxy)
    {
        Result.AddUnique(HitProxy);
    }

    TPodVector<AHitProxy*>& Result;
    AHitProxy const*        Self;
};

struct SContactQueryActorCallback : public btCollisionWorld::ContactResultCallback
{
    SContactQueryActorCallback(TPodVector<AActor*>& _Result, int _CollisionGroup, int _CollisionMask, AActor const* _Self) :
        Result(_Result), Self(_Self)
    {
        m_collisionFilterGroup = _CollisionGroup;
        m_collisionFilterMask  = _CollisionMask;

        Result.Clear();
    }

    btScalar addSingleResult(btManifoldPoint& cp, const btCollisionObjectWrapper* colObj0Wrap, int partId0, int index0, const btCollisionObjectWrapper* colObj1Wrap, int partId1, int index1) override
    {
        AHitProxy* hitProxy;

        hitProxy = static_cast<AHitProxy*>(colObj0Wrap->getCollisionObject()->getUserPointer());
        if (hitProxy && hitProxy->GetOwnerActor() != Self /*&& (hitProxy->GetCollisionGroup() & CollisionMask)*/)
        {
            AddUnique(hitProxy->GetOwnerActor());
        }

        hitProxy = static_cast<AHitProxy*>(colObj1Wrap->getCollisionObject()->getUserPointer());
        if (hitProxy && hitProxy->GetOwnerActor() != Self /*&& (hitProxy->GetCollisionGroup() & CollisionMask)*/)
        {
            AddUnique(hitProxy->GetOwnerActor());
        }

        return 0.0f;
    }

    void AddUnique(AActor* Actor)
    {
        Result.AddUnique(Actor);
    }

    TPodVector<AActor*>& Result;
    AActor const*        Self;
};

void AHitProxy::CollisionContactQuery(TPodVector<AHitProxy*>& _Result) const
{
    SContactQueryCallback callback(_Result, CollisionGroup, CollisionMask, this);

    if (!CollisionObject)
    {
        LOG("AHitProxy::CollisionContactQuery: No collision object\n");
        return;
    }

    if (!bInWorld)
    {
        LOG("AHitProxy::CollisionContactQuery: The body is not in world\n");
        return;
    }

    GetWorld()->PhysicsSystem.GetInternal()->contactTest(CollisionObject, callback);
}

void AHitProxy::CollisionContactQueryActor(TPodVector<AActor*>& _Result) const
{
    SContactQueryActorCallback callback(_Result, CollisionGroup, CollisionMask, GetOwnerActor());

    if (!CollisionObject)
    {
        LOG("AHitProxy::CollisionContactQueryActor: No collision object\n");
        return;
    }

    if (!bInWorld)
    {
        LOG("AHitProxy::CollisionContactQueryActor: The body is not in world\n");
        return;
    }

    GetWorld()->PhysicsSystem.GetInternal()->contactTest(CollisionObject, callback);
}

void AHitProxy::DrawCollisionShape(ADebugRenderer* InRenderer)
{
    if (CollisionObject)
    {
        btDrawCollisionShape(InRenderer, CollisionObject->getWorldTransform(), CollisionObject->getCollisionShape());
    }
}
