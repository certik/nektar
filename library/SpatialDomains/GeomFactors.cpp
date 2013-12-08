////////////////////////////////////////////////////////////////////////////////
//
//  File: GeomFactors.cpp
//
//  For more information, please see: http://www.nektar.info/
//
//  The MIT License
//
//  Copyright (c) 2006 Division of Applied Mathematics, Brown University (USA),
//  Department of Aeronautics, Imperial College London (UK), and Scientific
//  Computing and Imaging Institute, University of Utah (USA).
//
//  License for the specific language governing rights and limitations under
//  Permission is hereby granted, free of charge, to any person obtaining a
//  copy of this software and associated documentation files (the "Software"),
//  to deal in the Software without restriction, including without limitation
//  the rights to use, copy, modify, merge, publish, distribute, sublicense,
//  and/or sell copies of the Software, and to permit persons to whom the
//  Software is furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included
//  in all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
//  OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
//  THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
//  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
//  DEALINGS IN THE SOFTWARE.
//
//  Description: Geometric factors base class.
//
////////////////////////////////////////////////////////////////////////////////

#include <SpatialDomains/GeomFactors.h>
#include <LibUtilities/Foundations/Interp.h>

namespace Nektar
{
    namespace SpatialDomains
    {
        /**
         * @class GeomFactors
         *
         * This class stores the various geometric factors associated with a
         * specific element, necessary for fundamental integration and
         * differentiation operations as well as edge and surface normals.
         * Dimension-specific versions of this class (GeomFactors1D,
         * GeomFactors2D and GeomFactors3D) provide the majority of
         * implementation.
         *
         * Initially, these algorithms are provided with a mapping from the
         * reference region element to the physical element. Practically, this
         * is represented using a corresponding reference region element for
         * each coordinate component. Note that for straight-sided elements,
         * these elements will be of linear order. Curved elements are
         * represented using higher-order coordinate mappings. This geometric
         * order is in contrast to the order of the spectral/hp expansion order
         * on the element.
         *
         * For application of the chain rule during differentiation we require
         * the partial derivatives \f[\frac{\partial \xi_i}{\partial \chi_j}\f]
         * evaluated at the physical points of the expansion basis. We also
         * construct the inverse metric tensor \f$g^{ij}\f$ which, in the case
         * of a domain embedded in a higher-dimensional space, supports the
         * conversion of covariant quantities to contravariant quantities.
         * When the expansion dimension is equal to the coordinate dimension the
         * Jacobian of the mapping \f$\chi_j\f$ is a square matrix and
         * consequently the required terms are the entries of the inverse of the
         * Jacobian. However, in general this is not the case, so we therefore
         * implement the construction of these terms following the derivation
         * in Cantwell, et. al. \cite CaYaKiPeSh13. Given the coordinate maps
         * \f$\chi_i\f$, this comprises of five steps
         * -# Compute the terms of the Jacobian \f$\frac{\partial \chi_i}{\partial \xi_j}\f$.
         * -# Compute the metric tensor \f$g_{ij}=\mathbf{t}_{(i)}\cdot\mathbf{t}_{(j)}\f$.
         * -# Compute the square of the Jacobian determinant \f$g=|\mathbf{g}|\f$.
         * -# Compute the inverse metric tensor \f$g^{ij}\f$.
         * -# Compute the terms \f$\frac{\partial \xi_i}{\partial \chi_j}\f$.
         *
         * @see GeomFactors1D, GeomFactors2D, GeomFactors3D
         */

        /**
         * This constructor is protected since only dimension-specific
         * GeomFactors classes should be instantiated externally.
         * @param   gtype       Specified whether the geometry is regular or
         *                      deformed.
         * @param   expdim      Specifies the dimension of the expansion.
         * @param   coordim     Specifies the dimension of the coordinate
         *                      system
         */
        GeomFactors::GeomFactors(const GeomType gtype,
                const int coordim,
                const Array<OneD, const StdRegions
                    ::StdExpansionSharedPtr> &Coords,
                const Array<OneD, const LibUtilities::BasisSharedPtr>
                    &tbasis) :
            m_type(gtype),
            m_expDim(Coords[0]->GetShapeDimension()),
            m_coordDim(coordim),
            m_valid(true),
            m_coords(Coords),
            m_pointsKey(m_expDim)
        {
            for (int i = 0; i < m_expDim; ++i)
            {
                m_pointsKey[i] = tbasis[i]->GetPointsKey();
            }
            CheckIfValid();
        }


