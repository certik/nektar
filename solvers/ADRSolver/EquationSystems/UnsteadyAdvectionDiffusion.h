#ifndef NEKTAR_SOLVERS_ADRSOLVER_EQUATIONSYSTEMS_UNSTEADYADVECTIONDIFFUSION_H
#define NEKTAR_SOLVERS_ADRSOLVER_EQUATIONSYSTEMS_UNSTEADYADVECTIONDIFFUSION_H

#include <Auxiliary/UnsteadySystem.h>

namespace Nektar
{
    class UnsteadyAdvectionDiffusion : public UnsteadySystem
    {
    public:
        friend class MemoryManager<UnsteadyAdvectionDiffusion>;

        /// Creates an instance of this class
        static EquationSystemSharedPtr create(
                const LibUtilities::CommSharedPtr& pComm,
                const LibUtilities::SessionReaderSharedPtr& pSession) {
            EquationSystemSharedPtr p = MemoryManager<UnsteadyAdvectionDiffusion>::AllocateSharedPtr(pComm, pSession);
            p->InitObject();
            return p;
        }
        /// Name of class
        static std::string className;

        virtual ~UnsteadyAdvectionDiffusion();

    protected:
        UnsteadyAdvectionDiffusion(
                const LibUtilities::CommSharedPtr& pComm,
                const LibUtilities::SessionReaderSharedPtr& pSession);

        virtual void DoOdeRhs(const Array<OneD, const  Array<OneD, NekDouble> >&inarray,
                          Array<OneD,        Array<OneD, NekDouble> >&outarray,
                    const NekDouble time);

        virtual void DoImplicitSolve(const Array<OneD, const Array<OneD,      NekDouble> >&inarray,
                      Array<OneD, Array<OneD, NekDouble> >&outarray,
                      NekDouble time,
                      NekDouble lambda);

        virtual void v_InitObject();

        virtual void v_GetFluxVector(const int i, Array<OneD, Array<OneD, NekDouble> > &physfield, Array<OneD, Array<OneD, NekDouble> > &flux);

        virtual void v_NumericalFlux(Array<OneD, Array<OneD, NekDouble> > &physfield, Array<OneD, Array<OneD, NekDouble> > &numflux);

        // Print Summary
        virtual void v_PrintSummary(std::ostream &out);

    private:
        NekDouble m_waveFreq;
        NekDouble m_epsilon;
        Array<OneD, Array<OneD, NekDouble> > m_velocity;
    };
}

#endif
