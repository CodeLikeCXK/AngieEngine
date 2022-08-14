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

enum EWidgetButtonTextAlign
{
    WIDGET_BUTTON_TEXT_ALIGN_CENTER,
    WIDGET_BUTTON_TEXT_ALIGN_LEFT,
    WIDGET_BUTTON_TEXT_ALIGN_RIGHT
};

class WButton : public WWidget
{
    HK_CLASS(WButton, WWidget)

public:
    TWidgetEvent<WButton*> E_OnButtonClick;

    WButton();
    ~WButton();

    template <typename T, typename... TArgs>
    WButton& SetOnClick(T* _Object, void (T::*_Method)(TArgs...))
    {
        E_OnButtonClick.Add(_Object, _Method);
        return *this;
    }

    WButton& SetToggleButton(bool _bToggleButton)
    {
        bToggleButton = _bToggleButton;
        return *this;
    }

    WButton& SetPressed(bool bPressed)
    {
        if (bToggleButton)
        {
            State = bPressed ? ST_PRESSED : ST_RELEASED;
        }
        return *this;
    }

    bool IsPressed() const { return State == ST_PRESSED; }
    bool IsReleased() const { return State == ST_RELEASED; }

    bool IsToggleButton() const { return bToggleButton; }

protected:
    void OnMouseButtonEvent(struct SMouseButtonEvent const& _Event, double _TimeStamp) override;

    void OnDrawEvent(ACanvas& _Canvas) override;

    int GetDrawState() const;

private:
    enum
    {
        ST_RELEASED = 0,
        ST_PRESSED  = 1
    };

    int  State;
    bool bToggleButton;
};

class WTextButton : public WButton
{
    HK_CLASS(WTextButton, WButton)

public:
    WTextButton& SetText(AStringView _Text);
    WTextButton& SetColor(Color4 const& _Color);
    WTextButton& SetHoverColor(Color4 const& _Color);
    WTextButton& SetPressedColor(Color4 const& _Color);
    WTextButton& SetTextColor(Color4 const& _Color);
    WTextButton& SetBorderColor(Color4 const& _Color);
    WTextButton& SetRounding(float _Rounding);
    WTextButton& SetRoundingCorners(CORNER_ROUND_FLAGS _RoundingCorners);
    WTextButton& SetBorderThickness(float _Thickness);
    WTextButton& SetTextAlign(EWidgetButtonTextAlign _TextAlign);
    WTextButton& SetFont(AFont* _Font);

    AString const& GetText() const { return Text; }

    WTextButton();
    ~WTextButton();

protected:
    void OnDrawEvent(ACanvas& _Canvas) override;

private:
    Color4                 Color;
    Color4                 HoverColor;
    Color4                 PressedColor;
    Color4                 TextColor;
    Color4                 BorderColor;
    CORNER_ROUND_FLAGS     RoundingCorners;
    EWidgetButtonTextAlign TextAlign;
    AString                Text;
    float                  Rounding;
    float                  BorderThickness;
    TRef<AFont>            Font;
};

class WImageButton : public WButton
{
    HK_CLASS(WImageButton, WButton)

public:
    WImageButton& SetImage(ATexture* _Image);
    WImageButton& SetHoverImage(ATexture* _Image);
    WImageButton& SetPressedImage(ATexture* _Image);

protected:
    WImageButton();
    ~WImageButton();

    void OnDrawEvent(ACanvas& _Canvas) override;

private:
    TRef<ATexture> Image;
    TRef<ATexture> HoverImage;
    TRef<ATexture> PressedImage;
};
