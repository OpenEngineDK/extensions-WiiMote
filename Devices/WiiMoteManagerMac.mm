#include <Devices/WiiMoteManager.h>
#include <Devices/WiiMote.h>
#include <Logging/Logger.h>

#include <map>

#import <WiiRemote/WiiRemote.h>
#import <WiiRemote/WiiRemoteDiscovery.h>

using namespace OpenEngine::Devices;

@interface WiiMoteMac : NSObject {
    WiiRemote* remote;
    WiiMote* oem;
}

- (id)initWithRemote:(WiiRemote* )remote andOEMote:(WiiMote*)oeMote;

@end


@interface WiiManagerMac : NSObject {
    WiiRemoteDiscovery* discovery;
    std::map<WiiMoteMac*,OpenEngine::Devices::WiiMote*> moteMap;

    OpenEngine::Devices::WiiMoteManager* mgr;
}

@property OpenEngine::Devices::WiiMoteManager* mgr;

- (void)discover;

// Delegate
- (void) willStartWiimoteConnections;
- (void) WiiRemoteDiscovered:(WiiRemote*)wiimote;
- (void) WiiRemoteDiscoveryError:(int)code;


@end


// Impl

@implementation WiiManagerMac 

@synthesize mgr;

- (id)init {
    self = [super init];
    discovery = [[WiiRemoteDiscovery alloc] init];
    [discovery setDelegate:self];
    return self;
}

- (void)discover {
    [discovery start];
}

- (void) willStartWiimoteConnections {

}
- (void) WiiRemoteDiscovered:(WiiRemote*)wiimote {
    logger.info << "Found mote!" << logger.end;
    WiiMote* oeMote = new WiiMote();
    WiiMoteMac *mote = [[WiiMoteMac alloc] initWithRemote:wiimote andOEMote:oeMote];

    moteMap[mote] = oeMote;
    mgr->WiiMoteFoundEvent().Notify(WiiMoteFoundEventArg(oeMote));
    
}
- (void) WiiRemoteDiscoveryError:(int)code {

}

@end

@implementation WiiMoteMac

- (id)initWithRemote:(WiiRemote*)mote andOEMote:(WiiMote*)oeMote {
    self = [super init];
    remote = [mote retain];
    oem = oeMote;
    [remote setDelegate:self];
    return self;
}

- (void) wiimoteWillSendData {}
- (void) wiimoteDidSendData {}

- (void) irPointMovedX:(float) px
                     Y:(float) py {
    
}
- (void) rawIRData: (IRData[4]) irData {}
- (void) buttonChanged:(WiiButtonType) type 
             isPressed:(BOOL) isPressed {
    WiiButtonEventArg arg;
    arg.type = isPressed?EVENT_PRESS:EVENT_RELEASE;
    switch (type) {
    case WiiRemoteAButton: arg.button = WII_REMOTE_A; break;
    case WiiRemoteBButton: arg.button = WII_REMOTE_B; break;
    case WiiRemoteOneButton: arg.button = WII_REMOTE_ONE; break;
    case WiiRemoteTwoButton: arg.button = WII_REMOTE_TWO; break;
    case WiiRemoteMinusButton: arg.button = WII_REMOTE_MINUS; break;
    case WiiRemoteHomeButton: arg.button = WII_REMOTE_HOME; break;
    case WiiRemotePlusButton: arg.button = WII_REMOTE_PLUS; break;
    case WiiRemoteUpButton: arg.button = WII_REMOTE_UP; break;
    case WiiRemoteDownButton: arg.button = WII_REMOTE_DOWN; break;
    case WiiRemoteLeftButton: arg.button = WII_REMOTE_LEFT; break;
    case WiiRemoteRightButton: arg.button = WII_REMOTE_RIGHT; break;
    
    default:
        break;
    }

    oem->WiiMoteButtonEvent().Notify(arg);
}
- (void) accelerationChanged:(WiiAccelerationSensorType)type 
                        accX:(unsigned short)accX 
                        accY:(unsigned short)accY 
                        accZ:(unsigned short)accZ {
    WiiAccelerationEventArg arg;
    Vector<3,unsigned short> raw(accX,accY,accZ);

    
    WiiAccCalibData data = [remote accCalibData:WiiRemoteAccelerationSensor];
    
    unsigned short x0,y0,z0,x3,y2,z1;
    x0 = data.accX_zero;
    x3 = data.accX_1g;
    y0 = data.accY_zero;
    y2 = data.accY_1g;
    z0 = data.accZ_zero;
    z1 = data.accZ_1g;

    double ax = (double)(accX - x0) / (x3 - x0);
    double ay = (double)(accY - y0) / (y2 - y0);
    double az = (double)(accZ - z0) / (z1 - z0) * (-1.0);


    arg.raw = raw;
    arg.acc = Vector<3,float>(ax,ay,az);

    oem->WiiMoteAccelerationEvent().Notify(arg);
}
- (void) joyStickChanged:(WiiJoyStickType) type tiltX:(unsigned short) tiltX tiltY:(unsigned short) tiltY {}
- (void) analogButtonChanged:(WiiButtonType) type amount:(unsigned short) press {}
- (void) pressureChanged:(WiiPressureSensorType) type pressureTR:(float) bPressureTR pressureBR:(float) bPressureBR 
			  pressureTL:(float) bPressureTL pressureBL:(float) bPressureBL {}
- (void) batteryLevelChanged:(double) level {}
- (void) wiiRemoteDisconnected:(IOBluetoothDevice*) device {}
- (void) gotMiiData: (Mii*) mii_data_buf at: (int) slot {}
- (void) rawPressureChanged:(WiiBalanceBoardGrid) bbData {}
- (void) allPressureChanged:(WiiPressureSensorType) type bbData:(WiiBalanceBoardGrid) bbData bbDataInKg:(WiiBalanceBoardGrid) bbDataInKg {}


@end

// Meh. Global variable...
WiiManagerMac* macManager;

namespace OpenEngine {
namespace Devices {


WiiMoteManager::WiiMoteManager() {
    macManager = [[WiiManagerMac alloc] init];
    macManager.mgr = this;
    logger.info << "Mac wii thingy" << logger.end;    
}

void WiiMoteManager::LookForMote() {
    [macManager discover];
}

}
}
