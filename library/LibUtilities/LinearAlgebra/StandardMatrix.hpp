///////////////////////////////////////////////////////////////////////////////
//
// File: StandardMatrix.hpp
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
// Description: Interface classes for matrices
//
///////////////////////////////////////////////////////////////////////////////

#ifndef NEKTAR_LIB_UTILITIES_LINEAR_ALGEBRA_STANDARD_MATRIX_HPP
#define NEKTAR_LIB_UTILITIES_LINEAR_ALGEBRA_STANDARD_MATRIX_HPP

#include <LibUtilities/LinearAlgebra/MatrixBase.hpp>
#include <LibUtilities/LinearAlgebra/FullMatrixStoragePolicy.hpp>
#include <LibUtilities/LinearAlgebra/DiagonalMatrixStoragePolicy.hpp>
#include <LibUtilities/LinearAlgebra/TriangularMatrixStoragePolicy.hpp>
#include <LibUtilities/LinearAlgebra/PointerWrapper.h>
#include <LibUtilities/LinearAlgebra/NekVectorFwd.hpp>
#include <LibUtilities/ExpressionTemplates/ExpressionTemplates.hpp>

namespace Nektar
{
     
    template<typename DataType, typename StorageType>
    class NekMatrix<DataType, StorageType, StandardMatrixTag> : public Matrix<DataType>
    {
        public:
            typedef Matrix<DataType> BaseType;
            typedef NekMatrix<DataType, StorageType, StandardMatrixTag> ThisType;
            typedef MatrixStoragePolicy<DataType, StorageType> StoragePolicy;
            typedef DataType NumberType;
            typedef typename StoragePolicy::PolicySpecificDataHolderType PolicySpecificDataHolderType;

            typedef typename StoragePolicy::GetValueReturnType GetValueType;
            typedef typename boost::call_traits<DataType>::const_reference ConstGetValueType;
            
            public:
                template<typename T, typename MatrixType>
                class iterator_impl
                {
                    public:
                        typedef typename StoragePolicy::template reference<T>::type value_type;
                        typedef std::input_iterator_tag iterator_category;
                        typedef unsigned int difference_type;
                        typedef typename boost::call_traits<value_type>::reference reference;
                        typedef typename boost::call_traits<value_type>::const_reference const_reference;
                        typedef typename boost::remove_reference<value_type>::type* pointer;

                    public:
                        iterator_impl(pointer d, pointer e, bool isEnd = false) :
                            m_data(d),
                            m_end(e),
                            m_curRow(std::numeric_limits<unsigned int>::max()),
                            m_curColumn(std::numeric_limits<unsigned int>::max()),
                            m_matrix(NULL)
                        {
                            if( isEnd )
                            {
                                m_data = m_end;
                            }
                        }

                        iterator_impl(MatrixType* m, bool isEnd = false) :
                            m_data(NULL),
                            m_end(NULL),
                            m_curRow(0),
                            m_curColumn(0),
                            m_matrix(m)
                        {
                            if( isEnd )
                            {
                                m_curRow = std::numeric_limits<unsigned int>::max();
                                m_curColumn = std::numeric_limits<unsigned int>::max();
                            }
                        }

                        iterator_impl(const iterator_impl<T, MatrixType>& rhs) :
                            m_data(rhs.m_data),
                            m_end(rhs.m_end),
                            m_curRow(rhs.m_curRow),
                            m_curColumn(rhs.m_curColumn),
                            m_matrix(rhs.m_matrix)
                        {
                        }

                        iterator_impl<T, MatrixType>& operator=(const iterator_impl<T, MatrixType>& rhs)
                        {
                            m_data = rhs.m_data;
                            m_end = rhs.m_end;
                            m_curRow = rhs.m_curRow;
                            m_curColumn = rhs.m_curColumn;
                            m_matrix = rhs.m_matrix;
                            return *this;
                        }

                        reference operator*()
                        {
                            if( m_data )
                            {
                                ASSERTL1(m_data < m_end, "Attempt to dereference matrix iterator after its end.");
                                return *m_data;
                            }
                            else
                            {
                                return (*m_matrix)(m_curRow, m_curColumn);
                            }
                        }

