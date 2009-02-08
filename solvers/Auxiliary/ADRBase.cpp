///////////////////////////////////////////////////////////////////////////////
//
// File ADRBase.cpp
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
// Description: Base definitions definiton for
// AdvectionReactiondiffusion, Euler and ShallowWater classes.
//
///////////////////////////////////////////////////////////////////////////////

#include <Auxiliary/ADRBase.h>
#include <cstdio>
#include <cstdlib>

#include <string>

namespace Nektar
{
    /**
     * Basic construnctor
     */
    ADRBase::ADRBase(void):
        m_fields(0)
    {     
    }
    
    /**
     * Constructor. Creates ... of #DisContField2D fields
     */
    ADRBase::ADRBase(string &fileNameString, bool UseInputFileForProjectionType,
                     bool UseContinuousField)
    {
        SpatialDomains::MeshGraph graph; 

        // Both the geometry and the expansion information should be read
        SpatialDomains::MeshGraphSharedPtr mesh = graph.Read(fileNameString);

        // Also read and store the boundary conditions
	SpatialDomains::MeshGraph *meshptr = mesh.get();
        m_boundaryConditions = MemoryManager<SpatialDomains::BoundaryConditions>::AllocateSharedPtr(meshptr);
        m_boundaryConditions->Read(fileNameString);
        
        // set space dimension for use in class
        m_spacedim = mesh->GetSpaceDimension();
        // save the input file name for output details. 
        m_sessionName = fileNameString;
        m_sessionName = m_sessionName.substr(0,m_sessionName.find_first_of(".")); // Pull out ending
        

        // Options to determine type of projection from file or
        // directly from constructor
        if(UseInputFileForProjectionType == true)
        {
            m_projectionType = eGalerkin;
            
            if(m_boundaryConditions->CheckForParameter("DisContinuous") == true)
            {
                if((int) m_boundaryConditions->GetParameter("DisContinuous") != 0)
                {
                    m_projectionType = eDiscontinuousGalerkin;
                }
            }
        }
        else 
        {
            if(UseContinuousField == true)
            {
                m_projectionType == eGalerkin;
                
            }
            else
            {
                m_projectionType == eDiscontinuousGalerkin;
            }
        }


        SetADRBase(mesh,m_boundaryConditions->GetNumVariables());
    }
    
