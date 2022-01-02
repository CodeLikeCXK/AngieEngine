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

#include <World/Public/Components/InputComponent.h>
#include <World/Public/World.h>
#include <Runtime/Public/Runtime.h>
#include <Platform/Public/Logger.h>
#include <Platform/Public/Platform.h>
#include <Core/Public/HashFunc.h>
#include <Core/Public/IntrusiveLinkedListMacro.h>

ARuntimeVariable in_MouseSensitivity( _CTS( "in_MouseSensitivity" ), _CTS( "6.8" ) );
ARuntimeVariable in_MouseSensX( _CTS( "in_MouseSensX" ), _CTS( "0.022" ) );
ARuntimeVariable in_MouseSensY( _CTS( "in_MouseSensY" ), _CTS( "0.022" ) );
ARuntimeVariable in_MouseFilter( _CTS( "in_MouseFilter" ), _CTS( "1" ) );
ARuntimeVariable in_MouseInvertY( _CTS( "in_MouseInvertY" ), _CTS( "0" ) );
ARuntimeVariable in_MouseAccel( _CTS( "in_MouseAccel" ), _CTS( "0" ) );

AN_CLASS_META( AInputAxis )
AN_CLASS_META( AInputAction )
AN_CLASS_META( AInputMappings )

AN_BEGIN_CLASS_META( AInputComponent )
AN_ATTRIBUTE_( bIgnoreKeyboardEvents, AF_DEFAULT )
AN_ATTRIBUTE_( bIgnoreMouseEvents, AF_DEFAULT )
AN_ATTRIBUTE_( bIgnoreJoystickEvents, AF_DEFAULT )
AN_ATTRIBUTE_( bIgnoreCharEvents, AF_DEFAULT )
AN_ATTRIBUTE_( ControllerId, AF_DEFAULT )
AN_END_CLASS_META()

struct AInputComponentStatic
{
    TArray< const char *, MAX_KEYBOARD_BUTTONS >  KeyNames;
    TArray< const char *, MAX_MOUSE_BUTTONS >     MouseButtonNames;
    TArray< const char *, MAX_MOUSE_AXES >        MouseAxisNames;
    TArray< const char *, MAX_INPUT_DEVICES >     DeviceNames;
    TArray< const char *, MAX_JOYSTICK_BUTTONS >  JoystickButtonNames;
    TArray< const char *, MAX_JOYSTICK_AXES >     JoystickAxisNames;
    TArray< const char *, MAX_MODIFIERS >         ModifierNames;
    TArray< const char *, MAX_INPUT_CONTROLLERS > ControllerNames;

    TArray< int, MAX_INPUT_DEVICES > DeviceButtonLimits;
    //TArray< SJoystick, MAX_JOYSTICKS_COUNT > Joysticks;
    TArray< TArray< float, MAX_JOYSTICK_AXES >, MAX_JOYSTICKS_COUNT > JoystickAxisState;