        /**
         * @param   S           An instance of a GeomFactors class from which
         *                      to construct a new instance.
         */
        GeomFactors::GeomFactors(const GeomFactors &S) :
            m_type(S.m_type),
            m_expDim(S.m_expDim),
            m_coordDim(S.m_coordDim),
            m_valid(S.m_valid),
            m_coords(S.m_coords),
            m_pointsKey(S.m_pointsKey)
        {
        }


        /**
         *
         */
        GeomFactors::~GeomFactors()
        {
        }


        /**
         * Member data equivalence is tested in the following order: shape type,
         * expansion dimension, coordinate dimension, points-keys, determinant
         * of Jacobian matrix, Laplacian coefficients.
         */
        bool operator==(const GeomFactors &lhs, const GeomFactors &rhs)
        {
            if(!(lhs.m_type == rhs.m_type))
            {
                return false;
            }

            if(!(lhs.m_expDim == rhs.m_expDim))
            {
                return false;
            }

            if(!(lhs.m_coordDim == rhs.m_coordDim))
            {
                return false;
            }

            for(int i = 0; i < lhs.m_expDim; i++)
            {
                if(!(lhs.m_pointsKey[i] == rhs.m_pointsKey[i]))
                {
                    return false;
                }
            }

            return true;
        }

        DerivStorage GeomFactors::GetDeriv(const LibUtilities::PointsKeyVector &tpoints) const
        {
            ASSERTL1(tpoints.size() == m_expDim,
                     "Dimension of target basis does not match expansion basis.");

            int i = 0, j = 0;
            int nqtot_map      = 1;
            int nqtot_tbasis   = 1;
            DerivStorage deriv = DerivStorage(m_expDim);
            DerivStorage d_map = DerivStorage(m_expDim);
            LibUtilities::PointsKeyVector map_points(m_expDim);

            // Allocate storage and compute number of points
            for (i = 0; i < m_expDim; ++i)
            {
                map_points[i]  = m_coords[0]->GetBasis(i)->GetPointsKey();
                nqtot_map     *= map_points[i].GetNumPoints();
                nqtot_tbasis  *= tpoints[i].GetNumPoints();
                deriv[i] = Array<OneD, Array<OneD,NekDouble> >(m_coordDim);
                d_map[i] = Array<OneD, Array<OneD,NekDouble> >(m_coordDim);
            }

            // Calculate local derivatives
            for(i = 0; i < m_coordDim; ++i)
            {
                // Transform from coefficient space to physical space
                m_coords[i]->BwdTrans(m_coords[i]->GetCoeffs(),
                                      m_coords[i]->UpdatePhys());

                // Allocate storage and take the derivative (calculated at the
                // points as specified in 'Coords')
                for (j = 0; j < m_expDim; ++j)
                {
                    d_map[j][i] = Array<OneD,NekDouble>(nqtot_map);
                    deriv[j][i] = Array<OneD,NekDouble>(nqtot_tbasis);
                    m_coords[i]->StdPhysDeriv(j, m_coords[i]->GetPhys(),
                                                 d_map[j][i]);
                }
            }

            for (i = 0; i < m_coordDim; ++i)
            {
                // Interpolate the derivatives:
                // - from the points as defined in the mapping ('Coords')
                // - to the points we at which we want to know the metrics
                //   ('tbasis')
                bool same = true;
                for (j = 0; j < m_expDim; ++j)
                {
                    same = same && (map_points[j] == tpoints[j]);
                }
                if( same )
                {
                    for (j = 0; j < m_expDim; ++j)
                    {
                        deriv[j][i] = d_map[j][i];
                    }
                }
                else
                {
                    for (j = 0; j < m_expDim; ++j)
                    {
                        Interp(map_points, d_map[j][i], tpoints, deriv[j][i]);
                    }
                }
            }

            return deriv;
        }