  void ADRBase::SetADRBase(SpatialDomains::MeshGraphSharedPtr &mesh,
			   int nvariables)
  {
    int i;
    
    m_fields   = Array<OneD, MultiRegions::ExpListSharedPtr>(nvariables);
    m_spacedim = mesh->GetSpaceDimension();
    m_expdim   = mesh->GetMeshDimension();
    
    if(m_projectionType == eGalerkin)
      {
	switch(m_expdim)
	  {
	  case 1:
	    {
	      SpatialDomains::MeshGraph1DSharedPtr mesh1D;
	      
	      if(!(mesh1D = boost::dynamic_pointer_cast<SpatialDomains::MeshGraph1D>(mesh)))
              {
		  ASSERTL0(false,"Dynamics cast failed");
              }
	      
	      for(i = 0 ; i < m_fields.num_elements(); i++)
              {
		  m_fields[i] = MemoryManager<MultiRegions::ContField1D>::AllocateSharedPtr(*mesh1D,*m_boundaryConditions,i);
              }
	    }
	    break;
	  case 2:
	    {
	      SpatialDomains::MeshGraph2DSharedPtr mesh2D;
              
	      if(!(mesh2D = boost::dynamic_pointer_cast<SpatialDomains::MeshGraph2D>(mesh)))
              {
		  ASSERTL0(false,"Dynamics cast failed");
              }
	      
	      for(i = 0 ; i < m_fields.num_elements(); i++)
              {
                  m_fields[i] = MemoryManager<MultiRegions::ContField2D>::AllocateSharedPtr(*mesh2D,*m_boundaryConditions,i);
              }
	      break;
	    }
	  case 3:
	    ASSERTL0(false,"3 D not set up");
	  default:
	    ASSERTL0(false,"Expansion dimension not recognised");
	    break;
	  }
      }
    else // Discontinuous Field
      {
	
	switch(m_expdim)
	  {
	  case 1:
	    {
	      SpatialDomains::MeshGraph1DSharedPtr mesh1D;
	      
	      if(!(mesh1D = boost::dynamic_pointer_cast<SpatialDomains::MeshGraph1D>(mesh)))
		{
		  ASSERTL0(false,"Dynamics cast failed");
		}
	      
	      for(i = 0 ; i < m_fields.num_elements(); i++)
		{
		  m_fields[i] = MemoryManager<MultiRegions::DisContField1D>::AllocateSharedPtr(*mesh1D,*m_boundaryConditions,i);
		}
	    }
	    break;
	  case 2:
	    {
	      SpatialDomains::MeshGraph2DSharedPtr mesh2D;
              
	      if(!(mesh2D = boost::dynamic_pointer_cast<SpatialDomains::MeshGraph2D>(mesh)))
		{
		  ASSERTL0(false,"Dynamics cast failed");
		}
	      
	      for(i = 0 ; i < m_fields.num_elements(); i++)
		{
		  m_fields[i] = MemoryManager<MultiRegions::DisContField2D>::AllocateSharedPtr(*mesh2D,*m_boundaryConditions,i);
		}
	    }
	    break;
	  case 3:
	    ASSERTL0(false,"3 D not set up");
	  default:
	    ASSERTL0(false,"Expansion dimension not recognised");
	    break;
	  }
	    
	// Set up Normals. 
	switch(m_expdim)
	  {
	  case 1:
	    // no need??... will always be unity except perhaps at the boundary edges....
	    break;
	  case 2:
	    {
	      m_traceNormals = Array<OneD, Array<OneD, NekDouble> >(m_spacedim);
	      
	      for(i = 0; i < m_spacedim; ++i)
		{
		  m_traceNormals[i] = Array<OneD, NekDouble> (m_fields[0]->GetTrace()->GetNpoints());
		}
	      
	      m_fields[0]->GetTrace()->GetNormals(m_traceNormals);
	    }
	    break;
	  case 3:
	    ASSERTL0(false,"3 D not set up");
	    break;
	  default:
	    ASSERTL0(false,"Expansion dimension not recognised");
	    break;
	  }
      }
    // Set Default Parameter
    
    if(m_boundaryConditions->CheckForParameter("Time") == true)
    {
	m_time  = m_boundaryConditions->GetParameter("Time");
    }
    else
    {
	m_time  = 0.0;
    }

    if(m_boundaryConditions->CheckForParameter("TimeStep") == true)
    {
        m_timestep   = m_boundaryConditions->GetParameter("TimeStep");
    }

    if(m_boundaryConditions->CheckForParameter("NumSteps") == true)
    {
        m_steps      = m_boundaryConditions->GetParameter("NumSteps");
    }
    else
    {
        m_steps  = 0;
    }
    
    if(m_boundaryConditions->CheckForParameter("IO_CheckSteps") == true)
    {
	m_checksteps = m_boundaryConditions->GetParameter("IO_CheckSteps");
    }
    else
    {
	m_checksteps = m_steps;
    }
  }

    void ADRBase::ZeroPhysFields(void)
    {
        for(int i = 0; i < m_fields.num_elements(); i++)
	{
            Vmath::Zero(m_fields[i]->GetNpoints(),m_fields[i]->UpdatePhys(),1);
        }
    }
  
