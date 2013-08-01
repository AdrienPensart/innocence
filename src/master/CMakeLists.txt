SET (CMAKE_PREFIX_PATH "C:\\Qt\\Qt5.1.0\\5.1.0\\msvc2012")
FIND_PACKAGE(Qt5Widgets REQUIRED)
SET(CMAKE_AUTOMOC OFF)
SET(CMAKE_INCLUDE_CURRENT_DIR ON)
INCLUDE_DIRECTORIES(${Qt5Widgets_INCLUDES})
ADD_DEFINITIONS(${Qt5Widgets_DEFINITIONS})

include_directories(${CMAKE_CURRENT_BINARY_DIR})

file(GLOB MASTER_SRC_FILES src/*.cpp src/*.hpp)
file(GLOB MASTER_MOC_FILES src/*.hpp)
file(GLOB MASTER_UI_FILES ui/*.ui)
file(GLOB MASTER_RSC_FILES *.qrc)

QT5_ADD_RESOURCES(MASTER_SRC_FILES ${MASTER_RSC_FILES})
QT5_WRAP_CPP(MASTER_MOC_SRC_FILES ${MASTER_MOC_FILES})
QT5_WRAP_UI(MASTER_UI_HEADER_FILES ${MASTER_UI_FILES})

if(WIN32)
	set(MASTER_GUI_TYPE WIN32)
endif(WIN32)

add_executable(
	master 
	${MASTER_GUI_TYPE}
	${MASTER_MOC_SRC_FILES}
	${MASTER_SRC_FILES}
	${MASTER_UI_HEADER_FILES}
	${MASTER_MOC_FILES}
)

# target_link_libraries(master system common network blaspheme ${QT_LIBRARIES})# ${QT_JPEG_PLUGIN})
QT5_USE_MODULES(master Widgets)
target_link_libraries(master system_md common_md network_md blaspheme_md)
set_target_properties(master PROPERTIES COMPILE_FLAGS ${BUILD_FLAGS_MD})

if(WIN32 AND NOT UNIX)
	install(PROGRAMS ${CMAKE_SOURCE_DIR}/utils/nc.exe DESTINATION innocence)
ELSE(WIN32 AND NOT UNIX)
	message(STATUS "NetCat est une dependance sur les systèmes UNIX")
ENDIF(WIN32 AND NOT UNIX)
install(TARGETS master RUNTIME DESTINATION innocence)
install(FILES "${QT_BINARY_DIR}/QtCore${QT_VERSION_MAJOR}.dll"
			  "${QT_BINARY_DIR}/QtGui${QT_VERSION_MAJOR}.dll" DESTINATION innocence)