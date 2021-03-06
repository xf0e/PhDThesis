#ifndef __CANDIDATE_RANKER_H_c4b92ee97fe288ca067d185a120eb5cbff40f9fa__
#define __CANDIDATE_RANKER_H_c4b92ee97fe288ca067d185a120eb5cbff40f9fa__

#include "candidate_generator.h"
#include "types.h"

#include <list>

namespace diagnostic {
class t_candidate_ranker : public t_algorithm {
public:
    typedef std::list<t_score> t_ret_type;

    // class t_ret_type : public std::list<t_score> {
    // public:
    // };

    virtual void operator () (const t_spectrum & spectrum,
                              const t_candidate_generator::t_ret_type & D,
                              t_ret_type & result,
                              const t_spectrum_filter * filter=NULL) const = 0;

    virtual t_score_type get_score_type () const = 0;
    inline virtual ~t_candidate_ranker () {}
};
}
#endif