    void ADRBase::SetInitialConditions(NekDouble initialtime)
    {
        int nq = m_fields[0]->GetNpoints();
      
        Array<OneD,NekDouble> x0(nq);
        Array<OneD,NekDouble> x1(nq);
        Array<OneD,NekDouble> x2(nq);
      
        // get the coordinates (assuming all fields have the same
        // discretisation)
        m_fields[0]->GetCoords(x0,x1,x2);
      

        for(int i = 0 ; i < m_fields.num_elements(); i++)
	{
            SpatialDomains::ConstInitialConditionShPtr ifunc = m_boundaryConditions->GetInitialCondition(i);
            for(int j = 0; j < nq; j++)
	    {
                (m_fields[i]->UpdatePhys())[j] = ifunc->Evaluate(x0[j],x1[j],x2[j],initialtime);
	    }
            m_fields[i]->SetPhysState(true);

            m_fields[i]->FwdTrans(*(m_fields[i]));
	}


	// dump initial conditions to file
	for(int i = 0; i < m_fields.num_elements(); ++i)
	  {
	    std::string outname = m_sessionName +"_" + m_boundaryConditions->GetVariable(i) + "_initial.chk";
            ofstream outfile(outname.c_str());
	    m_fields[i]->WriteToFile(outfile,eTecplot);
	  }       
    }
  

    void ADRBase::SetPhysForcingFunctions(Array<OneD, MultiRegions::ExpListSharedPtr> &force)
        
    {
        int nq = m_fields[0]->GetNpoints();
      
        Array<OneD,NekDouble> x0(nq);
        Array<OneD,NekDouble> x1(nq);
        Array<OneD,NekDouble> x2(nq);
      
        // get the coordinates (assuming all fields have the same
        // discretisation)
        force[0]->GetCoords(x0,x1,x2);
      
        for(int i = 0 ; i < m_fields.num_elements(); i++)
	{
            SpatialDomains::ConstForcingFunctionShPtr ffunc = m_boundaryConditions->GetForcingFunction(i);

            for(int j = 0; j < nq; j++)
	    {
                (force[i]->UpdatePhys())[j] = ffunc->Evaluate(x0[j],x1[j],x2[j]);
             }
             force[i]->SetPhysState(true);
        }
    }


    void ADRBase::EvaluateExactSolution(int field, Array<OneD, NekDouble> &outfield, const NekDouble time)
    {
        int nq = m_fields[field]->GetNpoints();
      
        Array<OneD,NekDouble> x0(nq);
        Array<OneD,NekDouble> x1(nq);
        Array<OneD,NekDouble> x2(nq);
      
        // get the coordinates of the quad points
        m_fields[field]->GetCoords(x0,x1,x2);
      
        SpatialDomains::ConstExactSolutionShPtr ifunc = m_boundaryConditions->GetExactSolution(field);
        for(int j = 0; j < nq; j++)
        {
            outfield[j] = ifunc->Evaluate(x0[j],x1[j],x2[j],time);
        }
    }
    
    void ADRBase::EvaluateUserDefinedEqn(Array<OneD, Array<OneD, NekDouble> > &outfield)
    {
        int nq = m_fields[0]->GetNpoints();
        
        Array<OneD,NekDouble> x0(nq);
        Array<OneD,NekDouble> x1(nq);
        Array<OneD,NekDouble> x2(nq);
      
        // get the coordinates (assuming all fields have the same
        // discretisation)
        m_fields[0]->GetCoords(x0,x1,x2);
      
        for(int i = 0 ; i < m_fields.num_elements(); i++)
	{
            SpatialDomains::ConstUserDefinedEqnShPtr ifunc = m_boundaryConditions->GetUserDefinedEqn(i);
            for(int j = 0; j < nq; j++)
	    {
                outfield[i][j] = ifunc->Evaluate(x0[j],x1[j],x2[j]);
	    }
	}
        
    }

  NekDouble ADRBase::L2Error(int field, const Array<OneD, NekDouble> &exactsoln)
    {
      
      if(exactsoln.num_elements())
	{
	  return m_fields[field]->L2(exactsoln);
	}
      else
	{
	  Array<OneD, NekDouble> exactsoln(m_fields[field]->GetNpoints());
	  
	  EvaluateExactSolution(field,exactsoln,m_time);
	  
	  return m_fields[field]->L2(exactsoln);
	}
    }


    //-------------------------------------------------------------
    // Compute weak Green form of advection terms (without boundary
    // integral, i.e (\grad \phi \cdot F) where for example F = uV
    //
    // Note: Assuming all fields are of the same expansion and order
    // so that we can use the parameters of m_fields[0].
    // ------------------------------------------------------------
  
