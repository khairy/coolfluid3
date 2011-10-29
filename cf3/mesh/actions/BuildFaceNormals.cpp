// Copyright (C) 2010-2011 von Karman Institute for Fluid Dynamics, Belgium
//
// This software is distributed under the terms of the
// GNU Lesser General Public License version 3 (LGPLv3).
// See doc/lgpl.txt and doc/gpl.txt for the license text.

#include <boost/foreach.hpp>
#include "common/Log.hpp"
#include "common/Builder.hpp"

#include "common/FindComponents.hpp"
#include "common/Foreach.hpp"
#include "common/StreamHelpers.hpp"
#include "common/StringConversion.hpp"

#include "mesh/actions/BuildFaceNormals.hpp"
#include "mesh/CellFaces.hpp"
#include "mesh/Region.hpp"
#include "mesh/FieldGroup.hpp"
#include "mesh/FaceCellConnectivity.hpp"
#include "mesh/NodeElementConnectivity.hpp"
#include "mesh/Node2FaceCellConnectivity.hpp"
#include "mesh/Cells.hpp"
#include "mesh/Space.hpp"
#include "mesh/Mesh.hpp"
#include "mesh/Faces.hpp"
#include "mesh/CellFaces.hpp"
#include "mesh/Field.hpp"

#include "math/Functions.hpp"

//////////////////////////////////////////////////////////////////////////////

namespace cf3 {
namespace mesh {
namespace actions {

  using namespace common;
  using namespace math::Functions;

////////////////////////////////////////////////////////////////////////////////

common::ComponentBuilder < BuildFaceNormals, MeshTransformer, mesh::actions::LibActions> BuildFaceNormals_Builder;

//////////////////////////////////////////////////////////////////////////////

BuildFaceNormals::BuildFaceNormals( const std::string& name )
: MeshTransformer(name)
{

  properties()["brief"] = std::string("Print information of the mesh");
  std::string desc;
  desc =
  "  Usage: Info \n\n"
  "          Information given: internal mesh hierarchy,\n"
  "      element distribution for each region, and element type";
  properties()["description"] = desc;
}

/////////////////////////////////////////////////////////////////////////////

std::string BuildFaceNormals::brief_description() const
{
  return properties().value<std::string>("brief");
}

/////////////////////////////////////////////////////////////////////////////


std::string BuildFaceNormals::help() const
{
  return "  " + properties().value<std::string>("brief") + "\n" +
      properties().value<std::string>("description");
}

/////////////////////////////////////////////////////////////////////////////

void BuildFaceNormals::execute()
{

  Mesh& mesh = *m_mesh.lock();

  const Uint dimension = mesh.geometry_fields().coordinates().row_size();

  FieldGroup& faces_P0 = mesh.create_space_and_field_group("faces_P0",FieldGroup::Basis::FACE_BASED,"cf3.mesh.LagrangeP0");
  Field& face_normals = faces_P0.create_field(mesh::Tags::normal(),std::string(mesh::Tags::normal())+"[vector]");
  face_normals.add_tag(mesh::Tags::normal());

  Component::Ptr component;
  Uint cell_idx(0);
  boost_foreach( Entities& faces, face_normals.entities_range() )
  {
    FaceCellConnectivity::Ptr face2cell_ptr = find_component_ptr<FaceCellConnectivity>(faces);
    if (is_not_null(face2cell_ptr))
    {
      FaceCellConnectivity& face2cell = *face2cell_ptr;
      common::Table<Uint>& face_nb = face2cell.face_number();
      RealMatrix face_coordinates(faces.element_type().nb_nodes(),faces.element_type().dimension());
      RealVector normal(faces.element_type().dimension());
      for (Uint face=0; face<face2cell.size(); ++face)
      {
        // The normal will be outward to the first connected element
        boost::tie(component,cell_idx) = face2cell.lookup().location(face2cell.connectivity()[face][FIRST]);
        Cells& cells = component->as_type<Cells>();
        Connectivity::ConstRow cell_nodes = cells.node_connectivity()[cell_idx];
        Uint i(0);
        boost_foreach(Uint node_id, cells.element_type().faces().nodes_range(face_nb[face][0]) )
        {
          Uint j(0);
          boost_foreach(const Real& coord, mesh.geometry_fields().coordinates()[cell_nodes[node_id]])
          {
            face_coordinates(i,j) = coord;
            ++j;
          }
          ++i;
        }

        if (faces.element_type().dimensionality() == 0) // cannot compute normal from element_type
        {
          RealVector cell_centroid(1);
          cells.element_type().compute_centroid(cells.get_coordinates(cell_idx),cell_centroid);
          RealVector normal(1);
          normal = face_coordinates.row(0) - cell_centroid;
          normal.normalize();
          face_normals[face_normals.indexes_for_element(faces,face)[0]][XX]=normal[XX];
        }
        else
        {
          faces.element_type().compute_normal(face_coordinates,normal);
          Uint field_index = face_normals.indexes_for_element(faces,face)[0];
          cf3_assert(field_index    < face_normals.size()    );
          cf3_assert(normal.size() == face_normals.row_size());
          for (Uint i=0; i<normal.size(); ++i)
            face_normals[field_index][i]=normal[i];

        }
      }
    }
  }
}

//////////////////////////////////////////////////////////////////////////////


} // actions
} // mesh
} // cf3