                        const_reference operator*() const
                        {
                            if( m_data )
                            {
                                ASSERTL1(m_data < m_end, "Attempt to dereference matrix iterator after its end.");
                                return *m_data;
                            }
                            else
                            {
                                return (*m_matrix)(m_curRow, m_curColumn);
                            }
                        }

                        /// \brief Prefix increment operator.
                        iterator_impl<T, MatrixType>& operator++()
                        {
                            if( m_data )
                            {
                                ++m_data;
                            }
                            else
                            {
                                boost::tie(m_curRow, m_curColumn) = 
                                    StoragePolicy::Advance(m_matrix->GetRows(), m_matrix->GetColumns(),
                                        m_curRow, m_curColumn, 
                                        m_matrix->GetPolicySpecificDataHolderType());
                            }
                            return *this;
                        }

                        /// \postfix increment operator.
                        iterator_impl<T, MatrixType> operator++(int)
                        {
                            iterator_impl<T, MatrixType> result = *this;
                            ++(*this);
                            return result;
                        }

                        bool operator==(const iterator_impl<T, MatrixType>& rhs)
                        {
                            return m_data == rhs.m_data &&
                                   m_end == rhs.m_end &&
                                   m_curRow == rhs.m_curRow &&
                                   m_curColumn == rhs.m_curColumn &&
                                   m_matrix == rhs.m_matrix;
                        }

                        bool operator!=(const iterator_impl<T, MatrixType>& rhs)
                        {
                            return !(*this == rhs);
                        }

                    private:
                        // Used when the matrix is not transposed
                        T* m_data;
                        T* m_end;

                        // Used when the matrix is transposed.
                        unsigned int m_curRow;
                        unsigned int m_curColumn;
                        MatrixType* m_matrix;
            };

        public:
            NekMatrix() :
                BaseType(0, 0),
                m_data(StoragePolicy::Initialize()),
                m_wrapperType(eCopy),
                m_policySpecificData(),
                m_transpose('N')
            {
            }
            
            NekMatrix(unsigned int rows, unsigned int columns, const PolicySpecificDataHolderType& policySpecificData = PolicySpecificDataHolderType()) :
                BaseType(rows, columns),
                m_data(StoragePolicy::Initialize(rows, columns, policySpecificData)),
                m_wrapperType(eCopy),
                m_policySpecificData(policySpecificData),
                m_transpose('N')
            {
            }

            NekMatrix(unsigned int rows, unsigned int columns, typename boost::call_traits<DataType>::const_reference initValue,
                      const PolicySpecificDataHolderType& policySpecificData = PolicySpecificDataHolderType()) :
                BaseType(rows, columns),
                m_data(StoragePolicy::Initialize(rows, columns, initValue, policySpecificData)),
                m_wrapperType(eCopy),
                m_policySpecificData(policySpecificData),
                m_transpose('N')
            {
            }
            
            NekMatrix(unsigned int rows, unsigned int columns, const DataType* data,
                      const PolicySpecificDataHolderType& policySpecificData = PolicySpecificDataHolderType()) :
                BaseType(rows, columns),
                m_data(StoragePolicy::Initialize(rows, columns, data, policySpecificData)),
                m_wrapperType(eCopy),
                m_policySpecificData(policySpecificData),
                m_transpose('N')
            {
            }
            
            NekMatrix(unsigned int rows, unsigned int columns, const ConstArray<OneD, DataType>& d,
                      const PolicySpecificDataHolderType& policySpecificData = PolicySpecificDataHolderType()) :
                BaseType(rows, columns),
                m_data(StoragePolicy::Initialize(rows, columns, d, policySpecificData)),
                m_wrapperType(eCopy),
                m_policySpecificData(policySpecificData),
                m_transpose('N')
            {
            }
            
            NekMatrix(unsigned int rows, unsigned int columns, const Array<OneD, DataType>& d, PointerWrapper wrapperType = eCopy,
                      const PolicySpecificDataHolderType& policySpecificData = PolicySpecificDataHolderType()) :
                BaseType(rows, columns),
                m_data(wrapperType == eCopy ? StoragePolicy::Initialize(rows, columns, d, policySpecificData) : d),
                m_wrapperType(wrapperType),
                m_policySpecificData(policySpecificData),
                m_transpose('N')
            {
            }
            
