///////////////////////////////////////////////////////////////////////////////
//
// File: NekVectorFwd.hpp
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
// Description: 
//
///////////////////////////////////////////////////////////////////////////////

#ifndef NEKTAR_LIB_UTILITIES_LINEAR_ALGEBRA_NEK_VECTOR_FWD_HPP
#define NEKTAR_LIB_UTILITIES_LINEAR_ALGEBRA_NEK_VECTOR_FWD_HPP

#include <boost/typeof/typeof.hpp>
#include <LibUtilities/LinearAlgebra/Space.h>
#include <boost/type_traits.hpp>

#include  BOOST_TYPEOF_INCREMENT_REGISTRATION_GROUP()

namespace Nektar
{
    struct VariableSizedVector {};
    
    // \param DataType The type of data held by each element of the vector.
    // \param dim The number of elements in the vector.  If set to 0, the vector
    //            will have a variable number of elements.
    // \param space The space of the vector.
    template<typename DataType, typename dim = VariableSizedVector, typename space = DefaultSpace>
    class NekVector;
    
    BOOST_TYPEOF_REGISTER_TEMPLATE(NekVector, 3);
    
    template<typename T>
    struct IsVector : public boost::false_type {};
    
    template<typename DataType, typename dim, typename space>
    struct IsVector<NekVector<DataType, dim, space> > : public boost::true_type {};
    
}

#endif //NEKTAR_LIB_UTILITIES_LINEAR_ALGEBRA_NEK_VECTOR_FWD_HPP

