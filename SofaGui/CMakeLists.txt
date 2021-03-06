cmake_minimum_required(VERSION 3.1)
project(SofaGui)


### Dependencies

## Sofa
find_package(SofaGeneral REQUIRED)
find_package(SofaMisc REQUIRED)
find_package(SofaAdvanced REQUIRED)

# Qt GUIs are all based on OpenGL
if(NOT SOFA_NO_OPENGL)
    ## Qt(Qt4/5)
    set(QT_USE_IMPORTED_TARGETS 1)

    find_package(Qt5 COMPONENTS Core QUIET)
    if(Qt5Core_FOUND)
        set(QT5_NO_LINK_QTMAIN 1)
        find_package(Qt5 COMPONENTS Gui OpenGL Xml REQUIRED)

        if(WIN32)
            file(GLOB_RECURSE QT5_DLLS "${_qt5Core_install_prefix}/bin/*.dll")
            if(CMAKE_CONFIGURATION_TYPES) # Multi-config generator (MSVC)
                foreach(CONFIG ${CMAKE_CONFIGURATION_TYPES})
                    file(COPY ${QT5_DLLS} DESTINATION "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${CONFIG}")
                endforeach()
            else()                      # Single-config generator (nmake)
                file(COPY ${QT5_DLLS} DESTINATION "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
            endif()
        endif()

        install(FILES ${QT5_DLLS} DESTINATION bin)
    else()
        set(QT4_NO_LINK_QTMAIN 1)
        find_package(Qt4 COMPONENTS qtcore qtgui qtopengl qtxml)
        if(WIN32)
            file(GLOB QT4_DLLS "${QT_BINARY_DIR}/*.dll")
            if(CMAKE_CONFIGURATION_TYPES) # Multi-config generator (MSVC)
                foreach(CONFIG ${CMAKE_CONFIGURATION_TYPES})
                    file(COPY ${QT4_DLLS} DESTINATION "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${CONFIG}")
                endforeach()
            else()                      # Single-config generator (nmake)
                file(COPY ${QT4_DLLS} DESTINATION "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
            endif()
        endif()

        install(FILES ${QT4_DLLS} DESTINATION bin)
    endif()

    ## Thoses variables are used in SofaGuiConfig.cmake.in
    if(Qt5_FOUND OR Qt4_FOUND)
        set(SOFA_HAVE_QT 1)
    endif()
    set(SOFA_QT4 ${Qt4_FOUND})
    set(SOFA_QT5 ${Qt5_FOUND})
else()
    set(SOFA_HAVE_QT 0)
    set(SOFA_QT4 0)
    set(SOFA_QT5 0)
endif()

set(SOFAGUI_TARGETS "")

## Version
set(SOFAGUI_VERSION ${SofaFramework_VERSION})

### Sub-projects

## SofaGuiCommon
add_subdirectory(../applications/sofa/gui/SofaGuiCommon ${CMAKE_CURRENT_BINARY_DIR}/SofaGuiCommon)
list(APPEND SOFAGUI_TARGETS SofaGuiCommon)

## OpenGL-based GUIs
if(NOT SOFA_NO_OPENGL)
    ## Glut GUI
    if(SOFA_HAVE_GLUT)
        option(SOFAGUI_GLUT "Compile the Glut GUI" OFF)
        if(SOFAGUI_GLUT)
            set(SOFA_GUI_GLUT 1) # cmakedefine
            list(APPEND SOFAGUI_TARGETS SofaGuiGlut)
            add_subdirectory(../applications/sofa/gui/glut ${CMAKE_CURRENT_BINARY_DIR}/SofaGuiGlut)
        endif()
    endif()

    ## Qt GUI
    # Check if Qt(4/5) is present ; if not, disable ability to compile Qt things
    set(SOFA_HAVE_QT_LIBRARIES 1)
    find_package(Qt5 COMPONENTS Core QUIET)
    if(NOT Qt5Core_FOUND)
        find_package(Qt4 QUIET COMPONENTS qtcore)
        if(NOT Qt4_FOUND)
            set(SOFA_HAVE_QT_LIBRARIES 0)
        endif()
    endif()

    if(SOFA_HAVE_QT_LIBRARIES)
        option(SOFAGUI_QT "Compile the Qt GUI" ON)
        if(SOFAGUI_QT)

            ## Qt GUI options, used in the relevant CMakeLists.txt

            ## SOFA_GUI_INTERACTION
            option(SOFAGUI_QT_INTERACTION
                    "Enable interaction mode for the Qt GUI, whatever that means (this
                     will define the SOFA_GUI_INTERACTION macro)." OFF)
            if(SOFAGUI_QT_INTERACTION)
                set(SOFA_GUI_INTERACTION 1) # cmakedefine
            endif()

            ## QtViewer
            option(SOFAGUI_QTVIEWER "Compile the QtViewer for the Qt GUI" ON)
            if(SOFAGUI_QTVIEWER)
                set(SOFA_GUI_QTVIEWER 1) # cmakedefine
            endif()

            ## QglViewer
            option(SOFAGUI_QGLVIEWER "Compile the QGLViewer for the Qt GUI" ON)
            if(SOFAGUI_QGLVIEWER)
                set(SOFA_GUI_QGLVIEWER 1) # cmakedefine
                add_subdirectory(${CMAKE_SOURCE_DIR}/extlibs/libQGLViewer-2.6.3/QGLViewer ${CMAKE_BINARY_DIR}/extlibs/QGLViewer)
            endif()

           ## QWT
           option(SOFAGUI_ENABLE_QWT "Enable QWT for Qt GUIs")
           if(SOFAGUI_ENABLE_QWT)
                set(SOFA_HAVE_QWT 1) # cmakedefine
            endif()


            set(SOFA_GUI_QT 1) # cmakedefine
            list(APPEND SOFAGUI_TARGETS SofaGuiQt)
            add_subdirectory(../applications/sofa/gui/qt ${CMAKE_CURRENT_BINARY_DIR}/SofaGuiQt)
        endif()
    else()
        message("Cannot find any Qt libraries on your system, please set Qt5_DIR or Qt4_DIR if you want to compile GUIs using Qt")
    endif()
endif()

## SofaGuiMain
list(APPEND SOFAGUI_TARGETS SofaGuiMain)
add_subdirectory(../applications/sofa/gui/SofaGuiMain ${CMAKE_CURRENT_BINARY_DIR}/SofaGuiMain)


## IDE folder
foreach(TARGET ${SOFAGUI_TARGETS})
    set_target_properties(${TARGET} PROPERTIES FOLDER "SofaGui")
    target_include_directories(${TARGET} PUBLIC "$<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/include>")
    target_include_directories(${TARGET} PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../applications>")
    # target_include_directories(${TARGET} PUBLIC "$<BUILD_INTERFACE:${QT_INCLUDES}>")
    set_target_properties(${TARGET} PROPERTIES DEBUG_POSTFIX "_d")
    set_target_properties(${TARGET} PROPERTIES VERSION ${SOFAGUI_VERSION})
endforeach()


## SofaGuiConfig/config.h
configure_file(config.h.in "${CMAKE_BINARY_DIR}/include/SofaGui/config.h")
install(FILES "${CMAKE_BINARY_DIR}/include/SofaGui/config.h" DESTINATION "include/SofaGui")

## CMake package configurations files
sofa_write_package_config_files(SofaGui ${SOFAGUI_VERSION})