        /**
         * This routine returns an array of values specifying the Jacobian
         * of the mapping at quadrature points in the element. The array
         * is either of size 1 in the case of elements having #GeomType
         * #eRegular, or of size equal to the number of quadrature points for
         * #eDeformed elements.
         *
         * @returns             Array containing the Jacobian of the coordinate
         *                      mapping at the quadrature points of the element.
         * @see                 GeomType
         */
        const Array<OneD, const NekDouble> GeomFactors::GetJac(
                const LibUtilities::PointsKeyVector &keyTgt)
        {
            std::map<LibUtilities::PointsKeyVector, Array<OneD, NekDouble> >::const_iterator x;
            if ((x=m_jacCache.find(keyTgt)) != m_jacCache.end())
            {
                return x->second;
            }

            int i = 0, j = 0, k = 0, l = 0;
            int ptsTgt   = 1;

            // Allocate storage and compute number of points
            for (i = 0; i < m_expDim; ++i)
            {
                ptsTgt   *= keyTgt[i].GetNumPoints();
            }

            // Get derivative at geometry points
            DerivStorage deriv = GetDeriv(keyTgt);

            Array<TwoD, NekDouble> tmp (m_expDim*m_expDim, ptsTgt, 0.0);
            Array<TwoD, NekDouble> gmat(m_expDim*m_expDim, ptsTgt, 0.0);
            Array<OneD, NekDouble> jac (ptsTgt, 0.0);

            // Compute g_{ij} as t_i \cdot t_j and store in tmp
            for (i = 0, l = 0; i < m_expDim; ++i)
            {
                for (j = 0; j < m_expDim; ++j, ++l)
                {
                    for (k = 0; k < m_coordDim; ++k)
                    {
                        Vmath::Vvtvp(ptsTgt, &deriv[i][k][0], 1,
                                             &deriv[j][k][0], 1,
                                             &tmp[l][0],      1,
                                             &tmp[l][0],      1);
                    }
                }
            }

            Adjoint(tmp, gmat);

            // Compute g = det(g_{ij}) (= Jacobian squared) and store
            // temporarily in m_jac.
            for (i = 0; i < m_expDim; ++i)
            {
                Vmath::Vvtvp(ptsTgt, &tmp[i][0], 1, &gmat[i*m_expDim][0], 1,
                                     &jac[0], 1, &jac[0], 1);
            }

            // Compute the Jacobian = sqrt(g)
            Vmath::Vsqrt(ptsTgt, &jac[0], 1, &jac[0], 1);

            m_jacCache[keyTgt] = jac;

            return jac;
        }


        /**
         * This routine returns a two-dimensional array of values specifying
         * the inverse metric terms associated with the coordinate mapping of
         * the corresponding reference region to the physical element. These
         * terms correspond to the \f$g^{ij}\f$ terms in \cite CaYaKiPeSh13 and,
         * in the case of an embedded manifold, map covariant quantities to
         * contravariant quantities. The leading index of the array is the index
         * of the term in the tensor numbered as
         * \f[\left(\begin{array}{ccc}
         *    0 & 1 & 2 \\
         *    1 & 3 & 4 \\
         *    2 & 4 & 5
         * \end{array}\right)\f].
         * The second dimension is either of size 1 in the case of elements
         * having #GeomType #eRegular, or of size equal to the number of
         * quadrature points for #eDeformed elements.
         *
         * @see [Wikipedia "Covariance and Contravariance of Vectors"]
         *      (http://en.wikipedia.org/wiki/Covariance_and_contravariance_of_vectors)
         * @returns             Two-dimensional array containing the inverse
         *                      metric tensor of the coordinate mapping.
         */
        const Array<TwoD, const NekDouble> GeomFactors::GetGmat(
                const LibUtilities::PointsKeyVector &keyTgt) const
        {
            int i = 0, j = 0, k = 0, l = 0;
            int ptsTgt   = 1;

            // Allocate storage and compute number of points
            for (i = 0; i < m_expDim; ++i)
            {
                ptsTgt   *= keyTgt[i].GetNumPoints();
            }

            // Get derivative at geometry points
            DerivStorage deriv = GetDeriv(keyTgt);

            Array<TwoD, NekDouble> tmp (m_expDim*m_expDim, ptsTgt, 0.0);
            Array<TwoD, NekDouble> gmat(m_expDim*m_expDim, ptsTgt, 0.0);
            Array<OneD, NekDouble> jac (ptsTgt, 0.0);

            // Compute g_{ij} as t_i \cdot t_j and store in tmp
            for (i = 0, l = 0; i < m_expDim; ++i)
            {
                for (j = 0; j < m_expDim; ++j, ++l)
                {
                    for (k = 0; k < m_coordDim; ++k)
                    {
                        Vmath::Vvtvp(ptsTgt, &deriv[i][k][0], 1,
                                             &deriv[j][k][0], 1,
                                             &tmp[l][0],      1,
                                             &tmp[l][0],      1);
                    }
                }
            }

            Adjoint(tmp, gmat);

            // Compute g = det(g_{ij}) (= Jacobian squared) and store
            // temporarily in m_jac.
            for (i = 0; i < m_expDim; ++i)
            {
                Vmath::Vvtvp(ptsTgt, &tmp[i][0], 1, &gmat[i*m_expDim][0], 1,
                                     &jac[0], 1, &jac[0], 1);
            }

            for (i = 0; i < m_expDim*m_expDim; ++i)
            {
                Vmath::Vdiv(ptsTgt, &gmat[i][0], 1, &jac[0], 1, &gmat[i][0], 1);
            }

            return gmat;
        }


