#ifndef __SAVE_HOOK_H__9bfc7dc852528cb46b831eae7a7f4e308bf88de4__
#define __SAVE_HOOK_H__9bfc7dc852528cb46b831eae7a7f4e308bf88de4__

#include "diagnosis/benchmark/hooks/benchmark_hook.h"

namespace diagnosis {
namespace benchmark {
class t_save_hook : public t_benchmark_hook {
public:

    virtual void init_system (t_collector & collector,
                              const randomizers::t_system & system) const;

    virtual void init (t_collector & collector,
                       const t_status_iteration_init & status) const;

    virtual void post_gen (t_collector & collector,
                           const t_status_post_gen & status) const;

    virtual void post_rank (t_collector & collector,
                            const t_status_post_rank & status) const;
};
}
}
#endif