///////////////////////////////////////////////////////////////////////////////
//
// File SegExp.h
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
// Description: Header file for SegExp routines
//
///////////////////////////////////////////////////////////////////////////////

#ifndef SEGEXP_H
#define SEGEXP_H

#include <LocalRegions/LocalRegions.hpp>

#include <StdRegions/StdSegExp.h>
#include <SpatialDomains/SegGeom.h>

#include <SpatialDomains/GeomFactors.h>

#include <LocalRegions/MatrixKey.h>

#include <fstream>

namespace Nektar
{
    namespace LocalRegions 
    {
    
    class SegExp: public StdRegions::StdSegExp
        {

        public:

            /// \brief Constructor using BasisKey class for quadrature
            /// points and order definition 
            SegExp(const LibUtilities::BasisKey &Ba, 
                   const SpatialDomains::Geometry1DSharedPtr &geom);


            /// \brief Constructor using BasisKey class for quadrature
            /// points and order definition where it has standard geometric factors 
            SegExp(const LibUtilities::BasisKey &Ba);

            ///Copy Constructor
            SegExp(const SegExp &S);

            ///Destructor
            ~SegExp();

            /// Return Shape of region, using  ShapeType enum list. i.e. Segment  
            StdRegions::ShapeType DetShapeType() const
            { 
                return StdRegions::eSegment;
            }    

            void GetCoords(Array<OneD,NekDouble> &coords_1,
               Array<OneD,NekDouble> &coords_2 = NullNekDouble1DArray,
               Array<OneD,NekDouble> &coords_3 = NullNekDouble1DArray);
            void GetCoord(const Array<OneD, const NekDouble>& Lcoords, 
              Array<OneD,NekDouble> &coords);


            const  SpatialDomains::GeomFactorsSharedPtr GetMetricInfo() const
            {
                return m_metricinfo;
            }

            const SpatialDomains::Geometry1DSharedPtr& GetGeom() const
            {
                return m_geom;
            }

            void WriteToFile(std::ofstream &outfile, OutputFormat format, const bool dumpVar = true);


            //----------------------------
            // Integration Methods
            //----------------------------

            /// \brief Integrate the physical point list \a inarray over region
            NekDouble Integral(const Array<OneD, const NekDouble>& inarray);


            /// \brief  Inner product of \a inarray over region with respect to the 
            /// expansion basis (this)->_Base[0] and return in \a outarray 
            void IProductWRTBase(const Array<OneD, const NekDouble>& inarray, 
                 Array<OneD,NekDouble> &outarray);


            //-----------------------------
            // Differentiation Methods
            //-----------------------------


            /** \brief Evaluate the derivative \f$ d/d{\xi_1} \f$ at the
            physical quadrature points given by \a inarray and return in \a
            outarray. */
            void PhysDeriv(const Array<OneD, const NekDouble>& inarray, 
               Array<OneD,NekDouble> &out_d0,
               Array<OneD,NekDouble> &out_d1 = NullNekDouble1DArray,
               Array<OneD,NekDouble> &out_d2 = NullNekDouble1DArray);

            //----------------------------
            // Evaluations Methods
            //---------------------------

            void SetCoeffsToOrientation(StdRegions::EdgeOrientation dir);

            void ReverseCoeffsAndSign(const Array<OneD,NekDouble> &inarray,
                                          Array<OneD,NekDouble> &outarray);
        
            /** \brief Forward transform from physical quadrature space
            stored in \a inarray and evaluate the expansion coefficients and
            store in \a (this)->_coeffs  */
            void FwdTrans(const Array<OneD, const NekDouble>& inarray, 
              Array<OneD,NekDouble> &outarray);

            void FwdTrans_BndConstrained(const Array<OneD, const NekDouble>& inarray, 
                                         Array<OneD, NekDouble> &outarray);

            NekDouble PhysEvaluate(const Array<OneD, const NekDouble>& coord);
            
            void AddUDGHelmholtzBoundaryTerms(const NekDouble tau, 
                                              const Array<OneD, const NekDouble> &inarray,
                                              Array<OneD,NekDouble> &outarray,
                                              bool MatrixTerms = true);

            void AddUDGHelmholtzTraceTerms(const NekDouble tau, 
                                           const Array<OneD, const NekDouble> &inarray,
                                           Array<OneD,NekDouble> &outarray)
            {
                AddUDGHelmholtzBoundaryTerms(tau,inarray,outarray,false);
            }

            
            void AddNormBoundaryInt(const int dir,
                                    Array<OneD, const NekDouble> &inarray,
                                    Array<OneD,NekDouble> &outarray);
            