        /// Return the derivative factors matrix.
        const Array<TwoD, const NekDouble> GeomFactors::GetDerivFactors(
                const LibUtilities::PointsKeyVector& keyTgt)
        {
            std::map<LibUtilities::PointsKeyVector, Array<TwoD, NekDouble> >::const_iterator x;
            if ((x=m_derivFactorCache.find(keyTgt)) != m_derivFactorCache.end())
            {
                return x->second;
            }

            int i = 0, j = 0, k = 0, l = 0;
            int ptsTgt   = 1;

            // Allocate storage and compute number of points
            for (i = 0; i < m_expDim; ++i)
            {
                ptsTgt   *= keyTgt[i].GetNumPoints();
            }

            // Get derivative at geometry points
            DerivStorage deriv = GetDeriv(keyTgt);

            Array<TwoD, NekDouble> tmp (m_expDim*m_expDim, ptsTgt, 0.0);
            Array<TwoD, NekDouble> gmat(m_expDim*m_expDim, ptsTgt, 0.0);
            Array<OneD, NekDouble> jac (ptsTgt, 0.0);
            Array<TwoD, NekDouble> factors(m_expDim*m_coordDim, ptsTgt, 0.0);

            // Compute g_{ij} as t_i \cdot t_j and store in tmp
            for (i = 0, l = 0; i < m_expDim; ++i)
            {
                for (j = 0; j < m_expDim; ++j, ++l)
                {
                    for (k = 0; k < m_coordDim; ++k)
                    {
                        Vmath::Vvtvp(ptsTgt, &deriv[i][k][0], 1,
                                             &deriv[j][k][0], 1,
                                             &tmp[l][0],      1,
                                             &tmp[l][0],      1);
                    }
                }
            }

            Adjoint(tmp, gmat);

            // Compute g = det(g_{ij}) (= Jacobian squared) and store
            // temporarily in m_jac.
            for (i = 0; i < m_expDim; ++i)
            {
                Vmath::Vvtvp(ptsTgt, &tmp[i][0], 1, &gmat[i*m_expDim][0], 1,
                                     &jac[0], 1, &jac[0], 1);
            }

            for (i = 0; i < m_expDim*m_expDim; ++i)
            {
                Vmath::Vdiv(ptsTgt, &gmat[i][0], 1, &jac[0], 1, &gmat[i][0], 1);
            }

            // Compute the Jacobian = sqrt(g)
            Vmath::Vsqrt(ptsTgt, &jac[0], 1, &jac[0], 1);

            // Compute the derivative factors
            for (k = 0, l = 0; k < m_coordDim; ++k)
            {
                for (j = 0; j < m_expDim; ++j, ++l)
                {
                    for (i = 0; i < m_expDim; ++i)
                    {
                        Vmath::Vvtvp(ptsTgt, &deriv[i][k][0],        1,
                                          &gmat[m_expDim*i+j][0], 1,
                                          &factors[l][0],    1,
                                          &factors[l][0],    1);
                    }
                }
            }

            m_derivFactorCache[keyTgt] = factors;

            return factors;
        }

