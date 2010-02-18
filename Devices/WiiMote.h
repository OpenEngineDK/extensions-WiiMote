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

namespace OpenEngine {
namespace Devices {

    struct WiiButtonEventArg {
        
        WiiButtonEventArg() {
        }

    };

class WiiMote {
private:
    Core::Event<WiiButtonEventArg> buttonEvent;
public:
    WiiMote();
    Core::IEvent<WiiButtonEventArg>& WiiMoteButtonEvent() { return buttonEvent;}
};

} // NS Devices
} // NS OpenEngine

#endif // _OE_WII_MOTE_H_
