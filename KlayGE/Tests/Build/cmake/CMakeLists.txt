PROJECT(Tests)

CMAKE_MINIMUM_REQUIRED(VERSION 2.8.6)

IF(COMMAND cmake_policy)
	CMAKE_POLICY(SET CMP0003 NEW)
ENDIF(COMMAND cmake_policy)

FIND_PATH(KLAYGE_PROJECT_DIR klayge_logo.ico PATHS ../../../)
MARK_AS_ADVANCED(KLAYGE_PROJECT_DIR)
SET(KLAYGE_ROOT_DIR "${KLAYGE_PROJECT_DIR}/..")

INCLUDE(${KLAYGE_ROOT_DIR}/cmake/Common.cmake)
INCLUDE(${KLAYGE_ROOT_DIR}/cmake/Platform.cmake)
INCLUDE(${KLAYGE_ROOT_DIR}/cmake/Compiler.cmake)
INCLUDE(${KLAYGE_ROOT_DIR}/cmake/ConfigBoost.cmake)

DOWNLOAD_FILE("KlayGE/Tests/media/leaf_v3_green_tex.dds")
DOWNLOAD_FILE("KlayGE/Tests/media/leaf_v3_green_tex_bc2.dds")
DOWNLOAD_FILE("KlayGE/Tests/media/leaf_v3_green_tex_bc3.dds")
DOWNLOAD_FILE("KlayGE/Tests/media/leaf_v3_green_tex_bc7.dds")
DOWNLOAD_FILE("KlayGE/Tests/media/Lenna.dds")
DOWNLOAD_FILE("KlayGE/Tests/media/Lenna_bc1.dds")
DOWNLOAD_FILE("KlayGE/Tests/media/Lenna_bc7.dds")
DOWNLOAD_FILE("KlayGE/Tests/media/memorial.dds")
DOWNLOAD_FILE("KlayGE/Tests/media/memorial_bc6u.dds")
DOWNLOAD_FILE("KlayGE/Tests/media/uffizi_probe.dds")
DOWNLOAD_FILE("KlayGE/Tests/media/uffizi_probe_bc6s.dds")

SET_PROPERTY(GLOBAL PROPERTY USE_FOLDERS ON)

SET(KLAYGE_OUTPUT_DIR "${KLAYGE_PROJECT_DIR}/lib/${KLAYGE_PLATFORM_NAME}")
SET(KLAYGE_BIN_DIR "${KLAYGE_PROJECT_DIR}/bin/${KLAYGE_PLATFORM_NAME}")

SET(SOURCE_FILES
	${KLAYGE_PROJECT_DIR}/Tests/src/EncodeDecodeTexTest.cpp
	${KLAYGE_PROJECT_DIR}/Tests/src/KlayGETests.cpp
	${KLAYGE_PROJECT_DIR}/Tests/src/MathTest.cpp
)
SET(HEADER_FILES "")
SET(RESOURCE_FILES "")
SET(EFFECT_FILES "")
SET(POST_PROCESSORS "")
SET(UI_FILES "")

SOURCE_GROUP("Source Files" FILES ${SOURCE_FILES})
SOURCE_GROUP("Header Files" FILES ${HEADER_FILES})
SOURCE_GROUP("Resource Files" FILES ${RESOURCE_FILES})
SOURCE_GROUP("Effect Files" FILES ${EFFECT_FILES})
SOURCE_GROUP("Post Processors" FILES ${POST_PROCESSORS})
SOURCE_GROUP("UI Files" FILES ${UI_FILES})

SET(EXE_NAME "Tests")

INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})
INCLUDE_DIRECTORIES(${KLAYGE_PROJECT_DIR}/../KFL/include)
INCLUDE_DIRECTORIES(${KLAYGE_PROJECT_DIR}/Core/Include)
INCLUDE_DIRECTORIES(${EXTRA_INCLUDE_DIRS})
LINK_DIRECTORIES(${Boost_LIBRARY_DIR})
LINK_DIRECTORIES(${KLAYGE_PROJECT_DIR}/../KFL/lib/${KLAYGE_PLATFORM_NAME})
IF(KLAYGE_PLATFORM_DARWIN OR KLAYGE_PLATFORM_LINUX)
	LINK_DIRECTORIES(${KLAYGE_BIN_DIR})
ELSE()
	LINK_DIRECTORIES(${KLAYGE_OUTPUT_DIR})
ENDIF()
IF(KLAYGE_PLATFORM_ANDROID OR KLAYGE_PLATFORM_IOS)
	LINK_DIRECTORIES(${KLAYGE_PROJECT_DIR}/../glloader/lib/${KLAYGE_PLATFORM_NAME})
	LINK_DIRECTORIES(${KLAYGE_PROJECT_DIR}/../kfont/lib/${KLAYGE_PLATFORM_NAME})
	LINK_DIRECTORIES(${KLAYGE_PROJECT_DIR}/../MeshMLLib/lib/${KLAYGE_PLATFORM_NAME})
	LINK_DIRECTORIES(${KLAYGE_PROJECT_DIR}/../External/7z/lib/${KLAYGE_PLATFORM_NAME})
ENDIF()
LINK_DIRECTORIES(${EXTRA_LINKED_DIRS})

ADD_EXECUTABLE(${EXE_NAME} "" ${SOURCE_FILES} ${HEADER_FILES} ${RESOURCE_FILES} ${EFFECT_FILES} ${POST_PROCESSORS} ${UI_FILES})

SET_TARGET_PROPERTIES(${EXE_NAME} PROPERTIES
	PROJECT_LABEL ${EXE_NAME}
	DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX}
	OUTPUT_NAME ${EXE_NAME}${KLAYGE_OUTPUT_SUFFIX})

IF(MSVC)
	SET(EXTRA_LINKED_LIBRARIES ${EXTRA_LINKED_LIBRARIES})
ELSE()
	SET(EXTRA_LINKED_LIBRARIES ${EXTRA_LINKED_LIBRARIES}
		debug KlayGE_Core${KLAYGE_OUTPUT_SUFFIX}_d optimized KlayGE_Core${KLAYGE_OUTPUT_SUFFIX}
		debug KFL${KLAYGE_OUTPUT_SUFFIX}_d optimized KFL${KLAYGE_OUTPUT_SUFFIX}
		${Boost_UNIT_TEST_FRAMEWORK_LIBRARY} ${Boost_CHRONO_LIBRARY} ${Boost_THREAD_LIBRARY} ${Boost_SYSTEM_LIBRARY})
	IF(KLAYGE_PLATFORM_LINUX)
		SET(EXTRA_LINKED_LIBRARIES ${EXTRA_LINKED_LIBRARIES} dl pthread)
	ENDIF()
ENDIF()

TARGET_LINK_LIBRARIES(${EXE_NAME} ${EXTRA_LINKED_LIBRARIES})

ADD_POST_BUILD(${EXE_NAME} "")

INSTALL(TARGETS ${EXE_NAME}
	RUNTIME DESTINATION ${KLAYGE_BIN_DIR}
	LIBRARY DESTINATION ${KLAYGE_BIN_DIR}
	ARCHIVE DESTINATION ${KLAYGE_OUTPUT_DIR}
)

CREATE_PROJECT_USERFILE(Tests ${EXE_NAME})
