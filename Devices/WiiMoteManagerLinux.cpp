#include <Devices/WiiMoteManager.h>
#include <Devices/WiiMote.h>
#include <Logging/Logger.h>
#include <wiiuse.h>

namespace OpenEngine {
namespace Devices {

using OpenEngine::Core::ProcessEventArg;

WiiMoteManager::WiiMoteManager() {
    logger.info << "implement me wii thingy" << logger.end;    
}

void WiiMoteManager::LookForMote() {
    nmotes = 1;
    const int timeout = 5;
    wiimotes = wiiuse_init(nmotes);
    logger.info << "Looking for wiimotes for " << timeout << " seconds" << logger.end;
    int found = wiiuse_find(wiimotes, nmotes, timeout);
    logger.info << "Found " << found << " wiimotes!" << logger.end;
    int connected = wiiuse_connect(wiimotes,nmotes);
    logger.info << "connected to " << connected << " wiimotes" << logger.end;
    for (int i=0;i<connected;i++) {
	WiiMote* mote = new WiiMote();
	wiiuse_motion_sensing(wiimotes[i], 1); // enable accelerometer
	motes.push_back(mote);
	foundEvent.Notify(WiiMoteFoundEventArg(mote));
    }
}

#define CHK_BTN(oen, wiiun)				\
    arg.button = oen;					\
    if (IS_JUST_PRESSED(mote, wiiun)) {			\
	arg.type = EVENT_PRESS;				\
	oem->WiiMoteButtonEvent().Notify(arg);		\
    }							\
    else if(IS_RELEASED(mote, wiiun)) {			\
	arg.type = EVENT_RELEASE;			\
	oem->WiiMoteButtonEvent().Notify(arg);		\
    } 

void CheckAcc(WiiMote* oem, wiimote* mote) {
    WiiAccelerationEventArg arg;
    Vector<3,float> acc(mote->gforce.x, mote->gforce.y, mote->gforce.z);
    arg.acc = acc;
    oem->WiiMoteAccelerationEvent().Notify(arg);
}

void CheckButtons(WiiMote* oem, wiimote* mote) {
    WiiButtonEventArg arg;

    CHK_BTN(WII_REMOTE_TWO	, WIIMOTE_BUTTON_TWO)
    CHK_BTN(WII_REMOTE_ONE	, WIIMOTE_BUTTON_ONE)
    CHK_BTN(WII_REMOTE_A	, WIIMOTE_BUTTON_A)
    CHK_BTN(WII_REMOTE_B	, WIIMOTE_BUTTON_B)
    CHK_BTN(WII_REMOTE_MINUS	, WIIMOTE_BUTTON_MINUS)
    CHK_BTN(WII_REMOTE_HOME	, WIIMOTE_BUTTON_HOME)
    CHK_BTN(WII_REMOTE_PLUS	, WIIMOTE_BUTTON_PLUS)
    CHK_BTN(WII_REMOTE_UP	, WIIMOTE_BUTTON_UP)
    CHK_BTN(WII_REMOTE_DOWN	, WIIMOTE_BUTTON_DOWN)
    CHK_BTN(WII_REMOTE_LEFT	, WIIMOTE_BUTTON_LEFT)
    CHK_BTN(WII_REMOTE_RIGHT	, WIIMOTE_BUTTON_RIGHT)
    
}

void WiiMoteManager::Handle(ProcessEventArg arg) {
    while (wiiuse_poll(wiimotes,nmotes)) { // An event have occured!
	for (int i=0;i<nmotes;i++) {
	    WiiMote* mote = motes[i];
	    switch(wiimotes[i]->event) {
	    case WIIUSE_EVENT:		 
		 CheckButtons(mote,wiimotes[i]);
		 CheckAcc(mote,wiimotes[i]);
		 break;
	    default:
		break;
	    }
	}
    }
}

}
}
