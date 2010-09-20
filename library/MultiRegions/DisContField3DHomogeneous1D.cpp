//////////////////////////////////////////////////////////////////////////////
//
// File DisContField3DHomogeneous1D.cpp
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
// Description: Field definition for 3D domain with boundary
// conditions using LDG flux and a 1D homogeneous direction
//
///////////////////////////////////////////////////////////////////////////////

#include <MultiRegions/ExpList2DHomogeneous1D.h>
#include <MultiRegions/DisContField3DHomogeneous1D.h>

namespace Nektar
{
    namespace MultiRegions
    {

        DisContField3DHomogeneous1D::DisContField3DHomogeneous1D(void):
            ExpList3DHomogeneous1D(),
            m_bndCondExpansions(),
            m_bndConditions()
        {
        }

        DisContField3DHomogeneous1D::DisContField3DHomogeneous1D(const LibUtilities::BasisKey &HomoBasis,
                                                                 const NekDouble lhom):
            ExpList3DHomogeneous1D(HomoBasis,lhom),
            m_bndCondExpansions(),
            m_bndConditions()
        {
        }

        DisContField3DHomogeneous1D::DisContField3DHomogeneous1D(const DisContField3DHomogeneous1D &In, bool DeclarePlanesSetCoeffPhys):
            ExpList3DHomogeneous1D (In,false),
            m_bndCondExpansions    (In.m_bndCondExpansions),
            m_bndConditions        (In.m_bndConditions)
        {
            if(DeclarePlanesSetCoeffPhys)
            {
                bool False = false;
                DisContField2DSharedPtr zero_plane = boost::dynamic_pointer_cast<DisContField2D> (In.m_planes[0]);
                
                for(int n = 0; n < m_planes.num_elements(); ++n)
                {
                    m_planes[n] = MemoryManager<DisContField2D>::AllocateSharedPtr(*zero_plane,False);
                }
                
                SetCoeffPhys();
            }
        }

        DisContField3DHomogeneous1D::DisContField3DHomogeneous1D(
                                       const LibUtilities::BasisKey &HomoBasis,
                                       const NekDouble lhom,
                                       SpatialDomains::MeshGraph2D &graph2D,
                                       SpatialDomains::BoundaryConditions &bcs,
                                       const int bc_loc,
                                       const GlobalSysSolnType solnType):  
            ExpList3DHomogeneous1D(HomoBasis,lhom),
            m_bndCondExpansions(),
            m_bndConditions()
        {
            int i,j,n,nel;
            bool True  = true; 
            bool False = false; 
            DisContField2DSharedPtr plane_zero;

            // note that nzplanes can be larger than nzmodes 
            m_planes[0] = plane_zero = MemoryManager<DisContField2D>::AllocateSharedPtr(graph2D,bcs,bc_loc,solnType,True,False); 

            m_exp = MemoryManager<StdRegions::StdExpansionVector>::AllocateSharedPtr();
            nel = m_planes[0]->GetExpSize();

            for(j = 0; j < nel; ++j)
            {
                (*m_exp).push_back(m_planes[0]->GetExp(j));
            }

            for(n = 1; n < m_homogeneousBasis->GetNumPoints(); ++n)
            {
                m_planes[n] = MemoryManager<DisContField2D>::AllocateSharedPtr(*plane_zero,graph2D,bcs,bc_loc,True,False); 
                for(j = 0; j < nel; ++j)
                {
                    (*m_exp).push_back((*m_exp)[j]);
                }
            }            

            // Setup Default optimisation information. 
            nel = GetExpSize();

            m_globalOptParam = MemoryManager<NekOptimize::GlobalOptParam>
                ::AllocateSharedPtr(nel);
            
            SetCoeffPhys();

            SetupBoundaryConditions(HomoBasis,lhom,bcs);
        }

        DisContField3DHomogeneous1D::~DisContField3DHomogeneous1D()
        {
        }


