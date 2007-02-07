///////////////////////////////////////////////////////////////////////////////
//
// File StdExpansion1D.h
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
// Description: Daughter of StdExpansion. This class contains routine
// which are common to 1d expansion. Typically this inolves physical
// space operations.
//
///////////////////////////////////////////////////////////////////////////////


#ifndef STDEXP1D_H
#define STDEXP1D_H

#include <StdRegions/StdExpansion.h>

namespace Nektar
{
    namespace StdRegions
    {

        class StdExpansion1D: public StdExpansion
        {
	    
        public:

            StdExpansion1D();
            StdExpansion1D(const LibUtilities::BasisKey &Ba, int numcoeffs);
            StdExpansion1D(const StdExpansion1D &T);
            ~StdExpansion1D();


	    /** \brief Evaluate the derivative \f$ d/d{\xi_1} \f$ at the physical
	     *  quadrature points in the expansion (i.e. \a (this)->m_phys)
	     *  and return in \a outarray. 
	     *
	     *  This is a wrapper function around
	     *  StdSegExp::Tensor_Deriv(inarray,outarray)
	     *
	     *	This function takes the physical value space array \a m_phys as
	     *  discrete function to be evaluated
	     *
	     *  \param outarray the resulting array of the derivative \f$
	     *  du/d_{\xi_1}|_{\xi_{1i}} \f$ will be stored in the array
	     *  \a outarray as output of the function
	     */
            void PhysTensorDeriv(double * outarray);
	    
	    /** \brief Evaluate the derivative \f$ d/d{\xi_1} \f$ at the
	     *  physical quadrature points given by \a inarray and return in
	     *  \a outarray.
	     *
	     *  \param inarray array of a function evaluated at the quadrature
	     *  points
	     *  \param outarray the resulting array of the derivative \f$
	     *  du/d_{\xi_1}|_{\xi_{1i}} \f$ will be stored in the array
	     *  \a outarray as output of the function
	     */
            void PhysTensorDeriv(const double *inarray, double * outarray);

            void PhysDeriv  (double *outarray) 
            {
                v_PhysDeriv  (outarray);
            }

            void StdPhysDeriv (double *outarray)
            {
                v_StdPhysDeriv (outarray) ;
            }

            void PhysDeriv  (const double *inarray, double *outarray) 
            {
                v_PhysDeriv (inarray, outarray);
            }

            void StdPhysDeriv (const double *inarray, double *outarray)
            {
                v_StdPhysDeriv (inarray,outarray);
            }
	    
            /** \brief Evaluate a function at points coords which is assumed
	     *  to be in local collapsed coordinate format. The function is
	     *  assumed to be in physical space
	     */
            double PhysEvaluate(const double *coords);

        protected:

        private:

            // Virtual Functions ----------------------------------------

	    virtual int v_GetNverts() = 0;
	    virtual int v_GetNedges()
	    {
		ASSERTL0(false,"This function is only valid for 2 and 3D expansions");
		return 0;
	    }
	    virtual int v_GetNfaces()
	    {
		ASSERTL0(false,"This function is only valid for 2 and 3D expansions");
		return 0;
	    }

            virtual ShapeType v_DetShapeType()                = 0;

            virtual int v_get_nodalpoints(const double *x, const double *y)
            {
                ASSERTL0(false, "This function is only valid for nodal expansions");
                return 0;
            }

            virtual void v_GenNBasisTransMatrix(double * outarray)
            {
                ASSERTL0(false, "This function is only valid for nodal expansions");
            }

            virtual StdMatContainer *v_GetNBasisTransMatrix()
            {
                ASSERTL0(false, "This function is only valid for nodal expansions");
                return NULL;
            }


	    virtual int v_GetCoordim(void)
	    {
                return 1; 
	    }

            virtual void   v_BwdTrans (double *outarray)      = 0;
            virtual void   v_FwdTrans (const double *inarray) = 0;

            virtual double v_Integral(const double *inarray ) = 0;
            virtual double v_Evaluate(const double * coords) = 0;

            virtual void   v_PhysDeriv    (double *outarray) = 0;
            virtual void   v_StdPhysDeriv (double *outarray) = 0;
            virtual void   v_PhysDeriv    (const double *inarray, double *outarray) = 0;
            virtual void   v_StdPhysDeriv (const double *inarray, double *outarray) = 0;

        };

    } //end of namespace
} //end of namespace

#endif //STDEXP1D_H

/**
* $Log: StdExpansion1D.h,v $
* Revision 1.7  2007/01/28 18:34:23  sherwin
* More modifications to make Demo Project1D compile
*
* Revision 1.6  2007/01/21 02:28:07  sherwin
* Compiling under new revision
*
* Revision 1.5  2007/01/20 22:35:21  sherwin
* Version with StdExpansion compiling
*
* Revision 1.4  2007/01/15 11:30:22  pvos
* Updating doxygen documentation
*
* Revision 1.3  2006/07/02 17:16:18  sherwin
*
* Modifications to make MultiRegions work for a connected domain in 2D (Tris)
*
* Revision 1.2  2006/06/13 18:05:02  sherwin
* Modifications to make MultiRegions demo ProjectLoc2D execute properly.
*
* Revision 1.1  2006/05/04 18:58:31  kirby
* *** empty log message ***
*
* Revision 1.17  2006/05/02 21:21:12  sherwin
* Corrected libraries to compile new version of spatialdomains and demo Graph1D
*
* Revision 1.16  2006/04/25 20:23:33  jfrazier
* Various fixes to correct bugs, calls to ASSERT, etc.
*
* Revision 1.15  2006/03/13 18:29:35  sherwin
*
* Corrected error with definition of GetCoords
*
* Revision 1.14  2006/03/05 22:11:02  sherwin
*
* Sorted out Project1D, Project2D and Project_Diff2D as well as some test scripts
*
* Revision 1.13  2006/03/04 20:26:54  bnelson
* Added comments after #endif.
*
* Revision 1.12  2006/03/01 08:25:03  sherwin
*
* First compiling version of StdRegions
*
* Revision 1.11  2006/02/27 23:47:23  sherwin
*
* Standard coding update upto compilation of StdHexExp.cpp
*
* Revision 1.10  2006/02/26 23:37:29  sherwin
*
* Updates and compiling checks upto StdExpansions1D
*
**/



