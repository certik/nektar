///////////////////////////////////////////////////////////////////////////////
//
// File ExpList1D.cpp
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
// Description: Expansion list 1D definition
//
///////////////////////////////////////////////////////////////////////////////

#include <MultiRegions/ExpList1D.h>

namespace Nektar
{
    namespace MultiRegions
    {
    
        ExpList1D::ExpList1D()
        {
        }
        
        ExpList1D::~ExpList1D()
        {
        }
        
        ExpList1D::ExpList1D(const ExpList1D &In):
            ExpList(In)
        {
            m_coeffs = Array<OneD, NekDouble>(m_ncoeffs);
            m_phys   = Array<OneD, NekDouble>(m_npoints);
        }

        ExpList1D::ExpList1D(const LibUtilities::BasisKey &Ba,
                             const SpatialDomains::MeshGraph1D &graph1D)
        {
            int i,j;
            int nel;
            LocalRegions::SegExpSharedPtr seg;
            SpatialDomains::Composite comp;
            
            m_ncoeffs = 0;
            m_npoints = 0;
            
            m_transState = eNotSet; 
            m_physState  = false;

            const SpatialDomains::ExpansionVector &expansions = graph1D.GetExpansions();
            
            for(i = 0; i < expansions.size(); ++i)
            {
                SpatialDomains::SegGeomSharedPtr SegmentGeom;
                
                if(SegmentGeom = boost::dynamic_pointer_cast<SpatialDomains::SegGeom>(expansions[i]->m_GeomShPtr))
                {
                    seg = MemoryManager<LocalRegions::SegExp>::AllocateSharedPtr(Ba,SegmentGeom);
                    (*m_exp).push_back(seg);
                }
                else
                {
                    ASSERTL0(false,"dynamic cast to a SegGeom failed");
                }  
            
                m_ncoeffs += Ba.GetNumModes();
                m_npoints += Ba.GetNumPoints();
            } 
            
            m_coeffs = Array<OneD, NekDouble>(m_ncoeffs);
            m_phys   = Array<OneD, NekDouble>(m_npoints);            
        }

        ExpList1D::ExpList1D(SpatialDomains::MeshGraph1D &graph1D)
        {
            int i;
            int nel;
            LocalRegions::SegExpSharedPtr seg;
            SpatialDomains::Composite comp;
            
            m_ncoeffs = 0;
            m_npoints = 0;
            
            m_transState = eNotSet; 
            m_physState  = false;

            const SpatialDomains::ExpansionVector &expansions = graph1D.GetExpansions();
            
            for(i = 0; i < expansions.size(); ++i)
            {
                SpatialDomains::SegGeomSharedPtr SegmentGeom;
                LibUtilities::BasisKey bkey = graph1D.GetBasisKey(expansions[i],0);

                if(SegmentGeom = boost::dynamic_pointer_cast<SpatialDomains::SegGeom>(expansions[i]->m_GeomShPtr))
                {
                    seg = MemoryManager<LocalRegions::SegExp>::AllocateSharedPtr(bkey, SegmentGeom);
                    (*m_exp).push_back(seg);
                }
                else
                {
                    ASSERTL0(false,"dynamic cast to a SegGeom failed");
                }  

                m_ncoeffs += bkey.GetNumModes();
                m_npoints += bkey.GetNumPoints();
            }
      
            m_coeffs = Array<OneD, NekDouble>(m_ncoeffs);
            m_phys   = Array<OneD, NekDouble>(m_npoints);
            
        }

        ExpList1D::ExpList1D(const SpatialDomains::CompositeVector &domain, SpatialDomains::MeshGraph2D &graph2D)
        {
            int i,j;
            int nel;
            int edgeid;
            SpatialDomains::Composite comp;
            SpatialDomains::EdgeComponentSharedPtr edgeComp;
            SpatialDomains::VertexComponentSharedPtr vert1;
            SpatialDomains::VertexComponentSharedPtr vert2;
            SpatialDomains::SegGeomSharedPtr SegmentGeom;
            LocalRegions::SegExpSharedPtr seg;
            
            m_ncoeffs = 0;
            m_npoints = 0;
            
            m_transState = eNotSet; 
            m_physState  = false;
            
            for(i = 0; i < domain.size(); ++i)
            {
                comp = domain[i];
                // This line should be removed
                // are we sure that flag should be set to zero (for triangles for example?)
                LibUtilities::BasisKey bkey = graph2D.GetBasisKey((graph2D.GetExpansions())[0],0);
                
                for(j = 0; j < comp->size(); ++j)
                {                    
                    if(edgeComp = boost::dynamic_pointer_cast<SpatialDomains::EdgeComponent>((*comp)[j]))
                    {
//                                 edgeExp = graph2D.GetExpansion(edgeComp);
//                                 // are we sure that flag should be set to zero (for triangles for example?)                        
//                                 LibUtilities::BasisKey bkey = graph2D.GetBasisKey(edgeExp,0);

                        edgeid = edgeComp->GetEid();
                        vert1 = edgeComp->GetVertex(0);
                        vert2 = edgeComp->GetVertex(1);
                        SegmentGeom = MemoryManager<SpatialDomains::SegGeom>::AllocateSharedPtr(edgeid,vert1,vert2);
                        seg = MemoryManager<LocalRegions::SegExp>::AllocateSharedPtr(bkey, SegmentGeom);
                        (*m_exp).push_back(seg);   
                    }
                    else
                    {
                        ASSERTL0(false,"dynamic cast to a EdgeComp failed");
                    }  
                }
                
                m_ncoeffs += (comp->size())*bkey.GetNumModes();
                m_npoints += (comp->size())*bkey.GetNumPoints();
            } 
            
            m_coeffs = Array<OneD, NekDouble>(m_ncoeffs);
            m_phys   = Array<OneD, NekDouble>(m_npoints);
            
        }

    } //end of namespace
} //end of namespace

/**
* $Log: ExpList1D.cpp,v $
* Revision 1.21  2007/11/07 20:29:53  jfrazier
* Modified to use new expansion list contained in meshgraph.
*
* Revision 1.20  2007/09/25 14:25:29  pvos
* Update for helmholtz1D with different expansion orders
*
* Revision 1.19  2007/07/22 23:04:20  bnelson
* Backed out Nektar::ptr.
*
* Revision 1.18  2007/07/20 02:04:12  bnelson
* Replaced boost::shared_ptr with Nektar::ptr
*
* Revision 1.17  2007/07/13 16:48:47  pvos
* Another HelmHoltz update (homogeneous dir BC multi-elemental solver does work)
*
* Revision 1.16  2007/07/10 08:54:29  pvos
* Updated ContField1D constructor
*
* Revision 1.15  2007/07/06 18:39:34  pvos
* ContField1D constructor updates
*
* Revision 1.14  2007/06/05 16:36:55  pvos
* Updated Explist2D ContExpList2D and corresponding demo-codes
*
**/