            NekMatrix(const ThisType& rhs) :
                BaseType(rhs),
                m_data(),
                m_wrapperType(rhs.m_wrapperType),
                m_policySpecificData(rhs.m_policySpecificData),
                m_transpose(rhs.m_transpose)
            {
                if( m_wrapperType == eCopy )
                {
                    m_data = StoragePolicy::Initialize(this->GetRows(), this->GetColumns(), m_policySpecificData);
                    CopyArray(rhs.m_data, m_data);
                }
                else
                {
                    m_data = rhs.m_data;
                }
            }
            
            #ifdef NEKTAR_USE_EXPRESSION_TEMPLATES
                template<typename ExpressionPolicyType>
                NekMatrix(const Expression<ExpressionPolicyType>& rhs) :
                    BaseType(rhs.GetMetadata().Rows, rhs.GetMetadata().Columns),
                    m_data(),
                    m_wrapperType(eCopy),
                    m_policySpecificData(),
                    m_transpose('N')
                {
                    BOOST_MPL_ASSERT(( boost::is_same<typename Expression<ExpressionPolicyType>::ResultType, NekMatrix<DataType, StorageType, StandardMatrixTag> > ));
                    m_data = StoragePolicy::Initialize(this->GetRows(), this->GetColumns(), m_policySpecificData);
                    rhs.Apply(*this);
                }
            #endif //NEKTAR_USE_EXPRESSION_TEMPLATES

            MatrixStorage GetStorageType() const 
            {
                return static_cast<MatrixStorage>(ConvertToMatrixStorageEnum<StorageType>::Value);
            }
            
            // TODO - Copy constructors from other types of matrices.
            
            ThisType& Assign(unsigned int newRows, unsigned int newColumns, Array<OneD, DataType>& data)
            {
                this->Resize(newRows, newColumns);
                m_wrapperType = eWrapper;
                m_policySpecificData = PolicySpecificDataHolderType();
                m_transpose = 'N';
                m_data = data;
                return *this;
            }
             
            ThisType& operator=(const ThisType& rhs)
            {
                if( this != &rhs )
                {
                    BaseType::operator=(rhs);
                    m_policySpecificData = rhs.m_policySpecificData;
                    if( m_wrapperType == eCopy  )
                    {
                        // If the current vector is a matrix, then regardless of the rhs type 
                        // we just copy over the values, resizing if needed.
                        if( m_data.num_elements() != rhs.m_data.num_elements() )
                        {
                            m_data = Array<OneD, DataType>(rhs.m_data.num_elements());
                        }
                    }
                    else if( m_wrapperType == eWrapper )
                    {
                        // If the current matrix is wrapped, then just copy over the top,
                        // but the sizes of the two matrices must be the same.
                        ASSERTL0(m_data.num_elements() == rhs.m_data.num_elements(), "Wrapped NekMatrices must have the same dimension in operator=");
                    }
    
                    CopyArray(rhs.m_data, m_data);
                    m_transpose = rhs.m_transpose;
                }
                
                return *this;
            }
            
            #ifdef NEKTAR_USE_EXPRESSION_TEMPLATES
                template<typename ExpressionPolicyType>
                ThisType& operator=(const Expression<ExpressionPolicyType>& rhs)
                {
                    BOOST_MPL_ASSERT(( boost::is_same<typename Expression<ExpressionPolicyType>::ResultType, NekMatrix<DataType, StorageType, StandardMatrixTag> > ));
                    m_policySpecificData = PolicySpecificDataHolderType();
                    if( this->GetRows() != rhs.GetMetadata().Rows ||
                        this->GetColumns() != rhs.GetMetadata().Columns )
                    {
                        Resize(rhs.GetMetadata().Rows, rhs.GetMetadata().Columns);
                        m_data = StoragePolicy::Initialize(this->GetRows(), this->GetColumns(), m_policySpecificData);
                    }
                    m_wrapperType = eCopy;
                    m_transpose = 'N';
                    rhs.Apply(*this);
                    return *this;
                }
            #endif //NEKTAR_USE_EXPRESSION_TEMPLATES
            
