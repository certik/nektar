///////////////////////////////////////////////////////////////////////////////
//
// File Expansion.h
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
// Description: Header file for Expansion routines
//
///////////////////////////////////////////////////////////////////////////////

#ifndef EXPANSION_H

#include <StdRegions/StdExpansion1D.h>
#include <LocalRegions/LocalRegions.hpp>
#include <LocalRegions/MatrixKey.h>

namespace Nektar
{
    namespace LocalRegions 
    {
        
        class Expansion
        {
        public:
            
            Expansion(); // default constructor. 


            protected:
            
            DNekMatSharedPtr GenMatrix(const StdRegions::StdMatrixKey &mkey);
                        
            virtual void v_AddHDGHelmholtzMatrixBoundaryTerms(const NekDouble tau, 
                                                        const Array<OneD,
                                                        const NekDouble> &inarray,
                                                        Array<OneD,NekDouble> &outarray)
            {
                NEKERROR(ErrorUtil::efatal, "This method is not valid in this class");
            }

            virtual void v_AddHDGHelmholtzTraceTerms(const NekDouble tau, 
                                                     const Array<OneD,const NekDouble> &inarray,  Array<OneD,NekDouble> &outarray)
            {
                NEKERROR(ErrorUtil::efatal, "This method is not valid in this class");
            }


            virtual void v_AddNormTraceInt(const int dir,
                                 Array<OneD, const NekDouble> &inarray,
                                 Array<OneD,NekDouble> &outarray)
            {
                NEKERROR(ErrorUtil::efatal, "This method is not valid in this class");
            }


            virtual void v_SetTraceToGeomOrientation(Array<OneD, NekDouble> &inout)
            {
                NEKERROR(ErrorUtil::efatal, "This method is not valid in this class");
            }


            virtual const LibUtilities::BasisSharedPtr& v_GetBasis(int dir) const
            {
                NEKERROR(ErrorUtil::efatal, "This method is not valid in this class");
                return LibUtilities::NullBasisSharedPtr; 
            }

            virtual int v_GetNcoeffs(void) const 
            {
                NEKERROR(ErrorUtil::efatal, "This method is not valid in this class");
                return -1;
            }

            virtual int v_GetNumPoints(const int dir) const 
            {
                NEKERROR(ErrorUtil::efatal, "This method is not valid in this class");
                return -1;
            }

            virtual int v_NumBndryCoeffs() const 
            {
                ASSERTL0(false, "This function is needs defining for this shape");
                return 0;
            }
            
            virtual int v_NumDGBndryCoeffs() const 
            {
                ASSERTL0(false, "This function is needs defining for this shape");
                return 0;
            }
            
            virtual const  SpatialDomains::GeomFactorsSharedPtr &v_GetMetricInfo() const 
            {
                NEKERROR(ErrorUtil::efatal, "This function is only valid for LocalRegions");
                return SpatialDomains::NullGeomFactorsSharedPtr;
            }

            virtual bool  v_IsBoundaryInteriorExpansion() 
            {
                ASSERTL0(false,"This function has not been defined for this expansion");
                return false;
            }


            virtual DNekScalMatSharedPtr& v_GetLocMatrix(const LocalRegions::MatrixKey &mkey)
            {
                NEKERROR(ErrorUtil::efatal, "This function is only valid for LocalRegions");
                return NullDNekScalMatSharedPtr;
            }

            virtual DNekScalMatSharedPtr& v_GetLocMatrix(const StdRegions::MatrixType mtype, NekDouble lambdaval = NekUnsetDouble, NekDouble tau = NekUnsetDouble)
            {
                NEKERROR(ErrorUtil::efatal, "This function is only valid for LocalRegions");
                return NullDNekScalMatSharedPtr;
            }

            virtual void   v_BwdTrans   (const Array<OneD, const NekDouble>& inarray, 
                                         Array<OneD, NekDouble> &outarray) 
            {
                NEKERROR(ErrorUtil::efatal, "This function is not valid in this class ");
            }
            
            virtual void   v_PhysDeriv (const Array<OneD, const NekDouble>& inarray,
                                        Array<OneD, NekDouble> &out_d0,
                                        Array<OneD, NekDouble> &out_d1 = NullNekDouble1DArray,
                                        Array<OneD, NekDouble> &out_d2 = NullNekDouble1DArray)
            {
                NEKERROR(ErrorUtil::efatal, "This function is not valid in this class ");
            }


            private:
            // Do not add members here since it may lead to conflicts.
            // Only use this class for member functions
        };
        
        // type defines for use of PrismExp in a boost vector
        typedef boost::shared_ptr<Expansion> ExpansionSharedPtr;
        typedef std::vector< ExpansionSharedPtr > ExpansionVector;
        typedef std::vector< ExpansionSharedPtr >::iterator ExpansionVectorIter;
        
    } //end of namespace
} //end of namespace

#define EXPANSION_H
#endif

/** 
 *    $Log: Expansion.h,v $
 *
 **/