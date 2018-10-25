#ifndef BOOST_GEOMETRY_PROJECTIONS_LCC_HPP
#define BOOST_GEOMETRY_PROJECTIONS_LCC_HPP

// Boost.Geometry - extensions-gis-projections (based on PROJ4)
// This file is automatically generated. DO NOT EDIT.

// Copyright (c) 2008-2015 Barend Gehrels, Amsterdam, the Netherlands.

// This file was modified by Oracle on 2017.
// Modifications copyright (c) 2017, Oracle and/or its affiliates.
// Contributed and/or modified by Adam Wulkiewicz, on behalf of Oracle.

// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// This file is converted from PROJ4, http://trac.osgeo.org/proj
// PROJ4 is originally written by Gerald Evenden (then of the USGS)
// PROJ4 is maintained by Frank Warmerdam
// PROJ4 is converted to Boost.Geometry by Barend Gehrels

// Last updated version of proj: 4.9.1

// Original copyright notice:

// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included
// in all copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
// OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.

#include <boost/geometry/util/math.hpp>
#include <boost/math/special_functions/hypot.hpp>

#include <boost/geometry/srs/projections/impl/base_static.hpp>
#include <boost/geometry/srs/projections/impl/base_dynamic.hpp>
#include <boost/geometry/srs/projections/impl/projects.hpp>
#include <boost/geometry/srs/projections/impl/factory_entry.hpp>
#include <boost/geometry/srs/projections/impl/pj_msfn.hpp>
#include <boost/geometry/srs/projections/impl/pj_phi2.hpp>
#include <boost/geometry/srs/projections/impl/pj_tsfn.hpp>


namespace boost { namespace geometry
{

namespace srs { namespace par4
{
    struct lcc {};

}} //namespace srs::par4

namespace projections
{
    #ifndef DOXYGEN_NO_DETAIL
    namespace detail { namespace lcc
    {
            static const double EPS10 = 1.e-10;

            template <typename T>
            struct par_lcc
            {
                T   phi1;
                T   phi2;
                T   n;
                T   rho0;
                T   c;
                int ellips;
            };

