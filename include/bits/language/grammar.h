#pragma once

#include <map>
#include <queue>

#include "production.h"

namespace compiler
{
    template <typename symbol_type>
    class grammar
    {
    public:
        using symbol_set = std::vector<symbol_type>;
        using production_set = std::vector<production<symbol_type>>;

        symbol_set& terminal_symbols() const { return terminals_; }
        symbol_set& nonterminal_symbols() const { return nonterminals_; }
        production_set& productions() const { return productions_; }
        symbol_type start() const { return start; }

        explicit grammar(symbol_set const& terminals,
            symbol_set const& nonterminals,
            production_set const& productions,
            symbol_type const& start) :
            terminals_ {terminals},
            nonterminals_ {nonterminals},
            productions_ {productions},
            start_ {start}
        {
            // todo: add assertions for productions

            /*
             * check whether for all nonterminals on right sides of productions
             * there exists a production with the nonterminal on the left side
             */
            assert(check_nonterminals());
        }

    private:
        production_set productions_starting_with(symbol_type const& n)
        {
            production_set result;
            for (production<symbol_type> const& p : productions_)
            {
                if (p.left() == n)
                {
                    result.push_back(p);
                }
            }
            return result;
        }

        bool check_nonterminals() const
        {
            std::map<symbol_type, bool> visited;
            for (symbol_type const& n : nonterminals_.get())
            {
                visited.insert({n, false});
            }
            std::queue<symbol_type> q;
            q.push(start_);

            while (!q.empty())
            {
                auto current = q.front();
                q.pop();
                // find all productions 'current -> rhs'
                auto ps = productions_starting_with(current);

                // no production with matching left-hand-side found
                if (ps.empty())
                {
                    return false;
                }

                visited[current] = true;

                for (production<symbol_type> const& p : ps)
                {
                    // find all nonterminals N s.t. p = A -> alpha N beta
                    auto nts = p.nonterminals();
                    for (symbol_type const& n : nts)
                    {
                        // check not yet visited nonterminals
                        if (!visited[n])
                        {
                            q.push(n);
                        }
                    }
                }
            }

            return true;
        }

    private:
        symbol_set terminals_;
        symbol_set nonterminals_;
        production_set productions_;
        symbol_type start_;
    };
} // namespace compiler