            void AddNormTraceInt(const int dir,
                             Array<OneD, const NekDouble> &inarray,
                             Array<OneD,NekDouble> &outarray)
            {
                AddNormBoundaryInt(dir,inarray,outarray);
            }
            
        protected:


            void GenMetricInfo();    
            
            DNekMatSharedPtr GenMatrix(const StdRegions::StdMatrixKey &mkey);
            DNekMatSharedPtr CreateStdMatrix(const StdRegions::StdMatrixKey &mkey);
            DNekScalMatSharedPtr  CreateMatrix(const MatrixKey &mkey);
            DNekScalBlkMatSharedPtr  CreateStaticCondMatrix(const MatrixKey &mkey);

            SpatialDomains::Geometry1DSharedPtr m_geom;
            SpatialDomains::GeomFactorsSharedPtr  m_metricinfo;

            LibUtilities::NekManager<MatrixKey, DNekScalMat, MatrixKey::opLess> m_matrixManager;
            LibUtilities::NekManager<MatrixKey, DNekScalBlkMat, MatrixKey::opLess> m_staticCondMatrixManager;

            /// \brief  Inner product of \a inarray over region with respect to
            /// the expansion basis \a base and return in \a outarray 
            inline void IProductWRTBase(const Array<OneD, const NekDouble>& base, 
                    const Array<OneD, const NekDouble>& inarray, 
                    Array<OneD,NekDouble> &outarray, 
                    const int coll_check);

        private:

            SegExp();

            virtual StdRegions::ShapeType v_DetShapeType() const
            {
                return DetShapeType();
            }

            virtual SpatialDomains::GeomFactorsSharedPtr v_GetMetricInfo() const
            {
                return GetMetricInfo();
            }

            virtual const  SpatialDomains::Geometry1DSharedPtr& v_GetGeom()
            {
                return GetGeom();
            }

            virtual void v_GetCoords(Array<OneD,NekDouble> &coords_0,
                     Array<OneD,NekDouble> &coords_1 = NullNekDouble1DArray,
                     Array<OneD,NekDouble> &coords_2 = NullNekDouble1DArray)
        {
                GetCoords(coords_0, coords_1, coords_2);
            }

            virtual void v_GetCoord(const Array<OneD, const NekDouble>& lcoord, 
                     Array<OneD,NekDouble> &coord)
            {
                GetCoord(lcoord, coord);
            }

            virtual int v_GetCoordim()
            {
                return m_geom->GetCoordim();
            }

            virtual void v_WriteToFile(std::ofstream &outfile, OutputFormat format, const bool dumpVar = true)
            {
                WriteToFile(outfile,format,dumpVar);
            }

            virtual SpatialDomains::GeomType v_MetricInfoType()
            {
                return m_metricinfo->GetGtype();
            }

            /// \brief Virtual call to integrate the physical point list \a inarray
            /// over region (see SegExp::Integral) 
            virtual NekDouble v_Integral(const Array<OneD, const NekDouble>& inarray )
            {
                return Integral(inarray);
            }

            /** \brief Virtual call to SegExp::IProduct_WRT_B */

            virtual void v_IProductWRTBase(const Array<OneD, const NekDouble>& inarray,
                       Array<OneD,NekDouble> &outarray)
            {
                IProductWRTBase(inarray,outarray);
            }

            /// Virtual call to SegExp::PhysDeriv
            virtual void v_StdPhysDeriv(const Array<OneD, const NekDouble>& inarray, 
                    Array<OneD,NekDouble> &outarray)
            {
                StdSegExp::PhysDeriv(inarray, outarray);
            }

            virtual void v_PhysDeriv(const Array<OneD, const NekDouble>& inarray, 
                     Array<OneD,NekDouble> &out_d0,
                     Array<OneD,NekDouble> &out_d1 = NullNekDouble1DArray,
                     Array<OneD,NekDouble> &out_d2 = NullNekDouble1DArray)
            {
                PhysDeriv(inarray, out_d0);
            }

            virtual void v_PhysDeriv(const int dir, 
                const Array<OneD, const NekDouble>& inarray,
                                     Array<OneD, NekDouble> &out_d0)
            {
                PhysDeriv(inarray, out_d0);
            }

            /// Virtual call to SegExp::FwdTrans
            virtual void v_FwdTrans(const Array<OneD, const NekDouble>& inarray, 
                    Array<OneD,NekDouble> &outarray)
            {
                FwdTrans(inarray,outarray);
            }

            virtual void v_FwdTrans_BndConstrained(const Array<OneD, const NekDouble>& inarray, 
                                                   Array<OneD, NekDouble> &outarray)
            {
                FwdTrans_BndConstrained(inarray, outarray); 
            }

