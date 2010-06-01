/* vim: set sw=4 sts=4 et foldmethod=syntax : */

#include <src/rare-b-decays/factory.hh>
#include <src/utils/destringify.hh>
#include <src/utils/lock.hh>
#include <src/utils/mutex.hh>
#include <src/utils/thread_pool.hh>

#include <cmath>
#include <cstdlib>
#include <iostream>
#include <list>
#include <map>
#include <tr1/functional>
#include <utility>
#include <vector>

using namespace wf;

struct Input
{
    const double min;

    const double max;

    const double o_min;

    const double o;

    const double o_max;

    const std::string o_name;

    const std::string o_options;
};

class Scan2
{
    public:
        Mutex mutex;

        std::list<std::pair<Input, ObservablePtr>> bins;

        double max_likelihood;

        std::map<std::pair<int, int>, double> results;

        std::string x_name;

        double x_min, x_max;

        std::string y_name;

        double y_min, y_max;

        unsigned points;

        std::vector<std::string> variations;

        Scan2(const std::string & x_name, const double & x_min, const double & x_max,
                const std::string & y_name, const double & y_min, const double & y_max,
                const std::list<Input> & inputs) :
            max_likelihood(0.0),
            x_name(x_name),
            x_min(x_min),
            x_max(x_max),
            y_name(y_name),
            y_min(y_min),
            y_max(y_max),
            points(60)
        {
            for (auto i(inputs.begin()), i_end(inputs.end()) ; i != i_end ; ++i)
            {
                //TODO: Create options from i->o_opions!
                ObservableOptions options;
                bins.push_back(std::make_pair(*i, RareBFactory::make(i->o_name, Parameters::Defaults(), options)));
            }
        }

        void calc_likelihood(const int & ix, const int & iy)
        {
            double x = x_min + (x_max - x_min) / points * ix;
            double y = y_min + (y_max - y_min) / points * iy;

            double chi_squared(0.0);
            Kinematics k;
            k.declare("s_min");
            k.declare("s_max");

            for (auto bin = bins.begin() ; bin != bins.end() ; ++bin)
            {
                ObservablePtr o(bin->second->clone());
                Parameters p = o->parameters();

                k.set("s_min", bin->first.min);
                k.set("s_max", bin->first.max);
                p.set(x_name, x);
                p.set(y_name, y);

                std::vector<Parameter> variations =
                {
                    p["CKM::A"],
                    p["CKM::lambda"],
                    p["formfactors::a1_uncertainty"],
                    p["formfactors::a2_uncertainty"],
                    p["formfactors::v_uncertainty"],
                    p["mass::s"],
                    p["mass::c"],
                };

                double central = o->evaluate(k);
                double delta_min = 0.0, delta_max = 0.0;

                for (auto p(variations.begin()), p_end(variations.end()) ; p != p_end ; ++p)
                {
                    double old_p = *p;
                    double max = 0.0, min = 0.0, value;

                    *p = p->min();
                    value = o->evaluate(k);
                    if (value > central)
                        max = value - central;

                    if (value < central)
                        min = central - value;

                    *p = p->max();
                    value = o->evaluate(k);
                    if (value > central)
                        max = std::max(max, value - central);

                    if (value < central)
                        min = std::max(min, central - value);

                    *p = old_p;

                    delta_min += min * min;
                    delta_max += max * max;
                }

                delta_max = std::sqrt(delta_max);
                delta_min = std::sqrt(delta_min);

                double chi = 0.0;
                if (bin->first.o - central > delta_max)
                    chi = bin->first.o - central - delta_max;
                else if (central - bin->first.o > delta_min)
                    chi = central - bin->first.o - delta_min;

                chi /= (bin->first.o_max - bin->first.o_min);

                chi_squared += chi * chi;
            }

            double likelihood = std::exp(-0.5 * chi_squared);

            {
                Lock l(mutex);
                results[std::make_pair(ix, iy)] = likelihood;
                max_likelihood = std::max(max_likelihood, likelihood);
            }
        }

