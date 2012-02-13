#ifndef NEKTAR_SOLVERS_ADRSOLVER_EQUATIONSYSTEMS_TENTUSSCHER_PANFILOV_2006_EPI_CELL_H
#define NEKTAR_SOLVERS_ADRSOLVER_EQUATIONSYSTEMS_TENTUSSCHER_PANFILOV_2006_EPI_CELL_H

//! @file
//! 
//! This source file was generated from CellML.
//! 
//! Model: tentusscher_panfilov_2006_epi_cell
//! 
//! Processed by pycml - CellML Tools in Python
//!     (translators: 12434, pycml: 12383, optimize: 12408)
//! on Mon Feb 13 10:21:29 2012
//! 
//! <autogenerated>

#include <CardiacEPSolver/CellModels/CellModel.h>
namespace Nektar
{
    class TenTusscher06Epi : public CellModel
    {

    public:
        /// Creates an instance of this class
        static CellModelSharedPtr create(const LibUtilities::SessionReaderSharedPtr& pSession, const MultiRegions::ExpListSharedPtr& pField)
        {
            return MemoryManager<TenTusscher06Epi>::AllocateSharedPtr(pSession, pField);
        }

        /// Name of class
        static std::string className;
        /// Constructor
        TenTusscher06Epi(const LibUtilities::SessionReaderSharedPtr& pSession, const MultiRegions::ExpListSharedPtr& pField);
        /// Desctructor
        virtual ~TenTusscher06Epi() {}

    protected:
        virtual void v_Update(
               const Array<OneD, const  Array<OneD, NekDouble> >&inarray,
                     Array<OneD,        Array<OneD, NekDouble> >&outarray,
               const NekDouble time);

        /// Prints a summary of the model parameters.
        virtual void v_PrintSummary(std::ostream &out);

        virtual void v_SetInitialConditions();
};


}

#endif