    AInputComponentStatic()
    {
#define InitKey( _Key )                 KeyNames[_Key] = AN_STRINGIFY( _Key ) + 4
#define InitKey2( _Key, _Name )         KeyNames[_Key] = _Name
#define InitButton( _Button, _Name )    MouseButtonNames[_Button] = _Name
#define InitMouseAxis( _Axis, _Name )   MouseAxisNames[_Axis - MOUSE_AXIS_BASE] = _Name
#define InitDevice( _DevId )            DeviceNames[_DevId] = AN_STRINGIFY( _DevId ) + 3
#define InitJoyButton( _Button, _Name ) JoystickButtonNames[_Button - JOY_BUTTON_BASE] = _Name
#define InitJoyAxis( _Axis, _Name )     JoystickAxisNames[_Axis - JOY_AXIS_BASE] = _Name
#define InitModifier( _Modifier )       ModifierNames[_Modifier] = AN_STRINGIFY( _Modifier ) + 4
#define InitController( _Controller )   ControllerNames[_Controller] = AN_STRINGIFY( _Controller ) + 11

        DeviceButtonLimits[ID_KEYBOARD] = MAX_KEYBOARD_BUTTONS;
        DeviceButtonLimits[ID_MOUSE]    = MAX_MOUSE_BUTTONS + MAX_MOUSE_AXES;
        for ( int i = ID_JOYSTICK_1; i <= ID_JOYSTICK_16; i++ ) {
            DeviceButtonLimits[i] = MAX_JOYSTICK_BUTTONS + MAX_JOYSTICK_AXES;
        }

        for ( int i = 0; i < KeyNames.Size(); i++ ) {
            KeyNames[i] = "";
        }

        for ( int i = 0; i < MouseButtonNames.Size(); i++ ) {
            MouseButtonNames[i] = "";
        }

        for ( int i = 0; i < MouseAxisNames.Size(); i++ ) {
            MouseAxisNames[i] = "";
        }

        for ( int i = 0; i < DeviceNames.Size(); i++ ) {
            DeviceNames[i] = "";
        }

        for ( int i = 0; i < JoystickButtonNames.Size(); i++ ) {
            JoystickButtonNames[i] = "";
        }

        for ( int i = 0; i < JoystickAxisNames.Size(); i++ ) {
            JoystickAxisNames[i] = "";
        }

        InitKey2( KEY_SPACE, "Space" );
        InitKey2( KEY_APOSTROPHE, "'" );
        InitKey2( KEY_COMMA, "," );
        InitKey2( KEY_MINUS, "-" );
        InitKey2( KEY_PERIOD, "Period" );
        InitKey2( KEY_SLASH, "/" );
        InitKey( KEY_0 );
        InitKey( KEY_1 );
        InitKey( KEY_2 );
        InitKey( KEY_3 );
        InitKey( KEY_4 );
        InitKey( KEY_5 );
        InitKey( KEY_6 );
        InitKey( KEY_7 );
        InitKey( KEY_8 );
        InitKey( KEY_9 );
        InitKey2( KEY_SEMICOLON, ";" );
        InitKey2( KEY_EQUAL, "=" );
        InitKey( KEY_A );
        InitKey( KEY_B );
        InitKey( KEY_C );
        InitKey( KEY_D );
        InitKey( KEY_E );
        InitKey( KEY_F );
        InitKey( KEY_G );
        InitKey( KEY_H );
        InitKey( KEY_I );
        InitKey( KEY_J );
        InitKey( KEY_K );
        InitKey( KEY_L );
        InitKey( KEY_M );
        InitKey( KEY_N );
        InitKey( KEY_O );
        InitKey( KEY_P );
        InitKey( KEY_Q );
        InitKey( KEY_R );
        InitKey( KEY_S );
        InitKey( KEY_T );
        InitKey( KEY_U );
        InitKey( KEY_V );
        InitKey( KEY_W );
        InitKey( KEY_X );
        InitKey( KEY_Y );
        InitKey( KEY_Z );
        InitKey2( KEY_LEFT_BRACKET, "{" );
        InitKey2( KEY_BACKSLASH, "\\" );
        InitKey2( KEY_RIGHT_BRACKET, "}" );
        InitKey2( KEY_GRAVE_ACCENT, "`" );
        InitKey2( KEY_ESCAPE, "Escape" );
        InitKey2( KEY_ENTER, "Enter" );
        InitKey2( KEY_TAB, "Tab" );
        InitKey2( KEY_BACKSPACE, "Backspace" );
        InitKey2( KEY_INSERT, "Insert" );
        InitKey2( KEY_DELETE, "Del" );
        InitKey2( KEY_RIGHT, "Right" );
        InitKey2( KEY_LEFT, "Left" );
        InitKey2( KEY_DOWN, "Down" );
        InitKey2( KEY_UP, "Up" );
        InitKey2( KEY_PAGE_UP, "Page Up" );
        InitKey2( KEY_PAGE_DOWN, "Page Down" );
        InitKey2( KEY_HOME, "Home" );
        InitKey2( KEY_END, "End" );
        InitKey2( KEY_CAPS_LOCK, "Caps Lock" );
        InitKey2( KEY_SCROLL_LOCK, "Scroll Lock" );
        InitKey2( KEY_NUM_LOCK, "Num Lock" );
        InitKey2( KEY_PRINT_SCREEN, "Print Screen" );
        InitKey2( KEY_PAUSE, "Pause" );
        InitKey( KEY_F1 );
        InitKey( KEY_F2 );
        InitKey( KEY_F3 );
        InitKey( KEY_F4 );
        InitKey( KEY_F5 );
        InitKey( KEY_F6 );
        InitKey( KEY_F7 );
        InitKey( KEY_F8 );
        InitKey( KEY_F9 );
        InitKey( KEY_F10 );
        InitKey( KEY_F11 );
        InitKey( KEY_F12 );
        InitKey( KEY_F13 );
        InitKey( KEY_F14 );
        InitKey( KEY_F15 );
        InitKey( KEY_F16 );
        InitKey( KEY_F17 );
        InitKey( KEY_F18 );
        InitKey( KEY_F19 );
        InitKey( KEY_F20 );
        InitKey( KEY_F21 );
        InitKey( KEY_F22 );
        InitKey( KEY_F23 );
        InitKey( KEY_F24 );
        InitKey2( KEY_KP_0, "Num 0" );
        InitKey2( KEY_KP_1, "Num 1" );
        InitKey2( KEY_KP_2, "Num 2" );
        InitKey2( KEY_KP_3, "Num 3" );
        InitKey2( KEY_KP_4, "Num 4" );
        InitKey2( KEY_KP_5, "Num 5" );
        InitKey2( KEY_KP_6, "Num 6" );
        InitKey2( KEY_KP_7, "Num 7" );
        InitKey2( KEY_KP_8, "Num 8" );
        InitKey2( KEY_KP_9, "Num 9" );
        InitKey2( KEY_KP_DECIMAL, "Num Decimal" );
        InitKey2( KEY_KP_DIVIDE, "Num /" );
        InitKey2( KEY_KP_MULTIPLY, "Num *" );
        InitKey2( KEY_KP_SUBTRACT, "Num -" );
        InitKey2( KEY_KP_ADD, "Num +" );
        InitKey2( KEY_KP_ENTER, "Num Enter" );
        InitKey2( KEY_KP_EQUAL, "Num =" );
        InitKey2( KEY_LEFT_SHIFT, "L. Shift" );
        InitKey2( KEY_LEFT_CONTROL, "L. Ctrl" );
        InitKey2( KEY_LEFT_ALT, "L. Alt" );
        InitKey2( KEY_LEFT_SUPER, "L. Super" );
        InitKey2( KEY_RIGHT_SHIFT, "R. Shift" );
        InitKey2( KEY_RIGHT_CONTROL, "R. Ctrl" );
        InitKey2( KEY_RIGHT_ALT, "R. Alt" );
        InitKey2( KEY_RIGHT_SUPER, "R. Super" );
        InitKey2( KEY_MENU, "Menu" );

        InitButton( MOUSE_BUTTON_LEFT, "LBM" );
        InitButton( MOUSE_BUTTON_RIGHT, "RBM" );
        InitButton( MOUSE_BUTTON_MIDDLE, "MBM" );
        InitButton( MOUSE_BUTTON_4, "MB4" );
        InitButton( MOUSE_BUTTON_5, "MB5" );
        InitButton( MOUSE_BUTTON_6, "MB6" );
        InitButton( MOUSE_BUTTON_7, "MB7" );
        InitButton( MOUSE_BUTTON_8, "MB8" );

        InitButton( MOUSE_WHEEL_UP, "Wheel Up" );
        InitButton( MOUSE_WHEEL_DOWN, "Wheel Down" );
        InitButton( MOUSE_WHEEL_LEFT, "Wheel Left" );
        InitButton( MOUSE_WHEEL_RIGHT, "Wheel Right" );

        InitMouseAxis( MOUSE_AXIS_X, "Mouse Axis X" );
        InitMouseAxis( MOUSE_AXIS_Y, "Mouse Axis Y" );

        InitDevice( ID_KEYBOARD );
        InitDevice( ID_MOUSE );
        InitDevice( ID_JOYSTICK_1 );
        InitDevice( ID_JOYSTICK_2 );
        InitDevice( ID_JOYSTICK_3 );
        InitDevice( ID_JOYSTICK_4 );
        InitDevice( ID_JOYSTICK_5 );
        InitDevice( ID_JOYSTICK_6 );
        InitDevice( ID_JOYSTICK_7 );
        InitDevice( ID_JOYSTICK_8 );
        InitDevice( ID_JOYSTICK_9 );
        InitDevice( ID_JOYSTICK_10 );
        InitDevice( ID_JOYSTICK_11 );
        InitDevice( ID_JOYSTICK_12 );
        InitDevice( ID_JOYSTICK_13 );
        InitDevice( ID_JOYSTICK_14 );
        InitDevice( ID_JOYSTICK_15 );
        InitDevice( ID_JOYSTICK_16 );

        InitJoyButton( JOY_BUTTON_1, "Joy Btn 1" );
        InitJoyButton( JOY_BUTTON_2, "Joy Btn 2" );
        InitJoyButton( JOY_BUTTON_3, "Joy Btn 3" );
        InitJoyButton( JOY_BUTTON_4, "Joy Btn 4" );
        InitJoyButton( JOY_BUTTON_5, "Joy Btn 5" );
        InitJoyButton( JOY_BUTTON_6, "Joy Btn 6" );
        InitJoyButton( JOY_BUTTON_7, "Joy Btn 7" );
        InitJoyButton( JOY_BUTTON_8, "Joy Btn 8" );
        InitJoyButton( JOY_BUTTON_9, "Joy Btn 9" );
        InitJoyButton( JOY_BUTTON_10, "Joy Btn 10" );
        InitJoyButton( JOY_BUTTON_11, "Joy Btn 11" );
        InitJoyButton( JOY_BUTTON_12, "Joy Btn 12" );
        InitJoyButton( JOY_BUTTON_13, "Joy Btn 13" );
        InitJoyButton( JOY_BUTTON_14, "Joy Btn 14" );
        InitJoyButton( JOY_BUTTON_15, "Joy Btn 15" );
        InitJoyButton( JOY_BUTTON_16, "Joy Btn 16" );
        InitJoyButton( JOY_BUTTON_17, "Joy Btn 17" );
        InitJoyButton( JOY_BUTTON_18, "Joy Btn 18" );
        InitJoyButton( JOY_BUTTON_19, "Joy Btn 19" );
        InitJoyButton( JOY_BUTTON_20, "Joy Btn 20" );
        InitJoyButton( JOY_BUTTON_21, "Joy Btn 21" );
        InitJoyButton( JOY_BUTTON_22, "Joy Btn 22" );
        InitJoyButton( JOY_BUTTON_23, "Joy Btn 23" );
        InitJoyButton( JOY_BUTTON_24, "Joy Btn 24" );
        InitJoyButton( JOY_BUTTON_25, "Joy Btn 25" );
        InitJoyButton( JOY_BUTTON_26, "Joy Btn 26" );
        InitJoyButton( JOY_BUTTON_27, "Joy Btn 27" );
        InitJoyButton( JOY_BUTTON_28, "Joy Btn 28" );
        InitJoyButton( JOY_BUTTON_29, "Joy Btn 29" );
        InitJoyButton( JOY_BUTTON_30, "Joy Btn 30" );
        InitJoyButton( JOY_BUTTON_31, "Joy Btn 31" );
        InitJoyButton( JOY_BUTTON_32, "Joy Btn 32" );

        InitJoyAxis( JOY_AXIS_1, "Joy Axis 1" );
        InitJoyAxis( JOY_AXIS_2, "Joy Axis 2" );
        InitJoyAxis( JOY_AXIS_3, "Joy Axis 3" );
        InitJoyAxis( JOY_AXIS_4, "Joy Axis 4" );
        InitJoyAxis( JOY_AXIS_5, "Joy Axis 5" );
        InitJoyAxis( JOY_AXIS_6, "Joy Axis 6" );
        InitJoyAxis( JOY_AXIS_7, "Joy Axis 7" );
        InitJoyAxis( JOY_AXIS_8, "Joy Axis 8" );
        InitJoyAxis( JOY_AXIS_9, "Joy Axis 9" );
        InitJoyAxis( JOY_AXIS_10, "Joy Axis 10" );
        InitJoyAxis( JOY_AXIS_11, "Joy Axis 11" );
        InitJoyAxis( JOY_AXIS_12, "Joy Axis 12" );
        InitJoyAxis( JOY_AXIS_13, "Joy Axis 13" );
        InitJoyAxis( JOY_AXIS_14, "Joy Axis 14" );
        InitJoyAxis( JOY_AXIS_15, "Joy Axis 15" );
        InitJoyAxis( JOY_AXIS_16, "Joy Axis 16" );
        InitJoyAxis( JOY_AXIS_17, "Joy Axis 17" );
        InitJoyAxis( JOY_AXIS_18, "Joy Axis 18" );
        InitJoyAxis( JOY_AXIS_19, "Joy Axis 19" );
        InitJoyAxis( JOY_AXIS_20, "Joy Axis 20" );
        InitJoyAxis( JOY_AXIS_21, "Joy Axis 21" );
        InitJoyAxis( JOY_AXIS_22, "Joy Axis 22" );
        InitJoyAxis( JOY_AXIS_23, "Joy Axis 23" );
        InitJoyAxis( JOY_AXIS_24, "Joy Axis 24" );
        InitJoyAxis( JOY_AXIS_25, "Joy Axis 25" );
        InitJoyAxis( JOY_AXIS_26, "Joy Axis 26" );
        InitJoyAxis( JOY_AXIS_27, "Joy Axis 27" );
        InitJoyAxis( JOY_AXIS_28, "Joy Axis 28" );
        InitJoyAxis( JOY_AXIS_29, "Joy Axis 29" );
        InitJoyAxis( JOY_AXIS_30, "Joy Axis 30" );
        InitJoyAxis( JOY_AXIS_31, "Joy Axis 31" );
        InitJoyAxis( JOY_AXIS_32, "Joy Axis 32" );

        InitModifier( KMOD_SHIFT );
        InitModifier( KMOD_CONTROL );
        InitModifier( KMOD_ALT );
        InitModifier( KMOD_SUPER );
        InitModifier( KMOD_CAPS_LOCK );
        InitModifier( KMOD_NUM_LOCK );

        InitController( CONTROLLER_PLAYER_1 );
        InitController( CONTROLLER_PLAYER_2 );
        InitController( CONTROLLER_PLAYER_3 );
        InitController( CONTROLLER_PLAYER_4 );
        InitController( CONTROLLER_PLAYER_5 );
        InitController( CONTROLLER_PLAYER_6 );
        InitController( CONTROLLER_PLAYER_7 );
        InitController( CONTROLLER_PLAYER_8 );
        InitController( CONTROLLER_PLAYER_9 );
        InitController( CONTROLLER_PLAYER_10 );
        InitController( CONTROLLER_PLAYER_11 );
        InitController( CONTROLLER_PLAYER_12 );
        InitController( CONTROLLER_PLAYER_13 );
        InitController( CONTROLLER_PLAYER_14 );
        InitController( CONTROLLER_PLAYER_15 );
        InitController( CONTROLLER_PLAYER_16 );

        //Platform::ZeroMem( Joysticks.ToPtr(), sizeof( Joysticks ) );
        Platform::ZeroMem( JoystickAxisState.ToPtr(), sizeof( JoystickAxisState ) );

        //for ( int i = 0 ; i < MAX_JOYSTICKS_COUNT ; i++ ) {
        //    Joysticks[i].Id = i;
        //}
    }
};

