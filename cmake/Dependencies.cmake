include(FetchContent)

FetchContent_Declare(eigen URL https://gitlab.com/libeigen/eigen/-/archive/3.4.0/eigen-3.4.0.tar.gz)

FetchContent_MakeAvailable(eigen)

IF (NOT TARGET uninstall)
  ADD_CUSTOM_TARGET(uninstall "${CMAKE_COMMAND}" -P "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake")
  if(ENABLE_SOLUTION_FOLDERS)
    set_target_properties(uninstall PROPERTIES FOLDER "CMakeTargets")
  endif()
endif()
