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

#include <World/Public/Actors/Actor.h>
#include <World/Public/Actors/Pawn.h>
#include <World/Public/Components/SceneComponent.h>
#include <World/Public/World.h>
#include <World/Public/Timer.h>
#include <Platform/Public/Logger.h>
#include <Runtime/Public/RuntimeVariable.h>
#include <angelscript.h>

AN_BEGIN_CLASS_META( AActor )
AN_END_CLASS_META()

ARuntimeVariable com_DrawRootComponentAxis( _CTS( "com_DrawRootComponentAxis" ), _CTS( "0" ), VAR_CHEAT );

static uint32_t UniqueName = 0;

AActor::AActor()
{
    //GUID.Generate();
    SetObjectName( "Actor" + Math::ToString( UniqueName ) );
    UniqueName++;
}

//void AActor::SetName( AString const & _Name )
//{
//    if ( !ParentWorld ) {
//        // In constructor
//        Name = _Name;
//        return;
//    }
//
//    AString newName = _Name;
//
//    // Clear name for GenerateActorUniqueName
//    Name.Clear();
//
//    // Generate new name
//    Name = ParentWorld->GenerateActorUniqueName( newName.CStr() );
//}

void AActor::Destroy()
{
    if ( bPendingKill ) {
        return;
    }

    // Mark actor to remove it from the world
    bPendingKill = true;
    NextPendingKillActor = ParentWorld->PendingKillActors;
    ParentWorld->PendingKillActors = this;

    // FIXME: do next code at end of the frame?

    DestroyComponents();

    EndPlay();

    if ( Instigator ) {
        Instigator->RemoveRef();
        Instigator = nullptr;
    }

    // Remove actor from level array of actors
    ALevel * level = Level;
    level->Actors[ IndexInLevelArrayOfActors ] = level->Actors[ level->Actors.Size() - 1 ];
    level->Actors[ IndexInLevelArrayOfActors ]->IndexInLevelArrayOfActors = IndexInLevelArrayOfActors;
    level->Actors.RemoveLast();
    IndexInLevelArrayOfActors = -1;
    Level = nullptr;
}

void AActor::DestroyComponents()
{
    for ( AActorComponent * component : Components ) {
        component->Destroy();
    }
}

void AActor::AddComponent( AActorComponent * _Component )
{
    Components.Append( _Component );
    _Component->ComponentIndex = Components.Size() - 1;
    _Component->OwnerActor = this;
    _Component->bCreatedDuringConstruction = bDuringConstruction;
}

//AString AActor::GenerateComponentUniqueName( const char * _Name )
//{
//    if ( !FindComponent( _Name ) ) {
//        return _Name;
//    }
//    int uniqueNumber = 0;
//    AString uniqueName;
//    do {
//        uniqueName.Resize( 0 );
//        uniqueName.Concat( _Name );
//        uniqueName.Concat( Int( ++uniqueNumber ).CStr() );
//    } while ( FindComponent( uniqueName.CStr() ) != nullptr );
//    return uniqueName;
//}

AActorComponent * AActor::CreateComponent( uint64_t _ClassId, const char * _Name )
{
    AActorComponent * component = static_cast< AActorComponent * >( AActorComponent::Factory().CreateInstance( _ClassId ) );
    if ( !component ) {
        return nullptr;
    }
    component->AddRef();
    component->SetObjectName( _Name );//GenerateComponentUniqueName( _Name );
    AddComponent( component );
    return component;
}

AActorComponent * AActor::CreateComponent( const char * _ClassName, const char * _Name )
{
    AActorComponent * component = static_cast< AActorComponent * >( AActorComponent::Factory().CreateInstance( _ClassName ) );
    if ( !component ) {
        return nullptr;
    }
    component->AddRef();
    component->SetObjectName( _Name );//GenerateComponentUniqueName( _Name );
    AddComponent( component );
    return component;
}

AActorComponent * AActor::CreateComponent( AClassMeta const * _ClassMeta, const char * _Name )
{
    AN_ASSERT( _ClassMeta->Factory() == &AActorComponent::Factory() );
    AActorComponent * component = static_cast< AActorComponent * >( _ClassMeta->CreateInstance() );
    if ( !component ) {
        return nullptr;
    }
    component->AddRef();
    component->SetObjectName( _Name );//GenerateComponentUniqueName( _Name );
    AddComponent( component );
    return component;
}

AActorComponent * AActor::GetComponent( uint64_t _ClassId )
{
    for ( AActorComponent * component : Components ) {
        if ( component->FinalClassId() == _ClassId ) {
            return component;
        }
    }
    return nullptr;
}

AActorComponent * AActor::GetComponent( const char * _ClassName )
{
    for ( AActorComponent * component : Components ) {
        if ( !Platform::Strcmp( component->FinalClassName(), _ClassName ) ) {
            return component;
        }
    }
    return nullptr;
}

AActorComponent * AActor::GetComponent( AClassMeta const * _ClassMeta )
{
    AN_ASSERT( _ClassMeta->Factory() == &AActorComponent::Factory() );
    for ( AActorComponent * component : Components ) {
        if ( &component->FinalClassMeta() == _ClassMeta ) {
            return component;
        }
    }
    return nullptr;
}

