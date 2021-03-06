#include "count_spectrum.h"

#include <sstream>
#include <cstring>

namespace diagnostic {

t_count t_count_spectrum::get_activations (t_component_id component,
                                          t_transaction_id transaction) const {
    assert(component > 0);
    assert(component <= get_component_count());
    assert(transaction > 0);
    assert(transaction <= get_transaction_count());

    return activity[(transaction - 1) * get_component_count() + (component - 1)];
}

void t_count_spectrum::set_activations (t_component_id component,
                                       t_transaction_id transaction,
                                       t_count count,
                                       bool ignore_unknown_components) {
    assert(component > 0);
    assert(transaction > 0);

    if (component > get_component_count() ||
        transaction > get_transaction_count()) {
        if (!ignore_unknown_components)
            set_count(std::max(component, get_component_count()),
                      std::max(transaction, get_transaction_count()));
        else
            return;
    }

    activity[(transaction - 1) * get_component_count() + (component - 1)] = count;
}

void t_count_spectrum::set_count (t_count component_count,
                                 t_count transaction_count) {
    t_activity_ptr old_activity(activity);

    t_component_id max_component = std::min(component_count, get_component_count());
    t_component_id max_transaction = std::min(transaction_count, get_transaction_count());


    activity = t_activity_ptr(new t_count[component_count * transaction_count]);
    memset(activity.get(), 0, component_count * transaction_count * sizeof(t_count));

    for (t_component_id component = 0;
         component < max_component;
         component++)
        for (t_transaction_id transaction = 0;
             transaction < max_transaction;
             transaction++) {
            activity[transaction * component_count + component] =
                old_activity[transaction * get_component_count() + component];
        }

    t_basic_spectrum::set_count(component_count,
                               transaction_count);
}

t_transaction_id t_count_spectrum::new_transaction () {
    set_count(get_component_count(), get_transaction_count() + 1);
    return get_transaction_count();
}

void t_count_spectrum::hit (t_component_id component,
                           t_transaction_id transaction,
                           t_count count,
                           bool ignore_unknown_components) {
    assert(component > 0);
    assert(transaction > 0);

    if (component > get_component_count() ||
        transaction > get_transaction_count()) {
        if (!ignore_unknown_components)
            set_count(std::max(component, get_component_count()),
                      std::max(transaction, get_transaction_count()));
        else
            return;
    }

    activity[(transaction - 1) * get_component_count() + (component - 1)] += count;
}

std::istream & t_count_spectrum::read (std::istream & in) {
    return read(in, false);
}

std::istream & t_count_spectrum::read (std::istream & in,
                                      bool has_confidence) {
    std::ios::iostate in_exceptions = in.exceptions();


    // assert(in.good());

    t_count_spectrum spectrum;
    t_count read_components = 0, read_transactions = 0;
    t_count c_count = 0, tr_count = 0;


    try {
        in.exceptions(std::istream::failbit | std::istream::badbit);

        in >> c_count >> tr_count;
        spectrum.set_count(c_count, tr_count);

        for (t_transaction_id tr = 1; tr <= tr_count; tr++) {
            for (t_component_id c = 1; c <= c_count; c++) {
                t_count value;
                in >> value;
                spectrum.hit(c, tr, value);
                read_components++;
            }

            spectrum.set_error(tr, read_error(in));

            if (has_confidence) {
                t_confidence conf;
                in >> conf;
                spectrum.set_confidence(tr, conf);
            }

            read_transactions++;
            read_components = 0;
        }

        *this = spectrum;
    }
    catch (std::ios_base::failure e) {
        if (c_count && tr_count) {
            if (c_count == read_components)
                std::cerr << "Problem reading error information in transaction " << read_transactions;
            else
                std::cerr << "Problem reading spectrum after " << read_transactions << " transactions and " << read_components << " components" << std::endl;
        }
        else if (!in.eof()) {
            std::cerr << "Problem reading spectrum size " << std::endl;
        }
    }

    std::ios::iostate in_state = in.rdstate();
    in.exceptions(in_exceptions);
    in.setstate(in_state);

    return in;
}

t_error t_count_spectrum::read_error (std::istream & in) const {
    std::string str;
    std::stringstream ss;


    in >> str;
    ss.str(str);
    ss.exceptions(std::istream::failbit | std::istream::badbit);

    try {
        t_error result;
        ss >> result;

        return result;
    }
    catch (std::ios_base::failure e) {}

    if (str == "1" || str == "-" || str == "x" || str == "X")
        return 1;

    return 0;
}
}
