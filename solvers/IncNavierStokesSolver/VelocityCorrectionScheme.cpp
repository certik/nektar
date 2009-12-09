///////////////////////////////////////////////////////////////////////////////
//
// File VelocityCorrection.cpp
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
// Description: Velocity Correction Scheme for the Incompressible
// Navier Stokes equations
///////////////////////////////////////////////////////////////////////////////

#include <IncNavierStokesSolver/VelocityCorrectionScheme.h>

namespace Nektar
{
    int nocase_cmp(const string & s1, const string& s2);


    /**
     * Basic construnctor
     */
    VelocityCorrectionScheme::VelocityCorrectionScheme(void):
        IncNavierStokes()
    {     
    }
    
    /**
     * Constructor. Creates ...
     *
     * \param 
     * \param
     */

    VelocityCorrectionScheme::VelocityCorrectionScheme(string &fileNameString):
        IncNavierStokes(fileNameString)
    {

        LibUtilities::TimeIntegrationMethod intMethod;
        std::string TimeIntStr = m_boundaryConditions->GetSolverInfo("TIMEINTEGRATIONMETHOD");
        int i;
        for(i = 0; i < (int) LibUtilities::SIZE_TimeIntegrationMethod; ++i)
        {
            if(NoCaseStringCompare(LibUtilities::TimeIntegrationMethodMap[i],TimeIntStr) == 0 )
            {
                intMethod = (LibUtilities::TimeIntegrationMethod)i; 
                break;
            }
        }

        ASSERTL0(i != (int) LibUtilities::SIZE_TimeIntegrationMethod, "Invalid time integration type.");
        
        // Set to 1 for first step and it will then be increased in
        // time advance routines
        switch(intMethod)
        {
        case LibUtilities::eIMEXOrder1: 
            {
                m_intSteps = 1;
                m_integrationScheme = Array<OneD, LibUtilities::TimeIntegrationSchemeSharedPtr> (m_intSteps);
                LibUtilities::TimeIntegrationSchemeKey       IntKey0(intMethod);
                m_integrationScheme[0] = LibUtilities::TimeIntegrationSchemeManager()[IntKey0];
            }
            break;
        case LibUtilities::eIMEXOrder2: 
            {
                m_intSteps = 2;
                m_integrationScheme = Array<OneD, LibUtilities::TimeIntegrationSchemeSharedPtr> (m_intSteps);
                LibUtilities::TimeIntegrationSchemeKey       IntKey0(LibUtilities::eIMEXOrder1);
                m_integrationScheme[0] = LibUtilities::TimeIntegrationSchemeManager()[IntKey0];
                LibUtilities::TimeIntegrationSchemeKey       IntKey1(intMethod);
                m_integrationScheme[1] = LibUtilities::TimeIntegrationSchemeManager()[IntKey1];
            }
            break;
        case LibUtilities::eIMEXOrder3: 
            {
                m_intSteps = 3;
                m_integrationScheme = Array<OneD, LibUtilities::TimeIntegrationSchemeSharedPtr> (m_intSteps);
                LibUtilities::TimeIntegrationSchemeKey       IntKey0(LibUtilities::eIMEXOrder1);
                m_integrationScheme[0] = LibUtilities::TimeIntegrationSchemeManager()[IntKey0];
                LibUtilities::TimeIntegrationSchemeKey       IntKey1(LibUtilities::eIMEXOrder2);
                m_integrationScheme[1] = LibUtilities::TimeIntegrationSchemeManager()[IntKey1];
                LibUtilities::TimeIntegrationSchemeKey       IntKey2(intMethod);
                m_integrationScheme[2] = LibUtilities::TimeIntegrationSchemeManager()[IntKey2];
            }
            break;
        default:
            ASSERTL0(0,"Integration method not suitable: Options include IMEXOrder1, IMEXOrder2 or IMEXOrder3");
            break;
        }
            
        // Set up mapping from pressure boundary condition to pressure
        // element details.
        m_pressure->GetBoundaryToElmtMap(m_pressureBCtoElmtID,
                                         m_pressureBCtoTraceID);

        // Storage array for high order pressure BCs
        m_pressureHBCs = Array<OneD, Array<OneD, NekDouble> > (m_intSteps);

        // Count number of HBC conditions
        Array<OneD, const SpatialDomains::BoundaryConditionShPtr > PBndConds = m_pressure->GetBndConditions();
        Array<OneD, MultiRegions::ExpList1DSharedPtr>  PBndExp = m_pressure->GetBndCondExpansions();
        
        int n,cnt;
        for(cnt = n = 0; n < PBndConds.num_elements(); ++n)
        {
            // High order boundary condition;
            if(PBndConds[n]->GetUserDefined().GetEquation() == "H")
            {
                cnt += PBndExp[n]->GetNcoeffs();
            }
        }

        for(n = 0; n < m_intSteps; ++n)
        {
            m_pressureHBCs[n] = Array<OneD, NekDouble>(cnt);
        }
        
        m_integrationOps.DefineOdeRhs(&VelocityCorrectionScheme::EvaluateAdvection_SetPressureBCs, this);
        
        m_integrationOps.DefineImplicitSolve(&VelocityCorrectionScheme::SolveUnsteadyStokesSystem,this);

    }