        void GeomFactors::CheckIfValid()
        {
            switch (m_expDim)
            {
                case 1:
                {
                    m_valid = true;
                    break;
                }
                case 2:
                {
                    // Jacobian test only makes sense in 2D coordinates
                    if (GetCoordim() != 2)
                    {
                        return;
                    }

                    LibUtilities::PointsKeyVector p(m_expDim);
                    p[0] = m_coords[0]->GetBasis(0)->GetPointsKey();
                    p[1] = m_coords[0]->GetBasis(1)->GetPointsKey();
                    int nqtot = p[0].GetNumPoints() *
                                p[1].GetNumPoints();
                    int pts = (m_type == eRegular || m_type == eMovingRegular)
                                    ? 1 : nqtot;

                    DerivStorage deriv = GetDeriv(p);

                    Array<OneD, NekDouble> jac(pts, 0.0);
                    Vmath::Vvtvvtm(pts, &deriv[0][0][0], 1, &deriv[1][1][0], 1,
                                        &deriv[1][0][0], 1, &deriv[0][1][0], 1,
                                        &jac[0],           1);

                    if(Vmath::Vmin(pts, &jac[0], 1) < 0)
                    {
                        m_valid = false;
                    }
                    break;
                }
                case 3:
                {
                    LibUtilities::PointsKeyVector p(m_expDim);
                    p[0] = m_coords[0]->GetBasis(0)->GetPointsKey();
                    p[1] = m_coords[0]->GetBasis(1)->GetPointsKey();
                    p[2] = m_coords[0]->GetBasis(2)->GetPointsKey();
                    int nqtot = p[0].GetNumPoints() *
                                p[1].GetNumPoints() *
                                p[2].GetNumPoints();
                    int pts = (m_type == eRegular || m_type == eMovingRegular)
                                    ? 1 : nqtot;

                    Array<OneD, NekDouble> jac(pts, 0.0);
                    Array<OneD, NekDouble> tmp(pts, 0.0);
                    DerivStorage deriv = GetDeriv(p);

                    // J3D - Spencers book page 158
                    Vmath::Vvtvvtm(pts, &deriv[1][1][0], 1, &deriv[2][2][0], 1,
                                        &deriv[2][1][0], 1, &deriv[1][2][0], 1,
                                        &tmp[0],           1);
                    Vmath::Vvtvp  (pts, &deriv[0][0][0], 1, &tmp[0],           1,
                                        &jac[0],           1, &jac[0],           1);

                    Vmath::Vvtvvtm(pts, &deriv[2][1][0], 1, &deriv[0][2][0], 1,
                                        &deriv[0][1][0], 1, &deriv[2][2][0], 1,
                                        &tmp[0],           1);
                    Vmath::Vvtvp  (pts, &deriv[1][0][0], 1, &tmp[0],           1,
                                        &jac[0],           1, &jac[0],           1);

                    Vmath::Vvtvvtm(pts, &deriv[0][1][0], 1, &deriv[1][2][0], 1,
                                        &deriv[1][1][0], 1, &deriv[0][2][0], 1,
                                        &tmp[0],           1);
                    Vmath::Vvtvp  (pts, &deriv[2][0][0], 1, &tmp[0],           1,
                                        &jac[0],           1, &jac[0],           1);

                    if (Vmath::Vmin(pts, &jac[0], 1) < 0)
                    {
                        m_valid = false;
                    }
                    break;
                }
            }
        }

        void GeomFactors::Interp(
                    const LibUtilities::PointsKeyVector &map_points,
                    const Array<OneD, const NekDouble> &src,
                    const LibUtilities::PointsKeyVector &tpoints,
                    Array<OneD, NekDouble> &tgt) const
        {
            switch (m_expDim)
            {
                case 1:
                    LibUtilities::Interp1D(map_points[0], src, tpoints[0], tgt);
                    break;
                case 2:
                    LibUtilities::Interp2D(map_points[0], map_points[1], src,
                                           tpoints[0], tpoints[1], tgt);
                    break;
                case 3:
                    LibUtilities::Interp3D(map_points[0], map_points[1], map_points[2],
                                           src,
                                           tpoints[0], tpoints[1], tpoints[2], tgt);
                    break;
            }
        }

        void GeomFactors::Adjoint(
                    const Array<TwoD, const NekDouble>& src,
                    Array<TwoD, NekDouble>& tgt) const
        {
            int n = src[0].num_elements();
            switch (m_expDim)
            {
                case 1:
                    Vmath::Fill(n, 1.0, &tgt[0][0], 1);
                    break;
                case 2:
                    Vmath::Vcopy(n, &src[3][0], 1, &tgt[0][0], 1);
                    Vmath::Smul (n, -1.0, &src[1][0], 1, &tgt[1][0], 1);
                    Vmath::Smul (n, -1.0, &src[2][0], 1, &tgt[2][0], 1);
                    Vmath::Vcopy(n, &src[0][0], 1, &tgt[3][0], 1);
                    break;
                case 3:
                {
                    int a, b, c, d, e, i, j;

                    // Compute g^{ij} by computing Cofactors(g_ij)^T
                    for (i = 0; i < m_expDim; ++i)
                    {
                        for (j = 0; j < m_expDim; ++j)
                        {
                            a = ((i+1)%m_expDim)*m_expDim + ((j+1)%m_expDim);
                            b = ((i+1)%m_expDim)*m_expDim + ((j+2)%m_expDim);
                            c = ((i+2)%m_expDim)*m_expDim + ((j+1)%m_expDim);
                            d = ((i+2)%m_expDim)*m_expDim + ((j+2)%m_expDim);
                            e = j*m_expDim + i;
                            Vmath::Vvtvvtm(n, &src[a][0], 1, &src[d][0], 1,
                                                &src[b][0], 1, &src[c][0], 1,
                                                &tgt[e][0], 1);
                        }
                    }
                    break;
                }
            }
        }

    }; //end of namespace
}; //end of namespace