//AActorComponent * AActor::FindComponent( const char * _UniqueName )
//{
//    for ( AActorComponent * component : Components ) {
//        if ( !component->GetName().Icmp( _UniqueName ) ) {
//            return component;
//        }
//    }
//    return nullptr;
//}

//AActorComponent * AActor::FindComponentGUID( AGUID const & _GUID )
//{
//    for ( AActorComponent * component : Components ) {
//        if ( component->GUID == _GUID ) {
//            return component;
//        }
//    }
//    return nullptr;
//}

void AActor::Initialize( STransform const & _SpawnTransform )
{
    if ( RootComponent ) {
        RootComponent->SetTransform( _SpawnTransform );
    }

    PreInitializeComponents();
    InitializeComponents();
    PostInitializeComponents();

    BeginPlayComponents();
    BeginPlay();
}

void AActor::InitializeComponents()
{
    for ( AActorComponent * component : Components ) {
        component->InitializeComponent();
        component->bInitialized = true;
    }
}

void AActor::BeginPlayComponents()
{
    for ( AActorComponent * component : Components ) {
        if ( !component->IsPendingKill() ) {
            component->BeginPlay();
        }
    }
}

void AActor::TickComponents( float _TimeStep )
{
    for ( AActorComponent * component : Components ) {
        if ( component->bCanEverTick && !component->IsPendingKill() ) {
            component->TickComponent( _TimeStep );
        }
    }
}

TRef< ADocObject > AActor::Serialize()
{
    TRef< ADocObject > object = Super::Serialize();

    //object->AddString( "GUID", GUID.ToString() );

    //if ( RootComponent ) {
    //    object->AddString( "Root", RootComponent->GetName() );
    //}

    //TRef< ADocMember >  components = object->AddArray( "Components" );

    //for ( AActorComponent * component : Components ) {
    //    if ( component->IsPendingKill() ) {
    //        continue;
    //    }
    //    TRef< ADocumentObject > componentObject = component->Serialize();
    //    components->AddValue( componentObject );
    //}

    return object;
}

#if 0
AActorComponent * AActor::LoadComponent( ADocument const & _Document, int _FieldsHead )
{
    SDocumentField const * classNameField = _Document.FindField( _FieldsHead, "ClassName" );
    if ( !classNameField ) {
        GLogger.Printf( "AActor::LoadComponent: invalid component class\n" );
        return nullptr;
    }

    SDocumentValue const * classNameValue = &_Document.Values[ classNameField->ValuesHead ];

    AClassMeta const * classMeta = AActorComponent::Factory().LookupClass( classNameValue->Token.ToString().CStr() );
    if ( !classMeta ) {
        GLogger.Printf( "AActor::LoadComponent: invalid component class \"%s\"\n", classNameValue->Token.ToString().CStr() );
        return nullptr;
    }

    AString name, guid;

    SDocumentField * field = _Document.FindField( _FieldsHead, "Name" );
    if ( field ) {
        name = _Document.Values[ field->ValuesHead ].Token.ToString();
    }

    field = _Document.FindField( _FieldsHead, "GUID" );
    if ( field ) {
        guid = _Document.Values[field->ValuesHead].Token.ToString();
    }

    bool bCreatedDuringConstruction = false;

    AActorComponent * component = nullptr;

    if ( !guid.IsEmpty() ) {
        component = FindComponentGUID( AGUID().FromString( guid ) );
        if ( component && &component->FinalClassMeta() == classMeta ) {
            bCreatedDuringConstruction = component->bCreatedDuringConstruction;
        }
    }

    if ( !bCreatedDuringConstruction ) {
        component = AddComponent( classMeta, name.IsEmpty() ? "Unnamed" : name.CStr() );
    }

    if ( component ) {
        component->LoadAttributes( _Document, _FieldsHead );
    }

    return component;
}
#endif

void AActor::DrawDebug( ADebugRenderer * InRenderer )
{
    for ( AActorComponent * component : Components ) {
        component->DrawDebug( InRenderer );
    }

    if ( com_DrawRootComponentAxis ) {
        if ( RootComponent ) {
            InRenderer->SetDepthTest( false );
            InRenderer->DrawAxis( RootComponent->GetWorldTransformMatrix(), false );
        }
    }
}

#define CALL_SCRIPT(Function, ...)                                        \
    if (pScriptInstance)                                                  \
    {                                                                     \
        AActorScript* pScript = AActorScript::GetScript(pScriptInstance); \
        pScript->Function(pScriptInstance, __VA_ARGS__);                  \
    }

void AActor::BeginPlay()
{
    CALL_SCRIPT(BeginPlay);
}

void AActor::EndPlay()
{
    CALL_SCRIPT(EndPlay);
}

void AActor::Tick(float _TimeStep)
{
    CALL_SCRIPT(Tick, _TimeStep);
}

void AActor::TickPrePhysics(float _TimeStep)
{
    CALL_SCRIPT(TickPrePhysics, _TimeStep);
}

void AActor::TickPostPhysics(float _TimeStep)
{
    CALL_SCRIPT(TickPostPhysics, _TimeStep);
}

void AActor::ApplyDamage( SActorDamage const & Damage )
{
    CALL_SCRIPT(ApplyDamage, Damage);
}

asILockableSharedBool* AActor::ScriptGetWeakRefFlag()
{
    if (!pWeakRefFlag)
        pWeakRefFlag = asCreateLockableSharedBool();

    return pWeakRefFlag;
}
