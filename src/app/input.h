/*
 * This file is part of the Colobot: Gold Edition source code
 * Copyright (C) 2001-2014, Daniel Roux, EPSITEC SA & TerranovaTeam
 * http://epsiteс.ch; http://colobot.info; http://github.com/colobot
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see http://gnu.org/licenses
 */

/**
 * \file app/input.h
 * \brief CInput class
 */

#pragma once

#include "common/event.h"
#include "common/key.h"
#include "common/singleton.h"

#include "math/intpoint.h"


/**
 * \enum TrackedKey
 * \brief Additional keys whose state (pressed/released) is tracked by CInput
 */
enum TrackedKey
{
    TRKEY_NUM_UP    = (1<<0),
    TRKEY_NUM_DOWN  = (1<<1),
    TRKEY_NUM_LEFT  = (1<<2),
    TRKEY_NUM_RIGHT = (1<<3),
    TRKEY_NUM_PLUS  = (1<<4),
    TRKEY_NUM_MINUS = (1<<5),
    TRKEY_PAGE_UP   = (1<<6),
    TRKEY_PAGE_DOWN = (1<<7)
};

/**
 * \struct InputBinding
 * \brief Binding for input slot
 */
struct InputBinding
{
    //! Primary and secondary bindings
    //! Can be regular key, virtual key or virtual joystick button
    unsigned int primary, secondary;
    
    InputBinding(unsigned int p = KEY_INVALID, unsigned int s = KEY_INVALID)
    : primary(p), secondary(s) {}
};

/**
 * \struct JoyAxisBinding
 * \brief Binding for joystick axis
 */
struct JoyAxisBinding
{
    //! Axis index or AXIS_INVALID
    int axis;
    //! True to invert axis value
    bool invert;
};

//! Invalid value for axis binding (no axis assigned)
const int AXIS_INVALID = -1;

/**
 * \class CInput
 * \brief Management of mouse, keyboard and joystick
 */
class CInput : public CSingleton<CInput>
{
public:
    //! Constructor
    CInput();
    
    //! Process an incoming event, also sets .trackedKeysState, .kmodState, .mousePos, .mouseButtonsState and .key.slot
    void EventProcess(Event &event);
    
    //! Called by CApplication on SDL MOUSE_MOTION event
    void MouseMove(Math::IntPoint pos);
    
    
    //! Returns the current key modifiers
    int         GetKmods() const;
    
    //! Returns whether the given kmod is active
    bool        GetKmodState(int kmod) const;
    
    //! Returns whether the tracked key is pressed
    bool        GetTrackedKeyState(TrackedKey key) const;
    
    //! Returns whether the mouse button is pressed
    bool        GetMouseButtonState(int index) const;
    
    //! Resets tracked key states and modifiers
    void        ResetKeyStates();
    
    //! Returns the position of mouse cursor (in interface coords)
    Math::Point GetMousePos() const;
    
    
    //! Sets the default input bindings (key and axes)
    void        SetDefaultInputBindings();
    
    //! Management of input bindings
    //@{
    void        SetInputBinding(InputSlot slot, InputBinding binding);
    const InputBinding& GetInputBinding(InputSlot slot);
    //@}
    
    //! Management of joystick axis bindings
    //@{
    void        SetJoyAxisBinding(JoyAxisSlot slot, JoyAxisBinding binding);
    const JoyAxisBinding& GetJoyAxisBinding(JoyAxisSlot slot);
    //@}
    
    //! Management of joystick deadzone
    //@{
    void        SetJoystickDeadzone(float zone);
    float       GetJoystickDeadzone();
    //@}
    
    //! Get binding slot for given key
    InputSlot   FindBinding(unsigned int key);
    
    //! Saving/loading key bindings to string (for storing in colobot.ini)
    //@{
    std::string SaveKeyBindings();
    void        LoadKeyBindings(std::string keys);
    //@}
    
    //! Seeks a InputSlot by id. Returns INPUT_SLOT_MAX if not found
    InputSlot   SearchKeyById(std::string name);
    
    //! Returns string describing keys to pressed
    //@{
    std::string GetKeysString(InputBinding binding);
    std::string GetKeysString(InputSlot slot);
    //@}
    
private:
    //! Current state of key modifiers (bitmask of SDLMod)
    unsigned int    m_kmodState;
    //! Current state of some tracked keys (bitmask of TrackedKey enum values)
    unsigned int    m_trackedKeys;
    
    
    //! Current position of mouse cursor
    Math::Point     m_mousePos;
    //! Current state of mouse buttons (bitmask of MouseButton enum values)
    unsigned int    m_mouseButtonsState;
    
    
    //! Motion vector set by keyboard or joystick buttons
    Math::Vector    m_keyMotion;
    //! Motion vector set by joystick axes
    Math::Vector    m_joyMotion;
    
    
    //! Bindings for user inputs
    InputBinding    m_inputBindings[INPUT_SLOT_MAX];
    JoyAxisBinding  m_joyAxisBindings[JOY_AXIS_SLOT_MAX];
    float           m_joystickDeadzone;
};