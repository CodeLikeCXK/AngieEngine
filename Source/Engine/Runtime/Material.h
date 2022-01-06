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

#include "ResourceManager.h"
#include "Texture.h"
#include "VirtualTextureResource.h"

#define MAX_MATERIAL_UNIFORMS           16
#define MAX_MATERIAL_UNIFORM_VECTORS    (16>>2)

class MGMaterialGraph;

/**

Material

*/
class AMaterial : public AResource {
    AN_CLASS( AMaterial, AResource )

public:
    /** Initialize from graph */
    void Initialize( MGMaterialGraph * Graph );

    void Purge();

    /** Get material type */
    EMaterialType GetType() const { return Def.Type; }

    bool IsTranslucent() const { return Def.bTranslucent; }

    bool IsTwoSided() const { return Def.bTwoSided; }

    bool CanCastShadow() const { return !Def.bNoCastShadow; }

    AMaterialGPU * GetGPUResource() { return &MaterialGPU; }

    int GetNumUniformVectors() const { return Def.NumUniformVectors; }

    uint8_t GetRenderingPriority() const { return Def.RenderingPriority; }

    static void RebuildMaterials();

protected:
    AMaterial();
    ~AMaterial();

    /** Load resource from file */
    bool LoadResource( IBinaryStream & Stream ) override;

    /** Create internal resource */
    void LoadInternalResource( const char * _Path ) override;

    const char * GetDefaultResourcePath() const override { return "/Default/Materials/Unlit"; }

private:
    /** Material GPU representation */
    AMaterialGPU MaterialGPU;

    /** Source graph */
    TWeakRef< MGMaterialGraph > MaterialGraph;

    /** Material definition */
    SMaterialDef Def;

    AMaterial * pNext = nullptr;
    AMaterial * pPrev = nullptr;
};

AN_FORCEINLINE AMaterial * CreateMaterial( MGMaterialGraph * InGraph )
{
    AMaterial * material = CreateInstanceOf< AMaterial >();
    material->Initialize( InGraph );
    return material;
}


/**

Material Instance

*/
class AMaterialInstance : public AResource {
    AN_CLASS( AMaterialInstance, AResource )

public:
    union
    {
        /** Instance uniforms */
        float Uniforms[MAX_MATERIAL_UNIFORMS];

        /** Instance uniform vectors */
        Float4 UniformVectors[MAX_MATERIAL_UNIFORM_VECTORS];
    };

    /** Set material */
    void SetMaterial( AMaterial * _Material );

    /** Helper. Set material by alias */
    template< char... Chars >
    void SetMaterial( TCompileTimeString<Chars...> const & _Alias ) {
        static TStaticResourceFinder< AMaterial > Resource( _Alias );
        SetMaterial( Resource.GetObject() );
    }

    /** Get material. Never return null. */
    AMaterial * GetMaterial() const;

    /** Set texture for the slot */
    void SetTexture( int _TextureSlot, ATexture * _Texture );

    ATexture * GetTexture( int _TextureSlot );

    /** Helper. Set texture for the slot */
    template< char... Chars >
    void SetTexture( int _TextureSlot, TCompileTimeString<Chars...> const & _Alias ) {
        static TStaticResourceFinder< ATexture > Resource( _Alias );
        SetTexture( _TextureSlot, Resource.GetObject() );
    }

    void SetVirtualTexture( AVirtualTextureResource * VirtualTex );

    /** Internal. Used by render frontend */
    SMaterialFrameData* PreRenderUpdate(class AFrameLoop* FrameLoop, int _FrameNumber);

protected:
    AMaterialInstance();
    ~AMaterialInstance() {}

    /** Load resource from file */
    bool LoadResource( IBinaryStream & _Stream ) override;

    /** Create internal resource */
    void LoadInternalResource( const char * _Path ) override;

    const char * GetDefaultResourcePath() const override { return "/Default/MaterialInstance/Default"; }

    bool LoadTextVersion( IBinaryStream & Stream );

private:
    TRef< AMaterial > Material;
    SMaterialFrameData * FrameData = nullptr;
    TRef< ATexture > Textures[ MAX_MATERIAL_TEXTURES ];
    TRef< AVirtualTextureResource > VirtualTexture;
    int VisFrame = -1;
};