  void ADRBase::WeakAdvectionGreensDivergenceForm(const Array<OneD, Array<OneD, NekDouble> > &F, Array<OneD, NekDouble> &outarray)
    {
        // use dimension of Velocity vector to dictate dimension of operation
        int ndim    = F.num_elements();
        int nCoeffs = m_fields[0]->GetNcoeffs();

        Array<OneD, NekDouble> iprod(nCoeffs);
        Vmath::Zero(nCoeffs,outarray,1);
        
        for (int i = 0; i < ndim; ++i)
        {
            m_fields[0]->IProductWRTDerivBase(i,F[i],iprod);
            Vmath::Vadd(nCoeffs,iprod,1,outarray,1,outarray,1);
        }
       
    }

    //-------------------------------------------------------------
    // Calculate Inner product of the divergence advection form
    // .....(\phi, Div \cdot F) where for example F = uV
    // -------------------------------------------------------------
    
    void ADRBase::WeakAdvectionDivergenceForm(const Array<OneD, Array<OneD, NekDouble> > &F, Array<OneD, NekDouble> &outarray)
    {
        // use dimension of Velocity vector to dictate dimension of operation
        int ndim       = F.num_elements();
        int nPointsTot = m_fields[0]->GetNpoints();
        Array<OneD, NekDouble> tmp(nPointsTot);
        Array<OneD, NekDouble> div(nPointsTot,0.0);
        
        // Evaluate the divergence 
        for(int i = 0; i < ndim; ++i)
        {
            m_fields[0]->PhysDeriv(i,F[i],tmp);
            Vmath::Vadd(nPointsTot,tmp,1,div,1,div,1);
        }

        m_fields[0]->IProductWRTBase(div,outarray);
    }

    //-------------------------------------------------------------
    // Calculate Inner product of the divergence advection form
    // ..... (\phi, V\cdot Grad(u))
    // -------------------------------------------------------------

    void ADRBase::WeakAdvectionNonConservativeForm(const Array<OneD, Array<OneD, NekDouble> > &V, const Array<OneD, const NekDouble> &u, Array<OneD, NekDouble> &outarray)
    {
        // use dimension of Velocity vector to dictate dimension of operation
      int ndim       = V.num_elements();
      //int ndim = m_expdim;
      
      // ToDo: here we should add a check that V has right dimension
      
        int nPointsTot = m_fields[0]->GetNpoints();
        Array<OneD, NekDouble> tmp(nPointsTot);
        Array<OneD, NekDouble> grad0(ndim*nPointsTot,0.0),grad1,grad2;

        // Evaluate V\cdot Grad(u)
        switch(ndim)
        {
        case 1:
            m_fields[0]->PhysDeriv(u,grad0);
            Vmath::Vmul(nPointsTot,grad0,1,V[0],1,tmp,1);
            break;
        case 2:
            grad1 = grad0 + nPointsTot;
            m_fields[0]->PhysDeriv(u,grad0,grad1);
            Vmath::Vmul (nPointsTot,grad0,1,V[0],1,tmp,1);
            Vmath::Vvtvp(nPointsTot,grad1,1,V[1],1,tmp,1,tmp,1);
            break;
        case 3:
            grad1 = grad0 + nPointsTot;
            grad2 = grad1 + nPointsTot;
            m_fields[0]->PhysDeriv(u,grad0,grad1,grad2);
            Vmath::Vmul (nPointsTot,grad0,1,V[0],1,tmp,1);
            Vmath::Vvtvp(nPointsTot,grad1,1,V[1],1,tmp,1,tmp,1);
            Vmath::Vvtvp(nPointsTot,grad2,1,V[2],1,tmp,1,tmp,1);
            break;
        default:
            ASSERTL0(false,"dimension unknown");
        }

        m_fields[0]->IProductWRTBase_IterPerExp(tmp,outarray);
    }
                                       
