// 
// -------------------------------------------------------------------
// Copyright (C) 2007 OpenEngine.dk (See AUTHORS)
//
// This program is free software; It is covered by the GNU General
// Public License version 2 or any later version.
// See the GNU General Public License for more details (see LICENSE).
//--------------------------------------------------------------------


#ifndef _OE_WII_MOTE_MANAGER_H_
#define _OE_WII_MOTE_MANAGER_H_

#include <Core/Event.h>

namespace OpenEngine {
namespace Devices {

class WiiMote;

class WiiMoteFoundEventArg {
public:
    WiiMote* wiimote;
    WiiMoteFoundEventArg(WiiMote* m) : wiimote(m) {}
};

/**
 * Short description.
 *
 * @class WiiMoteManager WiiMoteManager.h ons/WiiMote/Devices/WiiMoteManager.h
 */
class WiiMoteManager {
private:
    Core::Event<WiiMoteFoundEventArg> foundEvent;

public:
    WiiMoteManager();
    
    void LookForMote();

    Core::IEvent<WiiMoteFoundEventArg>& WiiMoteFoundEvent() {
        return foundEvent;
    }
};

} // NS Devices
} // NS OpenEngine

#endif // _OE_WII_MOTE_MANAGER_H_