            // template class, using CRTP to implement forward/inverse
            template <typename CalculationType, typename Parameters>
            struct base_lcc_ellipsoid : public base_t_fi<base_lcc_ellipsoid<CalculationType, Parameters>,
                     CalculationType, Parameters>
            {

                typedef CalculationType geographic_type;
                typedef CalculationType cartesian_type;

                par_lcc<CalculationType> m_proj_parm;

                inline base_lcc_ellipsoid(const Parameters& par)
                    : base_t_fi<base_lcc_ellipsoid<CalculationType, Parameters>,
                     CalculationType, Parameters>(*this, par) {}

                // FORWARD(e_forward)  ellipsoid & spheroid
                // Project coordinates from geographic (lon, lat) to cartesian (x, y)
                inline void fwd(geographic_type& lp_lon, geographic_type& lp_lat, cartesian_type& xy_x, cartesian_type& xy_y) const
                {
                    static const CalculationType FORTPI = detail::FORTPI<CalculationType>();
                    static const CalculationType HALFPI = detail::HALFPI<CalculationType>();

                    CalculationType rho;
                    if (fabs(fabs(lp_lat) - HALFPI) < EPS10) {
                        if ((lp_lat * this->m_proj_parm.n) <= 0.)
                            BOOST_THROW_EXCEPTION( projection_exception(-20) );
                        rho = 0.;
                    } else
                        rho = this->m_proj_parm.c * (this->m_proj_parm.ellips ? pow(pj_tsfn(lp_lat, sin(lp_lat),
                            this->m_par.e), this->m_proj_parm.n) : pow(tan(FORTPI + .5 * lp_lat), -this->m_proj_parm.n));
                    xy_x = this->m_par.k0 * (rho * sin( lp_lon *= this->m_proj_parm.n ) );
                    xy_y = this->m_par.k0 * (this->m_proj_parm.rho0 - rho * cos(lp_lon) );
                }

                // INVERSE(e_inverse)  ellipsoid & spheroid
                // Project coordinates from cartesian (x, y) to geographic (lon, lat)
                inline void inv(cartesian_type& xy_x, cartesian_type& xy_y, geographic_type& lp_lon, geographic_type& lp_lat) const
                {
                    static const CalculationType HALFPI = detail::HALFPI<CalculationType>();

                    CalculationType rho;
                    xy_x /= this->m_par.k0;
                    xy_y /= this->m_par.k0;
                    if( (rho = boost::math::hypot(xy_x, xy_y = this->m_proj_parm.rho0 - xy_y)) != 0.0) {
                        if (this->m_proj_parm.n < 0.) {
                            rho = -rho;
                            xy_x = -xy_x;
                            xy_y = -xy_y;
                        }
                        if (this->m_proj_parm.ellips) {
                            if ((lp_lat = pj_phi2(pow(rho / this->m_proj_parm.c, 1./this->m_proj_parm.n), this->m_par.e))
                                == HUGE_VAL)
                                BOOST_THROW_EXCEPTION( projection_exception(-20) );
                        } else
                            lp_lat = 2. * atan(pow(this->m_proj_parm.c / rho, 1./this->m_proj_parm.n)) - HALFPI;
                        lp_lon = atan2(xy_x, xy_y) / this->m_proj_parm.n;
                    } else {
                        lp_lon = 0.;
                        lp_lat = this->m_proj_parm.n > 0. ? HALFPI : -HALFPI;
                    }
                }

                // SPECIAL(fac)
                #ifdef SPECIAL_FACTORS_NOT_CONVERTED
                inline void fac(Geographic lp, Factors &fac) const
                {
                    static const CalculationType FORTPI = detail::FORTPI<CalculationType>();
                    static const CalculationType HALFPI = detail::HALFPI<CalculationType>();

                    CalculationType rho;
                    if (fabs(fabs(lp_lat) - HALFPI) < EPS10) {
                        if ((lp_lat * this->m_proj_parm.n) <= 0.) return;
                        rho = 0.;
                    } else
                        rho = this->m_proj_parm.c * (this->m_proj_parm.ellips ? pow(pj_tsfn(lp_lat, sin(lp_lat),
                            this->m_par.e), this->m_proj_parm.n) : pow(tan(FORTPI + .5 * lp_lat), -this->m_proj_parm.n));
                    this->m_fac.code |= IS_ANAL_HK + IS_ANAL_CONV;
                    this->m_fac.k = this->m_fac.h = this->m_par.k0 * this->m_proj_parm.n * rho /
                        pj_msfn(sin(lp_lat), cos(lp_lat), this->m_par.es);
                    this->m_fac.conv = - this->m_proj_parm.n * lp_lon;
                }
                #endif

                static inline std::string get_name()
                {
                    return "lcc_ellipsoid";
                }

            };

