# Try to find syslog.h for Syslog


IF (SIGCPP_INCLUDE_DIRS)
	# Already in cache, be silent
	SET(SIGCPP_FIND_QUIETLY TRUE)
ENDIF (SIGCPP_INCLUDE_DIRS)

FIND_PATH(SIGCPP_INCLUDE_DIRS sigc++/sigc++.h)

IF(SIGCPP_INCLUDE_DIRS) 
	SET(SIGCPP_FOUND TRUE) 
ENDIF(SIGCPP_INCLUDE_DIRS) 

IF(SIGCPP_FOUND) 
	set(CMAKE_REQUIRED_INCLUDES ${SIGCPP_INCLUDE_DIRS})
	set(CMAKE_REQUIRED_INCLUDES)
	IF(NOT SIGCPP_FIND_QUIETLY) 
		MESSAGE(STATUS "Found sigc++ headers: ${SIGCPP_INCLUDE_DIRS}/sigc++/sigc++.h") 
	ENDIF(NOT SIGCPP_FIND_QUIETLY) 
ELSE(SIGCPP_FOUND) 
	IF(Syslog_FIND_REQUIRED) 
		MESSAGE(FATAL_ERROR "Could not find sigc++") 
	ENDIF(Syslog_FIND_REQUIRED) 
ENDIF(SIGCPP_FOUND) 

MARK_AS_ADVANCED(
	SIGCPP_INCLUDE_DIRS
	SIGCPP_LIBRARIES
	)