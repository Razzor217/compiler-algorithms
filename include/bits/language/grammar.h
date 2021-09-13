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
        using production_type = production<symbol_type>;
        using set_type = std::set<production_type>;
        using vector_type = std::vector<production_type>;
        // vocabulary
        using symbol_set = typename production_type::set_type;

        inline symbol_set& terminal_symbols() const { return terminals_; }
        inline symbol_set& nonterminal_symbols() const { return nonterminals_; }
        inline set_type& productions() const { return productions_; }
        inline symbol_type start() const { return start; }

        explicit grammar(symbol_set const& terminals,
            symbol_set const& nonterminals,
            set_type const& productions,
            symbol_type const& start) :
            terminals_ {terminals},
            nonterminals_ {nonterminals},
            productions_ {productions},
            start_ {start}
        {
            /*
             * check whether for all nonterminals on right sides of productions
             * there exists a production with the nonterminal on the left side
             */
            assert(check_nonterminals());
        }

    private:
        vector_type productions_starting_with(symbol_type const& n)
        {
            vector_type result;
            for (production_type const& p : productions_)
            {
                if (p.left() == n) { result.push_back(p); }
            }
            return result;
        }

        bool check_nonterminals() const
        {
            std::map<symbol_type, bool> visited;
            for (symbol_type const& n : nonterminals_) { visited.insert({n, false}); }
            std::queue<symbol_type> q;
            q.push(start_);

            while (!q.empty())
            {
                auto current = q.front();
                q.pop();
                // find all productions 'current -> rhs'
                auto ps = productions_starting_with(current);

                // no production with matching left-hand-side found
                if (ps.empty()) { return false; }

                visited[current] = true;

                for (production_type const& prod : ps)
                {
                    // find all nonterminals N s.t. prod = A -> alpha N beta
                    auto nts = prod.nonterminals();
                    for (symbol_type const& n : nts)
                    {
                        // check not yet visited nonterminals
                        if (!visited[n]) { q.push(n); }
                    }
                }
            }

            return true;
        }

    private:
        symbol_set terminals_;
        symbol_set nonterminals_;
        set_type productions_;
        symbol_type start_;
    };
} // namespace compiler