static AInputComponentStatic Static;

AInputComponent * AInputComponent::InputComponents     = nullptr;
AInputComponent * AInputComponent::InputComponentsTail = nullptr;

const char * AInputHelper::TranslateDevice( int _DevId )
{
    if ( _DevId < 0 || _DevId >= MAX_INPUT_DEVICES ) {
        return "UNKNOWN";
    }
    return Static.DeviceNames[_DevId];
}

const char * AInputHelper::TranslateModifier( int _Modifier )
{
    if ( _Modifier < 0 || _Modifier > KMOD_LAST ) {
        return "UNKNOWN";
    }
    return Static.ModifierNames[_Modifier];
}

const char * AInputHelper::TranslateDeviceKey( int _DevId, int _Key )
{
    switch ( _DevId ) {
        case ID_KEYBOARD:
            if ( _Key < 0 || _Key > KEY_LAST ) {
                return "UNKNOWN";
            }
            return Static.KeyNames[_Key];
        case ID_MOUSE:
            if ( _Key >= MOUSE_AXIS_BASE ) {
                if ( _Key > MOUSE_AXIS_LAST ) {
                    return "UNKNOWN";
                }
                return Static.MouseAxisNames[_Key - MOUSE_AXIS_BASE];
            }
            if ( _Key < MOUSE_BUTTON_BASE || _Key > MOUSE_BUTTON_LAST ) {
                return "UNKNOWN";
            }
            return Static.MouseButtonNames[_Key - MOUSE_BUTTON_BASE];
    }
    if ( _DevId >= ID_JOYSTICK_1 && _DevId <= ID_JOYSTICK_16 ) {
        if ( _Key >= JOY_AXIS_BASE ) {
            if ( _Key > JOY_AXIS_LAST ) {
                return "UNKNOWN";
            }
            return Static.JoystickAxisNames[_Key - JOY_AXIS_BASE];
        }

        if ( _Key < JOY_BUTTON_BASE || _Key > JOY_BUTTON_LAST ) {
            return "UNKNOWN";
        }
        return Static.JoystickButtonNames[_Key - JOY_BUTTON_BASE];
    }
    return "UNKNOWN";
}

const char * AInputHelper::TranslateController( int _ControllerId )
{
    if ( _ControllerId < 0 || _ControllerId >= MAX_INPUT_CONTROLLERS ) {
        return "UNKNOWN";
    }
    return Static.ControllerNames[_ControllerId];
}

int AInputHelper::LookupDevice( const char * _Device )
{
    for ( int i = 0; i < MAX_INPUT_DEVICES; i++ ) {
        if ( !Platform::Stricmp( Static.DeviceNames[i], _Device ) ) {
            return i;
        }
    }
    return -1;
}

int AInputHelper::LookupModifier( const char * _Modifier )
{
    for ( int i = 0; i < MAX_MODIFIERS; i++ ) {
        if ( !Platform::Stricmp( Static.ModifierNames[i], _Modifier ) ) {
            return i;
        }
    }
    return -1;
}

int AInputHelper::LookupDeviceKey( int _DevId, const char * _Key )
{
    switch ( _DevId ) {
        case ID_KEYBOARD:
            for ( int i = 0; i < MAX_KEYBOARD_BUTTONS; i++ ) {
                if ( !Platform::Stricmp( Static.KeyNames[i], _Key ) ) {
                    return i;
                }
            }
            return -1;
        case ID_MOUSE:
            for ( int i = 0; i < MAX_MOUSE_BUTTONS; i++ ) {
                if ( !Platform::Stricmp( Static.MouseButtonNames[i], _Key ) ) {
                    return MOUSE_BUTTON_BASE + i;
                }
            }
            for ( int i = 0; i < MAX_MOUSE_AXES; i++ ) {
                if ( !Platform::Stricmp( Static.MouseAxisNames[i], _Key ) ) {
                    return MOUSE_AXIS_BASE + i;
                }
            }
            return -1;
    }
    if ( _DevId >= ID_JOYSTICK_1 && _DevId <= ID_JOYSTICK_16 ) {
        for ( int i = 0; i < MAX_JOYSTICK_BUTTONS; i++ ) {
            if ( !Platform::Stricmp( Static.JoystickButtonNames[i], _Key ) ) {
                return JOY_BUTTON_BASE + i;
            }
        }
        for ( int i = 0; i < MAX_JOYSTICK_AXES; i++ ) {
            if ( !Platform::Stricmp( Static.JoystickAxisNames[i], _Key ) ) {
                return JOY_AXIS_BASE + i;
            }
        }
    }
    return -1;
}

