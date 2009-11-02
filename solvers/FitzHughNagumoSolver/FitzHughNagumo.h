///////////////////////////////////////////////////////////////////////////////
//
// File FitzHughNagumo.h
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
// Description: Basic Advection Diffusion Reaction Field definition in two-dimensions
//
///////////////////////////////////////////////////////////////////////////////

#ifndef NEKTAR_SOLVERS_FitzHughNagumo_FitzHughNagumo_H
#define NEKTAR_SOLVERS_FitzHughNagumo_FitzHughNagumo_H

#include <MultiRegions/DisContField2D.h>
#include <Auxiliary/ADRBase.h>
#include <LibUtilities/TimeIntegration/TimeIntegrationScheme.h>

namespace Nektar
{     
    enum EquationType
    {
        eNoEquationType,
        eIMEXtest,
	eFHNtest1,
	eFHNtest2,
	eFHNmonoplane,
        eEquationTypeSize
    };
    
    // Keep this consistent with the enums in EquationType.
    const std::string kEquationTypeStr[] = 
    {
        "NoType",
        "IMEXtest",
	"FHNtest1",
	"FHNtest2",
	"FHNmonoplane",
    };

    /**
     * \brief This class is the base class for the development of solvers.
     *
     * It is basically a class handling vector valued fields where every field is
     * a DisContField2D class 
     */
    
    class FitzHughNagumo: public ADRBase
    {
    public:           

        /**
         * Default constructor. 
         * 
         */ 
        FitzHughNagumo();

    
        /**
         * Constructor.
         * /param 
         * 
         *
         */
        FitzHughNagumo(string &fileStringName);

        EquationType GetEquationType(void)
        {
            return m_equationType;
        }


        bool  GetExplicitAdvection(void)
        {
            return m_explicitAdvection;
        }


        bool  GetExplicitDiffusion(void)
        {
            return m_explicitDiffusion;
        }


        bool  GetExplicitReaction(void)
        {
            return m_explicitReaction;
        }

	inline int Usespiralwave(void)
        {
            return m_spiralwave;
        }

        LibUtilities::TimeIntegrationMethod GetTimeIntMethod(void)
        {
            return m_timeIntMethod;
        }

       void NumFluxforScalar(Array<OneD, Array<OneD, NekDouble> > &ufield, 
                              Array<OneD, Array<OneD, Array<OneD, NekDouble> > > &uflux);

	void NumFluxforVector(Array<OneD, Array<OneD, NekDouble> > &ufield,
			      Array<OneD, Array<OneD, Array<OneD, NekDouble> > >  &qfield,
			      Array<OneD, Array<OneD, NekDouble> >  &qflux);
						   
	void WeakPenaltyforScalar(const int var,
                                  const Array<OneD, const NekDouble> &physfield, 
                                  Array<OneD, NekDouble> &penaltyflux,
                                  NekDouble time=0.0);
									 
        void WeakPenaltyforVector(const int var,
                                  const int dir,
                                  const Array<OneD, const NekDouble> &physfield,
                                  Array<OneD, NekDouble> &penaltyflux,
                                  NekDouble C11,
                                  NekDouble time=0.0);	   
			   
        void ODElhs(const Array<OneD, const  Array<OneD, NekDouble> >&inarray, 
		           Array<OneD,       Array<OneD, NekDouble> >&outarray, 
                    const NekDouble time);

        void ODElhsSolve(const Array<OneD, const  Array<OneD, NekDouble> >&inarray, 
                               Array<OneD,        Array<OneD, NekDouble> >&outarray, 
                         const NekDouble time);

        void ODErhs(const Array<OneD, const  Array<OneD, NekDouble> >&inarray, 
                          Array<OneD,        Array<OneD, NekDouble> >&outarray, 
                    const NekDouble time);

        void ODEeReactionIMEXtest(const Array<OneD, const Array<OneD, NekDouble> >&inarray,  
                                  Array<OneD, Array<OneD, NekDouble> >&outarray, 
                                  const NekDouble time);

	void ODEeReactiontest1(const Array<OneD, const Array<OneD, NekDouble> >&inarray,  
			       Array<OneD, Array<OneD, NekDouble> >&outarray, 
			       const NekDouble time);

	void ODEeReactiontest2(const Array<OneD, const Array<OneD, NekDouble> >&inarray,  
			       Array<OneD, Array<OneD, NekDouble> >&outarray, 
			       const NekDouble time);

	void ODEeReactionmono(const Array<OneD, const Array<OneD, NekDouble> >&inarray,  
			       Array<OneD, Array<OneD, NekDouble> >&outarray, 
			       const NekDouble time);

	void ODEeReaction(const Array<OneD, const Array<OneD, NekDouble> >&inarray,  
			  Array<OneD, Array<OneD, NekDouble> >&outarray, 
			  const NekDouble time);
	
	void ODEhelmSolvetest(const Array<OneD, const Array<OneD, NekDouble> >&inarray,
                              Array<OneD, Array<OneD, NekDouble> >&outarray,
                              NekDouble time, 
                              NekDouble lambda);
				
	void ODEhelmSolvemono(const Array<OneD, const Array<OneD, NekDouble> >&inarray,
                              Array<OneD, Array<OneD, NekDouble> >&outarray,
                              NekDouble time, 
                              NekDouble lambda);

