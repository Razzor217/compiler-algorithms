#pragma once

#include <cassert>
#include <memory>
#include <vector>

#include "../util.h"

namespace compiler
{
    template <typename symbol_type>
    class production
    {
    public:
        using symbol_set = std::vector<symbol_type>;
        using symbol_set_ptr = std::shared_ptr<symbol_set>;

        symbol_set_ptr& terminal_symbols() const { return terminals_; }
        symbol_set_ptr& nonterminal_symbols() const { return nonterminals_; }
        symbol_type left() const { return left_; }
        symbol_set right() const { return right_; }

        std::size_t cursor() const { return cursor_; }
        void set_cursor(const std::size_t c) { cursor_ = (c <= right_.size()) ? c : cursor_; }
        void inc_cursor() { set_cursor(cursor_ + 1); }

        bool nonterminal_at_cursor() const { return util::contains(nonterminals_.get(), right_[cursor_]); }

        symbol_set nonterminals() const
        {
            symbol_set nts;
            for (auto const& s : right_)
            {
                // s is nonterminal
                if (util::contains(nonterminals_.get(), s))
                {
                    nts.push_back(s);
                }
            }
            return nts;
        }

        explicit production(symbol_set_ptr const& terminals,
            symbol_set_ptr const& nonterminals,
            symbol_type const& left,
            symbol_set const& right) :
            terminals_ {terminals},
            nonterminals_ {nonterminals},
            left_ {left},
            right_ {right}
        {
            // assumption: terminals intersect nonterminals is empty

            // no terminal symbol on left side of production
            assert(!util::contains(terminals_.get(), left_));
            // non-terminal symbol on left side of production
            assert(util::contains(nonterminals_.get(), left_));

            // every symbol on the right side is terminal or non-terminal
            for (auto& s : right_)
            {
                // right side symbol is either terminal or non-terminal
                assert(util::contains(terminals_.get(), s) || util::contains(nonterminals_.get(), s));
            }
        }
    private:
        symbol_set_ptr terminals_;
        symbol_set_ptr nonterminals_;

        symbol_type left_;
        symbol_set right_;

        std::size_t cursor_ {};
    };

    template <typename symbol_type>
    bool operator == (production<symbol_type> const& a, production<symbol_type> const& b)
    {
        return a.left() == b.left && a.right() == b.right();
    }
} // namespace compiler