int AInputHelper::LookupController( const char * _Controller )
{
    for ( int i = 0; i < MAX_INPUT_CONTROLLERS; i++ ) {
        if ( !Platform::Stricmp( Static.ControllerNames[i], _Controller ) ) {
            return i;
        }
    }
    return -1;
}

AInputComponent::AInputComponent()
{
    DeviceButtonDown[ID_KEYBOARD] = KeyboardButtonDown.ToPtr();
    DeviceButtonDown[ID_MOUSE]    = MouseButtonDown.ToPtr();
    for ( int i = 0; i < MAX_JOYSTICKS_COUNT; i++ ) {
        DeviceButtonDown[ID_JOYSTICK_1 + i] = JoystickButtonDown[i].ToPtr();

        //Devices[ ID_JOYSTICK_1 + i ] = MakeRef< AInputDeviceJoystick >();

        Platform::Memset( JoystickButtonDown[i].ToPtr(), 0xff, sizeof( JoystickButtonDown[0] ) );
    }

    Platform::Memset( KeyboardButtonDown.ToPtr(), 0xff, sizeof( KeyboardButtonDown ) );
    Platform::Memset( MouseButtonDown.ToPtr(), 0xff, sizeof( MouseButtonDown ) );

    MouseAxisState[0].Clear();
    MouseAxisState[1].Clear();

    INTRUSIVE_ADD( this, Next, Prev, InputComponents, InputComponentsTail );
}

void AInputComponent::DeinitializeComponent()
{
    Super::DeinitializeComponent();

    INTRUSIVE_REMOVE( this, Next, Prev, InputComponents, InputComponentsTail );

    InputMappings = nullptr;
}

void AInputComponent::SetInputMappings( AInputMappings * _InputMappings )
{
    InputMappings = _InputMappings;
}

AInputMappings * AInputComponent::GetInputMappings()
{
    return InputMappings;
}

void AInputComponent::UpdateAxes( float _TimeStep )
{
    if ( !InputMappings ) {
        return;
    }

    bool bPaused = GetWorld()->IsPaused();

    for ( SAxisBinding & binding : AxisBindings ) {
        binding.AxisScale = 0.0f;
    }

    for ( SPressedKey * key = PressedKeys.ToPtr(); key < &PressedKeys[NumPressedKeys]; key++ ) {
        if ( key->HasAxis() ) {
            AxisBindings[key->AxisBinding].AxisScale += key->AxisScale * _TimeStep;
        }
    }

    Float2 mouseDelta;

    if ( in_MouseFilter ) {
        mouseDelta = ( MouseAxisState[0] + MouseAxisState[1] ) * 0.5f;
    }
    else {
        mouseDelta = MouseAxisState[MouseIndex];
    }

    if ( in_MouseInvertY ) {
        mouseDelta.Y = -mouseDelta.Y;
    }

    float timeStepMsec     = Math::Max( _TimeStep * 1000, 200 );
    float mouseInputRate   = mouseDelta.Length() / timeStepMsec;
    float mouseCurrentSens = in_MouseSensitivity.GetFloat() + mouseInputRate * in_MouseAccel.GetFloat();
    float mouseSens[2]     = { in_MouseSensX.GetFloat() * mouseCurrentSens, in_MouseSensY.GetFloat() * mouseCurrentSens };

    TStdVector< TRef<AInputAxis> > const & inputAxes = InputMappings->GetAxes();
    for ( int i = 0; i < inputAxes.Size(); i++ ) {
        AInputAxis * inputAxis = inputAxes[i];

        int axisBinding = GetAxisBinding( inputAxis );
        if ( axisBinding == -1 ) {
            // axis is not binded
            continue;
        }

        SAxisBinding & binding = AxisBindings[axisBinding];
        if ( bPaused && !binding.bExecuteEvenWhenPaused ) {
            continue;
        }

        if ( !bIgnoreJoystickEvents ) {
            for ( int joyNum = 0; joyNum < MAX_JOYSTICKS_COUNT; joyNum++ ) {
                //if ( !Static.Joysticks[joyNum].bConnected ) {
                //    continue;
                //}

                for ( int joystickAxis = 0; joystickAxis < MAX_JOYSTICK_AXES; joystickAxis++ ) {
                    AInputMappings::AArrayOfMappings & mappings = InputMappings->JoystickMappings[joyNum].AxisMappings[joystickAxis];

                    for ( AInputMappings::SMapping & mapping : mappings ) {
                        if ( mapping.AxisOrActionIndex == i && mapping.ControllerId == ControllerId ) {
                            binding.AxisScale += Static.JoystickAxisState[joyNum][joystickAxis] * mapping.AxisScale * _TimeStep;
                        }
                    }
                }
            }
        }

        if ( !bIgnoreMouseEvents ) {
            for ( int mouseAxis = 0; mouseAxis < MAX_MOUSE_AXES; mouseAxis++ ) {
                AInputMappings::AArrayOfMappings & mappings = InputMappings->MouseAxisMappings[mouseAxis];

                for ( AInputMappings::SMapping & mapping : mappings ) {
                    if ( mapping.AxisOrActionIndex == i && mapping.ControllerId == ControllerId ) {
                        //binding.AxisScale += MouseAxisState[MouseIndex][ mouseAxis ] * mapping.AxisScale;

                        binding.AxisScale += mouseDelta[mouseAxis] * ( mapping.AxisScale * mouseSens[mouseAxis] );
                    }
                }
            }
        }

        binding.Callback( binding.AxisScale );
    }

    // Reset mouse axes
    MouseIndex ^= 1;
    MouseAxisState[MouseIndex].Clear();
}