        void scan()
        {
            TicketList tickets;

            for (int i(0) ; i <= points ; ++i)
            {
                for (int j(0) ; j <= points ; ++j)
                {
                    tickets.push_back(ThreadPool::instance()->enqueue(std::tr1::bind(std::tr1::mem_fn(&Scan2::calc_likelihood), this, i, j)));
                }
            }

            tickets.wait();

            std::vector<double> likelihoods;
            likelihoods.reserve(results.size());
            double integral = 0.0;

            double index(results.begin()->first.second);
            for (auto r(results.begin()), r_end(results.end()) ; r != r_end ; ++r)
            {
                if (r->first.second < index)
                    std::cout << std::endl;

                index = r->first.second;
                double x = x_min + (x_max - x_min) / points * r->first.first;
                double y = y_min + (y_max - y_min) / points * r->first.second;
                std::cout
                    << r->first.first << '\t'
                    << r->first.second << '\t'
                    << r->second / max_likelihood << '\t'
                    << x << '\t'
                    << y << '\t'
                    << std::endl;
                likelihoods.push_back(r->second / max_likelihood);
                integral += r->second / max_likelihood;
            }

            std::vector<double> upper(4, 1.0), lower(4, 0.0), value(4, 0.5);
            std::vector<double> ratio = { 0.683, 0.900, 0.950, 0.954 };
            for (unsigned i(0) ; i < 10 ; ++i)
            {
                std::vector<double> partial(4, 0.0);

                for (auto l(likelihoods.begin()), l_end(likelihoods.end()) ; l != l_end ; ++l)
                {
                    for (unsigned j(0) ; j < partial.size() ; ++j)
                        if (*l > value[j])
                            partial[j] += *l;
                }

                for (unsigned j(0) ; j < partial.size() ; ++j)
                {
                    if ((partial[j] / integral) > ratio[j])
                        lower[j] = value[j];
                    else
                        upper[j] = value[j];

                    value[j] = (upper[j] + lower[j]) / 2.0;
                }
            }

            std::cout << "# Confidence Levels" << std::endl;
            for (unsigned j(0) ; j < ratio.size() ; ++j)
            {
                std::cout << "# " << ratio[j] << " -> " << value[j] << std::endl;
            }

            std::cout << "# max(likelihood) = " << max_likelihood << std::endl;
        }
};

class DoUsage
{
    private:
        std::string _what;

    public:
        DoUsage(const std::string & what) :
            _what(what)
        {
        }

        const std::string & what() const
        {
            return _what;
        }
};

int
main(int argc, char * argv[])
{
    try
    {
        std::string x, y;
        double x_min = -10.0, x_max = +10.0;
        double y_min = -10.0, y_max = +10.0;
        std::list<Input> input;

        for (char ** a(argv + 1), ** a_end(argv + argc) ; a != a_end ; ++a)
        {
            std::string argument(*a);
            if ("--x" == argument)
            {
                x = std::string(*(++a));
                x_min = destringify<double>(*(++a));
                x_max = destringify<double>(*(++a));
                continue;
            }

            if ("--y" == argument)
            {
                y = std::string(*(++a));
                y_min = destringify<double>(*(++a));
                y_max = destringify<double>(*(++a));
                continue;
            }

            if ("--input" == argument)
            {
                std::string observable(*(++a));
                double k1 = destringify<double>(*(++a));
                double k2 = destringify<double>(*(++a));
                double min = destringify<double>(*(++a));
                double central = destringify<double>(*(++a));
                double max = destringify<double>(*(++a));

                input.push_back(Input{k1, k2, min, central, max, observable});

                continue;
            }
        }

        if (x.empty())
            throw DoUsage("Need a name for the 'x' parameter");

        if (y.empty())
            throw DoUsage("Need a name for the 'y' parameter");

        if (input.empty())
            throw DoUsage("Need at least one input");

        Scan2 scanner(x, x_min, x_max, y, y_min, y_max, input);
        scanner.scan();
    }
    catch(DoUsage & e)
    {
        std::cerr << e.what() << std::endl;
    }
    catch(Exception & e)
    {
        std::cerr << "Caught exception: '" << e.what() << "'" << std::endl;
        return EXIT_FAILURE;
    }
    catch(...)
    {
        std::cerr << "Aborting after unknown exception" << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
