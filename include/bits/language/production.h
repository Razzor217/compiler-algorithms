#pragma once

#include <cassert>
#include <memory>
#include <set>

namespace compiler
{
    template <typename symbol_type>
    class production
    {
    public:
        using set_type = std::set<symbol_type>;
        using set_pointer = std::shared_ptr<set_type>;
        using vector_type = std::vector<symbol_type>;
        using size_type = std::size_t;

        inline set_pointer& terminal_symbols() const { return terminals_; }
        inline set_pointer& nonterminal_symbols() const { return nonterminals_; }
        inline symbol_type left() const { return left_; }
        inline vector_type right() const { return right_; }

        inline size_type cursor() const { return cursor_; }
        void set_cursor(const size_type c) { cursor_ = (c <= right_.size()) ? c : cursor_; }
        void inc_cursor() { set_cursor(cursor_ + 1); }

        bool nonterminal_at_cursor() const 
        {
            if (right_.empty()) { return false; }
            return nonterminals_->contains(right_[cursor_]);
        }

        vector_type nonterminals() const
        {
            vector_type nts;
            for (auto const& s : right_)
            {
                // s is nonterminal symbol
                if (nonterminals_->contains(s)) { nts.push_back(s); }
            }
            return nts;
        }

        explicit production(set_pointer const& terminals,
            set_pointer const& nonterminals,
            symbol_type const& left,
            vector_type const& right) :
            terminals_ {terminals},
            nonterminals_ {nonterminals},
            left_ {left},
            right_ {right}
        {
            // assumption: terminals intersect nonterminals is empty

            // no terminal symbol on left side of production
            assert(!terminals_->contains(left_));
            // non-terminal symbol on left side of production
            assert(nonterminals_->contains(left_));

            // every symbol on the right side is terminal or non-terminal
            for (auto& s : right_)
            {
                // right side symbol is either terminal or non-terminal
                assert(terminals_->contains(s) || nonterminals_->contains(s));
            }
        }
    private:
        set_pointer terminals_;
        set_pointer nonterminals_;

        symbol_type left_;
        vector_type right_;

        size_type cursor_ {};
    };

    template <typename symbol_type>
    bool operator == (production<symbol_type> const& a, production<symbol_type> const& b)
    {
        return a.left() == b.left && a.right() == b.right();
    }

    template <typename symbol_type>
    class compare_productions
    {
    public:
        using production_type = production<symbol_type>;

        bool operator () (production_type const& a, production_type const& b)
        {
            // compare nonterminals of lhs
            if (a.left() < b.left()) { return true; }
            if (a.left() > b.left()) { return false; }

            // compare all symbols of rhs
            for (auto it_a = a.right().begin(), it_b = b.right().begin();
                it_a != a.right().end() && it_b != b.right().end();
                ++it_a, ++it_b)
            {
                if (*it_a < *it_b) { return true; }
                if (*it_a > *it_b) { return false; }
            }

            // first k symbols of rhs are equal, check length
            if (a.right().size() < b.right().size()) { return true; }
            if (a.right().size() > b.right().size()) { return false; }

            // productions have same symbols, check cursors
            if (a.cursor() <= b.cursor()) { return true; }
            return false;
        }
    };
} // namespace compiler