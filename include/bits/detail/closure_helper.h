#pragma once

#include "../language/grammar.h"

namespace compiler
{
    namespace detail
    {
        enum class fixed_point
        {
            naive,
            worklist
        };

        template <typename symbol_type, fixed_point fp = fixed_point::worklist>
        class closure_helper
        {
        public:
            using grammar_type = grammar<symbol_type>;
            using production_type = typename grammar_type::production_type;
            using set_type = typename grammar_type::set_type;

            set_type operator () (set_type const& items) const
            {
                switch (fp)
                {
                    case fixed_point::naive:    return naive(items);
                    case fixed_point::worklist: return worklist(items);
                    default:                    return items;
                }
            }

            explicit closure_helper(grammar_type const& grammar) :
                grammar_ {grammar}
            { }

        private:
            set_type naive(set_type const& items) const
            {
                set_type result {items};
                bool changed {false};
                do
                {
                    for (auto const& prod : result)
                    {
                        // check whether the production has a nonterminal B at the cursor, i.e. A -> alpha . B beta
                        // note: this implicitly checks whether the rhs is the empty word
                        if (prod.nonterminal_at_cursor())
                        {
                            // search grammar for productions with lhs B, i.e. B -> . gamma
                            for (auto const& p : grammar_.productions())
                            {
                                /*
                                * 1. check whether p starts with B
                                * 2. if so, check whether p is in the result set
                                * 3. if not, add p to the result set
                                */
                                if (p.left() == prod.right()[prod.cursor()]
                                    && p.cursor() == 0
                                    && !result.contains(p))
                                {
                                    result.insert(p);
                                }
                            }
                        }
                    }
                } while (changed);

                return result;
            }

            set_type worklist(set_type const& items) const
            {
                set_type result {items};
                std::queue<production_type> q {items};

                while (!q.empty())
                {
                    auto prod = q.front();
                    q.pop();

                    // check whether the production has a nonterminal B at the cursor, i.e. A -> alpha . B beta
                    // note: this implicitly checks whether the rhs is the empty word
                    if (prod.nonterminal_at_cursor())
                    {
                        // search grammar for productions with lhs B, i.e. B -> . gamma
                        for (auto const& p : grammar_.productions())
                        {
                            /*
                            * 1. check whether p starts with B
                            * 2. if so, check whether p is in the result set
                            * 3. if not, add p to the queue and the result set
                            */
                            if (p.left() == prod.right()[prod.cursor()]
                                && p.cursor() == 0
                                && !result.contains(p))
                            {
                                q.push(p);
                                result.insert(p);
                            }
                        }
                    }
                }

                return result;
            }

        private:
            grammar_type grammar_;
        };
    } // namespace detail
} // namespace compiler