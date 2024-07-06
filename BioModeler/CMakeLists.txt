# CMakeList.txt: projekt CMake dla elementu BioModeler, dołącz źródło i zdefiniuj
# tutaj logikę specyficzną dla projektu.
#

# Dodaj źródło do pliku wykonywalnego tego projektu.
add_executable (BioModeler "BioModeler.cpp" "BioModeler.h" "DICOM/DebugUtilty.h"   "DICOM/Reader.h" "DICOM/Reader.cpp" "DICOM/DcmFile.h" "DICOM/DcmFile.cpp" "DICOM/DcmTags.h" "DICOM/DcmTags.cpp" )

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET BioModeler PROPERTY CXX_STANDARD 20)
endif()

# TODO: Dodaj testy i zainstaluj elementy docelowe w razie potrzeby.
