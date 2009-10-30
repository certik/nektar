
# Use NIST_SPARSE_BLAS_ADDITIONAL_INCLUDE_DIRS to add additional search directories.

SET(NIST_SPARSE_BLAS_SEARCH_PATHS 
	${NIST_SPARSE_BLAS_ADDITIONAL_INCLUDE_DIRS}
	${CMAKE_SOURCE_DIR}/../ThirdParty/spblastk0.9b/lib
	${CMAKE_SOURCE_DIR}/../../ThirdParty/spblastk0.9b/lib )

FIND_LIBRARY(NIST_SPARSE_BLAS NAMES sbtk PATHS ${NIST_SPARSE_BLAS_SEARCH_PATHS})
FIND_LIBRARY(NIST_SPARSE_BLAS_DEBUG NAMES sbtk-g PATHS ${NIST_SPARSE_BLAS_SEARCH_PATHS})

GET_FILENAME_COMPONENT(LIB_PATH ${NIST_SPARSE_BLAS} PATH CACHE)
SET(NIST_SPARSE_BLAS_INCLUDE_DIR ${LIB_PATH}/../include CACHE FILEPATH "Sparse matrix solve include directory.")

 MARK_AS_ADVANCED(NIST_SPARSE_BLAS)
 MARK_AS_ADVANCED(NIST_SPARSE_BLAS_DEBUG)
 MARK_AS_ADVANCED(NIST_SPARSE_BLAS_INCLUDE_DIR)

IF (NIST_SPARSE_BLAS)
  SET(NIST_SPARSE_BLAS_FOUND ON)
ENDIF (NIST_SPARSE_BLAS)

IF (NIST_SPARSE_BLAS_FOUND)
  IF (NOT NIST_SPARSE_BLAS_FIND_QUIETLY)
     MESSAGE(STATUS "Found NIST Sparse Blas")
  ENDIF (NOT NIST_SPARSE_BLAS_FIND_QUIETLY)
ELSE(NIST_SPARSE_BLAS_FOUND)
  IF (NIST_SPARSE_BLAS_FIND_REQUIRED)
     MESSAGE(FATAL_ERROR "Could not find NIST sparse blas library.")
  ENDIF (NIST_SPARSE_BLAS_FIND_REQUIRED)
ENDIF (NIST_SPARSE_BLAS_FOUND)



