// Copyright (C) 2010 von Karman Institute for Fluid Dynamics, Belgium
//
// This software is distributed under the terms of the
// GNU Lesser General Public License version 3 (LGPLv3).
// See doc/lgpl.txt and doc/gpl.txt for the license text.

#include "Common/ObjectProvider.hpp"
#include "Common/OptionT.hpp"
#include "Actions/CSchemeLDA.hpp"

/////////////////////////////////////////////////////////////////////////////////////

using namespace CF::Common;

namespace CF {
namespace Actions {

///////////////////////////////////////////////////////////////////////////////////////

Common::ObjectProvider < CSchemeLDA, CAction, LibActions, NB_ARGS_1 > CSchemeLDAProvider( "CSchemeLDA" );

///////////////////////////////////////////////////////////////////////////////////////
  
void CSchemeLDA::defineConfigProperties( Common::PropertyList& options )
{
  options.add_option< OptionT<URI> > ("SolutionField","Solution Field for calculation", URI("cpath://"))->mark_basic();
  options.add_option< OptionT<URI> > ("ResidualField","Residual Field updated after calculation", URI("cpath://"))->mark_basic();
}

///////////////////////////////////////////////////////////////////////////////////////

CSchemeLDA::CSchemeLDA ( const CName& name ) : 
  CElementOperation(name)
{
  BUILD_COMPONENT;
  m_property_list["SolutionField"].as_option().attach_trigger ( boost::bind ( &CSchemeLDA::trigger_SolutionField,   this ) );  
  m_property_list["ResidualField"].as_option().attach_trigger ( boost::bind ( &CSchemeLDA::trigger_ResidualField,   this ) );  
}

/////////////////////////////////////////////////////////////////////////////////////

void CSchemeLDA::trigger_SolutionField()
{
  CPath field_path (property("SolutionField").value<URI>());
  CFdebug << "field_path = " << field_path.string() << CFendl;
  m_solution_field = look_component_type<CField>(field_path);
}

void CSchemeLDA::trigger_ResidualField()
{
  CPath field_path (property("ResidualField").value<URI>());
  CFdebug << "field_path = " << field_path.string() << CFendl;
  m_residual_field = look_component_type<CField>(field_path);
}

/////////////////////////////////////////////////////////////////////////////////////

void CSchemeLDA::execute()
{
  BOOST_FOREACH(const Uint node, data->connectivity_table[m_elm_idx])
  {
    data->residual[node][0] = data->solution[node][0];
  }
}

////////////////////////////////////////////////////////////////////////////////////

} // Actions
} // CF

////////////////////////////////////////////////////////////////////////////////////

