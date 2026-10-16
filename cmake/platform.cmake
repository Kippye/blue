######### PLATFORM #########
IF (WIN32)
    IF (MINGW)
        set(WINDOWS_MINGW TRUE)
        target_compile_definitions(${PROGRAM_NAME} PUBLIC "BLUE_MINGW")
    ELSEIF(MSVC)
        set(WINDOWS_MSVC TRUE)
        target_compile_definitions(${PROGRAM_NAME} PUBLIC "BLUE_MSVC")
    ENDIF()
    # DEFINES
    target_compile_definitions(${PROGRAM_NAME} PUBLIC "WIN32")
    target_compile_definitions(${PROGRAM_NAME} PUBLIC "_WIN32")
    target_compile_definitions(${PROGRAM_NAME} PUBLIC "BLUE_WINDOWS")
ELSEIF(LINUX)
    # DEFINES
    target_compile_definitions(${PROGRAM_NAME} PUBLIC "BLUE_LINUX")
ENDIF()