    //-------------------------------------------------------------
    // Calculate weak DG advection in the form 
    //  <\phi, \hat{F}\cdot n> - (\grad \phi \cdot F)
    // -------------------------------------------------------------
  void ADRBase::WeakDGAdvection(const Array<OneD, Array<OneD, NekDouble> >& InField, 
				Array<OneD, Array<OneD, NekDouble> >& OutField,
				bool NumericalFluxIncludesNormal, bool InFieldIsInPhysSpace, int nvariables)
    {
        int i;
        int nVelDim         = m_spacedim;
        int nPointsTot      = GetNpoints();
        int ncoeffs         = GetNcoeffs();
        int nTracePointsTot = GetTraceNpoints();
	
	if (!nvariables)
	  {
	    nvariables      = m_fields.num_elements();
	  }
	
        Array<OneD, Array<OneD, NekDouble> > fluxvector(nVelDim);
	Array<OneD, Array<OneD, NekDouble> > physfield (nvariables);
        
        for(i = 0; i < nVelDim; ++i)
        {
            fluxvector[i]    = Array<OneD, NekDouble>(nPointsTot);
        }

	
	//--------------------------------------------
	// Get the variables in physical space
	
	// already in physical space
	if(InFieldIsInPhysSpace == true)
	  {
            for(i = 0; i < nvariables; ++i)
	      {
                physfield[i] = InField[i];
	      }
	  }
	// otherwise do a backward transformation
        else
	  {
	    for(i = 0; i < nvariables; ++i)
	      {
		// Could make this point to m_fields[i]->UpdatePhys();
		physfield[i] = Array<OneD, NekDouble>(nPointsTot);
		m_fields[i]->BwdTrans(InField[i],physfield[i]);
	      }
	  }
	//--------------------------------------------

        
	//--------------------------------------------
	// Get the advection part (without numerical flux) 
	
        for(i = 0; i < nvariables; ++i)
        {
            
            // Get the ith component of the  flux vector in (physical space)
            GetFluxVector(i, physfield, fluxvector);
            
            // Calculate the i^th value of (\grad_i \phi, F)
            WeakAdvectionGreensDivergenceForm(fluxvector,OutField[i]);
        }
	//--------------------------------------------
        

	//----------------------------------------------
	// Get the numerical flux and add to the modal coeffs
	
	// if the NumericalFlux function already includes the
	// normal in the output
	if (NumericalFluxIncludesNormal == true)
	  {
	    
	    Array<OneD, Array<OneD, NekDouble> > numflux   (nvariables);

	    for(i = 0; i < nvariables; ++i)
	      {
		numflux[i]   = Array<OneD, NekDouble>(nTracePointsTot);
	      }
	    
	    // Evaluate numerical flux in physical space which may in
	    // general couple all component of vectors
	    NumericalFlux(physfield, numflux);
	    
	    // Evaulate  <\phi, \hat{F}\cdot n> - OutField[i] 
	    for(i = 0; i < nvariables; ++i)
	      {
		Vmath::Neg(ncoeffs,OutField[i],1);
		m_fields[i]->AddTraceIntegral(numflux[i],OutField[i]);
	      }
	  }
	// if the NumericalFlux function does not include the
	// normal in the output
	else
	  {
	    Array<OneD, Array<OneD, NekDouble> > numfluxX   (nvariables);
	    Array<OneD, Array<OneD, NekDouble> > numfluxY   (nvariables);
	    
	    for(i = 0; i < nvariables; ++i)
	      {
		numfluxX[i]   = Array<OneD, NekDouble>(nTracePointsTot);
		numfluxY[i]   = Array<OneD, NekDouble>(nTracePointsTot);
	      }

	    // Evaluate numerical flux in physical space which may in
	    // general couple all component of vectors
	    NumericalFlux(physfield, numfluxX, numfluxY);
	    
	    // Evaulate  <\phi, \hat{F}\cdot n> - OutField[i] 
	    for(i = 0; i < nvariables; ++i)
	      {
		Vmath::Neg(ncoeffs,OutField[i],1);
		m_fields[i]->AddTraceIntegral(numfluxX[i],numfluxY[i],OutField[i]);
	      }
	    
	  }
    }
  
