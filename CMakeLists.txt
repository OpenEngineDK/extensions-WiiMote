
SET(WII_SOURCES
  Devices/WiiMoteManager.h
  Devices/WiiMote.h
  Devices/WiiMote.cpp
  )

IF(APPLE)
  SET(WII_SOURCES ${WII_SOURCES} Devices/WiiMoteManagerMac.mm)
  FIND_LIBRARY(WII_REMOTE_FRAMEWORK NAMES WiiRemote PATHS ${OE_LIB_DIR})
ELSE(APPLE)
  SET(WII_SOURCES ${WII_SOURCES} Devices/WiiMoteManagerLinux.cpp)
  FIND_LIBRARY(WIIUSE_LIBRARY NAMES wiiuse PATHS ${OE_LIB_DIR})
ENDIF(APPLE)


# Create the extension library
ADD_LIBRARY(Extensions_WiiMote
  ${WII_SOURCES}
)

IF(APPLE)
  TARGET_LINK_LIBRARIES(Extensions_WiiMote
                        ${WII_REMOTE_FRAMEWORK})
ELSE(APPLE) # linux
  TARGET_LINK_LIBRARIES(Extensions_WiiMote
                        ${WIIUSE_LIBRARY})

ENDIF(APPLE)