#include "RegexTree.h"

#include <cstddef>
#include <memory>
#include <string_view>
#include <unordered_set>

using namespace std;

RegexTree::Node::~Node() {}

// test

unique_ptr<RegexTree::Node> RegexTree::BuildTree(string_view regex, bool star, bool plus)
{
    if (regex.length() == 1 || (regex.front() == '\\' && regex.length() == 2))
    {
        if (star)
        {
            return make_unique<StarNode>(BuildTree(regex));
        }
        else if (plus)
        {
            return make_unique<PlusNode>(BuildTree(regex));
        }
        else
        {
            auto symb = regex.length() == 1 ? regex[0] : regex[1];
            auto leaf = make_unique<LeafNode>(
                leaves.size(), symb);
            leaves.emplace_back(leaf.get());
            return leaf;
        }
    }

    int openParens = 0;
    for (size_t i = 0; i < regex.length(); ++i)
    {
        if (regex[i] == '\\')
        {
            ++i;
            continue;
        }

        openParens += regex[i] == '(';
        openParens -= regex[i] == ')';
        if (regex[i] == '|' && openParens == 0)
        {
            return make_unique<UnionNode>
            (
                BuildTree(regex.substr(0, i)),
                BuildTree(regex.substr(i + 1))
            );
        }
    }

    switch (regex.back())
    {
    case '+':
    {
        return BuildTree(regex.substr(0, regex.length() - 1), false, true);
    }
    case '*':
    {
        return BuildTree(regex.substr(0, regex.length() - 1), true);
    }
    case ')':
    {
        const auto open_paren_idx = [&]
            {
            int close_parens = 1;
            for (size_t i = regex.length() - 2; i >= 0; --i)
            {
                close_parens += regex[i] == ')';
                close_parens -= regex[i] == '(';
                if (close_parens == 0) return i;
            }
            throw exception();
            }();

            if (open_paren_idx == 0)
            {
                PrintTree();
                if (star)
                {
                    return make_unique<StarNode>(
                        BuildTree(regex.substr(1, regex.length() - 2))
                    );
                }
                else if (plus)
                {
                    return make_unique<PlusNode>(
                        BuildTree(regex.substr(1, regex.length() - 2))
                    );
                }
                else
                {
                    return BuildTree(regex.substr(1, regex.length() - 2));
                }
            }
            else
            {
                auto left = BuildTree(regex.substr(0, open_paren_idx));
                auto right = [&]() -> unique_ptr<Node>
                    {
                    if (star)
                    {
                        return make_unique<StarNode>(
                            BuildTree(regex.substr(open_paren_idx))
                        );
                    }
                    else if (plus)
                    {

                        return make_unique<PlusNode>(
                            BuildTree(regex.substr(open_paren_idx))
                        );
                    }
                    else
                    {
                        return BuildTree(regex.substr(open_paren_idx));
                    }
                    }();
                return make_unique<ConcatNode>(move(left), move(right));
            }
    }
    default:
    {
        auto left = BuildTree(regex.substr(0, regex.length() - 1));
        auto right = [&]() -> unique_ptr<Node>
            {
            if (star)
            {
                return make_unique<StarNode>(
                    BuildTree(regex.substr(regex.length() - 1)));
            }
            else if (plus)
            {
                return make_unique<PlusNode>(
                    BuildTree(regex.substr(regex.length() - 1))
                );
            }
            else
            {
                return BuildTree(regex.substr(regex.length() - 1));
            }
            }();
            return make_unique<ConcatNode>(move(left), move(right));
    }
    }
}

unordered_set<char> RegexTree::Alphabet(RegexTree::Node* n)
{
    if (auto node = dynamic_cast<RegexTree::ConcatNode*>(n))
    {
        auto alphabet1 = Alphabet(node->left.get());
        auto alphabet2 = Alphabet(node->right.get());
        alphabet1.insert(alphabet2.begin(), alphabet2.end());
        return alphabet1;
    }
    else if (auto node = dynamic_cast<RegexTree::UnionNode*>(n))
    {
        auto alphabet1 = Alphabet(node->left.get());
        auto alphabet2 = Alphabet(node->right.get());
        alphabet1.insert(alphabet2.begin(), alphabet2.end());
        return alphabet1;
    }
    else if (auto node = dynamic_cast<RegexTree::PlusNode*>(n))
    {
        return Alphabet(node->child.get());
    }
    else if (auto node = dynamic_cast<RegexTree::StarNode*>(n))
    {
        return Alphabet(node->child.get());
    }
    else if (auto node = dynamic_cast<RegexTree::LeafNode*>(n))
    {
        return unordered_set<char>({ node->label });
    }
    else if (auto node = dynamic_cast<RegexTree::EndNode*>(n))
    {
        return unordered_set<char>();
    }
    else
    {
        throw exception();
    }
}

unique_ptr<RegexTree::Node> RegexTree::ConcatEndNode(
    unique_ptr<RegexTree::Node> root)
{
    auto end_node = make_unique<EndNode>(EndPos());
    return make_unique<ConcatNode>(move(root), move(end_node));
}

void RegexTree::CalcFollowPos(RegexTree::Node* n)
{
    if (auto node = dynamic_cast<RegexTree::UnionNode*>(n))
    {
        CalcFollowPos(node->left.get());
        CalcFollowPos(node->right.get());
    }
    else if (auto node = dynamic_cast<RegexTree::ConcatNode*>(n))
    {
        for (auto i : node->left->lastPos)
        {
            leaves[i]->followpos.insert(node->right->firstPos.cbegin(),
                node->right->firstPos.cend());
        }
        CalcFollowPos(node->left.get());
        CalcFollowPos(node->right.get());
    }
    else if (auto node = dynamic_cast<RegexTree::PlusNode*>(n))
    {
        for (auto i : node->lastPos)
        {
            leaves[i]->followpos.insert(node->firstPos.cbegin(),
                node->firstPos.cend());
        }
        CalcFollowPos(node->child.get());
    }
    else if (auto node = dynamic_cast<RegexTree::StarNode*>(n))
    {
        for (auto i : node->lastPos)
        {
            leaves[i]->followpos.insert(node->firstPos.cbegin(),
                node->firstPos.cend());
        }
        CalcFollowPos(node->child.get());
    }
}