    void VelocityCorrectionScheme::Summary(std::ostream &out)
    {
        cout << "\nIncompressible Navier Stokes Solver" << endl;
        cout <<  "\tType of scheme  : Velocity Correction" <<endl;
        IncNavierStokes::Summary(out);
        cout << "\tTime integration:" << LibUtilities::TimeIntegrationMethodMap[m_integrationScheme[m_intSteps-1]->GetIntegrationMethod()] << endl;
    }

    void VelocityCorrectionScheme::EvaluateAdvection_SetPressureBCs(const Array<OneD, const Array<OneD, NekDouble> > &inarray, 
                                                                    Array<OneD, Array<OneD, NekDouble> > &outarray, 
                                                                    const NekDouble time)
    {
        // evaluate convection terms
        EvaluateAdvectionTerms(inarray,outarray);

        // Set pressure BCs
        EvaluatePressureBCs(inarray, outarray);
    }

    void VelocityCorrectionScheme::SolveUnsteadyStokesSystem(const Array<OneD, const Array<OneD, NekDouble> > &inarray, 
                                                             Array<OneD, Array<OneD, NekDouble> > &outarray, 
                                                             const NekDouble time, 
                                                             const NekDouble aii_Dt)
    {
        int i,n;
        int phystot = m_fields[0]->GetTotPoints();
        int ncoeffs = m_fields[0]->GetNcoeffs();
        Array<OneD, Array< OneD, NekDouble> > F(m_nConvectiveFields);
        NekDouble  lambda = 1.0/aii_Dt/m_kinvis; 

        F[0] = Array<OneD, NekDouble> (phystot*m_nConvectiveFields);
        for(n = 1; n < m_nConvectiveFields; ++n)
        {
            F[n] = F[n-1] + phystot;
        }
        
        // Pressure Forcing = Divergence Velocity; 
        SetUpPressureForcing(inarray, F, aii_Dt);
        
        // Solver Pressure Poisson Equation 
        m_pressure->HelmSolve(F[0], m_pressure->UpdateCoeffs(),0.0);
        
        // Viscous Term forcing
        SetUpViscousForcing(inarray, F, aii_Dt);
    
        // Solve Helmholtz system and put in Physical space 
        for(i = 0; i < m_nConvectiveFields; ++i)
        {
            m_fields[i]->HelmSolve(F[i], m_fields[i]->UpdateCoeffs(), lambda);
            m_fields[i]->BwdTrans(m_fields[i]->GetCoeffs(),outarray[i]);
        }
    }

