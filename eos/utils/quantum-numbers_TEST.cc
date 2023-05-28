/* vim: set sw=4 sts=4 et foldmethod=syntax : */

/*
 * Copyright (c) 2021 Danny van Dyk
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
#include <eos/utils/quantum-numbers.hh>
#include <eos/utils/stringify.hh>

using namespace test;
using namespace eos;

class LeptonFlavorTest :
    public TestCase
{
    public:
        LeptonFlavorTest() :
            TestCase("lepton_flavor_test")
        {
        }

        virtual void run() const
        {
            TEST_CHECK_EQUAL_STR("e",   stringify(LeptonFlavor::electron));
            TEST_CHECK_EQUAL_STR("mu",  stringify(LeptonFlavor::muon));
            TEST_CHECK_EQUAL_STR("tau", stringify(LeptonFlavor::tauon));
        }
} lepton_flavor_test;

class QuarkFlavorTest :
    public TestCase
{
    public:
        QuarkFlavorTest() :
            TestCase("quark_flavor_test")
        {
        }

        virtual void run() const
        {
            TEST_CHECK_EQUAL_STR("u", stringify(QuarkFlavor::up));
            TEST_CHECK_EQUAL_STR("d", stringify(QuarkFlavor::down));
            TEST_CHECK_EQUAL_STR("s", stringify(QuarkFlavor::strange));
            TEST_CHECK_EQUAL_STR("c", stringify(QuarkFlavor::charm));
            TEST_CHECK_EQUAL_STR("b", stringify(QuarkFlavor::bottom));
            TEST_CHECK_EQUAL_STR("t", stringify(QuarkFlavor::top));
        }
} quark_flavor_test;