void AInputComponent::SetButtonState( int _DevId, int _Button, int _Action, int _ModMask, double _TimeStamp )
{
    AN_ASSERT( _DevId >= 0 && _DevId < MAX_INPUT_DEVICES );

    if ( _DevId == ID_KEYBOARD && bIgnoreKeyboardEvents ) {
        return;
    }
    if ( _DevId == ID_MOUSE && bIgnoreMouseEvents ) {
        return;
    }
    if ( _DevId >= ID_JOYSTICK_1 && _DevId <= ID_JOYSTICK_16 && bIgnoreJoystickEvents ) {
        return;
    }

    //AInputDevice * device = Devices[_DevId];

    //int8_t * ButtonIndex = device->GetButtonState();

    int8_t * ButtonIndex = DeviceButtonDown[_DevId];

#ifdef AN_DEBUG
    switch ( _DevId ) {
        case ID_KEYBOARD:
            AN_ASSERT( _Button < MAX_KEYBOARD_BUTTONS );
            break;
        case ID_MOUSE:
            AN_ASSERT( _Button < MAX_MOUSE_BUTTONS );
            break;
        default:
            AN_ASSERT( _Button < MAX_JOYSTICK_BUTTONS );
            break;
    }
#endif

    if ( _Action == IA_PRESS ) {
        if ( ButtonIndex[_Button] == -1 ) {
            if ( NumPressedKeys < MAX_PRESSED_KEYS ) {
                SPressedKey & pressedKey = PressedKeys[NumPressedKeys];
                pressedKey.DevId         = _DevId;
                pressedKey.Key           = _Button;
                pressedKey.AxisBinding   = -1;
                pressedKey.ActionBinding = -1;
                pressedKey.AxisScale     = 0;

                if ( InputMappings ) {
                    const AInputMappings::AArrayOfMappings * keyMappings;
                    if ( _DevId == ID_KEYBOARD ) {
                        keyMappings = &InputMappings->KeyboardMappings[_Button];
                    }
                    else if ( _DevId == ID_MOUSE ) {
                        keyMappings = &InputMappings->MouseMappings[_Button];
                    }
                    else {
                        keyMappings = &InputMappings->JoystickMappings[_DevId - ID_JOYSTICK_1].ButtonMappings[_Button];
                    }

                    bool bUseActionMapping = false;

                    // Find action mapping with modifiers
                    for ( AInputMappings::SMapping const & mapping : *keyMappings ) {
                        if ( mapping.ControllerId != ControllerId ) {
                            continue;
                        }
                        if ( mapping.bAxis ) {
                            continue;
                        }
                        // Filter by key modifiers
                        if ( _ModMask != mapping.ModMask ) {
                            continue;
                        }

                        AInputAction * inputAction = InputMappings->GetActions()[mapping.AxisOrActionIndex];
                        pressedKey.ActionBinding   = GetActionBinding( inputAction );
                        bUseActionMapping          = true;
                        break;
                    }

                    // Find action without modifiers
                    if ( !bUseActionMapping ) {
                        for ( AInputMappings::SMapping const & mapping : *keyMappings ) {
                            if ( mapping.ControllerId != ControllerId ) {
                                continue;
                            }
                            if ( mapping.bAxis ) {
                                continue;
                            }
                            // Filter by key modifiers
                            if ( 0 != mapping.ModMask ) {
                                continue;
                            }

                            AInputAction * inputAction = InputMappings->GetActions()[mapping.AxisOrActionIndex];
                            pressedKey.ActionBinding   = GetActionBinding( inputAction );
                            bUseActionMapping          = true;
                            break;
                        }
                    }

                    if ( !bUseActionMapping ) {
                        // Find axis mapping
                        for ( AInputMappings::SMapping const & mapping : *keyMappings ) {
                            if ( mapping.ControllerId != ControllerId ) {
                                continue;
                            }
                            if ( !mapping.bAxis ) {
                                continue;
                            }

                            AInputAxis * inputAxis = InputMappings->GetAxes()[mapping.AxisOrActionIndex];
                            pressedKey.AxisScale   = mapping.AxisScale;
                            pressedKey.AxisBinding = GetAxisBinding( inputAxis );
                            break;
                        }
                    }
                }

                ButtonIndex[_Button] = NumPressedKeys;

                NumPressedKeys++;

                if ( pressedKey.ActionBinding != -1 ) {
                    SActionBinding & binding = ActionBindings[pressedKey.ActionBinding];

                    if ( GetWorld()->IsPaused() && !binding.bExecuteEvenWhenPaused ) {
                        pressedKey.ActionBinding = -1;
                    }
                    else {
                        binding.Callback[IA_PRESS]();
                    }
                }
            }
            else {
                GLogger.Printf( "MAX_PRESSED_KEYS hit\n" );
            }
        }
        else {

            // Button is repressed

            //AN_ASSERT( 0 );
        }
    }
    else if ( _Action == IA_RELEASE ) {
        if ( ButtonIndex[_Button] != -1 ) {

            int index = ButtonIndex[_Button];

            int actionBinding = PressedKeys[index].ActionBinding;

            DeviceButtonDown[PressedKeys[index].DevId][PressedKeys[index].Key] = -1;

            if ( index != NumPressedKeys - 1 ) {
                // Move last array element to position "index"
                PressedKeys[index] = PressedKeys[NumPressedKeys - 1];

                // Refresh index of moved element
                DeviceButtonDown[PressedKeys[index].DevId][PressedKeys[index].Key] = index;
            }

            // Pop back
            NumPressedKeys--;
            AN_ASSERT( NumPressedKeys >= 0 );

            if ( actionBinding != -1 /*&& !PressedKeys[ index ].bMarkedReleased*/ ) {
                ActionBindings[actionBinding].Callback[IA_RELEASE]();
            }
        }
    }
}

bool AInputComponent::GetButtonState( int _DevId, int _Button ) const
{
    AN_ASSERT( _DevId >= 0 && _DevId < MAX_INPUT_DEVICES );

    //return Devices[ _DevId ]->GetButtonState()[ _Button ] != -1;
    return DeviceButtonDown[_DevId][_Button] != -1;
}

void AInputComponent::UnpressButtons()
{
    double timeStamp = Platform::SysSeconds_d();
    for ( int i = 0; i < MAX_KEYBOARD_BUTTONS; i++ ) {
        SetButtonState( ID_KEYBOARD, i, IA_RELEASE, 0, timeStamp );
    }
    for ( int i = 0; i < MAX_MOUSE_BUTTONS; i++ ) {
        SetButtonState( ID_MOUSE, i, IA_RELEASE, 0, timeStamp );
    }
    for ( int j = 0; j < MAX_JOYSTICKS_COUNT; j++ ) {
        for ( int i = 0; i < MAX_JOYSTICK_BUTTONS; i++ ) {
            SetButtonState( ID_JOYSTICK_1 + j, i, IA_RELEASE, 0, timeStamp );
        }
    }
}

//bool AInputComponent::IsJoyDown( SJoystick const * _Joystick, int _Button ) const {
//    return GetButtonState( ID_JOYSTICK_1 + _Joystick->Id, _Button );
//}

bool AInputComponent::IsJoyDown( int _JoystickId, int _Button ) const
{
    return GetButtonState( ID_JOYSTICK_1 + _JoystickId, _Button );
}

void AInputComponent::NotifyUnicodeCharacter( SWideChar _UnicodeCharacter, int _ModMask, double _TimeStamp )
{
    if ( bIgnoreCharEvents ) {
        return;
    }

    if ( !CharacterCallback.IsValid() ) {
        return;
    }

    if ( GetWorld()->IsPaused() && !bCharacterCallbackExecuteEvenWhenPaused ) {
        return;
    }

    CharacterCallback( _UnicodeCharacter, _ModMask, _TimeStamp );
}

void AInputComponent::SetMouseAxisState( float _X, float _Y )
{
    if ( bIgnoreMouseEvents ) {
        return;
    }

    MouseAxisState[MouseIndex].X += _X;
    MouseAxisState[MouseIndex].Y += _Y;
}

float AInputComponent::GetMouseAxisState( int _Axis )
{
    return MouseAxisState[MouseIndex][_Axis];
}

//void AInputComponent::SetJoystickState( int _Joystick, int _NumAxes, int _NumButtons, bool _bGamePad, bool _bConnected ) {
//    Static.Joysticks[_Joystick].NumAxes = _NumAxes;
//    Static.Joysticks[_Joystick].NumButtons = _NumButtons;
//    Static.Joysticks[_Joystick].bGamePad = _bGamePad;
//    Static.Joysticks[_Joystick].bConnected = _bConnected;
//}

void AInputComponent::SetJoystickAxisState( int _Joystick, int _Axis, float _Value )
{
    Static.JoystickAxisState[_Joystick][_Axis] = _Value;
}

float AInputComponent::GetJoystickAxisState( int _Joystick, int _Axis )
{
    return Static.JoystickAxisState[_Joystick][_Axis];
}

//const SJoystick * AInputComponent::GetJoysticks() {
//    return Static.Joysticks;
//}

void AInputComponent::BindAxis( const char * _Axis, TCallback< void( float ) > const & _Callback, bool _ExecuteEvenWhenPaused )
{
    int hash = Core::HashCase( _Axis, Platform::Strlen( _Axis ) );

    for ( int i = AxisBindingsHash.First( hash ); i != -1; i = AxisBindingsHash.Next( i ) ) {
        if ( !AxisBindings[i].Name.Icmp( _Axis ) ) {
            AxisBindings[i].Callback = _Callback;
            return;
        }
    }

    if ( AxisBindings.size() >= MAX_AXIS_BINDINGS ) {
        GLogger.Printf( "MAX_AXIS_BINDINGS hit\n" );
        return;
    }

    AxisBindingsHash.Insert( hash, AxisBindings.size() );
    SAxisBinding binding;
    binding.Name                   = _Axis;
    binding.Callback               = _Callback;
    binding.AxisScale              = 0.0f;
    binding.bExecuteEvenWhenPaused = _ExecuteEvenWhenPaused;
    AxisBindings.Append( binding );
}