            ConstGetValueType operator()(unsigned int row, unsigned int column) const
            {
                ASSERTL2(row < this->GetRows(), std::string("Row ") + boost::lexical_cast<std::string>(row) + 
                    std::string(" requested in a matrix with a maximum of ") + boost::lexical_cast<std::string>(this->GetRows()) +
                    std::string(" rows"));
                ASSERTL2(column < this->GetColumns(), std::string("Column ") + boost::lexical_cast<std::string>(column) + 
                    std::string(" requested in a matrix with a maximum of ") + boost::lexical_cast<std::string>(this->GetColumns()) +
                    std::string(" columns"));
                    
                return StoragePolicy::GetValue(this->GetRows(), this->GetColumns(), row, column, m_data, m_transpose, m_policySpecificData);    
            }
            
            GetValueType operator()(unsigned int row, unsigned int column)
            {
                ASSERTL2(row < this->GetRows(), std::string("Row ") + boost::lexical_cast<std::string>(row) + 
                    std::string(" requested in a matrix with a maximum of ") + boost::lexical_cast<std::string>(this->GetRows()) +
                    std::string(" rows"));
                ASSERTL2(column < this->GetColumns(), std::string("Column ") + boost::lexical_cast<std::string>(column) + 
                    std::string(" requested in a matrix with a maximum of ") + boost::lexical_cast<std::string>(this->GetColumns()) +
                    std::string(" columns"));
                    
                return StoragePolicy::GetValue(this->GetRows(), this->GetColumns(), row, column, m_data, m_transpose, m_policySpecificData);    
            }

            void SetValue(unsigned int row, unsigned int column, typename boost::call_traits<DataType>::const_reference d)
            {
                ASSERTL2(row < this->GetRows(), std::string("Row ") + boost::lexical_cast<std::string>(row) + 
                    std::string(" requested in a matrix with a maximum of ") + boost::lexical_cast<std::string>(this->GetRows()) +
                    std::string(" rows"));
                ASSERTL2(column < this->GetColumns(), std::string("Column ") + boost::lexical_cast<std::string>(column) + 
                    std::string(" requested in a matrix with a maximum of ") + boost::lexical_cast<std::string>(this->GetColumns()) +
                    std::string(" columns"));
                StoragePolicy::SetValue(this->GetRows(), this->GetColumns(), row, column, m_data, d, m_transpose, m_policySpecificData);
            }
            
            typename boost::call_traits<DataType>::const_reference GetValue(unsigned int row, unsigned int column) const
            {
                ASSERTL2(row < this->GetRows(), std::string("Row ") + boost::lexical_cast<std::string>(row) + 
                    std::string(" requested in a matrix with a maximum of ") + boost::lexical_cast<std::string>(this->GetRows()) +
                    std::string(" rows"));
                ASSERTL2(column < this->GetColumns(), std::string("Column ") + boost::lexical_cast<std::string>(column) + 
                    std::string(" requested in a matrix with a maximum of ") + boost::lexical_cast<std::string>(this->GetColumns()) +
                    std::string(" columns"));
                return StoragePolicy::GetValue(this->GetRows(), this->GetColumns(), row, column, m_data, m_transpose, m_policySpecificData);
            }
            
            const PolicySpecificDataHolderType& GetPolicySpecificDataHolderType() const 
            {
                return m_policySpecificData;
            }

            Array<OneD, DataType>& GetPtr()
            {
                return m_data;
            }
            
            const ConstArray<OneD, DataType>& GetPtr() const
            {
                return m_data;
            }
            
            DataType Scale() const
            {
                return DataType(1);
            }

            DataType* GetRawPtr()
            {
                return m_data.data();
            }
            
            const DataType* GetRawPtr() const
            {
                return m_data.data();
            }
            
            //typedef DataType* iterator;
            //typedef const DataType* const_iterator;
            //iterator begin() { return m_data.data(); }
            //iterator end() { return m_data.data() + m_data.num_elements(); }
            
            //const_iterator begin() const { return m_data.data(); }
            //const_iterator end() const { return m_data.data() + m_data.num_elements(); }