            /** \brief Virtual call to SegExp::FwdTrans */
            virtual void v_FwdTrans(const StdExpansion1D &in)
            {
                FwdTrans(((SegExp &) in).GetPhys(), m_coeffs);
            }
            
            /// Virtual call to SegExp::Evaluate
            virtual NekDouble v_PhysEvaluate(const Array<OneD, const NekDouble>& coords)
            {
                return PhysEvaluate(coords);
            }

            /** \brief Virtual function to evaluate the discrete \f$ L_\infty\f$
            error \f$ |\epsilon|_\infty = \max |u - u_{exact}|\f$ where \f$
            u_{exact}\f$ is given by the array \a sol. 

            The full function is defined in StdExpansion::Linf 

            Input: 

            - \a _phys: takes the physical value space array as
            approximate solution

            - \a sol: array of solution function  at physical quadrature points

            output: 

            - returns the \f$ L_\infty \f$ error as a NekDouble. 
            */
            virtual NekDouble v_Linf(const Array<OneD, const NekDouble>& sol)
            {
                return Linf(sol);
            }

            /** \brief Virtual function to evaluate the \f$ L_\infty \f$ norm of
            the function defined at the physical points \a (this)->_phys. 

            The full function is defined in StdExpansion::Linf 

            Input: 

            - \a _phys: uses the physical value space array as discrete
            function to be evaulated.

            output: 

            - returns the \f$ L_\infty \f$  as a NekDouble. 
            */
            virtual NekDouble v_Linf()
            {
                return Linf();
            }

            /** \brief Virtual function to evaluate the \f$ L_2\f$, \f$ |
            \epsilon |_{2} = \left [ \int^1_{-1} [u - u_{exact}]^2 dx
            \right]^{1/2} d\xi_1 \f$ where \f$ u_{exact}\f$ is given by the
            array sol.

            The full function is defined in StdExpansion::L2 

            Input: 

            - \a _phys: takes the physical value space array as
            approximate solution
            - \a sol: array of solution function  at physical quadrature points

            output: 

            - returns the \f$ L_2 \f$ error as a NekDouble. 
            */
            virtual NekDouble v_L2(const Array<OneD, const NekDouble>& sol)
            {
                return StdExpansion::L2(sol);
            }

            /** \brief Virtual function to evaluate the \f$ L_2\f$ norm of the
            function defined at the physical points \a (this)->_phys.  

            The full function is defined in StdExpansion::L2 

            Input: 

            - \a _phys: uses the physical value space array as discrete
            function to be evaulated.

            output: 

            - returns the \f$ L_2 \f$  as a NekDouble. 
            */
            virtual NekDouble v_L2()
            {
                return StdExpansion::L2();
            }

            virtual DNekMatSharedPtr v_CreateStdMatrix(const StdRegions::StdMatrixKey &mkey)
            {
                return CreateStdMatrix(mkey);
            }

            virtual DNekScalMatSharedPtr& v_GetLocMatrix(const MatrixKey &mkey)
            {
                return m_matrixManager[mkey];
            }


            virtual DNekScalMatSharedPtr& v_GetLocMatrix(const StdRegions::MatrixType mtype, NekDouble lambdaval, NekDouble tau)
            {
                MatrixKey mkey(mtype,DetShapeType(),*this,lambdaval,tau);
                return m_matrixManager[mkey];
            }

            virtual DNekScalBlkMatSharedPtr& v_GetLocStaticCondMatrix(const MatrixKey &mkey)
            {
                return m_staticCondMatrixManager[mkey];
            }

            virtual void v_AddNormTraceInt(const int dir,
                                           Array<OneD, const NekDouble> &inarray,
                                           Array<OneD,NekDouble> &outarray)
            {
                AddNormBoundaryInt(dir,inarray,outarray);
            }

            virtual void v_AddNormBoundaryInt(const int dir,
                                              Array<OneD, const NekDouble> &inarray,
                                              Array<OneD,NekDouble> &outarray)
            {
                AddNormBoundaryInt(dir,inarray,outarray);
            }

            virtual void v_AddUDGHelmholtzBoundaryTerms(const NekDouble tau, 
                                                        const Array<OneD, const NekDouble> &inarray,
                                                        Array<OneD,NekDouble> &outarray)
            {
                AddUDGHelmholtzBoundaryTerms(tau,inarray,outarray,true);
            }

            virtual void v_AddUDGHelmholtzTraceTerms(const NekDouble tau, 
                                                     const Array<OneD, const NekDouble> &inarray,
                                                     Array<OneD,NekDouble> &outarray)
            {
                AddUDGHelmholtzTraceTerms(tau,inarray,outarray);
            }

        };