void AInputComponent::UnbindAxis( const char * _Axis )
{
    int hash = Core::HashCase( _Axis, Platform::Strlen( _Axis ) );

    for ( int i = AxisBindingsHash.First( hash ); i != -1; i = AxisBindingsHash.Next( i ) ) {
        if ( !AxisBindings[i].Name.Icmp( _Axis ) ) {
            AxisBindingsHash.RemoveIndex( hash, i );
            auto it = AxisBindings.begin() + i;
            AxisBindings.erase( it );

            for ( SPressedKey * pressedKey = PressedKeys.ToPtr(); pressedKey < &PressedKeys[NumPressedKeys]; pressedKey++ ) {
                if ( pressedKey->AxisBinding == i ) {
                    pressedKey->AxisBinding = -1;
                }
            }

            return;
        }
    }
}

void AInputComponent::BindAction( const char * _Action, int _Event, TCallback< void() > const & _Callback, bool _ExecuteEvenWhenPaused )
{
    if ( _Event != IA_PRESS && _Event != IA_RELEASE ) {
        GLogger.Printf( "AInputComponent::BindAction: expected IE_Press or IE_Release event\n" );
        return;
    }

    int hash = Core::HashCase( _Action, Platform::Strlen( _Action ) );

    for ( int i = ActionBindingsHash.First( hash ); i != -1; i = ActionBindingsHash.Next( i ) ) {
        if ( !ActionBindings[i].Name.Icmp( _Action ) ) {
            ActionBindings[i].Callback[_Event] = _Callback;
            return;
        }
    }

    if ( ActionBindings.size() >= MAX_ACTION_BINDINGS ) {
        GLogger.Printf( "MAX_ACTION_BINDINGS hit\n" );
        return;
    }

    ActionBindingsHash.Insert( hash, ActionBindings.size() );
    SActionBinding binding;
    binding.Name                   = _Action;
    binding.Callback[_Event]       = _Callback;
    binding.bExecuteEvenWhenPaused = _ExecuteEvenWhenPaused;
    ActionBindings.Append( binding );
}

void AInputComponent::UnbindAction( const char * _Action )
{
    int hash = Core::HashCase( _Action, Platform::Strlen( _Action ) );

    for ( int i = ActionBindingsHash.First( hash ); i != -1; i = ActionBindingsHash.Next( i ) ) {
        if ( !ActionBindings[i].Name.Icmp( _Action ) ) {
            ActionBindingsHash.RemoveIndex( hash, i );
            auto it = ActionBindings.begin() + i;
            ActionBindings.erase( it );

            for ( SPressedKey * pressedKey = PressedKeys.ToPtr(); pressedKey < &PressedKeys[NumPressedKeys]; pressedKey++ ) {
                if ( pressedKey->ActionBinding == i ) {
                    pressedKey->ActionBinding = -1;
                }
            }

            return;
        }
    }
}

void AInputComponent::UnbindAll()
{
    AxisBindingsHash.Clear();
    AxisBindings.Clear();

    ActionBindingsHash.Clear();
    ActionBindings.Clear();

    for ( SPressedKey * pressedKey = PressedKeys.ToPtr(); pressedKey < &PressedKeys[NumPressedKeys]; pressedKey++ ) {
        pressedKey->AxisBinding   = -1;
        pressedKey->ActionBinding = -1;
    }
}

void AInputComponent::SetCharacterCallback( TCallback< void( SWideChar, int, double ) > const & _Callback, bool _ExecuteEvenWhenPaused )
{
    CharacterCallback                       = _Callback;
    bCharacterCallbackExecuteEvenWhenPaused = _ExecuteEvenWhenPaused;
}

void AInputComponent::UnsetCharacterCallback()
{
    CharacterCallback.Clear();
}

int AInputComponent::GetAxisBinding( const char * _Axis ) const
{
    return GetAxisBindingHash( _Axis, Core::HashCase( _Axis, Platform::Strlen( _Axis ) ) );
}

int AInputComponent::GetAxisBinding( const AInputAxis * _Axis ) const
{
    return GetAxisBindingHash( _Axis->GetObjectName().CStr(), _Axis->GetNameHash() );
}

int AInputComponent::GetAxisBindingHash( const char * _Axis, int _Hash ) const
{
    for ( int i = AxisBindingsHash.First( _Hash ); i != -1; i = AxisBindingsHash.Next( i ) ) {
        if ( !AxisBindings[i].Name.Icmp( _Axis ) ) {
            return i;
        }
    }
    return -1;
}

int AInputComponent::GetActionBinding( const char * _Action ) const
{
    return GetActionBindingHash( _Action, Core::HashCase( _Action, Platform::Strlen( _Action ) ) );
}

int AInputComponent::GetActionBinding( const AInputAction * _Action ) const
{
    return GetActionBindingHash( _Action->GetObjectName().CStr(), _Action->GetNameHash() );
}

int AInputComponent::GetActionBindingHash( const char * _Action, int _Hash ) const
{
    for ( int i = ActionBindingsHash.First( _Hash ); i != -1; i = ActionBindingsHash.Next( i ) ) {
        if ( !ActionBindings[i].Name.Icmp( _Action ) ) {
            return i;
        }
    }
    return -1;
}

AInputMappings::AInputMappings()
{
}

AInputMappings::~AInputMappings()
{
}

TRef< ADocObject > AInputMappings::Serialize()
{
    return TRef< ADocObject >();
#if 0
    TRef< ADocumentObject > object = Super::Serialize();
    if ( !Axes.IsEmpty() ) {
        int axes = pDocument->AddArray( object, "Axes" );

        for ( AInputAxis const * axis : Axes ) {
            AString const & axisName = axis->GetObjectName();

            for ( int deviceId = 0 ; deviceId < MAX_INPUT_DEVICES ; deviceId++ ) {
                const char * deviceName = AInputHelper::TranslateDevice( deviceId );
                for ( unsigned short key : axis->MappedKeys[ deviceId ] ) {
                    AArrayOfMappings * mappings = GetKeyMappings( deviceId, key );
                    for ( SMapping & mapping : *mappings ) {
                        TRef< ADocumentObject > axisObject = MakeRef< ADocumentObject >();
                        axisObject->AddString( "Name", axisName );
                        axisObject->AddString( "Device", deviceName );
                        axisObject->AddString( "Key", AInputHelper::TranslateDeviceKey( deviceId, key ) );
                        axisObject->AddString( "Scale", Math::ToString( mapping.AxisScale ) );
                        axisObject->AddString( "Owner", AInputHelper::TranslateController( mapping.ControllerId ) );
                        axes->AddValue( axisObject );
                    }
                }
            }

            if ( axis->MappedMouseAxes ) {
                const char * deviceName = AInputHelper::TranslateDevice( ID_MOUSE );
                for ( int i = 0 ; i < MAX_MOUSE_AXES ; i++ ) {
                    if ( axis->MappedMouseAxes & ( 1 << i ) ) {
                        AArrayOfMappings * mappings = GetKeyMappings( ID_MOUSE, MOUSE_AXIS_BASE + i );
                        for ( SMapping & mapping : *mappings ) {
                            TRef< ADocumentObject > axisObject = MakeRef< ADocumentObject >();
                            axisObject->AddString( "Name", axisName );
                            axisObject->AddString( "Device", deviceName );
                            axisObject->AddString( "Key", AInputHelper::TranslateDeviceKey( ID_MOUSE, MOUSE_AXIS_BASE + i ) );
                            axisObject->AddString( "Scale", Math::ToString( mapping.AxisScale ) );
                            axisObject->AddString( "Owner", AInputHelper::TranslateController( mapping.ControllerId ) );
                            axes->AddValue( axisObject );
                        }
                    }
                }
            }

            for ( int joyId = 0 ; joyId < MAX_JOYSTICKS_COUNT ; joyId++ ) {
                if ( axis->MappedJoystickAxes[ joyId ] ) {
                    const char * deviceName = AInputHelper::TranslateDevice( ID_JOYSTICK_1 + joyId );
                    for ( int i = 0 ; i < MAX_JOYSTICK_AXES ; i++ ) {
                        if ( axis->MappedJoystickAxes[ joyId ] & ( 1 << i ) ) {
                            AArrayOfMappings * mappings = GetKeyMappings( ID_JOYSTICK_1 + joyId, JOY_AXIS_BASE + i );
                            for ( SMapping & mapping : *mappings ) {
                                TRef< ADocumentObject > axisObject = MakeRef< ADocumentObject >();
                                axisObject->AddString( "Name", axisName );
                                axisObject->AddString( "Device", deviceName );
                                axisObject->AddString( "Key", AInputHelper::TranslateDeviceKey( ID_JOYSTICK_1 + joyId, JOY_AXIS_BASE + i ) );
                                axisObject->AddString( "Scale", Math::ToString( mapping.AxisScale ) );
                                axisObject->AddString( "Owner", AInputHelper::TranslateController( mapping.ControllerId ) );
                                axes->AddValue( axisObject );
                            }
                        }
                    }
                }
            }
        }
    }

    if ( !Actions.IsEmpty() ) {
        int actions = pDocument->AddArray( object, "Actions" );

        for ( AInputAction const * action : Actions ) {

            AString const & actionName = action->GetObjectName();

            for ( int deviceId = 0 ; deviceId < MAX_INPUT_DEVICES ; deviceId++ ) {
                if ( !action->MappedKeys[ deviceId ].IsEmpty() ) {
                    const char * deviceName = AInputHelper::TranslateDevice( deviceId );
                    for ( unsigned short key : action->MappedKeys[ deviceId ] ) {
                        AArrayOfMappings * mappings = GetKeyMappings( deviceId, key );
                        for ( SMapping & mapping : *mappings ) {
                            TRef< ADocumentObject > actionObject = MakeRef< ADocumentObject >();
                            actionObject->AddString( "Name", actionName );
                            actionObject->AddString( "Device", deviceName );
                            actionObject->AddString( "Key", AInputHelper::TranslateDeviceKey( deviceId, key ) );
                            actionObject->AddString( "Owner", AInputHelper::TranslateController( mapping.ControllerId ) );
                            if ( mapping.ModMask ) {
                                actionObject->AddString( "ModMask", Math::ToString( mapping.ModMask ) );
                            }
                            actions->AddValue( actionObject );
                        }
                    }
                }
            }
        }
    }

    return object;
#endif
}

