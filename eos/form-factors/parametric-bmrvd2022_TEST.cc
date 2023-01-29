/* vim: set sw=4 sts=4 et foldmethod=syntax : */

/*
 * Copyright (c) 2021-2022 Danny van Dyk
 * Copyright (c) 2021-2022 Muslem Rahimi
 *
 * This file is part of the EOS project. EOS is free software;
 * you can redistribute it and/or modify it under the terms of the GNU General
 * Public License version 2, as published by the Free Software Foundation.
 *
 * EOS is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc., 59 Temple
 * Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include <test/test.hh>
#include <eos/form-factors/parametric-bmrvd2022.hh>
#include <eos/form-factors/parametric-bmrvd2022-impl.hh>

#include <eos/models/model.hh>
#include <eos/maths/power-of.hh>

#include <cmath>
#include <limits>
#include <vector>

using namespace test;
using namespace eos;

class BMRvD2022FormFactorsTest :
    public TestCase
{
    public:
        BMRvD2022FormFactorsTest() :
            TestCase("BMRvD2022_form_factor_test")
        {
        }

        virtual void run() const
        {
            static const double eps = 1e-5;

            /* Lb -> L FFs */
            {
                Parameters p = Parameters::Defaults();
                // f_time_v
                p["Lambda_b->Lambda::a^(t,V)_1@BMRvD2022"] = -0.2;
                p["Lambda_b->Lambda::a^(t,V)_2@BMRvD2022"] = +0.3;
                p["Lambda_b->Lambda::a^(t,V)_3@BMRvD2022"] = -0.4;
                p["Lambda_b->Lambda::a^(t,V)_4@BMRvD2022"] = +0.5;
                // f_long_v
                p["Lambda_b->Lambda::a^(0,V)_0@BMRvD2022"] = +0.1;
                p["Lambda_b->Lambda::a^(0,V)_1@BMRvD2022"] = -0.2;
                p["Lambda_b->Lambda::a^(0,V)_2@BMRvD2022"] = +0.3;
                p["Lambda_b->Lambda::a^(0,V)_3@BMRvD2022"] = -0.4;
                p["Lambda_b->Lambda::a^(0,V)_4@BMRvD2022"] = +0.5;
                // f_perp_v
                p["Lambda_b->Lambda::a^(perp,V)_0@BMRvD2022"] = +0.1;
                p["Lambda_b->Lambda::a^(perp,V)_1@BMRvD2022"] = -0.2;
                p["Lambda_b->Lambda::a^(perp,V)_2@BMRvD2022"] = +0.3;
                p["Lambda_b->Lambda::a^(perp,V)_3@BMRvD2022"] = -0.4;
                p["Lambda_b->Lambda::a^(perp,V)_4@BMRvD2022"] = +0.5;
                // f_time_a
                p["Lambda_b->Lambda::a^(t,A)_1@BMRvD2022"] = -0.2;
                p["Lambda_b->Lambda::a^(t,A)_2@BMRvD2022"] = +0.3;
                p["Lambda_b->Lambda::a^(t,A)_3@BMRvD2022"] = -0.4;
                p["Lambda_b->Lambda::a^(t,A)_4@BMRvD2022"] = +0.5;
                // f_long_a
                p["Lambda_b->Lambda::a^(0,A)_0@BMRvD2022"] = +0.1;
                p["Lambda_b->Lambda::a^(0,A)_1@BMRvD2022"] = -0.2;
                p["Lambda_b->Lambda::a^(0,A)_2@BMRvD2022"] = +0.3;
                p["Lambda_b->Lambda::a^(0,A)_3@BMRvD2022"] = -0.4;
                p["Lambda_b->Lambda::a^(0,A)_4@BMRvD2022"] = +0.5;
                // f_perp_a
                p["Lambda_b->Lambda::a^(perp,A)_1@BMRvD2022"] = -0.2;
                p["Lambda_b->Lambda::a^(perp,A)_2@BMRvD2022"] = +0.3;
                p["Lambda_b->Lambda::a^(perp,A)_3@BMRvD2022"] = -0.4;
                p["Lambda_b->Lambda::a^(perp,A)_4@BMRvD2022"] = +0.5;
                // f_long_t
                p["Lambda_b->Lambda::a^(0,T)_0@BMRvD2022"] = +0.1;
                p["Lambda_b->Lambda::a^(0,T)_1@BMRvD2022"] = -0.2;
                p["Lambda_b->Lambda::a^(0,T)_2@BMRvD2022"] = +0.3;
                p["Lambda_b->Lambda::a^(0,T)_3@BMRvD2022"] = -0.4;
                p["Lambda_b->Lambda::a^(0,T)_4@BMRvD2022"] = +0.5;
                // f_perp_t
                p["Lambda_b->Lambda::a^(perp,T)_1@BMRvD2022"] = -0.2;
                p["Lambda_b->Lambda::a^(perp,T)_2@BMRvD2022"] = +0.3;
                p["Lambda_b->Lambda::a^(perp,T)_3@BMRvD2022"] = -0.4;
                p["Lambda_b->Lambda::a^(perp,T)_4@BMRvD2022"] = +0.5;
                // f_long_t5
                p["Lambda_b->Lambda::a^(0,T5)_1@BMRvD2022"] = -0.2;
                p["Lambda_b->Lambda::a^(0,T5)_2@BMRvD2022"] = +0.3;
                p["Lambda_b->Lambda::a^(0,T5)_3@BMRvD2022"] = -0.4;
                p["Lambda_b->Lambda::a^(0,T5)_4@BMRvD2022"] = +0.5;
                // f_perp_t5
                p["Lambda_b->Lambda::a^(perp,T5)_0@BMRvD2022"] = +0.1;
                p["Lambda_b->Lambda::a^(perp,T5)_1@BMRvD2022"] = -0.2;
                p["Lambda_b->Lambda::a^(perp,T5)_2@BMRvD2022"] = +0.3;
                p["Lambda_b->Lambda::a^(perp,T5)_3@BMRvD2022"] = -0.4;
                p["Lambda_b->Lambda::a^(perp,T5)_4@BMRvD2022"] = +0.5;
                // Resonance masses
                p["mass::B_s@BSZ2015"]   = 5.367;
                p["mass::B_s,0@BSZ2015"] = 5.711;
                p["mass::B_s^*@BSZ2015"] = 5.416;
                p["mass::B_s,1@BSZ2015"] = 5.750;
                // Fix tp_a to tp_v to match the initial publication [BMRvD:2022A]
                p["Lambda_b->Lambda::tp_a@BMRvD2022"] = p["Lambda_b->Lambda::tp_v@BMRvD2022"].evaluate();


                BMRvD2022FormFactors<LambdaBToLambda> ff(p, Options{ });

                Diagnostics diagnostics = ff.diagnostics();
                for (const auto & d : diagnostics)
                {
                    std::cout << d.description << ": " << d.value << std::endl;
                }
                static const std::vector<std::pair<double, double>> reference
                {
                    std::make_pair( 0.230324,  eps), // z(q2 =  0)
                    std::make_pair( 0.107523,  eps), // phi(q2 =  0, f_time^V)
                    std::make_pair( 0.0511848, eps), // phi(q2 =  0, f_long^V)
                    std::make_pair( 0.12409,   eps), // phi(q2 =  0, f_perp^V)
                    std::make_pair( 0.213071,  eps), // phi(q2 =  0, f_time^A)
                    std::make_pair( 0.0253141, eps), // phi(q2 =  0, f_long^A)
                    std::make_pair( 0.0917756, eps), // phi(q2 =  0, f_perp^A)
                    std::make_pair( 0.107298,  eps), // phi(q2 =  0, f_long^T)
                    std::make_pair( 0.0885165, eps), // phi(q2 =  0, f_perp^T)
                    std::make_pair( 0.0797895, eps), // phi(q2 =  0, f_long^T5)
                    std::make_pair( 0.044016,  eps), // phi(q2 =  0, f_perp^T5)
                    std::make_pair( 0.144329,  eps), // z(q2 = 10)
                    std::make_pair( 0.0978787, eps), // phi(q2 = 10, f_time^V)
                    std::make_pair( 0.0516911, eps), // phi(q2 = 10, f_long^V)
                    std::make_pair( 0.115081,  eps), // phi(q2 = 10, f_perp^V)
                    std::make_pair( 0.197602,  eps), // phi(q2 = 10, f_time^A)
                    std::make_pair( 0.0250934, eps), // phi(q2 = 10, f_long^A)
                    std::make_pair( 0.083544,  eps), // phi(q2 = 10, f_perp^A)
                    std::make_pair( 0.0995077, eps), // phi(q2 = 10, f_long^T)
                    std::make_pair( 0.089392,  eps), // phi(q2 = 10, f_perp^T)
                    std::make_pair( 0.0726330, eps), // phi(q2 = 10, f_long^T5)
                    std::make_pair( 0.0436322, eps), // phi(q2 = 10, f_perp^T5)
                    std::make_pair( 0.557107,  eps), // p_0(z = 0.0)
                    std::make_pair(-0.440501,  eps), // p_1(z = 0.0)
                    std::make_pair( 0.633591,  eps), // p_2(z = 0.0)
                    std::make_pair(-0.884426,  eps), // p_3(z = 0.0)
                    std::make_pair( 1.22601,   eps), // p_4(z = 0.0)
                    std::make_pair(-1.69717,   eps), // p_5(z = 0.0)
                    std::make_pair( 0.557107,  eps), // p_0(z = z(q2 = 10))
                    std::make_pair(-0.337996,  eps), // p_1(z = z(q2 = 10))
                    std::make_pair( 0.511499,  eps), // p_2(z = z(q2 = 10))
                    std::make_pair(-0.664035,  eps), // p_3(z = z(q2 = 10))
                    std::make_pair( 0.863691,  eps), // p_4(z = z(q2 = 10))
                    std::make_pair(-1.12016,   eps), // p_5(z = z(q2 = 10))
                };
                TEST_CHECK_DIAGNOSTICS(diagnostics, reference);

                // form factor values
                TEST_CHECK_NEARLY_EQUAL( 33.25224570, ff.f_time_v ( 0.0), eps);
                TEST_CHECK_NEARLY_EQUAL( 33.25224570, ff.f_long_v ( 0.0), eps);
                TEST_CHECK_NEARLY_EQUAL( 13.71590521, ff.f_perp_v ( 0.0), eps);
                TEST_CHECK_NEARLY_EQUAL( 39.04787827, ff.f_time_a ( 0.0), eps);
                TEST_CHECK_NEARLY_EQUAL( 39.04787827, ff.f_long_a ( 0.0), eps);
                TEST_CHECK_NEARLY_EQUAL( 43.76278718, ff.f_perp_a ( 0.0), eps);
                TEST_CHECK_NEARLY_EQUAL( 15.86249033, ff.f_long_t ( 0.0), eps);
                TEST_CHECK_NEARLY_EQUAL( 22.45684904, ff.f_perp_t ( 0.0), eps);
                TEST_CHECK_NEARLY_EQUAL( 21.61605973, ff.f_long_t5( 0.0), eps);
                TEST_CHECK_NEARLY_EQUAL( 22.45684904, ff.f_perp_t5( 0.0), eps);

                TEST_CHECK_NEARLY_EQUAL( 40.87968764, ff.f_time_v (10.0), eps);
                TEST_CHECK_NEARLY_EQUAL( 45.45565962, ff.f_long_v (10.0), eps);
                TEST_CHECK_NEARLY_EQUAL( 20.41738244, ff.f_perp_v (10.0), eps);
                TEST_CHECK_NEARLY_EQUAL( 51.93704349, ff.f_time_a (10.0), eps);
                TEST_CHECK_NEARLY_EQUAL( 48.08756014, ff.f_long_a (10.0), eps);
                TEST_CHECK_NEARLY_EQUAL( 51.98391863, ff.f_perp_a (10.0), eps);
                TEST_CHECK_NEARLY_EQUAL( 23.61277120, ff.f_long_t (10.0), eps);
                TEST_CHECK_NEARLY_EQUAL( 30.02964424, ff.f_perp_t (10.0), eps);
                TEST_CHECK_NEARLY_EQUAL( 27.11308541, ff.f_long_t5(10.0), eps);
                TEST_CHECK_NEARLY_EQUAL( 27.65566599, ff.f_perp_t5(10.0), eps);

                const auto tm = LambdaBToLambda::tm;
                TEST_CHECK_NEARLY_EQUAL( 59.87052714, ff.f_time_v (tm),   eps);
                TEST_CHECK_NEARLY_EQUAL( 88.45600970, ff.f_long_v (tm),   eps);
                TEST_CHECK_NEARLY_EQUAL( 44.89007817, ff.f_perp_v (tm),   eps);
                TEST_CHECK_NEARLY_EQUAL( 96.34996613, ff.f_time_a (tm),   eps);
                TEST_CHECK_NEARLY_EQUAL( 70.92712078, ff.f_long_a (tm),   eps);
                TEST_CHECK_NEARLY_EQUAL( 70.92712078, ff.f_perp_a (tm),   eps);
                TEST_CHECK_NEARLY_EQUAL( 51.91552581, ff.f_long_t (tm),   eps);
                TEST_CHECK_NEARLY_EQUAL( 56.60821914, ff.f_perp_t (tm),   eps);
                TEST_CHECK_NEARLY_EQUAL( 40.79093961, ff.f_long_t5(tm),   eps);
                TEST_CHECK_NEARLY_EQUAL( 40.79093961, ff.f_perp_t5(tm),   eps);
            }
        }
} bmrvd2022_form_factor_test;
