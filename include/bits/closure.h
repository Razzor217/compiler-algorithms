#pragma once

#include "language/grammar.h"

namespace compiler
{
    template <typename symbol_type>
    std::vector<production<symbol_type>> fixed_point_closure(std::vector<production<symbol_type>> const& items, grammar<symbol_type> const& g)
    {
        std::vector<production<symbol_type>> result {items};
        bool changed {false};
        do
        {
            for (auto const& prod : result)
            {
                // check whether the production has a nonterminal B at the cursor, i.e. A -> alpha . B beta
                if (prod.nonterminal_at_cursor())
                {
                    // search grammar for productions with lhs B, i.e. B -> . gamma
                    for (auto const& p : g.productions())
                    {
                        /*
                         * 1. check whether p starts with B
                         * 2. if so, check whether p is in the result set
                         * 3. if not, add p to the result set
                         */
                        if (p.left() == prod.right()[prod.cursor()] && !util::contains(result, p))
                        {
                            p.set_cursor(0);
                            result.push_back(p);
                        }
                    }
                }
            }
        } while (changed);

        return result;
    }

    template <typename symbol_type>
    std::vector<production<symbol_type>> worklist_closure(std::vector<production<symbol_type>> const& items, grammar<symbol_type> const& g)
    {
        std::vector<production<symbol_type>> result {items};
        std::queue<production<symbol_type>> q {items};

        while (!q.empty())
        {
            auto prod = q.front();
            q.pop();

            // check whether the production has a nonterminal B at the cursor, i.e. A -> alpha . B beta
            if (prod.nonterminal_at_cursor())
            {
                // search grammar for productions with lhs B, i.e. B -> . gamma
                for (auto const& p : g.productions())
                {
                    /*
                     * 1. check whether p starts with B
                     * 2. if so, check whether p is in the result set
                     * 3. if not, add p to the queue and the result set
                     */
                    if (p.left() == prod.right()[prod.cursor()] && !util::contains(result, p))
                    {
                        p.set_cursor(0);
                        q.push(p);
                        result.push_back(p);
                    }
                }
            }
        }

        return result;
    }
}