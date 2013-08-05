#include <boost/test/unit_test.hpp>
#include "diagnosis/algorithms/mhs.h"
#include "diagnosis/heuristics/sort.h"
#include "diagnosis/heuristics/similarity.h"
#include "diagnosis/spectra/count_spectra.h"
#include "diagnosis/spectra/randomizer/bernoulli.h"

#include <fstream>
using namespace diagnosis;
using namespace diagnosis::algorithms;
using namespace diagnosis::structs;

BOOST_AUTO_TEST_SUITE(MHS2)

BOOST_AUTO_TEST_CASE(stop_flags) {
    t_count_spectra spectra;
    t_bernoulli_randomizer randomizer(0.25, 1);
    t_heuristic heuristic;
    t_trie D;


    randomizer.n_comp = 100;
    randomizer.n_tran = 100;
    randomizer.randomize(spectra);

    heuristic.push(new heuristics::t_ochiai());
    heuristic.push(new heuristics::t_sort());

    t_mhs mhs(heuristic);
    mhs.max_candidate_size = 2;
    mhs.calculate(spectra, D);

    // Max Candidate Size
    t_trie::iterator it = D.begin();

    while (it != D.end())
        BOOST_CHECK((it++)->size() <= mhs.max_candidate_size);

    // Max Candidate Size
    mhs = t_mhs(heuristic);
    mhs.max_candidates = 4000;
    D.clear();
    mhs.calculate(spectra, D);
    BOOST_CHECK(D.size() <= mhs.max_candidates);
}

BOOST_AUTO_TEST_CASE(mhs) {
    for (t_id i = 0; i < 30; i++) {
        t_count_spectra spectra;
        diagnosis::structs::t_trie D, D_ref;

        std::stringstream s;
        std::ifstream f;

        // Reading Input
        s << "io/mhs/in." << i << ".txt";
        f.open(s.str().c_str());
        f >> spectra;
        BOOST_CHECK(f.good());

        f.close();
        s.str("");

        // Reading Output
        s << "io/mhs/out." << i << ".txt";
        f.open(s.str().c_str());
        f >> D_ref;
        BOOST_CHECK(f.good());
        f.close();

        t_heuristic heuristic;
        heuristic.push(new heuristics::t_ochiai());
        heuristic.push(new heuristics::t_sort());

        algorithms::t_mhs mhs(heuristic);

        mhs.calculate(spectra, D);
        BOOST_CHECK(D == D_ref);
    }
}

BOOST_AUTO_TEST_SUITE_END()