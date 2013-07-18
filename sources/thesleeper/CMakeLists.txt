SET (CMAKE_PREFIX_PATH "C:\\Qt\\Qt5.1.0\\5.1.0\\msvc2012")
FIND_PACKAGE(Qt5Widgets REQUIRED)
SET(CMAKE_AUTOMOC OFF)
SET(CMAKE_INCLUDE_CURRENT_DIR ON)
INCLUDE_DIRECTORIES(${Qt5Widgets_INCLUDES})
ADD_DEFINITIONS(${Qt5Widgets_DEFINITIONS})

include_directories(${CMAKE_CURRENT_BINARY_DIR})

file(GLOB SERVER_SRC_FILES src/*.cpp src/*.hpp)
file(GLOB SERVER_MOC_FILES src/*.hpp)
file(GLOB SERVER_UI_FILES ui/*.ui)
file(GLOB SERVER_RSC_FILES *.qrc)

QT5_ADD_RESOURCES(SERVER_SRC_FILES ${SERVER_RSC_FILES})
QT5_WRAP_CPP(SERVER_MOC_SRC_FILES ${SERVER_MOC_FILES})
QT5_WRAP_UI(SERVER_UI_HEADER_FILES ${SERVER_UI_FILES})

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

# target_link_libraries(thesleeper system common network blaspheme ${QT_LIBRARIES})# ${QT_JPEG_PLUGIN})
QT5_USE_MODULES(thesleeper Widgets)
target_link_libraries(thesleeper system common network blaspheme)

if(WIN32 AND NOT UNIX)
	install(PROGRAMS ${CMAKE_SOURCE_DIR}/utils/nc.exe DESTINATION innocence)
ELSE(WIN32 AND NOT UNIX)
	message(STATUS "NetCat est une dependance sur les systèmes UNIX")
ENDIF(WIN32 AND NOT UNIX)
install(TARGETS thesleeper RUNTIME DESTINATION innocence)
install(FILES "${QT_BINARY_DIR}/QtCore${QT_VERSION_MAJOR}.dll"
			  "${QT_BINARY_DIR}/QtGui${QT_VERSION_MAJOR}.dll" DESTINATION innocence)

			  