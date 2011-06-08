
SET(GS_LIB_SEARCH_PATHS 
	${CMAKE_SOURCE_DIR}/../ThirdParty/gslib-1.0/
	${CMAKE_SOURCE_DIR}/../ThirdParty/gslib-1.0/build/
	${CMAKE_SOURCE_DIR}/../../ThirdParty/gslib-1.0/
	${CMAKE_SOURCE_DIR}/../../ThirdParty/gslib-1.0/build )

FIND_LIBRARY(GS_LIB NAMES gs PATHS ${GS_LIB_SEARCH_PATHS})


SET(GS_FOUND FALSE)
IF (GS_LIB)
  SET(GS_FOUND TRUE)
  MARK_AS_ADVANCED(GS_LIB)
ENDIF (GS_LIB)

IF (GS_FOUND)
  IF (NOT GS_LIB_FIND_QUIETLY)
     MESSAGE(STATUS "Found GSLib")
  ENDIF (NOT GS_LIB_FIND_QUIETLY)
ELSE(GS_FOUND)
  IF (GS_LIB_FIND_REQUIRED)
     MESSAGE(FATAL_ERROR "Could not find GSLib")
  ENDIF (GS_LIB_FIND_REQUIRED)
ENDIF (GS_FOUND)