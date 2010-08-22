#ifndef NEKTAR_SOLVERS_ADRSOLVER_EQUATIONSYSTEMS_UNSTEADYINVISCIDBURGER_H
#define NEKTAR_SOLVERS_ADRSOLVER_EQUATIONSYSTEMS_UNSTEADYINVISCIDBURGER_H

#include <ADRSolver/EquationSystems/UnsteadySystem.h>

namespace Nektar
{
    class UnsteadyInviscidBurger : public UnsteadySystem
    {
    public:
        /// Creates an instance of this class
        static EquationSystemSharedPtr create(SessionReaderSharedPtr& pSession) {
            return MemoryManager<UnsteadyInviscidBurger>::AllocateSharedPtr(pSession);
        }
        /// Name of class
        static std::string className;

        UnsteadyInviscidBurger(SessionReaderSharedPtr& pSession);
        virtual ~UnsteadyInviscidBurger();

    protected:
        void DoOdeRhs(
                const Array<OneD, const  Array<OneD, NekDouble> >&inarray,
                      Array<OneD,        Array<OneD, NekDouble> >&outarray,
                const NekDouble time);

        void DoOdeProjection(const Array<OneD,  const  Array<OneD, NekDouble> > &inarray,
                          Array<OneD,  Array<OneD, NekDouble> > &outarray,
                          const NekDouble time);

        virtual void v_GetFluxVector(const int i, Array<OneD, Array<OneD, NekDouble> > &physfield, Array<OneD, Array<OneD, NekDouble> > &flux);
        virtual void v_GetFluxVector(const int i, const int j, Array<OneD, Array<OneD, NekDouble> > &physfield, Array<OneD, Array<OneD, NekDouble> > &flux);

        virtual void v_NumericalFlux(Array<OneD, Array<OneD, NekDouble> > &physfield, Array<OneD, Array<OneD, NekDouble> > &numflux);

    private:
        NekDouble m_waveFreq;
        Array<OneD, Array<OneD, NekDouble> > m_velocity;

    };
}

#endif