        void GeneralTimeIntegration(int nsteps, 
		                   LibUtilities::TimeIntegrationMethod IntMethod,
				   LibUtilities::TimeIntegrationSchemeOperators ode);

        void SolveHelmholtz(NekDouble lambda);
	void Evaluateepsilon();
	void Evaluatebeta();

        void SetFHNInitialConditions(const int Usespiralwave, NekDouble initialtime = 0.0);

        void Generatesecondstimulus(const int Usespiralwave, Array<OneD, NekDouble>&outarray);

	void MassMultiply(const Array<OneD, NekDouble> &inarray, 
			  Array<OneD, NekDouble> &outarray, 
			  const int direction, bool turnon = false);

        void Summary(std::ostream &out);

    protected:

    private: 
        int          m_infosteps;    ///< dump info to stdout at steps time
        EquationType m_equationType; ///< equation type;
        NekDouble  m_epsilon;         /// constant epsilon
        NekDouble  m_beta;            /// constant beta
        NekDouble   m_timedelay;      // Spiral wave activation time delay
        NekDouble   m_duration;       /// Impulse duration time
        int        m_spiralwave;      /// Spiral wave activation 
        
        bool m_explicitAdvection;  ///< Flag to identify explicit Advection
        bool m_explicitDiffusion;  ///< Flag to identify explicit Diffusion
        bool m_explicitReaction;   ///< Flag to identify explicit Reaction
        
        LibUtilities::TimeIntegrationMethod m_timeIntMethod; /// Time integration method

        Array<OneD, Array<OneD, NekDouble> >  m_velocity;

	Array<OneD, Array<OneD, NekDouble> > m_traceNormals_tbasis; // forward normals in tangential basis

        void EvaluateAdvectionVelocity();

	void SetBoundaryConditions(NekDouble time); 
        
        virtual void v_NumFluxforScalar(Array<OneD, Array<OneD, NekDouble> > &ufield, 
                                        Array<OneD, Array<OneD, Array<OneD, NekDouble> > > &uflux)
        {
            NumFluxforScalar(ufield, uflux);
	}
	
	virtual void v_NumFluxforVector(Array<OneD, Array<OneD, NekDouble> > &ufield,
					Array<OneD, Array<OneD, Array<OneD, NekDouble> > >  &qfield,
					Array<OneD, Array<OneD, NekDouble> >  &qflux)
	{
            NumFluxforVector(ufield, qfield, qflux);
	}
      
    };
    
    typedef boost::shared_ptr<FitzHughNagumo> FitzHughNagumoSharedPtr;
    
} //end of namespace

#endif //NEKTAR_SOLVERS_FitzHughNagumo_FitzHughNagumo_H

/**
* $Log: FitzHughNagumo.h,v $
* Revision 1.1  2009/07/23 12:40:08  sehunchun
* *** empty log message ***
*
* Revision 1.12  2009/07/23 05:32:28  sehunchun
* Implicit and Explicit diffusion debugging
*
* Revision 1.11  2009/07/09 21:24:57  sehunchun
* Upwind function is update
*
* Revision 1.10  2009/06/11 01:54:08  claes
* Added Inviscid Burger
*
* Revision 1.9  2009/04/29 20:45:09  sherwin
* Update for new eNum definition of EQTYPE
*
* Revision 1.8  2009/04/27 21:37:14  sherwin
* Updated to dump .fld and .chk file in compressed coefficient format
*
* Revision 1.7  2009/03/06 12:00:10  sehunchun
* Some minor changes on nomenclatures and tabbing errors
*
* Revision 1.6  2009/03/05 11:50:32  sehunchun
* Implicit scheme and IMEX scheme are now implemented
*
* Revision 1.5  2009/02/28 21:59:09  sehunchun
* Explicit Diffusion solver is added
*
* Revision 1.4  2009/02/16 16:07:04  pvos
* Update of TimeIntegration classes
*
* Revision 1.3  2009/02/02 16:12:15  claes
* Moved nocase_cm to ADRBase
*
* Revision 1.2  2009/01/28 13:35:07  pvos
* Modified Time Integration class to take LHS and RHS operator (+support for DIRK)
*
* Revision 1.1  2009/01/13 10:59:32  pvos
* added new solvers file
*
* Revision 1.4  2009/01/06 21:10:34  sherwin
* Updates for virtual calls to IProductWRTBase and introduced reader to handle SOLVERINFO section to specify different solvers
*
* Revision 1.3  2008/11/17 08:20:14  claes
* Temporary fix for CG schemes. 1D CG working (but not for userdefined BC). 1D DG not working
*
* Revision 1.2  2008/11/12 12:12:26  pvos
* Time Integration update
*
* Revision 1.1  2008/10/31 10:50:10  pvos
* Restructured directory and CMakeFiles
*
* Revision 1.3  2008/10/29 22:51:07  sherwin
* Updates for const correctness and ODEforcing
*
* Revision 1.2  2008/10/19 15:59:20  sherwin
* Added Summary method
*
* Revision 1.1  2008/10/16 15:25:45  sherwin
* Working verion of restructured AdvectionDiffusionReactionSolver
*
* Revision 1.1  2008/08/22 09:48:23  pvos
* Added Claes' AdvectionDiffusionReaction, ShallowWater and Euler solver
*
**/
