# On systems without Git installed, there were errors since execute_process seemed to not throw an error without it?
find_package(Git QUIET)
if(Git_FOUND)
	# Gets the latest tag as a string like "v0.6.6"
	# Can silently fail if git isn't on the system
	execute_process(COMMAND ${GIT_EXECUTABLE} describe --tags --abbrev=0
		WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
		OUTPUT_VARIABLE _raw_version_string
		ERROR_VARIABLE _git_tag_error
	)
endif()

# execute_process can fail silenty, so check for an error
# if there was an error, just use the user agent as a version
if(_git_tag_error OR NOT Git_FOUND)
	message(WARNING "OpenWinch failed to find the latest Git tag, falling back to using user agent as the version.")
  set(OW_VERSION "0.0.0")
endif()

# Needed since git tags have "v" prefixing them.
# Also used if the fallback to user agent string is being used.
#string(REGEX MATCH "([0-9]+\\.?)+" OW_VERSION "${_raw_version_string}")