            typedef iterator_impl<DataType, ThisType> iterator;
            typedef iterator_impl<const DataType, const ThisType> const_iterator;
            
            iterator begin() 
            { 
                if( GetTransposeFlag() == 'N' )
                {
                    return iterator(m_data.data(), m_data.data() + m_data.num_elements());
                }
                else
                {
                    return iterator(this);
                }
            }

            iterator end()
            {
                if( GetTransposeFlag() == 'N' )
                {
                    return iterator(m_data.data(), m_data.data() + m_data.num_elements(), true);
                }
                else
                {
                    return iterator(this, true);
                }
            }

            const_iterator begin() const
            { 
                if( GetTransposeFlag() == 'N' )
                {
                    return const_iterator(m_data.data(), m_data.data() + m_data.num_elements());
                }
                else
                {
                    return const_iterator(this);
                }
            }

            const_iterator end() const
            {
                if( GetTransposeFlag() == 'N' )
                {
                    return const_iterator(m_data.data(), m_data.data() + m_data.num_elements(), true);
                }
                else
                {
                    return const_iterator(this, true);
                }
            }
            
            
            unsigned int GetStorageSize() const
            {
                return m_data.num_elements();
            }
            
            bool operator==(const NekMatrix<DataType, StorageType, StandardMatrixTag>& rhs) const
            {
                if( GetStorageSize() != rhs.GetStorageSize() )
                {
                    return false;
                }
                
                if( this->GetRows() != rhs.GetRows() ||
                    this->GetColumns() != rhs.GetColumns() )
                {
                    return false;
                }

                if( GetTransposeFlag() == rhs.GetTransposeFlag() )
                {
                    return std::equal(begin(), end(), rhs.begin());
                }
                else
                {
                    for(unsigned int i = 0; i < this->GetRows(); ++i)
                    {
                        for(unsigned int j = 0; j < this->GetColumns(); ++j)
                        {
                            if( (*this)(i,j) != rhs(i,j) )
                            {
                                return false;
                            }
                        }
                    }
                }

                return true;
            }
            
            void Invert()
            {
                StoragePolicy::Invert(this->GetRows(), this->GetColumns(), m_data, m_transpose, m_policySpecificData);
            }
            
            char GetTransposeFlag() const 
            {
                return m_transpose;
            }

            void Transpose() 
            {
                this->PerformRowColumnInterchange();
                if( m_transpose == 'N' )
                {
                    m_transpose = 'T';
                }
                else
                {
                    m_transpose = 'N';
                }
            }

            unsigned int GetLeadingDimension() const
            {
                if( m_transpose == 'N' )
                {
                    return this->GetRows();
                }
                else
                {
                    return this->GetColumns();
                }
            }

       

        protected:
            
            
        private:
            virtual typename boost::call_traits<DataType>::value_type v_GetValue(unsigned int row, unsigned int column) const 
            {
                return ThisType::operator()(row, column);
            }
            
            virtual unsigned int v_GetStorageSize() const 
            {
                return ThisType::GetStorageSize();
            }
            
            virtual MatrixStorage v_GetStorageType() const
            {
                return ThisType::GetStorageType();
            }
            
            virtual void v_SetValue(unsigned int row, unsigned int column, typename boost::call_traits<DataType>::const_reference d)
            {
                return ThisType::SetValue(row, column, d);
            }
            
            Array<OneD, DataType> m_data;
            PointerWrapper m_wrapperType;
            PolicySpecificDataHolderType m_policySpecificData;
            char m_transpose;
    };

    template<typename DataType, typename StorageType>
    NekMatrix<DataType, StorageType, StandardMatrixTag>
    Transpose(NekMatrix<DataType, StorageType, StandardMatrixTag>& rhs)
    {
        NekMatrix<DataType, StorageType, StandardMatrixTag> result(rhs.GetRows(), rhs.GetColumns(), 
            rhs.GetPtr(), eWrapper, rhs.GetPolicySpecificDataHolderType());
        result.Transpose();
        return result;
    }
}


#endif //NEKTAR_LIB_UTILITIES_LINEAR_ALGEBRA_STANDARD_MATRIX_HPP
