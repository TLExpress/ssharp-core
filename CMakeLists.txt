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
	"src/exceptions/exceptions.h" 
	"src/exceptions/exceptions.cpp"
)

#transcoder-utils
add_library (transcoder-utils STATIC 
	"src/transcoder/transcoder-utils/transcoder-utils.h" 
	"src/transcoder/transcoder-utils/transcoder-utils.cpp"
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

#buff-loader
add_library(path-spliter STATIC
	"src/utilities/path-spliter/path-spliter.h"
	"src/utilities/path-spliter/path-spliter.cpp"
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

#directory
add_library(ssharp-directory STATIC
	"src/parser/directory/ssharp-directory.h"
	"src/parser/directory/ssharp-directory.cpp"
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

 #entry-objects
 add_library(ssharp-entry-object STATIC
	"src/objects/entry-objects/entry-objects.h"
	"src/objects/entry-objects/entry-objects.cpp"
 )

 #entry-list
 add_library(ssharp-entry-list STATIC
	"src/objects/entry-list/entry-list.h"
	"src/objects/entry-list/entry-list.cpp"
 )

 #entry-list
 add_library(ssharp-hashfs STATIC
	"src/fs/hashfs/ssharp-hashfs.h"
	"src/fs/hashfs/ssharp-hashfs.cpp"
 )

 #dictionary
 add_library(ssharp-dictionary STATIC
	"src/dictionary/ssharp-dictionary.h"
	"src/dictionary/ssharp-dictionary.cpp"
 )

 add_executable(tester
	"src/tools/tester.cpp"
 )

#include_directories

set(CITYHASH_INC
	"src/libs/cityhash"
	"src/utilities/hash"
)

set(PATH_SPLITER_INC
	"src/types"
	"src/utilities/path-spliter"
)

set(STREAM_LOADER_INC
	"src/types"
	"src/utilities/stream-loader"
)

set(TRANSCODER_UTILS_INC
	"src/types"
	"src/exceptions"
	"src/transcoder/transcoder-utils"
	${STREAM_LOADER_INC}
)

set(PARSER_UTILS_INC
	"src/types"
	"src/exceptions"
	"src/parser/parser-utils"
	${STREAM_LOADER_INC}
)

set(DICTIONARY_INC
	"src/types"
	"src/dictionary"
	${CITYHASH_INC}
)

set(3NK_INC
	${TRANSCODER_UTILS_INC}
	${STREAM_LOADER_INC}
	"src/transcoder/3nk"
)

set(SII_INC 
	${PARSER_UTILS_INC}
	"src/parser/sii"
)

set(PMD_INC 
	${PARSER_UTILS_INC}
	"src/parser/pmd"
)

set(MAT_INC 
	${PARSER_UTILS_INC}
	"src/parser/mat"
)

set(TOBJ_INC 
	${PARSER_UTILS_INC}
	"src/parser/tobj"
)

set(SOUNDREF_INC 
	${PARSER_UTILS_INC}
	"src/parser/soundref"
)

set(DIRECTORY_INC 
	${PARSER_UTILS_INC}
	"src/parser/directory"
)

set(DEFLATE_INC
	"src/types"
	"src/libs/zlib"
	"src/exceptions"
	"src/utilities/deflate"
)

set(ENTRY_OBJECT_INC
	"src/types"
	"src/exceptions"
	"src/objects/containers"
	"src/objects/entry-objects"
	${SII_INC}
	${PMD_INC}
	${DIRECTORY_INC}
	${DICTIONARY_INC}
	${MAT_INC}
	${TOBJ_INC}
	${SOUNDREF_INC}
	${PARSER_UTILS_INC}
	${DEFLATE_INC}
	${CITYHASH_INC}
	${3NK_INC}
)

set(ENTRY_LIST_INC
	"src/objects/containers"
	"src/objects/entry-list"
	${ENTRY_OBJECT_INC}
	${PATH_SPLITER_INC}
)
set(HASHFS_INC
	"src/fs/hashfs"
	${ENTRY_LIST_INC}
)
set(TESTER_INC
	${HASHFS_INC}
)

target_include_directories(ssharp-cityhash PRIVATE ${CITYHASH_INC})
target_include_directories(ssharp-dictionary PRIVATE ${DICTIONARY_INC})
target_include_directories(transcoder-utils PRIVATE	${TRANSCODER_UTILS_INC})
target_include_directories(ssharp-3nk PRIVATE ${3NK_INC})
target_include_directories(stream-loader PRIVATE ${STREAM_LOADER_INC})
target_include_directories(path-spliter PRIVATE ${PATH_SPLITER_INC})
target_include_directories(parser-utils PRIVATE ${PARSER_UTILS_INC})
target_include_directories(ssharp-sii PRIVATE ${SII_INC})
target_include_directories(ssharp-pmd PRIVATE ${PMD_INC})
target_include_directories(ssharp-directory PRIVATE ${DIRECTORY_INC})
target_include_directories(ssharp-mat PRIVATE ${MAT_INC})
target_include_directories(ssharp-tobj PRIVATE ${TOBJ_INC})
target_include_directories(ssharp-soundref PRIVATE ${SOUNDREF_INC})
target_include_directories(ssharp-deflate PRIVATE ${DEFLATE_INC})
target_include_directories(ssharp-entry-object PRIVATE ${ENTRY_OBJECT_INC})
target_include_directories(ssharp-entry-list PRIVATE ${ENTRY_LIST_INC})
target_include_directories(ssharp-hashfs PRIVATE ${HASHFS_INC})
target_include_directories(tester PRIVATE ${HASHFS_INC})

#link_libraries
target_link_libraries(transcoder-utils PRIVATE ssharp-exceptions stream-loader)
target_link_libraries(ssharp-3nk PRIVATE ssharp-exceptions transcoder-utils stream-loader)
target_link_libraries(ssharp-dictionary PRIVATE ssharp-exceptions)
target_link_libraries(parser-utils PRIVATE ssharp-exceptions stream-loader)
target_link_libraries(ssharp-sii PRIVATE ssharp-exceptions stream-loader parser-utils)
target_link_libraries(ssharp-pmd PRIVATE ssharp-exceptions stream-loader parser-utils)
target_link_libraries(ssharp-mat PRIVATE ssharp-exceptions stream-loader parser-utils)
target_link_libraries(ssharp-tobj PRIVATE ssharp-exceptions stream-loader parser-utils)
target_link_libraries(ssharp-soundref PRIVATE ssharp-exceptions stream-loader parser-utils)
target_link_libraries(ssharp-deflate PRIVATE zlib-lite ssharp-exceptions)
target_link_libraries(ssharp-entry-object PRIVATE
	ssharp-exceptions
	stream-loader
	parser-utils
	ssharp-sii
	ssharp-pmd
	ssharp-directory
	ssharp-mat
	ssharp-tobj
	ssharp-deflate
	ssharp-soundref
	zlib-lite
	ssharp-cityhash
	ssharp-3nk
)
target_link_libraries(ssharp-entry-list PRIVATE
	ssharp-exceptions
	stream-loader
	path-spliter
	parser-utils
	ssharp-sii
	ssharp-pmd
	ssharp-directory
	ssharp-mat
	ssharp-tobj
	ssharp-deflate
	ssharp-soundref
	zlib-lite
	ssharp-cityhash
	ssharp-3nk
	ssharp-entry-object
)
target_link_libraries(ssharp-hashfs PRIVATE
	ssharp-exceptions
	stream-loader
	path-spliter
	parser-utils
	ssharp-sii
	ssharp-pmd
	ssharp-directory
	ssharp-mat
	ssharp-tobj
	ssharp-deflate
	ssharp-soundref
	zlib-lite
	ssharp-cityhash
	ssharp-3nk
	ssharp-entry-object
	ssharp-entry-list
)
target_link_libraries(tester PRIVATE 
	ssharp-exceptions
	stream-loader
	path-spliter
	parser-utils
	ssharp-sii
	ssharp-pmd
	ssharp-directory
	ssharp-mat
	ssharp-tobj
	ssharp-deflate
	ssharp-soundref
	zlib-lite
	ssharp-cityhash
	ssharp-3nk
	ssharp-entry-object
	ssharp-entry-list
	ssharp-hashfs
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
	target_compile_definitions(zlib-lite PRIVATE NO_GZIP)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET
	  ssharp-cityhash
	  ssharp-exceptions
	  transcoder-utils
	  ssharp-3nk
	  stream-loader
	  path-spliter
	  parser-utils
	  ssharp-sii
	  ssharp-pmd
	  ssharp-directory
	  ssharp-dictionary
	  ssharp-mat
	  ssharp-tobj
	  ssharp-soundref
	  zlib-lite
	  ssharp-deflate
	  tester
	  #ssharp-container
	  ssharp-entry-object
	  ssharp-entry-list
	  ssharp-hashfs
  PROPERTY CXX_STANDARD 20)
endif()
