//
// -------------------------------------------------------------------
// Copyright (C) 2007 OpenEngine.dk (See AUTHORS)
//
// This program is free software; It is covered by the GNU General
// Public License version 2 or any later version.
// See the GNU General Public License for more details (see LICENSE).
//--------------------------------------------------------------------


#ifndef _OE_WII_MOTE_H_
#define _OE_WII_MOTE_H_

#include <Core/Event.h>
#ifndef __APPLE__
#include <Core/LockedQueuedEvent.h>
#endif
#include <Devices/Symbols.h>

#include <Math/Vector.h>

namespace OpenEngine {
namespace Devices {

    using Math::Vector;
    enum WiiButton {
        WII_REMOTE_A,
        WII_REMOTE_B,
        WII_REMOTE_ONE,
        WII_REMOTE_TWO,
        WII_REMOTE_MINUS,
        WII_REMOTE_HOME,
        WII_REMOTE_PLUS,
        WII_REMOTE_UP,
        WII_REMOTE_DOWN,
        WII_REMOTE_LEFT,
        WII_REMOTE_RIGHT,
        WII_NONE
    };

    struct WiiButtonEventArg {
        ButtonEvent type;
        WiiButton button;
        WiiButtonEventArg() : type(EVENT_NONE),button(WII_NONE) {}
    };




    struct WiiAccelerationEventArg {
        // source
        Vector<3,unsigned short> raw;
        Vector<3,float> acc;
    };

class WiiMote {
protected:
#ifdef __APPLE__
    Core::Event<WiiButtonEventArg> buttonEvent;
    Core::Event<WiiAccelerationEventArg> accEvent;
#else
  friend class WiiMoteManager;
    Core::LockedQueuedEvent<WiiButtonEventArg> buttonEvent;
    Core::LockedQueuedEvent<WiiAccelerationEventArg> accEvent;    
#endif
public:
    WiiMote();
    Core::IEvent<WiiButtonEventArg>& WiiMoteButtonEvent() { return buttonEvent;}
    Core::IEvent<WiiAccelerationEventArg>& WiiMoteAccelerationEvent() { return accEvent;}
};

} // NS Devices
} // NS OpenEngine

#endif // _OE_WII_MOTE_H_