    void VelocityCorrectionScheme::AdvanceInTime(int nsteps)
    {
        int i,n;
        int phystot = m_fields[0]->GetTotPoints();
        static int nchk = 0;

        // Set up wrapper to fields data storage. 
        Array<OneD, Array<OneD, NekDouble> >   fields(m_nConvectiveFields);
        for(i = 0; i < m_nConvectiveFields; ++i)
        {
            fields[i]  = m_fields[i]->UpdatePhys();
            //fields[i]  = Array<OneD, NekDouble>(phystot,0.0);
        }
        
        // Initialise NS solver which is set up to use a GLM method
        // with calls to EvaluateAdvection_SetPressureBCs and
        // SolveUnsteadyStokesSystem
        LibUtilities::TimeIntegrationSolutionSharedPtr 
            IntegrationSoln = m_integrationScheme[m_intSteps-1]->InitializeScheme(m_timestep,
                                                                fields,
                                                                m_time,
                                                                m_integrationOps);
        //Time advance
        for(n = 0; n < nsteps; ++n)
        {
            // Advance velocity fields
            fields = m_integrationScheme[min(n,m_intSteps-1)]->TimeIntegrate(m_timestep,
                                                                             IntegrationSoln,
                                                                             m_integrationOps);
            
            m_time += m_timestep;
			
            if(!((n+1)%m_infosteps))
            {
	      cout << "Step: " << n+1 << "  Time: " << m_time << endl;
            }

            // dump data in m_fields->m_coeffs to file. 
            if(n&&(!((n+1)%m_checksteps)))
            {
                Checkpoint_Output(nchk++);
            }
        }
		
		//updatig physical space
		for(i = 0; i < m_nConvectiveFields; ++i)
		{
			m_fields[i]->SetPhys(fields[i]);
		}			
		
    }
        
    void VelocityCorrectionScheme::EvaluatePressureBCs(const Array<OneD, const Array<OneD, NekDouble> >  &fields, const Array<OneD, const Array<OneD, NekDouble> >  &N)
    {
        static int ncalls = 1; // Number of time this function has been called. 
        Array<OneD, NekDouble> tmp;
        Array<OneD, const SpatialDomains::BoundaryConditionShPtr > PBndConds;
        Array<OneD, MultiRegions::ExpList1DSharedPtr>  PBndExp;
        int  n,cnt;
        int  nint    = min(ncalls++,m_intSteps);
        int  nlevels = m_pressureHBCs.num_elements();

        PBndConds   = m_pressure->GetBndConditions();
        PBndExp     = m_pressure->GetBndCondExpansions();

        // Reshuffle Bc Storage vector
        tmp = m_pressureHBCs[nlevels-1];
        for(n = nlevels-1; n > 0; --n)
        {
            m_pressureHBCs[n] = m_pressureHBCs[n-1];
        }
        m_pressureHBCs[0] = tmp;

        // Calculate BCs at current level
        CalcPressureBCs(fields,N);
        
        // Copy High order values into storage array 
        for(cnt = n = 0; n < PBndConds.num_elements(); ++n)
        {
            // High order boundary condition;
            if(PBndConds[n]->GetUserDefined().GetEquation() == "H")
            {
                int nq = PBndExp[n]->GetNcoeffs();
                Vmath::Vcopy(nq,&(PBndExp[n]->GetCoeffs()[0]),1,&(m_pressureHBCs[0])[cnt],1);
                cnt += nq;
            }
        }
        
        // Extrapolate to n+1
        Vmath::Smul(cnt,kHighOrderBCsExtrapolation[nint-1][nint-1],
                    m_pressureHBCs[nint-1],1,m_pressureHBCs[nlevels-1],1);
        for(n = 0; n < nint-1; ++n)
        {
            Vmath::Svtvp(cnt,kHighOrderBCsExtrapolation[nint-1][n],
                          m_pressureHBCs[n],1,m_pressureHBCs[nlevels-1],1,
                          m_pressureHBCs[nlevels-1],1);
        }

        // Reset Values into Pressure BCs
        for(cnt = n = 0; n < PBndConds.num_elements(); ++n)
        {
            // High order boundary condition;
            if(PBndConds[n]->GetUserDefined().GetEquation() == "H")
            {
                int nq = PBndExp[n]->GetNcoeffs();
                Vmath::Vcopy(nq,&(m_pressureHBCs[nlevels-1])[cnt],1,&(PBndExp[n]->UpdateCoeffs()[0]),1);
                cnt += nq;
            }
        }
    }
    
