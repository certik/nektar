///////////////////////////////////////////////////////////////////////////////
//
// File StdExpansion1D.cpp
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
// which are common to 1d expansion. Typically this inolves physiocal
// space operations.
//
///////////////////////////////////////////////////////////////////////////////

#include <StdRegions/StdExpansion1D.h>
#include <LibUtilities/NekMemoryManager.hpp>

namespace Nektar
{
  namespace StdRegions
  {

    StdExpansion1D::StdExpansion1D()
    {
    }
  
    StdExpansion1D::StdExpansion1D(const BasisKey &Ba, int numcoeffs, 
			   double * coeffs,double *phys, bool spaceowner):
      StdExpansion(1,Ba,BasisKey(),BasisKey(),numcoeffs,coeffs,phys,spaceowner)
    {
    }

    StdExpansion1D::StdExpansion1D(const StdExpansion1D &T):StdExpansion(T)
    {
    }

    StdExpansion1D::~StdExpansion1D()
    {
    }

  
    //----------------------------
    // Differentiation Methods
    //-----------------------------
    
    /** \brief Evaluate the derivative \f$ d/d{\xi_1} \f$ at the
	physical quadrature opoints in the expansion (i.e. (this)->_phys)
	and return in \a outarray. 
	
	This is a wrapper function around
	StdSegExp::Tensor_Deriv(inarray,outarray)
	
	Input:\n
	
	- \a (this)->_phys: array of function evaluated at the
        quadrature points
	
	Output: \n
	
	- \a outarray: array of the derivative \f$
        du/d_{\xi_1}|_{\xi_{1i}} \f$
    */
    inline void StdExpansion1D::TensorDeriv(double * outarray)
    {
      TensorDeriv(m_phys,outarray);
    }  

    /** \brief Evaluate the derivative \f$ d/d{\xi_1} \f$ at the
	physical quadrature points given by \a inarray and return in \a
	outarray.
	
	Input:\n
	
	- \a inarray: array of function evaluated at the quadrature
        points
	
	Output: \n
	
	- \a outarray: array of the derivative \f$
        du/d_{\xi_1}|_{\xi_{1i}} \f$
	
    */
    void StdExpansion1D::TensorDeriv(const double *inarray, double * outarray)
    {
      int    nquad = m_base[0]->GetPointsOrder();
      const double *D;
      double *tmp; 
      BstShrDArray  wsp;
      
      if(outarray == inarray)
      {  // check to see if calling array is inarray
	wsp = GetDoubleTmpSpace(nquad); 
	tmp = wsp.get();
	Vmath::Vcopy(nquad,inarray,1,tmp,1);
      }
      else
      {
	tmp = (double *)inarray;
      }

      BasisManagerSingleton::Instance().GetD(m_base[0],D);
      
      Blas::Dgemv('T',nquad,nquad,1.0,D,nquad,tmp,1,0.0,outarray,1);
      
    }
    
    double StdExpansion1D::PhysEvaluate(const double *Lcoord)
    {
      int    nquad = m_base[0]->GetPointsOrder();
      double  val;
      BstShrDArray wsp = GetDoubleTmpSpace(nquad);
      double *tmp = wsp.get();
    
      ASSERTL2(Lcoord[0] < -1,"Lcoord[0] < -1");
      ASSERTL2(Lcoord[0] >  1,"Lcoord[0] >  1");
    
      m_base[0]->GetInterpVec(Lcoord[0],tmp);
      val = Blas::Ddot(m_base[0]->GetPointsOrder(),tmp,1,m_phys,1);
    
      return val;    
    }
    
    void StdExpansion1D::GetCoords1D(double **coords)
    {
      const double *z,*w;

      BasisManagerSingleton::Instance().GetZW(m_base[0],z,w);    
      Blas::Dcopy(m_base[0]->GetPointsOrder(),z,1,coords[0],1);
    }
    
  }//end namespace
}//end namespace

/** 
 * $Log: StdExpansion1D.cpp,v $
 * Revision 1.15  2006/04/01 21:59:27  sherwin
 * Sorted new definition of ASSERT
 *
 * Revision 1.14  2006/03/21 09:21:32  sherwin
 * Introduced NekMemoryManager
 *
 * Revision 1.13  2006/03/13 18:29:35  sherwin
 *
 * Corrected error with definition of GetCoords
 *
 * Revision 1.12  2006/02/27 23:47:23  sherwin
 *
 * Standard coding update upto compilation of StdHexExp.cpp
 *
 * Revision 1.11  2006/02/26 23:37:29  sherwin
 *
 * Updates and compiling checks upto StdExpansions1D
 *
 **/ 

