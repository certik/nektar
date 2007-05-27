///////////////////////////////////////////////////////////////////////////////
//
// File LocalToGlobalMap.h
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
// Description: Local to Global mapping routines, header file
//
///////////////////////////////////////////////////////////////////////////////

#ifndef NEKTAR_LIB_MULTIREGIONS_LOC2GLOMAP_H
#define NEKTAR_LIB_MULTIREGIONS_LOC2GLOMAP_H

#include <MultiRegions/MultiRegions.hpp>
#include <MultiRegions/ExpList.h>
#include <vector>
#include <LibUtilities/Memory/NekMemoryManager.hpp>


namespace Nektar
{
    namespace MultiRegions
    {
	
	class LocalToGlobalMap
	{
        public:
            LocalToGlobalMap();
	    LocalToGlobalMap(const int totdata, NekIntSharedArray &map);
	    
            ~LocalToGlobalMap();
	    
	    inline int GetMap(const int i) const
	    {
		return m_locToContMap[i];
	    }

	    inline void LocalToCont(ConstArray<OneD, NekDouble> loc, Array<OneD, NekDouble> &cont)
	    {
                Vmath::Scatr(m_totLocLen, &loc[0],&m_locToContMap[0],&cont[0]);
            }                

	    
	    inline void ContToLocal(ConstArray<OneD, NekDouble> cont, Array<OneD, NekDouble> &loc)
	    {
                Vmath::Gathr(m_totLocLen,&cont[0],&m_locToContMap[0], &loc[0]);
	    }
	    
	    inline void Assemble(ConstArray<OneD, NekDouble> loc, Array<OneD, NekDouble> &cont)
	    {
		Vmath::Zero(m_totGloLen,&cont[0],1);

                Vmath::Assmb(m_totLocLen,&loc[0],&m_locToContMap[0],&cont[0]);
	    }
	    
	    inline int GetTotGloLen()
	    {
		return m_totGloLen;
	    }

        protected:
	    int                m_totLocLen;    //< length of local dofs
	    int                m_totGloLen;    //< length of global dofs
	    NekIntSharedArray  m_locToContMap; //< Vector of boost pointers to integer maps
        private:
	};
	
    } // end of namespace
} // end of namespace

#endif //LOC2GLOMAP_H


/** $Log: LocalToGlobalMap.h,v $
/** Revision 1.4  2007/04/26 15:00:16  sherwin
/** SJS compiling working version using SHaredArrays
/**
/** Revision 1.3  2007/03/20 16:58:42  sherwin
/** Update to use Array<OneD, NekDouble> storage and NekDouble usage, compiling and executing up to Demos/StdRegions/Project1D
/**
/** Revision 1.2  2007/03/14 21:24:08  sherwin
/** Update for working version of MultiRegions up to ExpList1D
/**
/** Revision 1.1  2006/07/02 17:16:17  sherwin
/**
/** Modifications to make MultiRegions work for a connected domain in 2D (Tris)
/**
/** Revision 1.3  2006/06/05 00:14:33  bnelson
/** Fixed a compiler error (couldn't find boost::shared_ptr) and a couple of formatting updates for the standard.
/** */

