#pragma once

#include <memory>
#include <string_view>
#include <unordered_set>
#include <vector>
#include <iostream>

namespace
{
    static const std::unordered_set<std::size_t> emptySet;
}

class RegexTree
{
public:
    explicit RegexTree(std::string_view regex)
        : root(ConcatEndNode(BuildTree(regex))), alphabet(Alphabet(root.get()))
    {
        CalcFollowPos(root.get());
    }
    const std::unordered_set<char>& Alphabet() const { return alphabet; }

    const std::unordered_set<std::size_t>& FirstPosRoot() const
    {
        return root->firstPos;
    }

    const std::unordered_set<std::size_t>& FollowPos(std::size_t pos) const
    {
        return pos < leaves.size() ? leaves[pos]->followpos : emptySet;
    }

    bool CharAtPos(char character, std::size_t pos) const
    {
        return pos < leaves.size() ? leaves[pos]->label == character : false;
    }

    std::size_t EndPos() const{ return leaves.size(); }

    void PrintTree() const
    {
        PrintTree(root.get(), 0);
    }

private:
    class Node
    {
    public:
        virtual ~Node() = 0;

        std::unordered_set<std::size_t> firstPos;
        std::unordered_set<std::size_t> lastPos;
        bool nullable;
    };

    class ConcatNode : public Node
    {
    public:
        explicit ConcatNode(std::unique_ptr<Node> l, std::unique_ptr<Node> r)
            : left(std::move(l)), right(std::move(r))
        {
            firstPos.insert(left->firstPos.cbegin(), left->firstPos.cend());
            if (left->nullable)
            {
                firstPos.insert(right->firstPos.cbegin(), right->firstPos.cend());
            }

            lastPos.insert(right->lastPos.cbegin(), right->lastPos.cend());
            if (right->nullable)
            {
                lastPos.insert(left->lastPos.cbegin(), left->lastPos.cend());
            }

            nullable = left->nullable && right->nullable;
        }

        std::unique_ptr<Node> left;
        std::unique_ptr<Node> right;
    };

    class UnionNode : public Node
    {
    public:
        explicit UnionNode(std::unique_ptr<Node> l, std::unique_ptr<Node> r)
            : left(std::move(l)), right(std::move(r))
        {
            firstPos.insert(left->firstPos.cbegin(), left->firstPos.cend());
            firstPos.insert(right->firstPos.cbegin(), right->firstPos.cend());

            lastPos.insert(left->lastPos.cbegin(), left->lastPos.cend());
            lastPos.insert(right->lastPos.cbegin(), right->lastPos.cend());

            nullable = left->nullable || right->nullable;
        }

        std::unique_ptr<Node> left;
        std::unique_ptr<Node> right;
    };

    class PlusNode : public Node
    {
    public:
        explicit PlusNode(std::unique_ptr<Node> c) : child(std::move(c))
        {
            firstPos = child->firstPos;
            lastPos = child->lastPos;
            nullable = false;
        }

        std::unique_ptr<Node> child;
    };

    class StarNode : public Node
    {
    public:
        explicit StarNode(std::unique_ptr<Node> c) : child(std::move(c))
        {
            firstPos = child->firstPos;
            lastPos = child->lastPos;
            nullable = true;
        }

        std::unique_ptr<Node> child;
    };

    class LeafNode : public Node
    {
    public:
        explicit LeafNode(std::size_t pos, char l) : label(l)
        {
            firstPos.emplace(pos);
            lastPos.emplace(pos);
            nullable = false;
        }

        std::unordered_set<std::size_t> followpos;
        char label;
    };

    class EndNode : public Node
    {
    public:
        explicit EndNode(std::size_t endPos) { firstPos.insert(endPos); }
    };

    std::unique_ptr<Node> BuildTree(std::string_view regex, bool star = false, bool plus = false);

    std::unique_ptr<Node> ConcatEndNode(std::unique_ptr<RegexTree::Node> root);

    std::unordered_set<char> Alphabet(Node* node);
    void CalcFollowPos(Node* node);

    void PrintTree(const Node* node, int indent) const
    {
        if (node == nullptr) return;

        // Вывод текущего узла с отступом
        std::cout << std::string(indent, ' ') << "Node: ";
        if (dynamic_cast<const ConcatNode*>(node))
        {
            std::cout << "ConcatNode";
        }
        else if (dynamic_cast<const UnionNode*>(node))
        {
            std::cout << "UnionNode";
        }
        else if (dynamic_cast<const PlusNode*>(node))
        {
            std::cout << "PlusNode";
        }
        else if (dynamic_cast<const StarNode*>(node))
        {
            std::cout << "StarNode";
        }
        else if (dynamic_cast<const LeafNode*>(node))
        {
            std::cout << "LeafNode: " << static_cast<const LeafNode*>(node)->label;
        }
        else if (dynamic_cast<const EndNode*>(node))
        {
            std::cout << "EndNode";
        }
        std::cout << std::endl;

        // Рекурсивный вывод дочерних узлов
        if (const ConcatNode* concatNode = dynamic_cast<const ConcatNode*>(node))
        {
            PrintTree(concatNode->left.get(), indent + 4);
            PrintTree(concatNode->right.get(), indent + 4);
        }
        else if (const UnionNode* unionNode = dynamic_cast<const UnionNode*>(node))
        {
            PrintTree(unionNode->left.get(), indent + 4);
            PrintTree(unionNode->right.get(), indent + 4);
        }
        else if (const PlusNode* plusNode = dynamic_cast<const PlusNode*>(node))
        {
            PrintTree(plusNode->child.get(), indent + 4);
        }
        else if (const StarNode* starNode = dynamic_cast<const StarNode*>(node))
        {
            PrintTree(starNode->child.get(), indent + 4);
        }
    }

    std::vector<LeafNode*> leaves;
    std::unique_ptr<Node> root;
    std::unordered_set<char> alphabet;
};