    void VelocityCorrectionScheme::CalcPressureBCs(const Array<OneD, const Array<OneD, NekDouble> > &fields, const Array<OneD, const Array<OneD, NekDouble> >  &N)
    {
        int  i,n;
        Array<OneD, const SpatialDomains::BoundaryConditionShPtr > PBndConds;
        Array<OneD, MultiRegions::ExpList1DSharedPtr>  PBndExp;

        PBndConds = m_pressure->GetBndConditions();
        PBndExp   = m_pressure->GetBndCondExpansions();
        Array<OneD, StdRegions::StdExpansionSharedPtr > elmtVel(m_velocity.num_elements());
        StdRegions::StdExpansion1DSharedPtr Pbc;
        Array<OneD, const NekDouble> U,V,Nu,Nv;
        Array<OneD, NekDouble> Pvals;
        int maxpts = 0,cnt;

        // find the maximum values of points 
        for(cnt = n = 0; n < PBndConds.num_elements(); ++n)
        {
            for(i = 0; i < PBndExp[n]->GetExpSize(); ++i)
            {
                maxpts = max(maxpts, m_fields[0]->GetExp(m_pressureBCtoElmtID[cnt++])->GetTotPoints());
            }
        }

        ASSERTL0(m_expdim == 2,"Not set up for 3D expansions");
        
        Array<OneD, NekDouble> Uy(5*maxpts);
        Array<OneD, NekDouble> Vx = Uy + maxpts; 
        Array<OneD, NekDouble> Q  = Vx + maxpts; 
        Array<OneD, NekDouble> Qx = Q  + maxpts; 
        Array<OneD, NekDouble> Qy = Qx + maxpts; 
        int    elmtid,nq,offset, edge;
        StdRegions::StdExpansionSharedPtr elmt;
        bool NegateNormals;

        for(cnt = n = 0; n < PBndConds.num_elements(); ++n)
        {
            string type = PBndConds[n]->GetUserDefined().GetEquation(); 
            
            if(type == "H")
            {
                if(m_expdim == 2) // 2D curl curl evaluation
                {
                    for(i = 0; i < PBndExp[n]->GetExpSize(); ++i,cnt++)
                    {
                        // find element and edge of this expansion. 
                        // calculate curl x curl v;
                        elmtid = m_pressureBCtoElmtID[cnt];
                        elmt   = m_fields[0]->GetExp(elmtid);
                        nq     = elmt->GetTotPoints();
                        offset = m_fields[0]->GetPhys_Offset(elmtid);
                        
                        U = fields[m_velocity[0]] + offset;
                        V = fields[m_velocity[1]] + offset; 
                        
                        // Calculating vorticity Q = (dv/dx - du/dy)
                        elmt->PhysDeriv(0,V,Vx);
                        elmt->PhysDeriv(1,U,Uy);
                        
                        Vmath::Vsub(nq,Vx,1,Uy,1,Q,1);
                        
                        // Calculate  Curl(Q) = Qy i - Qx j 
                        elmt->PhysDeriv(Q,Qx,Qy);
                        
                        Nu = N[0] + offset;
                        Nv = N[1] + offset; 
                        
                        // Evaluate [N - kinvis Curlx Curl V].n
                        // x-component (stored in Qy)
                        Vmath::Svtvp(nq,-m_kinvis,Qy,1,Nu,1,Qy,1);

                        // y-component (stored in Qx )
                        Vmath::Svtvp(nq,m_kinvis,Qx,1,Nv,1,Qx,1);
                        
                        Pbc =  boost::dynamic_pointer_cast<StdRegions::StdExpansion1D> (PBndExp[n]->GetExp(i));
                        
                        edge = m_pressureBCtoTraceID[cnt];

                        // Get edge values and put into Uy, Vx
                        elmt->GetEdgePhysVals(edge,Pbc,Qy,Uy);
                        elmt->GetEdgePhysVals(edge,Pbc,Qx,Vx);
                        
                        // calcuate (phi, dp/dn = [N-kinvis curl x curl v].n) 
                        Pvals = PBndExp[n]->UpdateCoeffs()+PBndExp[n]->GetCoeff_Offset(i);
                        // Decide if normals facing outwards
                        NegateNormals = (elmt->GetEorient(edge) == StdRegions::eForwards)? false:true;
                        
                        Pbc->NormVectorIProductWRTBase(Uy,Vx,Pvals,NegateNormals); 
                    }
                }
                else
                {
                    ASSERTL1(false,"Need to set up curl curl operator");
                }
                
            }
            else if(type == "") // setting if just standard BC without
                                // userdefined value
            {
                cnt += PBndExp[n]->GetExpSize();
            }
            else
            {
                ASSERTL0(false,"Unknown USERDEFINEDTYPE in pressure boundary condition");
            }
        }
    }

    
    // Evaluate divergence of velocity field. 
    void   VelocityCorrectionScheme::SetUpPressureForcing(const Array<OneD, const Array<OneD, NekDouble> > &fields, Array<OneD, Array<OneD, NekDouble> > &Forcing, const NekDouble aii_Dt)
    {                
        int       i;
        int       physTot = m_fields[0]->GetTotPoints();
        Array<OneD, NekDouble> wk = Array<OneD, NekDouble>(physTot);
                
        Vmath::Zero(physTot,Forcing[0],1);
        
        for(i = 0; i < m_velocity.num_elements(); ++i)
        {
            m_fields[m_velocity[i]]->PhysDeriv(i,fields[m_velocity[i]], wk);
            Vmath::Vadd(physTot,wk,1,Forcing[0],1,Forcing[0],1);
        }
        
        Vmath::Smul(physTot,1.0/aii_Dt,Forcing[0],1,Forcing[0],1);        
    }
    
