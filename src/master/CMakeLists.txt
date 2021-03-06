set(MASTER_TARGET "master")

set(CMAKE_PREFIX_PATH "C:\\Qt\\Qt5.1.0\\5.1.0\\msvc2012")
find_package(Qt5Widgets REQUIRED)
set(CMAKE_AUTOMOC OFF)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
include_directories(${Qt5Widgets_INCLUDES})
add_definitions(${Qt5Widgets_DEFINITIONS})

include_directories(${CMAKE_CURRENT_BINARY_DIR})

file(GLOB MASTER_SRC_FILES src/*.cpp src/*.hpp)
file(GLOB MASTER_MOC_FILES src/*.hpp)
file(GLOB MASTER_UI_FILES ui/*.ui)
file(GLOB MASTER_RSC_FILES *.qrc)

QT5_ADD_RESOURCES(MASTER_SRC_FILES ${MASTER_RSC_FILES})
QT5_WRAP_CPP(MASTER_MOC_SRC_FILES ${MASTER_MOC_FILES})
QT5_WRAP_UI(MASTER_UI_HEADER_FILES ${MASTER_UI_FILES})

add_executable(
	${MASTER_TARGET}
	${GUI_TYPE}
	${MASTER_MOC_SRC_FILES}
	${MASTER_SRC_FILES}
	${MASTER_UI_HEADER_FILES}
	${MASTER_MOC_FILES}
)

# target_link_libraries(${MASTER_TARGET} system common network blaspheme ${QT_LIBRARIES})# ${QT_JPEG_PLUGIN})
QT5_USE_MODULES(${MASTER_TARGET} Widgets)

if(WIN32 AND NOT UNIX)
	target_link_libraries(${MASTER_TARGET} system_md log_md network_md blaspheme_md)
	set_target_properties(${MASTER_TARGET} PROPERTIES COMPILE_FLAGS ${BUILD_FLAGS_MD} COMPILE_DEFINITIONS PROJECT_MODULE="${MASTER_TARGET}")
	install(PROGRAMS ${CMAKE_SOURCE_DIR}/utils/nc.exe DESTINATION innocence)
else()
	target_link_libraries(${MASTER_TARGET} system log network blaspheme)
	message(STATUS "netcat is a dependency on Unix")
endif()

# install(TARGETS ${MASTER_TARGET} RUNTIME DESTINATION innocence)
# install(FILES "${QT_BINARY_DIR}/QtCore${QT_VERSION_MAJOR}.dll"
# "${QT_BINARY_DIR}/QtGui${QT_VERSION_MAJOR}.dll" DESTINATION innocence)
