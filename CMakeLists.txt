cmake_minimum_required (VERSION 3.8)

set(AVX2 true)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("ssharp-core")

#cityhash
add_library(ssharp-cityhash STATIC 
	"src/utilities/hash/ssharp-cityhash.h" 
	"src/utilities/hash/ssharp-cityhash.cpp" 
	"src/libs/cityhash/city.c" 
	"src/libs/cityhash/city.h"
)

#exceptions
add_library(ssharp-exceptions STATIC 
	"src/utilities/exceptions/exceptions.h" 
	"src/utilities/exceptions/exceptions.cpp"
)

#3nk
add_library (ssharp-3nk STATIC 
	"src/transcoder/3nk/ssharp-3nk.h" 
	"src/transcoder/3nk/ssharp-3nk.cpp"
)

#buff-loader
add_library(stream-loader STATIC
	"src/utilities/stream-loader/stream-loader.h"
	"src/utilities/stream-loader/stream-loader.cpp"
)

#basic-parser
add_library(parser-utils STATIC
	"src/parser/parser-utils/parser-utils.h"
	"src/parser/parser-utils/parser-utils.cpp"
)

#sii
add_library(ssharp-sii STATIC
	"src/parser/sii/ssharp-sii.h"
	"src/parser/sii/ssharp-sii.cpp"
)

#pmd
add_library(ssharp-pmd STATIC
	"src/parser/pmd/ssharp-pmd.h"
	"src/parser/pmd/ssharp-pmd.cpp"
)

#mat
add_library(ssharp-mat STATIC
	"src/parser/mat/ssharp-mat.h"
	"src/parser/mat/ssharp-mat.cpp"
)

#tobj
add_library(ssharp-tobj STATIC
	"src/parser/tobj/ssharp-tobj.h"
	"src/parser/tobj/ssharp-tobj.cpp"
)

#soundref
add_library(ssharp-soundref STATIC
	"src/parser/soundref/ssharp-soundref.h"
	"src/parser/soundref/ssharp-soundref.cpp"
)

 #deflate
 add_library(zlib-lite STATIC
    "src/libs/zlib/crc32.h"
    "src/libs/zlib/deflate.h"
    "src/libs/zlib/inffast.h"
    "src/libs/zlib/inffixed.h"
    "src/libs/zlib/inflate.h"
    "src/libs/zlib/inftrees.h"
    "src/libs/zlib/trees.h"
    "src/libs/zlib/zutil.h"
    "src/libs/zlib/adler32.c"
    "src/libs/zlib/crc32.c"
    "src/libs/zlib/deflate.c"
    "src/libs/zlib/inflate.c"
    "src/libs/zlib/infback.c"
    "src/libs/zlib/inftrees.c"
    "src/libs/zlib/inffast.c"
    "src/libs/zlib/trees.c"
    "src/libs/zlib/zutil.c"
 )

 #ssharp-deflate
 add_library(ssharp-deflate STATIC
	"src/utilities/deflate/ssharp-deflate.h"
	"src/utilities/deflate/ssharp-deflate.cpp"
 )

 add_library(ssharp-absentable STATIC
	"src/objects/absentable/absentable.h"
	"src/objects/absentable/absentable.cpp"
 )

 #entry_objects
 add_library(ssharp-entry-object STATIC
	"src/objects/entry-objects/entry-objects.h"
	"src/objects/entry-objects/entry-objects.cpp"
 )

 add_executable(file-deflater
	"src/tools/file-deflater.cpp"
 )

#include_directories
target_include_directories(ssharp-cityhash PRIVATE "src/libs/cityhash")
target_include_directories(ssharp-3nk PRIVATE "src/types" "src/utilities/exceptions")
target_include_directories(stream-loader PRIVATE "src/types")
target_include_directories(parser-utils PRIVATE "src/types" "src/utilities/exceptions" "src/utilities/stream-loader")
target_include_directories(ssharp-sii PRIVATE "src/types" "src/utilities/exceptions" "src/utilities/stream-loader" "src/parser/parser-utils")
target_include_directories(ssharp-pmd PRIVATE "src/types" "src/utilities/exceptions" "src/utilities/stream-loader" "src/parser/parser-utils")
target_include_directories(ssharp-mat PRIVATE "src/types" "src/utilities/exceptions" "src/utilities/stream-loader" "src/parser/parser-utils")
target_include_directories(ssharp-tobj PRIVATE "src/types" "src/utilities/exceptions" "src/utilities/stream-loader" "src/parser/parser-utils")
target_include_directories(ssharp-soundref PRIVATE "src/types" "src/utilities/exceptions" "src/utilities/stream-loader" "src/parser/parser-utils")
target_include_directories(ssharp-deflate PRIVATE "src/types" "src/libs/zlib" "src/utilities/exceptions")
target_include_directories(file-deflater PRIVATE
	"src/utilities/deflate" 
	"src/utilities/stream-loader" 
	"src/types" 
	"src/libs/zlib" 
	"src/utilities/exceptions"
)

target_include_directories(ssharp-entry-object PRIVATE
	"src/types"
	"src/utilities/exceptions"
	"src/utilities/stream-loader" 
	"src/parser/sii"
	"src/parser/pmd"
	"src/parser/mat"
	"src/parser/tobj"
	"src/parser/soundref"
	"src/parser/parser-utils"
	"src/objects/absentable"
)

target_include_directories(ssharp-absentable PRIVATE
	"src/types"
	"src/utilities/exceptions"
)

#link_libraries
target_link_libraries(ssharp-3nk PRIVATE ssharp-exceptions)
target_link_libraries(parser-utils PRIVATE ssharp-exceptions stream-loader)
target_link_libraries(ssharp-sii PRIVATE ssharp-exceptions stream-loader parser-utils)
target_link_libraries(ssharp-pmd PRIVATE ssharp-exceptions stream-loader parser-utils)
target_link_libraries(ssharp-mat PRIVATE ssharp-exceptions stream-loader parser-utils)
target_link_libraries(ssharp-tobj PRIVATE ssharp-exceptions stream-loader parser-utils)
target_link_libraries(ssharp-soundref PRIVATE ssharp-exceptions stream-loader parser-utils)
target_link_libraries(ssharp-deflate PRIVATE zlib-lite ssharp-exceptions)
target_link_libraries(file-deflater PRIVATE ssharp-deflate stream-loader zlib-lite ssharp-exceptions)
target_link_libraries(ssharp-absentable PRIVATE ssharp-exceptions)
target_link_libraries(ssharp-entry-object PRIVATE
	ssharp-exceptions
	stream-loader
	parser-utils
	ssharp-sii
	ssharp-pmd
	ssharp-mat
	ssharp-tobj
	ssharp-soundref
	ssharp-absentable
)

#compiler_options
if(AVX2)
	if(MSVC)
	  target_compile_options(ssharp-3nk PRIVATE "/arch:AVX2")
	else()
	  target_compile_options(ssharp-3nk PRIVATE "-mavx2")
	endif()
	target_compile_definitions(ssharp-3nk PRIVATE "__AVX__" "__AVX2__") # Fixes MSVC code highlighting
endif()

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET
	  ssharp-cityhash
	  ssharp-exceptions
	  ssharp-3nk
	  stream-loader
	  parser-utils
	  ssharp-sii
	  ssharp-pmd
	  ssharp-mat
	  ssharp-tobj
	  ssharp-soundref
	  zlib-lite
	  ssharp-deflate
	  file-deflater
	  ssharp-absentable
	  ssharp-entry-object
  PROPERTY CXX_STANDARD 20)
endif()
