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

#include "WWidget.h"

class WWindow : public WWidget
{
    HK_CLASS(WWindow, WWidget)

public:
    WWindow& SetCaptionText(const char* _CaptionText);
    WWindow& SetCaptionHeight(float _CaptionHeight);
    WWindow& SetCaptionFont(AFont* _Font);
    WWindow& SetTextColor(Color4 const& _Color);
    WWindow& SetTextHorizontalAlignment(EWidgetAlignment _Alignment);
    WWindow& SetTextVerticalAlignment(EWidgetAlignment _Alignment);
    WWindow& SetWordWrap(bool _WordWrap);
    WWindow& SetTextOffset(Float2 const& _Offset);
    WWindow& SetCaptionColor(Color4 const& _Color);
    WWindow& SetCaptionColorNotActive(Color4 const& _Color);
    WWindow& SetBorderColor(Color4 const& _Color);
    WWindow& SetBorderThickness(float _Thickness);
    WWindow& SetBackgroundColor(Color4 const& _Color);
    WWindow& SetRounding(float _Rounding);
    WWindow& SetRoundingCorners(EDrawCornerFlags _RoundingCorners);

    WWindow();
    ~WWindow();

protected:
    void OnTransformDirty() override;
    void OnDrawEvent(ACanvas& _Canvas) override;

    Float2       GetTextPositionWithAlignment(ACanvas& _Canvas) const;
    AFont const* GetFont() const;

private:
    void UpdateDragShape();
    void UpdateMargin();

    AString          CaptionText;
    float            CaptionHeight;
    TRef<AFont>      Font;
    Color4           TextColor;
    Float2           TextOffset;
    bool             bWordWrap;
    EWidgetAlignment TextHorizontalAlignment;
    EWidgetAlignment TextVerticalAlignment;
    Color4           CaptionColor;
    Color4           CaptionColorNotActive;
    Color4           BorderColor;
    Color4           BgColor;
    EDrawCornerFlags RoundingCorners;
    float            BorderRounding;
    float            BorderThickness;
    bool             bWindowBorder;
    bool             bCaptionBorder;
};
