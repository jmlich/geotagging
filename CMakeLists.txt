cmake_minimum_required(VERSION 3.5)

project(com.github.jmlich.geotagging LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include(GNUInstallDirs)
include(FeatureSummary)

enable_testing()

find_package(Git QUIET REQUIRED)
find_package(exiv2 REQUIRED CONFIG NAMES exiv2)

execute_process(COMMAND
  "${GIT_EXECUTABLE}" describe --dirty --broken --always --tags
  WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
  OUTPUT_VARIABLE GIT_VERSION
  ERROR_QUIET OUTPUT_STRIP_TRAILING_WHITESPACE
)

add_definitions( -DGIT_VERSION="${GIT_VERSION}" )

find_package(Qt5 COMPONENTS Core Gui Network Widgets WebEngineWidgets WebChannel LinguistTools REQUIRED)

file(GLOB TS_FILES translation/*.ts)

add_executable(com.github.jmlich.geotagging WIN32
  aboutdialog.cpp
  changedatetime.cpp
  changetime.cpp
  datetimedialog.cpp
  datetimeedit.cpp
  exifinfo.cpp
  exifreaderwriter.cpp
  formathandler.cpp
  gpspanel.cpp
  gpspoint.cpp
  gpsroute.cpp
  gpsrouteslist.cpp
  gpssegment.cpp
  imagedata.cpp
  imageinfo.cpp
  imageview.cpp
  imagewidgetslist.cpp
  importfileprocessor.cpp
  keyeventhandler.cpp
  main.cpp
  mainwindow.cpp
  mapwidget.cpp
  picturesframe.cpp
  picturesscrollarea.cpp
  scrollarea.cpp
  synchdialog.cpp
  synchresultdialog.cpp
  synchresultitem.cpp
  geotagging.qrc
  geotagging.rc
  aboutdialog.ui
  changedatetime.ui
  changetime.ui
  imageinfo.ui
  mainwindow.ui
  synchdialog.ui
  ${TS_FILES}
)


add_definitions(-DQUICK_TEST_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}")



QT5_ADD_TRANSLATION(QM_FILES ${TS_FILES})
add_custom_target(translations DEPENDS ${QM_FILES})
add_dependencies(com.github.jmlich.geotagging translations)

QT5_CREATE_TRANSLATION(QT_FILES "${CMAKE_SOURCE_DIR}"
    OPTIONS "-no-obsolete")

install(FILES ${QM_FILES}
    DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/com.github.jmlich.geotagging/translation")

install(FILES "com.github.jmlich.geotagging.desktop"
    DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/applications")

install(FILES "icons/geotagging.png"
    DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/icons/hicolor/applications/86x86")

install(FILES "icons/geotagging.ico"
    DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/icons/hicolor/applications/64x64")

install(FILES "com.github.jmlich.geotagging.appdata.xml"
    DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/metainfo")

install(TARGETS com.github.jmlich.geotagging RUNTIME
        DESTINATION ${CMAKE_INSTALL_BINDIR})

target_compile_definitions(com.github.jmlich.geotagging
  PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)
target_link_libraries(com.github.jmlich.geotagging
  PRIVATE Qt5::Core Qt5::Gui Qt5::Network Qt5::Widgets Qt5::WebEngineWidgets Qt5::WebChannel exiv2)


option(DEPLOY_QT_LIBRARIES "Deploy Qt libraries to executable path" OFF)
if(DEPLOY_QT_LIBRARIES)
    if(WIN32)
        include(DeployQt)
        windeployqt(com.github.jmlich.geotagging)
    elseif(APPLE)
        include(DeployQt)
        macdeployqt(com.github.jmlich.geotagging)
    endif()
endif()
 feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)


# generate proper GUI program on specified platform
if(WIN32) # Check if we are on Windows
    if(MSVC) # Check if we are using the Visual Studio compiler
	set_target_properties(${PROJECT_NAME} PROPERTIES
	    WIN32_EXECUTABLE YES
	    LINK_FLAGS "/ENTRY:mainCRTStartup"
	)
    elseif(CMAKE_COMPILER_IS_GNUCXX)
	    # SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mwindows") # Not tested
    else()
	message(SEND_ERROR "You are using an unsupported Windows compiler! (Not MSVC or GCC)")
    endif(MSVC)
elseif(APPLE)
    set_target_properties(${PROJECT_NAME} PROPERTIES
	    MACOSX_BUNDLE YES
    )
elseif(UNIX)
    # Nothing special required
else()
    message(SEND_ERROR "You are on an unsupported platform! (Not Win32, Mac OS X or Unix)")
endif(WIN32)