            // Lambert Conformal Conic
            template <typename Parameters, typename T>
            inline void setup_lcc(Parameters& par, par_lcc<T>& proj_parm)
            {
                static const T FORTPI = detail::FORTPI<T>();
                static const T HALFPI = detail::HALFPI<T>();

                T cosphi, sinphi;
                int secant;

                proj_parm.phi1 = pj_param(par.params, "rlat_1").f;
                if (pj_param(par.params, "tlat_2").i)
                    proj_parm.phi2 = pj_param(par.params, "rlat_2").f;
                else {
                    proj_parm.phi2 = proj_parm.phi1;
                    if (!pj_param(par.params, "tlat_0").i)
                        par.phi0 = proj_parm.phi1;
                }
                if (fabs(proj_parm.phi1 + proj_parm.phi2) < EPS10)
                    BOOST_THROW_EXCEPTION( projection_exception(-21) );
                proj_parm.n = sinphi = sin(proj_parm.phi1);
                cosphi = cos(proj_parm.phi1);
                secant = fabs(proj_parm.phi1 - proj_parm.phi2) >= EPS10;
                if( (proj_parm.ellips = (par.es != 0.)) ) {
                    double ml1, m1;

                    par.e = sqrt(par.es);
                    m1 = pj_msfn(sinphi, cosphi, par.es);
                    ml1 = pj_tsfn(proj_parm.phi1, sinphi, par.e);
                    if (secant) { /* secant cone */
                        proj_parm.n = log(m1 /
                           pj_msfn(sinphi = sin(proj_parm.phi2), cos(proj_parm.phi2), par.es));
                        proj_parm.n /= log(ml1 / pj_tsfn(proj_parm.phi2, sinphi, par.e));
                    }
                    proj_parm.c = (proj_parm.rho0 = m1 * pow(ml1, -proj_parm.n) / proj_parm.n);
                    proj_parm.rho0 *= (fabs(fabs(par.phi0) - HALFPI) < EPS10) ? 0. :
                        pow(pj_tsfn(par.phi0, sin(par.phi0), par.e), proj_parm.n);
                } else {
                    if (secant)
                        proj_parm.n = log(cosphi / cos(proj_parm.phi2)) /
                           log(tan(FORTPI + .5 * proj_parm.phi2) /
                           tan(FORTPI + .5 * proj_parm.phi1));
                    proj_parm.c = cosphi * pow(tan(FORTPI + .5 * proj_parm.phi1), proj_parm.n) / proj_parm.n;
                    proj_parm.rho0 = (fabs(fabs(par.phi0) - HALFPI) < EPS10) ? 0. :
                        proj_parm.c * pow(tan(FORTPI + .5 * par.phi0), -proj_parm.n);
                }
            }

    }} // namespace detail::lcc
    #endif // doxygen

    /*!
        \brief Lambert Conformal Conic projection
        \ingroup projections
        \tparam Geographic latlong point type
        \tparam Cartesian xy point type
        \tparam Parameters parameter type
        \par Projection characteristics
         - Conic
         - Spheroid
         - Ellipsoid
        \par Projection parameters
         - lat_1: Latitude of first standard parallel (degrees)
         - lat_2: Latitude of second standard parallel (degrees)
         - lat_0: Latitude of origin
        \par Example
        \image html ex_lcc.gif
    */
    template <typename CalculationType, typename Parameters>
    struct lcc_ellipsoid : public detail::lcc::base_lcc_ellipsoid<CalculationType, Parameters>
    {
        inline lcc_ellipsoid(const Parameters& par) : detail::lcc::base_lcc_ellipsoid<CalculationType, Parameters>(par)
        {
            detail::lcc::setup_lcc(this->m_par, this->m_proj_parm);
        }
    };

    #ifndef DOXYGEN_NO_DETAIL
    namespace detail
    {

        // Static projection
        BOOST_GEOMETRY_PROJECTIONS_DETAIL_STATIC_PROJECTION(srs::par4::lcc, lcc_ellipsoid, lcc_ellipsoid)

        // Factory entry(s)
        template <typename CalculationType, typename Parameters>
        class lcc_entry : public detail::factory_entry<CalculationType, Parameters>
        {
            public :
                virtual base_v<CalculationType, Parameters>* create_new(const Parameters& par) const
                {
                    return new base_v_fi<lcc_ellipsoid<CalculationType, Parameters>, CalculationType, Parameters>(par);
                }
        };

        template <typename CalculationType, typename Parameters>
        inline void lcc_init(detail::base_factory<CalculationType, Parameters>& factory)
        {
            factory.add_to_factory("lcc", new lcc_entry<CalculationType, Parameters>);
        }

    } // namespace detail
    #endif // doxygen

} // namespace projections

}} // namespace boost::geometry

#endif // BOOST_GEOMETRY_PROJECTIONS_LCC_HPP
