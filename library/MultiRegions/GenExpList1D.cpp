///////////////////////////////////////////////////////////////////////////////
//
// File GenExpList1D.cpp
//
// For more information, please see: http://www.nektar.info
//
// The MIT License
//
// Copyright (c) 2006 Division of Applied Mathematics, Brown University (USA),
// Department of Aeronautics, Imperial College London (UK), and Scientific
// Computing and Imaging Institute, University of Utah (USA).
//
// License for the specific language governing rights and limitations under
// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included
// in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
// OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
//
// Description: Generalised Expansion list 1D definition
//
///////////////////////////////////////////////////////////////////////////////

#include <MultiRegions/GenExpList1D.h>

namespace Nektar
{
    namespace MultiRegions
    {
    
        GenExpList1D::GenExpList1D():
            ExpList1D()
        {
        }
        
        GenExpList1D::~GenExpList1D()
        {
        }
        
        GenExpList1D::GenExpList1D(const GenExpList1D &In):
            ExpList1D(In)
        {
        }


        GenExpList1D::GenExpList1D(const Array<OneD,const MultiRegions::ExpList1DSharedPtr> &bndConstraint,  
                                   const Array<OneD, const SpatialDomains::BoundaryConditionType>  &bndTypes, 
                                   const StdRegions::StdExpansionVector &locexp, 
                                   SpatialDomains::MeshGraph2D &graph2D)
        {
            int i,j,id, elmtid=0, edgeid;
            Array<OneD, int> EdgeDone(graph2D.GetNseggeoms(),0);
            
            StdRegions::EdgeOrientation      orient;
            LocalRegions::GenSegExpSharedPtr Seg;
            SpatialDomains::ElementEdgeVectorSharedPtr con_elmt;
            SpatialDomains::Geometry2DSharedPtr AdjGeom;
            SpatialDomains::Geometry1DSharedPtr SegGeom;

            // First loop over boundary conditions to renumber
            // Dirichlet boundaries
            for(i = 0; i < bndTypes.num_elements(); ++i)
            {
                if(bndTypes[i] == SpatialDomains::eDirichlet)
                {
                    for(j = 0; j < bndConstraint[i]->GetExpSize(); ++j)
                    {
                        LibUtilities::BasisKey bkey = bndConstraint[i]->GetExp(j)->GetBasis(0)->GetBasisKey();
                        SegGeom  = bndConstraint[i]->GetExp(j)->GetGeom1D();  
                        Seg = MemoryManager<LocalRegions::GenSegExp>::AllocateSharedPtr(bkey, SegGeom);
                        con_elmt = graph2D.GetElementsFromEdge(SegGeom);
                        
                        AdjGeom = (boost::dynamic_pointer_cast<SpatialDomains::Geometry2D>((*con_elmt)[0]->m_Element));
                        edgeid = (*con_elmt)[0]->m_EdgeIndx;
                        
                        orient = AdjGeom->GetEorient(edgeid);

                        // Negate normal sign for this case
                        if(orient == StdRegions::eBackwards)
                        {
                            Seg->SetUpPhysNormals(AdjGeom,edgeid,true);
                        }
                        else
                        {
                            Seg->SetUpPhysNormals(AdjGeom,edgeid,false);
                        }

                        
                        Seg->SetElmtId(elmtid++);
                        (*m_exp).push_back(Seg);   
                        
                        EdgeDone[SegGeom->GetEid()] = 1;
                    }
                }
            }
            
            
            
            // loop over all other edges and fill out other connectivities
            for(i = 0; i < locexp.size(); ++i)
            {
                for(j = 0; j < locexp[i]->GetNedges(); ++j)
                {   
                    const SpatialDomains::Geometry1DSharedPtr& SegGeom = (locexp[i]->GetGeom2D())->GetEdge(j);
                    
                    id = SegGeom->GetEid();
                    
                    if(!EdgeDone[id])
                    {
                        LibUtilities::BasisKey EdgeBkey = locexp[i]->DetEdgeBasisKey(j);
                        
                        Seg = MemoryManager<LocalRegions::GenSegExp>::AllocateSharedPtr(EdgeBkey, SegGeom);
                        
                        // Set up normals at all Segment Quadrature points
                        if(locexp[i]->GetGeom2D()->GetEorient(j) == StdRegions::eForwards)
                        {
                            Seg->SetUpPhysNormals(locexp[i]->GetGeom2D(),j,false);
                        }
                        else
                        {
                            Seg->SetUpPhysNormals(locexp[i]->GetGeom2D(),j,true);
                        }
                        
                        Seg->SetElmtId(elmtid++);
                        (*m_exp).push_back(Seg);
                        EdgeDone[id] = 1;
                    }
                }
            }

            ExpList::SetCoeffPhys();
        }


        

        // Upwind the left and right states given by the Arrays Fwd
        // and Bwd using the vector quantity Vec and ouput the
        // upwinded value in the array upwind
        void GenExpList1D::Upwind(Array<OneD,Array<OneD, const NekDouble> > &Vec, 
                                  Array<OneD, const NekDouble> &Fwd, 
                                  Array<OneD, const NekDouble> &Bwd, 
                                  Array<OneD, NekDouble> &Upwind)
        {
            int i,j,k,e_npoints,offset;
            Array<OneD,NekDouble> normals; 
            NekDouble Vn;

            // Assume whole array is of same coordimate dimention
            int coordim = (*m_exp)[0]->GetGeom1D()->GetCoordim();
            
            ASSERTL1(Vec.num_elements() >= coordim,
              "Input vector does not have sufficient dimensions to match coordim");

            for(i = 0; i < m_exp->size(); ++i)
            {
                e_npoints = (*m_exp)[i]->GetNumPoints(0);
                normals   = (*m_exp)[i]->GetPhysNormals();
                
                offset = m_phys_offset[i];

                for(j = 0; j < e_npoints; ++j)
                {
                    // Calculate normal velocity
                    Vn = 0.0;
                    for(k = 0; k < coordim; ++k)
                    {
                        Vn += Vec[k][offset+j]*normals[k*e_npoints + j];
                    }

                    // Upwind
                    if(Vn > 0.0)
                    {
                        Upwind[offset + j] = Fwd[offset + j];
                    }
                    else
                    {
                        Upwind[offset + j] = Bwd[offset + j];
                    }
                }
            }
            
        }
        
    } //end of namespace
} //end of namespace

/**
* $Log: GenExpList1D.cpp,v $
* Revision 1.1  2008/07/29 22:26:35  sherwin
* Generalised 1D Segment list which includes a normal direction at physical points
*
**/