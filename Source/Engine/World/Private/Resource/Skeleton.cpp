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

#include <World/Public/Resource/Skeleton.h>
#include <World/Public/Resource/IndexedMesh.h>
#include <World/Public/Resource/Asset.h>

#include <Platform/Public/Logger.h>

AN_CLASS_META( ASkeleton )

///////////////////////////////////////////////////////////////////////////////////////////////////////

ASkeleton::ASkeleton() {
    BindposeBounds.Clear();
}

ASkeleton::~ASkeleton() {
    Purge();
}

void ASkeleton::Purge() {
    Joints.Clear();
}

void ASkeleton::Initialize( SJoint * _Joints, int _JointsCount, BvAxisAlignedBox const & _BindposeBounds ) {
    Purge();

    if ( _JointsCount < 0 ) {
        GLogger.Printf( "ASkeleton::Initialize: joints count < 0\n" );
        _JointsCount = 0;
    }

    // Copy joints
    Joints.ResizeInvalidate( _JointsCount );
    if ( _JointsCount > 0 ) {
        Platform::Memcpy( Joints.ToPtr(), _Joints, sizeof( *_Joints ) * _JointsCount );
    }

    BindposeBounds = _BindposeBounds;
}

void ASkeleton::LoadInternalResource( const char * _Path ) {
    Purge();

    if ( !Platform::Stricmp( _Path, "/Default/Skeleton/Default" ) ) {
        Initialize( nullptr, 0, BvAxisAlignedBox::Empty() );
        return;
    }

    GLogger.Printf( "Unknown internal skeleton %s\n", _Path );

    LoadInternalResource( "/Default/Skeleton/Default" );
}

bool ASkeleton::LoadResource( IBinaryStream & Stream ) {
    uint32_t fileFormat = Stream.ReadUInt32();

    if ( fileFormat != FMT_FILE_TYPE_SKELETON ) {
        GLogger.Printf( "Expected file format %d\n", FMT_FILE_TYPE_SKELETON );
        return false;
    }

    uint32_t fileVersion = Stream.ReadUInt32();

    if ( fileVersion != FMT_VERSION_SKELETON ) {
        GLogger.Printf( "Expected file version %d\n", FMT_VERSION_SKELETON );
        return false;
    }

    Purge();

    AString guid;

    Stream.ReadObject( guid );
    Stream.ReadArrayOfStructs( Joints );
    Stream.ReadObject( BindposeBounds );

    return true;
}

int ASkeleton::FindJoint( const char * _Name ) const {
    for ( int j = 0 ; j < Joints.Size() ; j++ ) {
        if ( !Platform::Stricmp( Joints[j].Name, _Name ) ) {
            return j;
        }
    }
    return -1;
}

