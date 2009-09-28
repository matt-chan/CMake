/*============================================================================
  CMake - Cross Platform Makefile Generator
  Copyright 2000-2009 Kitware, Inc., Insight Software Consortium

  Distributed under the OSI-approved BSD License (the "License");
  see accompanying file Copyright.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even the
  implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the License for more information.
============================================================================*/
#include "cmGetFilenameComponentCommand.h"
#include "cmSystemTools.h"

// cmGetFilenameComponentCommand
bool cmGetFilenameComponentCommand
::InitialPass(std::vector<std::string> const& args, cmExecutionStatus &)
{
  if(args.size() < 3)
    {
    this->SetError("called with incorrect number of arguments");
    return false;
    }

  // Check and see if the value has been stored in the cache
  // already, if so use that value
  if(args.size() == 4 && args[3] == "CACHE")
    {
    const char* cacheValue = this->Makefile->GetDefinition(args[0].c_str());
    if(cacheValue && !cmSystemTools::IsNOTFOUND(cacheValue))
      {
      return true;
      }
    }
  
  std::string result;
  std::string filename = args[1];
  cmSystemTools::ExpandRegistryValues(filename);
  std::string storeArgs;
  std::string programArgs;
  if (args[2] == "PATH")
    {
    result = cmSystemTools::GetFilenamePath(filename);
    }
  else if (args[2] == "NAME")
    {
    result = cmSystemTools::GetFilenameName(filename);
    }
  else if (args[2] == "PROGRAM")
    {
    for(unsigned int i=2; i < args.size(); ++i)
      {
      if(args[i] == "PROGRAM_ARGS")
        {
        i++;
        if(i < args.size())
          {
          storeArgs = args[i];
          }
        }
      }
    cmSystemTools::SplitProgramFromArgs(filename.c_str(), 
                                        result, programArgs);
    }
  else if (args[2] == "EXT")
    {
    result = cmSystemTools::GetFilenameExtension(filename);
    }
  else if (args[2] == "NAME_WE")
    {
    result = cmSystemTools::GetFilenameWithoutExtension(filename);
    }
  else if (args[2] == "ABSOLUTE" ||
           args[2] == "REALPATH")
    {
    // Collapse the path to its simplest form.
    // If the path given is relative evaluate it relative to the
    // current source directory.
    result = cmSystemTools::CollapseFullPath(
      filename.c_str(), this->Makefile->GetCurrentDirectory());
    if(args[2] == "REALPATH")
      {
      // Resolve symlinks if possible
      result = cmSystemTools::GetRealPath(filename.c_str());
      }
    }
  else 
    {
    std::string err = "unknown component " + args[2];
    this->SetError(err.c_str());
    return false;
    }

  if(args.size() == 4 && args[3] == "CACHE")
    {
    if(programArgs.size() && storeArgs.size())
      {
      this->Makefile->AddCacheDefinition
        (storeArgs.c_str(), programArgs.c_str(),
         "", args[2] == "PATH" ? cmCacheManager::FILEPATH
         : cmCacheManager::STRING);
      }
    this->Makefile->AddCacheDefinition
      (args[0].c_str(), result.c_str(), "",
       args[2] == "PATH" ? cmCacheManager::FILEPATH
       : cmCacheManager::STRING);
    }
  else 
    {
    if(programArgs.size() && storeArgs.size())
      {
      this->Makefile->AddDefinition(storeArgs.c_str(), programArgs.c_str());
      }
    this->Makefile->AddDefinition(args[0].c_str(), result.c_str());
    }

  return true;
}

