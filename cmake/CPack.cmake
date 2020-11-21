set(DISTRO_CODENAME "buster")
set(PACKAGE_NAME_VERSION ${OW_VERSION})
set(PACKAGE_MAINTAINER "Mickael Gaillard <mick.gaillard@gmail.com>")
#set(DEBIAN_PACKAGE_VERSION ${OW_VERSION})
set(DEBIAN_PACKAGE_REVISION "1")
set(DEBIAN_PACKAGE_TYPE "stable")
set(DEBIAN_PACKAGE_EPOCH "1")
set(APP_VERSION_MAJOR 2)
set(APP_VERSION_MINOR 0)
string(TIMESTAMP PACKAGE_TIMESTAMP "%Y%m%d.%H%M" UTC)
set(CPACK_DEBIAN_PACKAGE_VERSION "${OW_VERSION}~git${PACKAGE_TIMESTAMP}-${DISTRO_CODENAME}")
#set()
#set()

set(CPACK_PROJECT_NAME ${OW_NAME})
set(CPACK_PROJECT_VERSION ${OW_VERSION})

set(CPACK_SOURCE_GENERATOR TGZ)
set(CPACK_SOURCE_PACKAGE_FILE_NAME ${APP_NAME_LC}_${APP_VERSION_MAJOR}.${APP_VERSION_MINOR}~git${PACKAGE_TIMESTAMP}-${RELEASE_IDENTIFIER}.orig)

set(CPACK_GENERATOR "DEB")
set(CPACK_DEBIAN_PACKAGE_NAME ${OW_NAME})
set(CPACK_DEBIAN_PACKAGE_MAINTAINER ${PACKAGE_MAINTAINER})
set(CPACK_DEBIAN_PACKAGE_HOMEPAGE "https://github.com/OpenWinch/OpenWinch-Cpp")
set(CPACK_DEBIAN_PACKAGE_DESCRIPTION "OpenWinch binary runtime package.")
set(CPACK_DEBIAN_PACKAGE_SECTION "misc")
set(CPACK_DEBIAN_PACKAGE_PRIORITY "optional")
set(CPACK_DEBIAN_PACKAGE_SOURCE ${OW_NAME})

# enforce Debian policy permission rules
set(CPACK_DEBIAN_PACKAGE_CONTROL_STRICT_PERMISSION ON)

set(CPACK_DEBIAN_FILE_NAME DEB-DEFAULT)
set(CPACK_DEBIAN_ENABLE_COMPONENT_DEPENDS ON)
#set(CPACK_DEBIAN_PACKAGE_CONTROL_EXTRA
#    "${CMAKE_CURRENT_SOURCE_DIR}/debian/prerm;${CMAKE_CURRENT_SOURCE_DIR}/debian/postrm")

# Generates an RFC2822 timestamp
#
# The following variable is set:
#   RFC2822_TIMESTAMP
function(rfc2822stamp)
  execute_process(COMMAND date -R
                  OUTPUT_VARIABLE RESULT)
  set(RFC2822_TIMESTAMP ${RESULT} PARENT_SCOPE)
endfunction()

# find stuff we need
find_program(LSB_RELEASE_CMD lsb_release)
find_program(DPKG_CMD dpkg)
find_package(Git)
find_program(GZIP_CMD gzip)

# set packaging dir
if(NOT CPACK_PACKAGE_DIRECTORY)
  set(CPACK_PACKAGE_DIRECTORY ${CMAKE_BINARY_DIR}/packages)
endif()

# force CPack generated DEBs to use the same path as CMAKE_INSTALL_PREFIX
set(CPACK_SET_DESTDIR true)

# package description common to all packages
if(NOT CPACK_DEBIAN_PACKAGE_DESCRIPTION)
  file(STRINGS ${CMAKE_SOURCE_DIR}/cmake/cpack/deb/package-description.txt DESC_LINES)
  foreach(LINE IN LISTS DESC_LINES)
    set(CPACK_DEBIAN_PACKAGE_DESCRIPTION "${CPACK_DEBIAN_PACKAGE_DESCRIPTION} ${LINE}\n")
  endforeach()
endif()


# generate a Debian compliant changelog
set(CHANGELOG_HEADER "${OW_NAME} (${CPACK_DEBIAN_PACKAGE_VERSION}) ${DISTRO_CODENAME}\; urgency=high")
rfc2822stamp()
# two spaces between maintainer and timestamp is NOT a mistake
set(CHANGELOG_FOOTER " -- ${CPACK_DEBIAN_PACKAGE_MAINTAINER}  ${RFC2822_TIMESTAMP}")

if(GIT_FOUND AND GZIP_CMD AND EXISTS ${CMAKE_SOURCE_DIR}/.git)
  execute_process(COMMAND ${GIT_EXECUTABLE} log --no-merges --pretty=format:"%n  [%an]%n   * %s" --since="30 days ago"
                  OUTPUT_VARIABLE CHANGELOG
                  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
                  OUTPUT_STRIP_TRAILING_WHITESPACE)
    string(REPLACE "\"" "" CHANGELOG ${CHANGELOG})
  file(WRITE ${CPACK_PACKAGE_DIRECTORY}/deb/changelog.Debian ${CHANGELOG_HEADER}\n${CHANGELOG}\n\n${CHANGELOG_FOOTER})
  execute_process(COMMAND ${GZIP_CMD} -f -9 -n ${CPACK_PACKAGE_DIRECTORY}/deb/changelog.Debian)
  unset(CHANGELOG_HEADER)
  unset(CHANGELOG_FOOTER)
  unset(RFC2822_TIMESTAMP)
else()
  message(WARNING "DEB Generator: Can't find git and/or gzip in your path. DEB packages will be missing changelog.Debian.gz.")
endif()


# Generate NEWS.Debian
configure_file(${CMAKE_SOURCE_DIR}/cmake/deb/NEWS.Debian
               ${CPACK_PACKAGE_DIRECTORY}/deb/NEWS.Debian @ONLY)
if(GZIP_CMD)
  execute_process(COMMAND ${GZIP_CMD} -f -9 -n ${CPACK_PACKAGE_DIRECTORY}/deb/NEWS.Debian)
else()
  message(WARNING "DEB Generator: Can't find gzip in your path. DEB packages will be missing NEWS.Debian.")
endif()

install(FILES ${CPACK_PACKAGE_DIRECTORY}/deb/changelog.Debian.gz
              ${CPACK_PACKAGE_DIRECTORY}/deb/NEWS.Debian.gz
              ${CMAKE_SOURCE_DIR}/cmake/deb/copyright
        DESTINATION share/doc/${file}
        COMPONENT bin)

# set packaging by components
set(CPACK_DEB_COMPONENT_INSTALL ON)
set(CPACK_COMPONENTS_ALL bin dev)



include(CPack)
cpack_add_component(bin)
cpack_add_component(dev DEPENDS bin)
