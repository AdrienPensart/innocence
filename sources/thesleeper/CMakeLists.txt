include(FindQt4)
find_package(Qt4 REQUIRED)
include( ${QT_USE_FILE} )

include_directories(${CMAKE_CURRENT_BINARY_DIR})

#FIND_LIBRARY(QT_JPEG_PLUGIN qjpeg PATHS ${QT_PLUGINS_DIR} PATH_SUFFIXES imageformats)
#SET(QT_USE_QTPLUGIN TRUE)

file(GLOB SERVER_SRC_FILES src/*.cpp src/*.hpp)
file(GLOB SERVER_MOC_FILES src/*.hpp)
file(GLOB SERVER_UI_FILES ui/*.ui)
file(GLOB SERVER_RSC_FILES *.qrc)

QT4_ADD_RESOURCES(SERVER_SRC_FILES ${SERVER_RSC_FILES})
QT4_WRAP_CPP(SERVER_MOC_SRC_FILES ${SERVER_MOC_FILES})
QT4_WRAP_UI(SERVER_UI_HEADER_FILES ${SERVER_UI_FILES})

if(WIN32)
set(THESLEEPER_GUI_TYPE WIN32)
endif(WIN32)

add_executable(
	thesleeper 
	${THESLEEPER_GUI_TYPE}
	${SERVER_MOC_SRC_FILES}
	${SERVER_SRC_FILES}
	${SERVER_UI_HEADER_FILES}
	${SERVER_MOC_FILES}
)

target_link_libraries(thesleeper system common network blaspheme ${QT_LIBRARIES})# ${QT_JPEG_PLUGIN})

if(WIN32 AND NOT UNIX)
	# Génération du package pour windows
	install(PROGRAMS ${CMAKE_SOURCE_DIR}/utils/nc.exe DESTINATION innocence)
ELSE(WIN32 AND NOT UNIX)
	message(STATUS "NetCat est une dependance sur les systèmes UNIX")
ENDIF(WIN32 AND NOT UNIX)
install(TARGETS thesleeper RUNTIME DESTINATION innocence)
install(FILES "${QT_BINARY_DIR}/QtCore${QT_VERSION_MAJOR}.dll"
			  "${QT_BINARY_DIR}/QtGui${QT_VERSION_MAJOR}.dll" DESTINATION innocence)