  void ADRBase::Output(void)
    {
        for(int i = 0; i < m_fields.num_elements(); ++i)
        {
            std::string outname = m_sessionName +"_" + m_boundaryConditions->GetVariable(i) +  ".dat";
            ofstream outfile(outname.c_str());
            m_fields[i]->BwdTrans(*(m_fields[i]));
            m_fields[i]->WriteToFile(outfile,eTecplot);
        }
    }

    void ADRBase::Checkpoint_Output(const int n)
    {
        for(int i = 0; i < m_fields.num_elements(); ++i)
        {
            char chkout[16] = "";
            sprintf(chkout, "%d", n);
            std::string outname = m_sessionName +"_" + m_boundaryConditions->GetVariable(i) + "_" + chkout + ".chk";
            ofstream outfile(outname.c_str());
            m_fields[i]->BwdTrans(*(m_fields[i]));
            m_fields[i]->WriteToFile(outfile,eTecplot);
        }
    }

    
    void ADRBase::Summary(std::ostream &out)
    {
        SessionSummary(out);
        TimeParamSummary(out);
    }
    
    void ADRBase::SessionSummary(std::ostream &out)
    {

        out << "\tSession Name    : " << m_sessionName << endl;
	out << "\tExp. Dimension  : " << m_expdim << endl;
        out << "\tMax Exp. Order  : " << m_fields[0]->EvalBasisNumModesMax() << endl;
        if(m_projectionType == eGalerkin)
        {
            out << "\tProjection Type : Galerkin" <<endl;
        }
        else
        {
            out << "\tProjection Type : Discontinuous Galerkin" <<endl;
        }
    }


    void ADRBase::TimeParamSummary(std::ostream &out)
    {
        out << "\tTime Step       : " << m_timestep << endl;
        out << "\tNo. of Steps    : " << m_steps << endl;
        out << "\tCheckpoints     : " << m_checksteps <<" steps" <<endl;
    }

  
  // case insensitive string comparison from web
  int ADRBase::nocase_cmp(const string & s1, const string& s2) 
  {
    string::const_iterator it1=s1.begin();
    string::const_iterator it2=s2.begin();
    
    //stop when either string's end has been reached
    while ( (it1!=s1.end()) && (it2!=s2.end()) ) 
      { 
	if(::toupper(*it1) != ::toupper(*it2)) //letters differ?
	  {
	    // return -1 to indicate smaller than, 1 otherwise
	    return (::toupper(*it1)  < ::toupper(*it2)) ? -1 : 1; 
	  }
	//proceed to the next character in each string
	++it1;
	++it2;
      }
    size_t size1=s1.size(), size2=s2.size();// cache lengths
    
    //return -1,0 or 1 according to strings' lengths
    if (size1==size2) 
      {
	return 0;
      }
    return (size1 < size2) ? -1 : 1;
  }
  

} //end of namespace

/**
* $Log: ADRBase.cpp,v $
* Revision 1.3  2009/02/02 16:10:16  claes
* Update to make SWE, Euler and Boussinesq solvers up to date with the time integrator scheme. Linear and classical Boussinsq solver working
*
* Revision 1.2  2009/01/27 12:07:18  pvos
* Modifications to make cont. Galerkin Advection solver working
*
* Revision 1.1  2009/01/13 10:59:32  pvos
* added new solvers file
*
* Revision 1.8  2009/01/10 23:50:32  sherwin
* Update ContField1D/2D to use different variable in constructors
*
* Revision 1.7  2009/01/06 21:11:03  sherwin
* Updates for Virtual ExpList calls
*
* Revision 1.6  2008/11/17 08:10:07  claes
* Removed functions that were no longer used after the solver library was restructured
*
* Revision 1.5  2008/11/02 22:39:27  sherwin
* Updated naming convention
*
* Revision 1.4  2008/10/31 10:50:10  pvos
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
**/