AInputMappings * AInputMappings::LoadMappings( ADocObject const * pObject )
{
    ADocMember const * classNameField = pObject->FindMember( "ClassName" );
    if ( !classNameField ) {
        GLogger.Printf( "AInputMappings::LoadMappings: invalid class\n" );
        return nullptr;
    }

    AString className = classNameField->GetString();

    AObjectFactory const * factory = AInputMappings::ClassMeta().Factory();

    AClassMeta const * classMeta = factory->LookupClass( className.CStr() );
    if ( !classMeta ) {
        GLogger.Printf( "AInputMappings::LoadMappings: invalid class \"%s\"\n", className.CStr() );
        return nullptr;
    }

    AInputMappings * inputMappings = static_cast< AInputMappings * >( classMeta->CreateInstance() );

    // Load attributes
    inputMappings->LoadAttributes( pObject );

    // Load axes
    ADocMember const * axesArray = pObject->FindMember( "Axes" );
    if ( axesArray ) {
        inputMappings->LoadAxes( axesArray );
    }

    // Load actions
    ADocMember const * actionsArray = pObject->FindMember( "Actions" );
    if ( actionsArray ) {
        inputMappings->LoadActions( actionsArray );
    }

    return inputMappings;
}

void AInputMappings::LoadAxes( ADocMember const * ArrayOfAxes )
{
    for ( ADocValue const * object = ArrayOfAxes->GetArrayValues(); object; object = object->GetNext() ) {
        if ( !object->IsObject() ) {
            continue;
        }

        ADocMember const * nameField = object->FindMember( "Name" );
        if ( !nameField ) {
            continue;
        }

        ADocMember const * deviceField = object->FindMember( "Device" );
        if ( !deviceField ) {
            continue;
        }

        ADocMember const * keyField = object->FindMember( "Key" );
        if ( !keyField ) {
            continue;
        }

        ADocMember const * scaleField = object->FindMember( "Scale" );
        if ( !scaleField ) {
            continue;
        }

        ADocMember const * ownerField = object->FindMember( "Owner" );
        if ( !ownerField ) {
            continue;
        }

        AString name       = nameField->GetString();
        AString device     = deviceField->GetString();
        AString key        = keyField->GetString();
        AString scale      = scaleField->GetString();
        AString controller = ownerField->GetString();

        int deviceId     = AInputHelper::LookupDevice( device.CStr() );
        int deviceKey    = AInputHelper::LookupDeviceKey( deviceId, key.CStr() );
        int controllerId = AInputHelper::LookupController( controller.CStr() );

        float scaleValue = Math::ToFloat( scale.CStr() );

        MapAxis( name.CStr(),
                 deviceId,
                 deviceKey,
                 scaleValue,
                 controllerId );
    }
}

void AInputMappings::LoadActions( ADocMember const * ArrayOfActions )
{
    for ( ADocValue const * object = ArrayOfActions->GetArrayValues(); object; object = object->GetNext() ) {
        if ( !object->IsObject() ) {
            continue;
        }

        ADocMember const * nameField = object->FindMember( "Name" );
        if ( !nameField ) {
            continue;
        }

        ADocMember const * deviceField = object->FindMember( "Device" );
        if ( !deviceField ) {
            continue;
        }

        ADocMember const * keyField = object->FindMember( "Key" );
        if ( !keyField ) {
            continue;
        }

        ADocMember const * ownerField = object->FindMember( "Owner" );
        if ( !ownerField ) {
            continue;
        }

        int32_t            modMask      = 0;
        ADocMember const * modMaskField = object->FindMember( "ModMask" );
        if ( modMaskField ) {
            modMask = Math::ToInt< int32_t >( modMaskField->GetString().CStr() );
        }

        AString name       = nameField->GetString();
        AString device     = deviceField->GetString();
        AString key        = keyField->GetString();
        AString controller = ownerField->GetString();

        int deviceId     = AInputHelper::LookupDevice( device.CStr() );
        int deviceKey    = AInputHelper::LookupDeviceKey( deviceId, key.CStr() );
        int controllerId = AInputHelper::LookupController( controller.CStr() );

        MapAction( name.CStr(),
                   deviceId,
                   deviceKey,
                   modMask,
                   controllerId );
    }
}

AInputAxis * AInputMappings::AddAxis( const char * _Name )
{
    AInputAxis * axis = CreateInstanceOf< AInputAxis >();
    axis->Parent             = this;
    axis->IndexInArrayOfAxes = Axes.Size();
    axis->SetObjectName( _Name );
    axis->NameHash = axis->GetObjectName().HashCase();
    Axes.Append( TRef<AInputAxis>(axis) );
    return axis;
}

AInputAction * AInputMappings::AddAction( const char * _Name )
{
    AInputAction * action = CreateInstanceOf< AInputAction >();
    action->Parent                = this;
    action->IndexInArrayOfActions = Actions.Size();
    action->SetObjectName( _Name );
    action->NameHash = action->GetObjectName().HashCase();
    Actions.Append( TRef<AInputAction>(action) );
    return action;
}

AInputAxis * AInputMappings::FindAxis( const char * _AxisName )
{
    for ( TRef<AInputAxis> & axis : Axes ) {
        if ( !axis->GetObjectName().Icmp( _AxisName ) ) {
            return axis;
        }
    }
    return nullptr;
}

AInputAction * AInputMappings::FindAction( const char * _ActionName )
{
    for ( TRef<AInputAction> & action : Actions ) {
        if ( !action->GetObjectName().Icmp( _ActionName ) ) {
            return action;
        }
    }
    return nullptr;
}

