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

#include "BaseObject.h"

class AResource : public ABaseObject
{
    AN_CLASS(AResource, ABaseObject)

    friend class AResourceManager;

public:
    /** Initialize default object representation */
    void InitializeDefaultObject();

    /** Initialize object from file */
    void InitializeFromFile(const char* _Path);

    /** Get physical resource path */
    AString const& GetResourcePath() const { return ResourcePath; }

protected:
    AResource() {}

    /** Load resource from file */
    virtual bool LoadResource(IBinaryStream& _Stream) { return false; }

    /** Create internal resource */
    virtual void LoadInternalResource(const char* _Path) {}

    virtual const char* GetDefaultResourcePath() const { return "/Default/UnknownResource"; }

private:
    bool LoadFromPath(const char* _Path);

    /** Set physical resource path */
    void SetResourcePath(const char* _ResourcePath) { ResourcePath = _ResourcePath; }

    /** Set physical resource path */
    void SetResourcePath(AString const& _ResourcePath) { ResourcePath = _ResourcePath; }

    AString ResourceAlias;
    AString ResourcePath;
};

class ABinaryResource : public AResource
{
    AN_CLASS(ABinaryResource, AResource)

public:
    void* GetBinaryData()
    {
        return pBinaryData;
    }

    size_t GetSizeInBytes() const
    {
        return SizeInBytes;
    }

    const char* GetAsString() const
    {
        return pBinaryData ? (const char*)pBinaryData : "";
    }

    ABinaryResource();
    ~ABinaryResource();

protected:

    void Purge();

    /** Load resource from file */
    bool LoadResource(IBinaryStream& _Stream) override;

    /** Create internal resource */
    void LoadInternalResource(const char* _Path) override;

private:
    void*  pBinaryData = nullptr;
    size_t SizeInBytes = 0;
};
