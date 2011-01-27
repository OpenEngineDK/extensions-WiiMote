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
  FIND_LIBRARY(WIIUSE_LIBS
    NAMES 
    wiiuse
    PATHS
    ${WIIUSE_DEPS_LIB_DIR}
    ENV LD_LIBRARY_PATH
    ENV LIBRARY_PATH
    /usr/lib
    /usr/local/lib
    /opt/local/lib
    NO_DEFAULT_PATH
    )

  FIND_LIBRARY(BLUETOOTH_LIBS
    NAMES 
    bluetooth
    PATHS
    ENV LD_LIBRARY_PATH
    ENV LIBRARY_PATH
    /usr/lib
    /usr/local/lib
    /opt/local/lib
    NO_DEFAULT_PATH
    )

  FIND_LIBRARY(M_LIBS
    NAMES 
    m
    PATHS
    ENV LD_LIBRARY_PATH
    ENV LIBRARY_PATH
    /usr/lib
    /usr/local/lib
    /opt/local/lib
    NO_DEFAULT_PATH
    )

  IF(WIIUSE_LIBS AND BLUETOOTH_LIBS AND M_LIBS)
    MARK_AS_ADVANCED(WIIUSE_LIBS BLUETOOTH_LIBS M_LIBS)
    SET(WIIUSE_LIBS ${WIIUSE_LIBS} ${BLUETOOTH_LIBS} ${M_LIBS})
  ELSE (WIIUSE_LIBS AND BLUETOOTH_LIBS AND M_LIBS)
    MESSAGE ("WARNING: Could not find wiiuse libraries - depending targets will be disabled.")
  ENDIF(WIIUSE_LIBS AND BLUETOOTH_LIBS AND M_LIBS)

ENDIF(APPLE)

# Create the extension library
ADD_LIBRARY(Extensions_WiiMote
  ${WII_SOURCES}
)

IF(APPLE)
  TARGET_LINK_LIBRARIES(Extensions_WiiMote
                        ${WII_REMOTE_FRAMEWORK})
ELSE(APPLE) # linux
  IF (WIIUSE_INCLUDE_DIR AND WIIUSE_LIBS)
    TARGET_LINK_LIBRARIES(Extensions_WiiMote
      ${WIIUSE_LIBS}
      OpenEngine_Core
      )
  ENDIF(WIIUSE_INCLUDE_DIR AND WIIUSE_LIBS)
ENDIF(APPLE)