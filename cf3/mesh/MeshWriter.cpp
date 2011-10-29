// Copyright (C) 2010-2011 von Karman Institute for Fluid Dynamics, Belgium
//
// This software is distributed under the terms of the
// GNU Lesser General Public License version 3 (LGPLv3).
// See doc/lgpl.txt and doc/gpl.txt for the license text.

#include "common/Signal.hpp"
#include "common/OptionURI.hpp"
#include "common/OptionArray.hpp"
#include "common/OptionT.hpp"
#include "common/Environment.hpp"
#include "common/Core.hpp"

#include "mesh/MeshWriter.hpp"
#include "mesh/MeshMetadata.hpp"
#include "mesh/FieldGroup.hpp"
#include "mesh/Field.hpp"

namespace cf3 {
namespace mesh {

using namespace common;

////////////////////////////////////////////////////////////////////////////////

MeshWriter::MeshWriter ( const std::string& name  ) :
  Action ( name )
{
  mark_basic();

  std::vector<URI> fields;
  options().add_option< OptionArrayT<URI> >("fields",fields)
      ->description("Fields to ouptut")
      ->mark_basic()
      ->attach_trigger( boost::bind( &MeshWriter::config_fields, this ) );

  // Path to the mesh to write
  options().add_option( OptionURI::create("mesh", URI(), URI::Scheme::CPATH) )
      ->description("Mesh to write")
      ->pretty_name("Mesh")
      ->mark_basic();

  // Output file path
  options().add_option( OptionURI::create("file", URI("mesh", URI::Scheme::FILE), URI::Scheme::FILE) )
      ->description("File to write")
      ->pretty_name("File")
      ->mark_basic();

  // signal for writing the mesh
  regist_signal( "write_mesh" )
    ->connect( boost::bind( &MeshWriter::signal_write, this, _1 ) )
    ->description("Write the mesh")
    ->pretty_name("Write Mesh");
}

////////////////////////////////////////////////////////////////////////////////

void MeshWriter::config_fields()
{
  std::vector<URI> field_uris;
  options()["fields"].put_value(field_uris);

  m_fields.resize(0);
  boost_foreach ( const URI& uri, field_uris)
  {
    m_fields.push_back(access_component_ptr_checked(uri)->as_ptr_checked<Field>());
    if ( is_null(m_fields.back().lock()) )
      throw ValueNotFound(FromHere(),"Invalid URI ["+uri.string()+"]");
  }
}

////////////////////////////////////////////////////////////////////////////////

void MeshWriter::set_fields(const std::vector<Field::Ptr>& fields)
{
  m_fields.resize(0);
  boost_foreach( Field::Ptr field, fields )
    m_fields.push_back(field);
}

////////////////////////////////////////////////////////////////////////////////

MeshWriter::~MeshWriter()
{
}

//////////////////////////////////////////////////////////////////////////////

void MeshWriter::signal_write( SignalArgs& node  )
{
  execute();
}

//////////////////////////////////////////////////////////////////////////////

void MeshWriter::execute()
{
  // Get the mesh
  const Mesh& mesh = access_component(option("mesh").value<URI>()).as_type<Mesh>();

  // Get the file path
  std::string file = option("file").value<URI>().string();

  // Call implementation
  write_from_to(mesh,file);
}

//////////////////////////////////////////////////////////////////////////////


} // mesh
} // cf3
