// Copyright (C) 2010 von Karman Institute for Fluid Dynamics, Belgium
//
// This software is distributed under the terms of the
// GNU Lesser General Public License version 3 (LGPLv3).
// See doc/lgpl.txt and doc/gpl.txt for the license text.

#ifndef CF_Tools_Shell_LibShell_hpp
#define CF_Tools_Shell_LibShell_hpp

////////////////////////////////////////////////////////////////////////////////

#include "Common/CLibrary.hpp"

////////////////////////////////////////////////////////////////////////////////

/// Define the macro Shell_API
/// @note build system defines COOLFLUID_MESHDIFF_EXPORTS when compiling
/// CommandLineInterpreter files
#ifdef COOLFLUID_MESHDIFF_EXPORTS
#   define Shell_API      CF_EXPORT_API
#   define Shell_TEMPLATE
#else
#   define Shell_API      CF_IMPORT_API
#   define Shell_TEMPLATE CF_TEMPLATE_EXTERN
#endif

////////////////////////////////////////////////////////////////////////////////

namespace CF {
namespace Tools {
namespace Shell {

////////////////////////////////////////////////////////////////////////////////

  /// Class defines the initialization and termination of the library CommandLineInterpreter
  /// @author Tiago Quintino
  class Shell_API LibShell : public Common::CLibrary
  {
  public:

    typedef boost::shared_ptr<LibShell> Ptr;
    typedef boost::shared_ptr<LibShell const> ConstPtr;

    /// Constructor
    LibShell ( const std::string& name) : Common::CLibrary(name) {   }

  public: // functions

    /// @return string of the library namespace
    static std::string library_namespace() { return "CF.Tools.CommandLineInterpreter"; }

    /// Static function that returns the module name.
    /// Must be implemented for CLibrary registration
    /// @return name of the library
    static std::string library_name() { return "CommandLineInterpreter"; }

    /// Static function that returns the description of the module.
    /// Must be implemented for CLibrary registration
    /// @return description of the library

    static std::string library_description()
    {
      return "This library implements the Command Line Interpreter API.";
    }

    /// Gets the Class name
    static std::string type_name() { return "LibCommandLineInterpreter"; }

  protected:

    /// initiate library
    virtual void initiate_impl();

    /// terminate library
    virtual void terminate_impl();

  }; // end LibCommandLineInterpreter

////////////////////////////////////////////////////////////////////////////////

} // Shell
} // Tools
} // CF

////////////////////////////////////////////////////////////////////////////////

#endif // CF_Tools_Shell_LibShell_hpp