    // type defines for use of SegExp in a boost vector
    typedef boost::shared_ptr<SegExp>      SegExpSharedPtr;
    typedef std::vector< SegExpSharedPtr > SegExpVector;
    typedef std::vector< SegExpSharedPtr >::iterator SegExpVectorIter;
    } //end of namespace
} //end of namespace

#endif // SEGEXP_H

//
// $Log: SegExp.h,v $
// Revision 1.33  2008/05/14 18:06:50  sherwin
// mods to fix Seggeom to Geometry1D casting
//
// Revision 1.32  2008/05/10 18:27:33  sherwin
// Modifications necessary for QuadExp Unified DG Solver
//
// Revision 1.31  2008/04/06 05:59:05  bnelson
// Changed ConstArray to Array<const>
//
// Revision 1.30  2008/04/02 22:19:26  pvos
// Update for 2D local to global mapping
//
// Revision 1.29  2008/03/12 15:24:29  pvos
// Clean up of the code
//
// Revision 1.28  2008/02/28 10:04:11  sherwin
// Modes for UDG codes
//
// Revision 1.27  2008/01/21 19:59:32  sherwin
// Updated to take SegGeoms instead of EdgeComponents
//
// Revision 1.26  2007/12/17 13:04:30  sherwin
// Modified GenMatrix to take a StdMatrixKey and removed m_constant from MatrixKey
//
// Revision 1.25  2007/11/20 16:28:46  sherwin
// Added terms for UDG Helmholtz solver
//
// Revision 1.24  2007/10/03 11:37:50  sherwin
// Updates relating to static condensation implementation
//
// Revision 1.23  2007/08/10 03:38:15  jfrazier
// Updated with new rev of NekManager.
//
// Revision 1.22  2007/07/28 05:09:33  sherwin
// Fixed version with updated MemoryManager
//
// Revision 1.21  2007/07/22 23:04:19  bnelson
// Backed out Nektar::ptr.
//
// Revision 1.20  2007/07/20 00:45:51  bnelson
// Replaced boost::shared_ptr with Nektar::ptr
//
// Revision 1.19  2007/07/16 18:28:42  sherwin
// Modification to introduce non-zero Dirichlet boundary conditions into the Helmholtz1D Demo
//
// Revision 1.18  2007/07/13 09:02:23  sherwin
// Mods for Helmholtz solver
//
// Revision 1.17  2007/07/11 19:26:04  sherwin
// update for new Manager structure
//
// Revision 1.16  2007/07/10 17:17:26  sherwin
// Introduced Scaled Matrices into the MatrixManager
//
// Revision 1.15  2007/05/31 19:13:12  pvos
// Updated NodalTriExp + LocalRegions/Project2D + some other modifications
//
// Revision 1.14  2007/05/28 16:15:00  sherwin
// Updated files in MultiRegions to make 1D demos work
//
// Revision 1.13  2007/05/28 08:35:25  sherwin
// Updated for localregions up to Project1D
//
// Revision 1.12  2007/05/27 16:10:29  bnelson
// Update to new Array type.
//
// Revision 1.11  2007/04/26 15:00:16  sherwin
// SJS compiling working version using SHaredArrays
//
// Revision 1.10  2007/04/08 03:33:31  jfrazier
// Minor reformatting and fixing SharedArray usage.
//
// Revision 1.9  2007/03/25 15:48:21  sherwin
// UPdate LocalRegions to take new NekDouble and shared_array formats. Added new Demos
//
// Revision 1.8  2007/03/20 09:13:38  kirby
// new geomfactor routines; update for metricinfo; update style
//
// Revision 1.7  2007/03/14 21:24:07  sherwin
// Update for working version of MultiRegions up to ExpList1D
//
// Revision 1.6  2007/03/02 12:01:55  sherwin
// Update for working version of LocalRegions/Project1D
//
// Revision 1.5  2007/01/15 21:12:26  sherwin
// First definition
//
// Revision 1.4  2006/06/13 18:05:01  sherwin
// Modifications to make MultiRegions demo ProjectLoc2D execute properly.
//
// Revision 1.3  2006/05/30 14:00:04  sherwin
// Updates to make MultiRegions and its Demos work
//
// Revision 1.2  2006/05/29 17:05:49  sherwin
// Modified to put shared_ptr around geom definitions
//
// Revision 1.1  2006/05/04 18:58:46  kirby
// *** empty log message ***
//
// Revision 1.33  2006/03/13 18:20:33  sherwin
//
// Fixed error in ResetGmat
//
// Revision 1.32  2006/03/12 21:59:48  sherwin
//
// compiling version of LocalRegions
//
//
