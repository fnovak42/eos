/* vim: set sw=4 sts=4 et foldmethod=syntax : */

/*
 * Copyright (c) 2021 Méril Reboud
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

#include <eos/rare-b-decays/bs-to-phi-ll-base.hh>
#include <eos/utils/destringify.hh>
#include <eos/utils/kinematic.hh>

namespace eos
{
    BsToPhiDilepton::AmplitudeGenerator::AmplitudeGenerator(const Parameters & p, const Options & o) :
        model(Model::make(o.get("model", "SM"), p, o)),
        form_factors(FormFactorFactory<PToV>::create("B_s->phi::" + o.get("form-factors", "BSZ2015"), p)),
        opt_l(o, "l", { "e", "mu", "tau" }, "mu"),
        mu(p["mu"], *this),
        alpha_e(p["QED::alpha_e(m_b)"], *this),
        g_fermi(p["WET::G_Fermi"], *this),
        tau(p["life_time::B_s"], *this),
        m_B(p["mass::B_s"], *this),
        m_V(p["mass::phi"], *this),
        m_l(p["mass::" + opt_l.value()], *this),
        cp_conjugate(destringify<bool>(o.get("cp-conjugate", "false"))),
        lepton_flavour(opt_l.value())
    {
        if (! form_factors.get())
            throw InternalError("Form factors not found!");

        if (0.0 == m_l())
        {
            throw InternalError("Zero lepton mass leads to NaNs in timelike amplitudes. Use tiny lepton mass > 0!");
        }

        this->uses(*form_factors);
        this->uses(*model);
    }

    BsToPhiDilepton::AmplitudeGenerator::~AmplitudeGenerator()
    {
    }

    double
    BsToPhiDilepton::AmplitudeGenerator::beta_l(const double & s) const
    {
        return std::sqrt(1.0 - 4.0 * m_l * m_l / s);
    }

    double
    BsToPhiDilepton::AmplitudeGenerator::lambda(const double & s) const
    {
        return eos::lambda(m_B() * m_B(), m_V() * m_V(), s);
    }

    double
    BsToPhiDilepton::AmplitudeGenerator::energy(const double & s) const
    {
        return (m_B() * m_B() + m_V() * m_V() - s) / (2.0 * m_B());
    }

    double
    BsToPhiDilepton::AmplitudeGenerator::s_hat(const double & s) const
    {
        return s / m_B() / m_B();
    }

}