void AInputMappings::MapAxis( const char * _AxisName, int _DevId, int _KeyToken, float _AxisScale, int _ControllerId )
{
    UnmapAxis( _DevId, _KeyToken );

    AInputAxis * axis = FindAxis( _AxisName );

    axis = ( !axis ) ? AddAxis( _AxisName ) : axis;

    AArrayOfMappings * keyMappings = GetKeyMappings( _DevId, _KeyToken );
    if ( !keyMappings ) {
        return;
    }

    SMapping & mapping        = keyMappings->Append();
    mapping.AxisOrActionIndex = axis->IndexInArrayOfAxes;
    mapping.bAxis             = true;
    mapping.AxisScale         = _AxisScale;
    mapping.ControllerId      = _ControllerId;
}

void AInputMappings::UnmapAxis( int _DevId, int _KeyToken )
{
    AArrayOfMappings * keyMappings = GetKeyMappings( _DevId, _KeyToken );
    if ( !keyMappings ) {
        return;
    }

    for ( auto it = keyMappings->Begin(); it != keyMappings->End(); ) {
        SMapping & mapping = *it;

        if ( mapping.bAxis ) {
            it = keyMappings->Erase( it );
        }
        else {
            it++;
        }
    }
}

void AInputMappings::MapAction( const char * _ActionName, int _DevId, int _KeyToken, int _ModMask, int _ControllerId )
{
    if ( _DevId < 0 || _DevId >= MAX_INPUT_DEVICES ) {
        return;
    }

    if ( _KeyToken < 0 || _KeyToken >= Static.DeviceButtonLimits[_DevId] ) {
        return;
    }

    if ( _DevId >= ID_JOYSTICK_1 && _DevId <= ID_JOYSTICK_16 && _KeyToken >= JOY_AXIS_BASE ) {
        // cannot map joystick axis and action!
        return;
    }

    if ( _DevId == ID_MOUSE && _KeyToken >= MOUSE_AXIS_BASE ) {
        // cannot map mouse axis and action!
        return;
    }

    UnmapAction( _DevId, _KeyToken, _ModMask );

    AInputAction * action = FindAction( _ActionName );

    action = ( !action ) ? AddAction( _ActionName ) : action;

    AArrayOfMappings * keyMappings;

    switch ( _DevId ) {
        case ID_KEYBOARD:
            keyMappings = &KeyboardMappings[_KeyToken];
            break;
        case ID_MOUSE:
            keyMappings = &MouseMappings[_KeyToken];
            break;
        case ID_JOYSTICK_1:
        case ID_JOYSTICK_2:
        case ID_JOYSTICK_3:
        case ID_JOYSTICK_4:
        case ID_JOYSTICK_5:
        case ID_JOYSTICK_6:
        case ID_JOYSTICK_7:
        case ID_JOYSTICK_8:
        case ID_JOYSTICK_9:
        case ID_JOYSTICK_10:
        case ID_JOYSTICK_11:
        case ID_JOYSTICK_12:
        case ID_JOYSTICK_13:
        case ID_JOYSTICK_14:
        case ID_JOYSTICK_15:
        case ID_JOYSTICK_16: {
            int joystickId = _DevId - ID_JOYSTICK_1;
            keyMappings    = &JoystickMappings[joystickId].ButtonMappings[_KeyToken];
            break;
        }
        default:
            AN_ASSERT( 0 );
            return;
    }

    //action->MappedKeys[_DevId].Append( _KeyToken );

    SMapping & mapping = keyMappings->Append();

    mapping.AxisOrActionIndex = action->IndexInArrayOfActions;
    mapping.bAxis             = false;
    mapping.AxisScale         = 0;
    mapping.ControllerId      = _ControllerId;
    mapping.ModMask           = _ModMask & 0xff;
}

void AInputMappings::UnmapAction( int _DevId, int _KeyToken, int _ModMask )
{
    if ( _DevId < 0 || _DevId >= MAX_INPUT_DEVICES ) {
        return;
    }

    if ( _KeyToken < 0 || _KeyToken >= Static.DeviceButtonLimits[_DevId] ) {
        return;
    }

    AArrayOfMappings * keyMappings;

    switch ( _DevId ) {
        case ID_KEYBOARD:
            keyMappings = &KeyboardMappings[_KeyToken];
            break;
        case ID_MOUSE:
            keyMappings = &MouseMappings[_KeyToken];
            break;
        case ID_JOYSTICK_1:
        case ID_JOYSTICK_2:
        case ID_JOYSTICK_3:
        case ID_JOYSTICK_4:
        case ID_JOYSTICK_5:
        case ID_JOYSTICK_6:
        case ID_JOYSTICK_7:
        case ID_JOYSTICK_8:
        case ID_JOYSTICK_9:
        case ID_JOYSTICK_10:
        case ID_JOYSTICK_11:
        case ID_JOYSTICK_12:
        case ID_JOYSTICK_13:
        case ID_JOYSTICK_14:
        case ID_JOYSTICK_15:
        case ID_JOYSTICK_16: {
            int joystickId = _DevId - ID_JOYSTICK_1;
            keyMappings    = &JoystickMappings[joystickId].ButtonMappings[_KeyToken];
            break;
        }
        default:
            AN_ASSERT( 0 );
            return;
    }

    for ( auto it = keyMappings->Begin(); it != keyMappings->End(); ) {
        SMapping & mapping = *it;

        if ( !mapping.bAxis && mapping.ModMask == _ModMask ) {
            it = keyMappings->Erase( it );
        }
        else {
            it++;
        }
    }
}

void AInputMappings::UnmapAll()
{
    Axes.Clear();
    Actions.Clear();
    for ( int i = 0; i < MAX_KEYBOARD_BUTTONS; i++ ) {
        KeyboardMappings[i].Clear();
    }
    for ( int i = 0; i < MAX_MOUSE_BUTTONS; i++ ) {
        MouseMappings[i].Clear();
    }
    for ( int i = 0; i < MAX_MOUSE_AXES; i++ ) {
        MouseAxisMappings[i].Clear();
    }
    for ( int i = 0; i < MAX_JOYSTICKS_COUNT; i++ ) {
        for ( int j = 0; j < MAX_JOYSTICK_BUTTONS; j++ ) {
            JoystickMappings[i].ButtonMappings[j].Clear();
        }
        for ( int j = 0; j < MAX_JOYSTICK_AXES; j++ ) {
            JoystickMappings[i].AxisMappings[j].Clear();
        }
    }
}

AInputMappings::AArrayOfMappings * AInputMappings::GetKeyMappings( int _DevId, int _KeyToken )
{
    if ( _KeyToken < 0 || _KeyToken >= Static.DeviceButtonLimits[_DevId] ) {
        CriticalError( "AInputMappings::GetKeyMappings: invalid key token\n" );
    }

    switch ( _DevId ) {
        case ID_KEYBOARD:
            return &KeyboardMappings[_KeyToken];
        case ID_MOUSE:
            return _KeyToken >= MOUSE_AXIS_BASE ? &MouseAxisMappings[_KeyToken - MOUSE_AXIS_BASE] : &MouseMappings[_KeyToken];
    }

    if ( _DevId >= ID_JOYSTICK_1 && _DevId <= ID_JOYSTICK_16 ) {
        int joystickId = _DevId - ID_JOYSTICK_1;
        return _KeyToken >= JOY_AXIS_BASE ? &JoystickMappings[joystickId].AxisMappings[_KeyToken - JOY_AXIS_BASE] : &JoystickMappings[joystickId].ButtonMappings[_KeyToken];
    }

    CriticalError( "AInputMappings::GetKeyMappings: invalid device ID\n" );

    return nullptr;
}

AInputAxis::AInputAxis()
{
    //MappedMouseAxes = 0;
    //Platform::ZeroMem( MappedJoystickAxes,  sizeof( MappedJoystickAxes ) );
}

void AInputAxis::Map( int _DevId, int _KeyToken, float _AxisScale, int _ControllerId )
{
    Parent->MapAxis( GetObjectNameCStr(), _DevId, _KeyToken, _AxisScale, _ControllerId );
}

void AInputAction::Map( int _DevId, int _KeyToken, int _ModMask, int _ControllerId )
{
    Parent->MapAction( GetObjectNameCStr(), _DevId, _KeyToken, _ModMask, _ControllerId );
}
