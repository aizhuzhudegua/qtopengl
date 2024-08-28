# Install script for directory: F:/assimp-5.1.6/code

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Program Files (x86)/Assimp")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set path to fallback-tool for dependency-resolution.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "F:/qt/Tools/mingw730_32/bin/objdump.exe")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "F:/assimp-5.1.6/build/lib/libassimp.dll.a")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "F:/assimp-5.1.6/build/bin/libassimp-5.dll")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/libassimp-5.dll" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/libassimp-5.dll")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "F:/qt/Tools/mingw730_32/bin/strip.exe" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/libassimp-5.dll")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "assimp-dev" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/assimp" TYPE FILE FILES
    "F:/assimp-5.1.6/code/../include/assimp/anim.h"
    "F:/assimp-5.1.6/code/../include/assimp/aabb.h"
    "F:/assimp-5.1.6/code/../include/assimp/ai_assert.h"
    "F:/assimp-5.1.6/code/../include/assimp/camera.h"
    "F:/assimp-5.1.6/code/../include/assimp/color4.h"
    "F:/assimp-5.1.6/code/../include/assimp/color4.inl"
    "F:/assimp-5.1.6/build/code/../include/assimp/config.h"
    "F:/assimp-5.1.6/code/../include/assimp/ColladaMetaData.h"
    "F:/assimp-5.1.6/code/../include/assimp/commonMetaData.h"
    "F:/assimp-5.1.6/code/../include/assimp/defs.h"
    "F:/assimp-5.1.6/code/../include/assimp/cfileio.h"
    "F:/assimp-5.1.6/code/../include/assimp/light.h"
    "F:/assimp-5.1.6/code/../include/assimp/material.h"
    "F:/assimp-5.1.6/code/../include/assimp/material.inl"
    "F:/assimp-5.1.6/code/../include/assimp/matrix3x3.h"
    "F:/assimp-5.1.6/code/../include/assimp/matrix3x3.inl"
    "F:/assimp-5.1.6/code/../include/assimp/matrix4x4.h"
    "F:/assimp-5.1.6/code/../include/assimp/matrix4x4.inl"
    "F:/assimp-5.1.6/code/../include/assimp/mesh.h"
    "F:/assimp-5.1.6/code/../include/assimp/pbrmaterial.h"
    "F:/assimp-5.1.6/code/../include/assimp/GltfMaterial.h"
    "F:/assimp-5.1.6/code/../include/assimp/postprocess.h"
    "F:/assimp-5.1.6/code/../include/assimp/quaternion.h"
    "F:/assimp-5.1.6/code/../include/assimp/quaternion.inl"
    "F:/assimp-5.1.6/code/../include/assimp/scene.h"
    "F:/assimp-5.1.6/code/../include/assimp/metadata.h"
    "F:/assimp-5.1.6/code/../include/assimp/texture.h"
    "F:/assimp-5.1.6/code/../include/assimp/types.h"
    "F:/assimp-5.1.6/code/../include/assimp/vector2.h"
    "F:/assimp-5.1.6/code/../include/assimp/vector2.inl"
    "F:/assimp-5.1.6/code/../include/assimp/vector3.h"
    "F:/assimp-5.1.6/code/../include/assimp/vector3.inl"
    "F:/assimp-5.1.6/code/../include/assimp/version.h"
    "F:/assimp-5.1.6/code/../include/assimp/cimport.h"
    "F:/assimp-5.1.6/code/../include/assimp/importerdesc.h"
    "F:/assimp-5.1.6/code/../include/assimp/Importer.hpp"
    "F:/assimp-5.1.6/code/../include/assimp/DefaultLogger.hpp"
    "F:/assimp-5.1.6/code/../include/assimp/ProgressHandler.hpp"
    "F:/assimp-5.1.6/code/../include/assimp/IOStream.hpp"
    "F:/assimp-5.1.6/code/../include/assimp/IOSystem.hpp"
    "F:/assimp-5.1.6/code/../include/assimp/Logger.hpp"
    "F:/assimp-5.1.6/code/../include/assimp/LogStream.hpp"
    "F:/assimp-5.1.6/code/../include/assimp/NullLogger.hpp"
    "F:/assimp-5.1.6/code/../include/assimp/cexport.h"
    "F:/assimp-5.1.6/code/../include/assimp/Exporter.hpp"
    "F:/assimp-5.1.6/code/../include/assimp/DefaultIOStream.h"
    "F:/assimp-5.1.6/code/../include/assimp/DefaultIOSystem.h"
    "F:/assimp-5.1.6/code/../include/assimp/ZipArchiveIOSystem.h"
    "F:/assimp-5.1.6/code/../include/assimp/SceneCombiner.h"
    "F:/assimp-5.1.6/code/../include/assimp/fast_atof.h"
    "F:/assimp-5.1.6/code/../include/assimp/qnan.h"
    "F:/assimp-5.1.6/code/../include/assimp/BaseImporter.h"
    "F:/assimp-5.1.6/code/../include/assimp/Hash.h"
    "F:/assimp-5.1.6/code/../include/assimp/MemoryIOWrapper.h"
    "F:/assimp-5.1.6/code/../include/assimp/ParsingUtils.h"
    "F:/assimp-5.1.6/code/../include/assimp/StreamReader.h"
    "F:/assimp-5.1.6/code/../include/assimp/StreamWriter.h"
    "F:/assimp-5.1.6/code/../include/assimp/StringComparison.h"
    "F:/assimp-5.1.6/code/../include/assimp/StringUtils.h"
    "F:/assimp-5.1.6/code/../include/assimp/SGSpatialSort.h"
    "F:/assimp-5.1.6/code/../include/assimp/GenericProperty.h"
    "F:/assimp-5.1.6/code/../include/assimp/SpatialSort.h"
    "F:/assimp-5.1.6/code/../include/assimp/SkeletonMeshBuilder.h"
    "F:/assimp-5.1.6/code/../include/assimp/SmallVector.h"
    "F:/assimp-5.1.6/code/../include/assimp/SmoothingGroups.h"
    "F:/assimp-5.1.6/code/../include/assimp/SmoothingGroups.inl"
    "F:/assimp-5.1.6/code/../include/assimp/StandardShapes.h"
    "F:/assimp-5.1.6/code/../include/assimp/RemoveComments.h"
    "F:/assimp-5.1.6/code/../include/assimp/Subdivision.h"
    "F:/assimp-5.1.6/code/../include/assimp/Vertex.h"
    "F:/assimp-5.1.6/code/../include/assimp/LineSplitter.h"
    "F:/assimp-5.1.6/code/../include/assimp/TinyFormatter.h"
    "F:/assimp-5.1.6/code/../include/assimp/Profiler.h"
    "F:/assimp-5.1.6/code/../include/assimp/LogAux.h"
    "F:/assimp-5.1.6/code/../include/assimp/Bitmap.h"
    "F:/assimp-5.1.6/code/../include/assimp/XMLTools.h"
    "F:/assimp-5.1.6/code/../include/assimp/IOStreamBuffer.h"
    "F:/assimp-5.1.6/code/../include/assimp/CreateAnimMesh.h"
    "F:/assimp-5.1.6/code/../include/assimp/XmlParser.h"
    "F:/assimp-5.1.6/code/../include/assimp/BlobIOSystem.h"
    "F:/assimp-5.1.6/code/../include/assimp/MathFunctions.h"
    "F:/assimp-5.1.6/code/../include/assimp/Exceptional.h"
    "F:/assimp-5.1.6/code/../include/assimp/ByteSwapper.h"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "assimp-dev" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/assimp/Compiler" TYPE FILE FILES
    "F:/assimp-5.1.6/code/../include/assimp/Compiler/pushpack1.h"
    "F:/assimp-5.1.6/code/../include/assimp/Compiler/poppack1.h"
    "F:/assimp-5.1.6/code/../include/assimp/Compiler/pstdint.h"
    )
endif()

