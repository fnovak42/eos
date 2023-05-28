/* vim: set sw=4 sts=4 et foldmethod=syntax : */

/*
 * Copyright (c) 2014-2017 Danny van Dyk
 * Copyright (c) 2018 Ahmet Kokulu
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

#include <eos/form-factors/baryonic-processes.hh>
#include <eos/form-factors/form-factors.hh>
#include <eos/form-factors/parametric-abr2022.hh>
#include <eos/form-factors/parametric-bfvd2014.hh>
#include <eos/form-factors/parametric-bbgorvd2018.hh>
#include <eos/form-factors/parametric-bmrvd2022.hh>
#include <eos/form-factors/parametric-dkmr2017.hh>
#include <eos/form-factors/parametric-dm2016.hh>
#include <eos/utils/destringify.hh>

#include <map>

namespace eos
{
    /* J=1/2^+ -> J=1/2^+ Processes */

    /* Lambda_b -> Lambda */

    const constexpr char * LambdaBToLambda::label;
    const constexpr double LambdaBToLambda::tm;
    const constexpr double LambdaBToLambda::tp;
    const constexpr double LambdaBToLambda::mR2_0m;
    const constexpr double LambdaBToLambda::mR2_0p;
    const constexpr double LambdaBToLambda::mR2_1m;
    const constexpr double LambdaBToLambda::mR2_1p;

    /* Lambda_b -> Lambda_c */

    FormFactors<OneHalfPlusToOneHalfPlus>::~FormFactors()
    {
    }

    const std::map<FormFactorFactory<OneHalfPlusToOneHalfPlus>::KeyType, FormFactorFactory<OneHalfPlusToOneHalfPlus>::ValueType>
    FormFactorFactory<OneHalfPlusToOneHalfPlus>::form_factors
    {
        { "Lambda_b->Lambda::BFvD2014",   &BFvD2014FormFactors::make                      },
        { "Lambda_b->Lambda::DM2016",     &DM2016FormFactors<LambdaBToLambda>::make       },
        { "Lambda_b->Lambda::BMRvD2022",  &BMRvD2022FormFactors<LambdaBToLambda>::make    },
        { "Lambda_b->Lambda_c::DKMR2017", &DKMR2017FormFactors<LambdaBToLambdaC>::make    }
    };

    std::shared_ptr<FormFactors<OneHalfPlusToOneHalfPlus>>
    FormFactorFactory<OneHalfPlusToOneHalfPlus>::create(const QualifiedName & name, const Parameters & parameters, const Options & options)
    {
        Context ctx("When creating a 1/2^+->1/2^+ form factor");

        std::shared_ptr<FormFactors<OneHalfPlusToOneHalfPlus>> result;

        auto i = FormFactorFactory<OneHalfPlusToOneHalfPlus>::form_factors.find(name);
        if (FormFactorFactory<OneHalfPlusToOneHalfPlus>::form_factors.end() != i)
        {
            result.reset(i->second(parameters, name.options() + options));
            return result;
        }

        throw NoSuchFormFactorError(name.prefix_part().str(), name.name_part().str());
        return result;
    }

    OptionSpecification
    FormFactorFactory<OneHalfPlusToOneHalfPlus>::option_specification(const qnp::Prefix & process)
    {
        OptionSpecification result { "form-factors", {}, "" };
        for (const auto & ff : FormFactorFactory<OneHalfPlusToOneHalfPlus>::form_factors)
        {
            if (process == std::get<0>(ff).prefix_part())
                result.allowed_values.push_back(std::get<0>(ff).name_part().str());
        }

        return result;
    }

    /* J=1/2^+ -> J=1/2^- Processes */

    /* Lambda_b -> Lambda_c(2595) */

    FormFactors<OneHalfPlusToOneHalfMinus>::~FormFactors()
    {
    }

    Diagnostics
    FormFactors<OneHalfPlusToOneHalfMinus>::diagnostics() const
    {
        return { };
    }

    const std::map<FormFactorFactory<OneHalfPlusToOneHalfMinus>::KeyType, FormFactorFactory<OneHalfPlusToOneHalfMinus>::ValueType>
    FormFactorFactory<OneHalfPlusToOneHalfMinus>::form_factors
    {
        { "Lambda_b->Lambda_c(2595)::HQET",        &HQETFormFactors<OneHalfPlusToOneHalfMinus, LambdaBToLambdaC2595>::make },
    };

    std::shared_ptr<FormFactors<OneHalfPlusToOneHalfMinus>>
    FormFactorFactory<OneHalfPlusToOneHalfMinus>::create(const QualifiedName & name, const Parameters & parameters, const Options & options)
    {
        Context ctx("When creating a 1/2^+->1/2^- form factor");

        std::shared_ptr<FormFactors<OneHalfPlusToOneHalfMinus>> result;

        auto i = FormFactorFactory<OneHalfPlusToOneHalfMinus>::form_factors.find(name);
        if (FormFactorFactory<OneHalfPlusToOneHalfMinus>::form_factors.end() != i)
        {
            result.reset(i->second(parameters, name.options() + options));
            return result;
        }

        throw NoSuchFormFactorError(name.prefix_part().str(), name.name_part().str());
        return result;
    }

    OptionSpecification
    FormFactorFactory<OneHalfPlusToOneHalfMinus>::option_specification(const qnp::Prefix & process)
    {
        OptionSpecification result { "form-factors", {}, "" };
        for (const auto & ff : FormFactorFactory<OneHalfPlusToOneHalfMinus>::form_factors)
        {
            if (process == std::get<0>(ff).prefix_part())
                result.allowed_values.push_back(std::get<0>(ff).name_part().str());
        }

        return result;
    }

    /* J=1/2^+ -> J=3/2^- Processes */

    /* Lambda_b -> Lambda_c(2625) */

    /* Lambda_b -> Lambda(1520) */

    const SzegoPolynomial<5> LambdaBToLambda1520::orthonormal_polynomials(SzegoPolynomial<5>::FlatMeasure(3.42519));


    FormFactors<OneHalfPlusToThreeHalfMinus>::~FormFactors()
    {
    }

    Diagnostics
    FormFactors<OneHalfPlusToThreeHalfMinus>::diagnostics() const
    {
        return { };
    }

    const std::map<FormFactorFactory<OneHalfPlusToThreeHalfMinus>::KeyType, FormFactorFactory<OneHalfPlusToThreeHalfMinus>::ValueType>
    FormFactorFactory<OneHalfPlusToThreeHalfMinus>::form_factors
    {
        { "Lambda_b->Lambda_c(2625)::HQET",          &HQETFormFactors<OneHalfPlusToThreeHalfMinus, LambdaBToLambdaC2625>::make },
        { "Lambda_b->Lambda(1520)::ABR2022",         &ABR2022FormFactors<LambdaBToLambda1520>::make },
    };

    std::shared_ptr<FormFactors<OneHalfPlusToThreeHalfMinus>>
    FormFactorFactory<OneHalfPlusToThreeHalfMinus>::create(const QualifiedName & name, const Parameters & parameters, const Options & options)
    {
        Context ctx("When creating a 1/2^+->3/2^- form factor");

        std::shared_ptr<FormFactors<OneHalfPlusToThreeHalfMinus>> result;

        auto i = FormFactorFactory<OneHalfPlusToThreeHalfMinus>::form_factors.find(name);
        if (FormFactorFactory<OneHalfPlusToThreeHalfMinus>::form_factors.end() != i)
        {
            result.reset(i->second(parameters, name.options() + options));
            return result;
        }

        throw NoSuchFormFactorError(name.prefix_part().str(), name.name_part().str());
        return result;
    }

    OptionSpecification
    FormFactorFactory<OneHalfPlusToThreeHalfMinus>::option_specification(const qnp::Prefix & process)
    {
        OptionSpecification result { "form-factors", {}, "" };
        for (const auto & ff : FormFactorFactory<OneHalfPlusToThreeHalfMinus>::form_factors)
        {
            if (process == std::get<0>(ff).prefix_part())
                result.allowed_values.push_back(std::get<0>(ff).name_part().str());
        }

        return result;
    }
}