        void DisContField3DHomogeneous1D::SetupBoundaryConditions(const LibUtilities::BasisKey &HomoBasis, const NekDouble lhom, SpatialDomains::BoundaryConditions &bcs)
        {

            int i,j,n;
            // Setup an ExpList2DHomogeneous1D expansion for boundary
            // conditions and link to class declared in m_planes.
            SpatialDomains::BoundaryRegionCollection  &bregions = bcs.GetBoundaryRegions();
            int nbnd = bregions.size();
            
            m_bndCondExpansions  = Array<OneD,MultiRegions::ExpList2DHomogeneous1DSharedPtr>(nbnd);
            
            m_bndConditions = m_planes[0]->UpdateBndConditions();
            
            boost::shared_ptr<StdRegions::StdExpansionVector> exp = MemoryManager<StdRegions::StdExpansionVector>::AllocateSharedPtr();
            
            int nplanes = m_planes.num_elements();
            Array<OneD, MultiRegions::ExpList1DSharedPtr> PlanesBndCondExp(nplanes);
            
            for(i = 0; i < nbnd; ++i)
            {
                for(n = 0; n < nplanes; ++n)
                {
                    PlanesBndCondExp[n] = m_planes[n]->UpdateBndCondExpansion(i);

                    for(j = 0; j < PlanesBndCondExp[n]->GetExpSize(); ++j)
                    {
                        (*exp).push_back(PlanesBndCondExp[n]->GetExp(j));
                    }
                }
                
                m_bndCondExpansions[i] = MemoryManager<ExpList2DHomogeneous1D>::AllocateSharedPtr(HomoBasis,lhom,exp,PlanesBndCondExp);
                
            }
            
            EvaluateBoundaryConditions();
        }

        void DisContField3DHomogeneous1D::EvaluateBoundaryConditions(const NekDouble time)
        {
            int n;
            const Array<OneD, const NekDouble> z = m_homogeneousBasis->GetZ();

            for(n = 0; n < m_planes.num_elements(); ++n)
            {
                m_planes[n]->EvaluateBoundaryConditions(time,0.5*m_lhom*(1.0+z[n]));
            }
            
            // Fourier transform coefficient space boundary values
            for(n = 0; n < m_bndCondExpansions.num_elements(); ++n)
            {
                m_bndCondExpansions[n]->Homogeneous1DFwdTrans(m_bndCondExpansions[n]->GetCoeffs(),m_bndCondExpansions[n]->UpdateCoeffs());
            }    
        }
        
        void DisContField3DHomogeneous1D::v_HelmSolve(
                    const Array<OneD, const NekDouble> &inarray,
                    Array<OneD,       NekDouble> &outarray,
                    NekDouble lambda,
                    const Array<OneD, const NekDouble> &varLambda,
                    const Array<OneD, const Array<OneD, NekDouble> > &varCoeff)
        {
            v_HelmSolveDG(inarray, outarray, lambda, varLambda, varCoeff, 1);
        }

        void DisContField3DHomogeneous1D::v_HelmSolveDG(
                    const Array<OneD, const NekDouble> &inarray,
                    Array<OneD,       NekDouble> &outarray,
                    NekDouble lambda,
                    const Array<OneD, const NekDouble> &varLambda,
                    const Array<OneD, const Array<OneD, NekDouble> > &varCoeff,
                    NekDouble tau)
        {
            int n;
            int cnt = 0;
            int cnt1 = 0;
            int nhom_modes = m_homogeneousBasis->GetNumModes();
            NekDouble beta; 
            Array<OneD, NekDouble> e_out;
            Array<OneD, NekDouble> fce(inarray.num_elements());

            // Fourier transform forcing function
            Homogeneous1DFwdTrans(inarray,fce);

            for(n = 0; n < nhom_modes; ++n)
            {
                beta = 2*M_PI*(n/2)/m_lhom;
                m_planes[n]->HelmSolve(fce + cnt,
                                         e_out = outarray + cnt1,
                                         lambda + beta*beta,varLambda,
                                         varCoeff);
                
                cnt  += m_planes[n]->GetTotPoints();
                cnt1 += m_planes[n]->GetNcoeffs();
            }
        }


    } // end of namespace
} //end of namespace