    void   VelocityCorrectionScheme::SetUpViscousForcing(const Array<OneD, const Array<OneD, NekDouble> > &inarray, Array<OneD, Array<OneD, NekDouble> > &Forcing, const NekDouble aii_Dt)
    {
        NekDouble aii_dtinv = 1.0/aii_Dt;
        int ncoeffs = m_fields[0]->GetNcoeffs();
        int phystot = m_fields[0]->GetTotPoints();

        // Grad p
        m_pressure->BwdTrans(m_pressure->GetCoeffs(),m_pressure->UpdatePhys());
        
        if(m_spacedim == 2)
        {
            m_pressure->PhysDeriv(m_pressure->GetPhys(), Forcing[m_velocity[0]], Forcing[m_velocity[1]]);
        }
        else
        {
            m_pressure->PhysDeriv(m_pressure->GetPhys(), Forcing[m_velocity[0]], Forcing[m_velocity[1]],Forcing[m_velocity[2]]);
        }
        
        // Subtract inarray/(aii.dt) and divide by kinvis. Kinvis will
        // need to be updated for the convected fields.
        for(int i = 0; i < m_nConvectiveFields; ++i)
        {
            Blas::Daxpy(phystot,-aii_dtinv,inarray[i],1,Forcing[i],1);
            Blas::Dscal(phystot,1.0/m_kinvis,&(Forcing[i])[0],1);
        }
    }
            
} //end of namespace

/**
* $Log: VelocityCorrectionScheme.cpp,v $
* Revision 1.3  2009/09/10 10:42:49  pvos
* Modification to bind object pointer rather than object itself to time-integration functions.
* (Prevents unwanted copy-constructor calls)
*
* Revision 1.2  2009/09/06 22:31:16  sherwin
* First working version of Navier-Stokes solver and input files
*
* Revision 1.1  2009/03/14 16:43:21  sherwin
* First non-working version
*
*
**/
