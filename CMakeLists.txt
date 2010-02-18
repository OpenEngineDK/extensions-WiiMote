
SET(WII_SOURCES
  Devices/WiiMoteManager.h
  Devices/WiiMote.h
  Devices/WiiMote.cpp
  )

IF(APPLE)
  SET(WII_SOURCES ${WII_SOURCES} Devices/WiiMoteManagerMac.mm)
  FIND_LIBRARY(WII_REMOTE_FRAMEWORK NAMES WiiRemote PATHS ${OE_LIB_DIR})
  MESSAGE("Hesten: ${WII_REMOTE_FRAMEWORK}")
ENDIF(APPLE)


# Create the extension library
ADD_LIBRARY(Extensions_WiiMote
  ${WII_SOURCES}
)

IF(APPLE)
  TARGET_LINK_LIBRARIES(Extensions_WiiMote
  ${WII_REMOTE_FRAMEWORK}
)
ENDIF(APPLE)