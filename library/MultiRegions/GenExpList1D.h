///////////////////////////////////////////////////////////////////////////////
//
// File GenExpList1D.h
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

#ifndef GENEXPLIST1D_H
#define GENEXPLIST1D_H

#include <LocalRegions/GenSegExp.h>
#include <MultiRegions/ExpList1D.h>

namespace Nektar
{
    namespace MultiRegions
    {     

        class GenExpList1D: 
        public ExpList1D
        {
        public:

            /**
             * \brief The default constructor.  
             */  
            GenExpList1D();
            
            GenExpList1D(const SpatialDomains::CompositeVector &domain, SpatialDomains::MeshGraph2D &graph2D);

            // constructor for trace space in connection with DisContField2D.cpp
            GenExpList1D(const Array<OneD,const MultiRegions::ExpList1DSharedPtr> &bndConstraint,  
                         const Array<OneD, const SpatialDomains::BoundaryConditionShPtr>  &bndCond, 
                         const StdRegions::StdExpansionVector &locexp, 
                         SpatialDomains::MeshGraph2D &graph2D,
                         const map<int,int> &periodicEdges);
            
            /**
             * \brief The copy constructor.
             */  
            GenExpList1D(const GenExpList1D &In);   
                        
            /**
             * \brief The default destructor.
             */  
            ~GenExpList1D();
            
            void SetUpPhysNormals(const StdRegions::StdExpansionVector &locexp);

            // direction =  1: Upwind
            // direction = -1: Downwind

            void Upwind(const Array<OneD, const Array<OneD, NekDouble> > &Vec,
                        const Array<OneD, const NekDouble> &Fwd, 
                        const Array<OneD, const NekDouble> &Bwd, 
                        Array<OneD, NekDouble> &Upwind,
                        int direction=1);

            void Upwind(const Array<OneD, const NekDouble> &Vn, 
                        const Array<OneD, const NekDouble> &Fwd, 
                        const Array<OneD, const NekDouble> &Bwd, 
                        Array<OneD, NekDouble> &Upwind,
                        int direction=1);
            
	    void GetNormals(Array<OneD, Array<OneD, NekDouble> > &normals); 
            
        protected:

        private:
            virtual void v_SetUpPhysNormals(const StdRegions::StdExpansionVector &locexp)
            {
                SetUpPhysNormals(locexp);
            }


        };

        typedef boost::shared_ptr<GenExpList1D>     GenExpList1DSharedPtr;
        typedef std::vector<GenExpList1DSharedPtr>   GenExpList1DVector;
        typedef std::vector<GenExpList1DSharedPtr>::iterator GenExpList1DVectorIter;

    } //end of namespace
} //end of namespace

#endif//GENEXPLIST1D_H

/**
 * $Log: GenExpList1D.h,v $
 * Revision 1.9  2009/09/06 22:28:45  sherwin
 * Updates for Navier-Stokes solver
 *
 * Revision 1.8  2009/07/09 09:01:49  sehunchun
 * Upwind function is modified in a faster form
 *
 * Revision 1.7  2009/02/28 21:28:40  sehunchun
 *  Now upwind has "forward" direction and "backward" direction. Default is forward and no changes are necessary for previous file.
 *
 * Revision 1.6  2008/10/29 22:46:35  sherwin
 * Updates for const correctness and a few other bits
 *
 * Revision 1.5  2008/10/09 21:47:14  ehan
 * Fixed error from the function Upwind().
 *
 * Revision 1.4  2008/10/04 19:54:28  sherwin
 * Added upwind method using only the normal flux
 *
 * Revision 1.3  2008/08/22 09:42:32  pvos
 * Updates for Claes' Shallow Water and Euler solver
 *
 * Revision 1.2  2008/08/14 22:15:51  sherwin
 * Added LocalToglobalMap and DGMap and depracted LocalToGlobalBndryMap1D,2D. Made DisContField classes compatible with updated ContField formats
 *
 * Revision 1.1  2008/07/29 22:26:35  sherwin
 * Generalised 1D Segment list which includes a normal direction at physical points
 *
 **/
