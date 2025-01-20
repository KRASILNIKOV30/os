#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <unordered_set>
#include "DFA.h"

enum TokenType
{
    ERROR = -1,
    PARAN_OPEN = 1,         // (
    PARAN_CLOSE,        // )
    CURLY_OPEN,         // {
    CURLY_CLOSE,        // }
    OP_PLUS,            // +
    OP_TIMES,           // *
    OP_ASSIGNMENT,      // =
    COLON,              // :
    SEMI_COLON,         // ;
    EOL,                // End of line
    WHITESPACE,         // white space
    ID,                 // Identifier
    COMMENT,            // Comment
    MULTILINE_COMMENT,  // Multiline comment
    INTEGER,            // Integer
    EXP,                // Expanent
    EXP_INVALID,        // Exp invalid
    DOT,                // DOT
    FLOAT,              // FLOAT
    STRING_LITERAL,     // String literal
    IF = 200,           // IF
    ELSE,               // ELSE
    STRING,             // STRING
    WHILE,              // WHILE
    INT,                // INT
    RETURN,             // RETURN
    COMMENT_INVALID = 400,
    MULTILINE_COMMENT_INVALID,
    STRING_LITERAL_ERROR,
    FLOAT_INVALID,
    LENGTH_ERROR,
    STRING_LITERAL_INCOMPLETE,
    MULTILINE_COMMENT_INCOMPLETE,
};

std::unordered_set<std::string> RESERVED_WORDS =
{
    "if",
    "else",
    "while",
    "return",
    "int",
    "string",
};

bool IsReservedWord(const std::string& lexeme)
{
    return RESERVED_WORDS.find(lexeme) != RESERVED_WORDS.end();
}

static TokenType getReservedWordToken(std::string const& reservedWord)
{
    if (reservedWord == "if")
        return TokenType::IF;
    else if (reservedWord == "else")
        return TokenType::ELSE;
    else if (reservedWord == "while")
        return TokenType::WHILE;
    else if (reservedWord == "return")
        return TokenType::RETURN;
    else if (reservedWord == "int")
        return TokenType::INT;
    else if (reservedWord == "string")
        return TokenType::STRING;
    return TokenType::ID;
}


struct Token
{
    TokenType type;
    string lexeme;
    size_t line;
    size_t pos;
};

static std::string getTokenName(TokenType token)
{
    switch (token)
    {
    case ERROR:                         return "ERROR";
    case COMMENT_INVALID:               return "COMMENT_INVALID";
    case MULTILINE_COMMENT_INVALID:     return "MULTILINE_COMMENT_INVALID";
    case MULTILINE_COMMENT_INCOMPLETE:  return "MULTILINE_COMMENT_INCOMPLETE";
    case STRING_LITERAL_ERROR:          return "STRING_LITERAL_ERROR";
    case STRING_LITERAL_INCOMPLETE:     return "STRING_LITERAL_INCOMPLETE";
    case FLOAT_INVALID:                 return "FLOAT_INVALID";
    case LENGTH_ERROR:                  return "LENGTH_ERROR";
    case MULTILINE_COMMENT: return "MULTILINE_COMMENT";
    case PARAN_OPEN:        return "PARAN_OPEN";
    case PARAN_CLOSE:       return "PARAN_CLOSE";
    case CURLY_OPEN:        return "CURLY_OPEN";
    case CURLY_CLOSE:       return "CURLY_CLOSE";
    case OP_PLUS:           return "OP_PLUS";
    case OP_TIMES:          return "OP_TIMES";
    case OP_ASSIGNMENT:     return "OP_ASSIGNMENT";
    case COLON:             return "COLON";
    case SEMI_COLON:        return "SEMI_COLON";
    case EOL:               return "EOL";
    case WHITESPACE:        return "WHITESPACE";
    case ID:                return "ID";
    case COMMENT:           return "COMMENT";
    case INTEGER:           return "INTEGER";
    case EXP:               return "EXP";
    case EXP_INVALID:       return "EXP_INVALID";
    case DOT:               return "DOT";
    case FLOAT:             return "FLOAT";
    case IF:                return "IF";
    case ELSE:              return "ELSE";
    case WHILE:             return "WHILE";
    case RETURN:            return "RETURN";
    case STRING:            return "STRING";
    case STRING_LITERAL:    return "STRING_LITERAL";
    case INT:               return "INTEGER";
    default:                return "default";
    }
}

class Lexer
{
public:
    explicit Lexer() :
        m_dfa(0, false)
    {
        m_dfa.AddState(TokenType::PARAN_OPEN, true);
        m_dfa.AddState(TokenType::PARAN_CLOSE, true);
        m_dfa.AddState(TokenType::CURLY_OPEN, true);
        m_dfa.AddState(TokenType::CURLY_CLOSE, true);
        m_dfa.AddState(TokenType::OP_PLUS, true);
        m_dfa.AddState(TokenType::OP_TIMES, true);
        m_dfa.AddState(TokenType::OP_ASSIGNMENT, true);
        m_dfa.AddState(TokenType::WHITESPACE, true);
        m_dfa.AddState(TokenType::ID, true);
        m_dfa.AddState(TokenType::INTEGER, true);
        m_dfa.AddState(TokenType::DOT, false);
        m_dfa.AddState(TokenType::FLOAT, true);
        m_dfa.AddState(TokenType::COMMENT_INVALID, false);
        m_dfa.AddState(TokenType::MULTILINE_COMMENT_INVALID, true);
        m_dfa.AddState(TokenType::MULTILINE_COMMENT_INCOMPLETE, true);
        m_dfa.AddState(TokenType::MULTILINE_COMMENT, true);
        m_dfa.AddState(TokenType::COMMENT, true);
        m_dfa.AddState(TokenType::COLON, true);
        m_dfa.AddState(TokenType::SEMI_COLON, true);
        m_dfa.AddState(TokenType::EOL, true);
        m_dfa.AddState(TokenType::STRING_LITERAL, true);
        m_dfa.AddState(TokenType::STRING_LITERAL_INCOMPLETE, true);
        m_dfa.AddState(TokenType::STRING_LITERAL_ERROR, true);
        m_dfa.AddState(TokenType::ERROR, true);
        m_dfa.AddState(TokenType::EXP, true);
        m_dfa.AddState(TokenType::EXP_INVALID, true);
        m_dfa.AddState(TokenType::FLOAT_INVALID, true);

        m_dfa.AddTransition(0, '(', TokenType::PARAN_OPEN);
        m_dfa.AddTransition(0, ')', TokenType::PARAN_CLOSE);
        m_dfa.AddTransition(0, '{', TokenType::CURLY_OPEN);
        m_dfa.AddTransition(0, '}', TokenType::CURLY_CLOSE);
        m_dfa.AddTransition(0, '+', TokenType::OP_PLUS);
        m_dfa.AddTransition(0, '*', TokenType::OP_TIMES);
        m_dfa.AddTransition(0, '=', TokenType::OP_ASSIGNMENT);
        m_dfa.AddTransition(0, ':', TokenType::COLON);
        m_dfa.AddTransition(0, ';', TokenType::SEMI_COLON);

        m_dfa.AddTransition(0, '\n', TokenType::EOL);
        m_dfa.AddTransition(TokenType::EOL, '\n', TokenType::EOL);
        m_dfa.AddTransition(0, '\r\n', TokenType::EOL);
        m_dfa.AddTransition(TokenType::EOL, '\r\n', TokenType::EOL);
        m_dfa.AddTransition(0, '\r', TokenType::EOL);
        m_dfa.AddTransition(TokenType::EOL, '\r', TokenType::EOL);

        m_dfa.AddTransition(0, ' ', TokenType::WHITESPACE);
        m_dfa.AddTransition(TokenType::WHITESPACE, ' ', TokenType::WHITESPACE);
        m_dfa.AddTransition(0, '"', TokenType::STRING_LITERAL_INCOMPLETE);
        m_dfa.AddTransition(TokenType::STRING_LITERAL_INCOMPLETE, '\n', TokenType::STRING_LITERAL_INCOMPLETE);
        m_dfa.AddTransition(TokenType::STRING_LITERAL_INCOMPLETE, '\r\n', TokenType::STRING_LITERAL_INCOMPLETE);
        m_dfa.AddTransition(TokenType::STRING_LITERAL_INCOMPLETE, '\r', TokenType::STRING_LITERAL_INCOMPLETE);
        m_dfa.AddTransition(TokenType::STRING_LITERAL_INCOMPLETE, ' ', TokenType::STRING_LITERAL_INCOMPLETE);
        m_dfa.AddTransition(TokenType::STRING_LITERAL_INCOMPLETE, '"', TokenType::STRING_LITERAL);

        auto addSymbTrans = [&](int symb)
        {
            if (symb != '"' && symb != '\\')
            {
                m_dfa.AddTransition(TokenType::STRING_LITERAL_INCOMPLETE, symb, TokenType::STRING_LITERAL_INCOMPLETE);
            }
            if (symb != '*' && symb != '/')
            {
                m_dfa.AddTransition(TokenType::COMMENT, symb, TokenType::COMMENT);
                m_dfa.AddTransition(TokenType::MULTILINE_COMMENT_INCOMPLETE, symb, TokenType::MULTILINE_COMMENT_INCOMPLETE);
            }
            m_dfa.AddTransition(TokenType::ERROR, symb, TokenType::ERROR);
        };
        for (char c = 'a'; c <= 'z'; c++)
        {
            char upperC = std::toupper(static_cast<int>(c));
            addSymbTrans(c);
            addSymbTrans(upperC);
            m_dfa.AddTransition(0, c, TokenType::ID);
            m_dfa.AddTransition(0, upperC, TokenType::ID);
            m_dfa.AddTransition(TokenType::ID, c, TokenType::ID);
            m_dfa.AddTransition(TokenType::ID, upperC, TokenType::ID);
        }
        for (auto c = '�'; c <= '�'; c++)
        {
            addSymbTrans(c);
        }
        for (auto c = '�'; c <= '�'; c++)
        {
            addSymbTrans(c);
        }
        for (auto c = 33; c <= 47; c++)  // ������� �� '!' �� '/'
        {
            addSymbTrans(c);
        }
        for (auto c = 58; c <= 64; c++)  //������� �� ':' �� '@'
        {
            addSymbTrans(c);
        }
        for (auto c = 91; c <= 96; c++) // ������� �� '[' �� '`'
        {
            addSymbTrans(c);
        }
        for (auto c = 123; c <= 126; c++) // ������� �� '{' �� '~'
        {
            addSymbTrans(c);
        }
        m_dfa.AddTransition(TokenType::ID, '_', TokenType::ID);
        m_dfa.AddTransition(TokenType::ID, '.', TokenType::ID);

        m_dfa.AddTransition(TokenType::STRING_LITERAL_INCOMPLETE, '\\', TokenType::STRING_LITERAL_ERROR);
        for (char c : {'"', '\\', 'n', 't', 'r'})
        {
            m_dfa.AddTransition(TokenType::STRING_LITERAL_ERROR, c, TokenType::STRING_LITERAL_INCOMPLETE);
        }

        m_dfa.AddTransition(0, '.', TokenType::DOT);
        m_dfa.AddTransition(TokenType::INTEGER, '.', TokenType::FLOAT);
        m_dfa.AddTransition(TokenType::FLOAT, '.', TokenType::FLOAT_INVALID);
        m_dfa.AddTransition(TokenType::FLOAT_INVALID, '.', TokenType::FLOAT_INVALID);

        m_dfa.AddTransition(0, '/', TokenType::COMMENT_INVALID);
        m_dfa.AddTransition(TokenType::COMMENT_INVALID, '/', TokenType::COMMENT);
        m_dfa.AddTransition(TokenType::COMMENT_INVALID, '*', TokenType::MULTILINE_COMMENT_INCOMPLETE);
        m_dfa.AddTransition(TokenType::COMMENT, ' ', TokenType::COMMENT);
        m_dfa.AddTransition(TokenType::MULTILINE_COMMENT_INCOMPLETE, ' ', TokenType::MULTILINE_COMMENT_INCOMPLETE);
        m_dfa.AddTransition(TokenType::MULTILINE_COMMENT_INCOMPLETE, '\n', TokenType::MULTILINE_COMMENT_INCOMPLETE);
        m_dfa.AddTransition(TokenType::MULTILINE_COMMENT_INCOMPLETE, '\t', TokenType::MULTILINE_COMMENT_INCOMPLETE);
        m_dfa.AddTransition(TokenType::MULTILINE_COMMENT_INCOMPLETE, '\r', TokenType::MULTILINE_COMMENT_INCOMPLETE);
        m_dfa.AddTransition(TokenType::MULTILINE_COMMENT_INCOMPLETE, '\r\n', TokenType::MULTILINE_COMMENT_INCOMPLETE);
        m_dfa.AddTransition(TokenType::MULTILINE_COMMENT_INCOMPLETE, '*', TokenType::MULTILINE_COMMENT_INVALID);
        m_dfa.AddTransition(TokenType::MULTILINE_COMMENT_INVALID, '/', TokenType::MULTILINE_COMMENT);

        for (char c = '0'; c <= '9'; c++)
        {
            m_dfa.AddTransition(0, c, TokenType::INTEGER);
            m_dfa.AddTransition(TokenType::ID, c, TokenType::ID);
            m_dfa.AddTransition(TokenType::INTEGER, c, TokenType::INTEGER);
            m_dfa.AddTransition(TokenType::FLOAT, c, TokenType::FLOAT);
            m_dfa.AddTransition(TokenType::FLOAT_INVALID, c, TokenType::FLOAT_INVALID);
            m_dfa.AddTransition(TokenType::DOT, c, TokenType::FLOAT);
            m_dfa.AddTransition(TokenType::EXP, c, TokenType::EXP);
            addSymbTrans(c);
        }

        m_dfa.AddTransition(TokenType::DOT, 'e', TokenType::EXP_INVALID);
        m_dfa.AddTransition(TokenType::DOT, 'E', TokenType::EXP_INVALID);
        m_dfa.AddTransition(TokenType::FLOAT, 'e', TokenType::EXP_INVALID);
        m_dfa.AddTransition(TokenType::FLOAT, 'E', TokenType::EXP_INVALID);
        m_dfa.AddTransition(TokenType::EXP_INVALID, '-', TokenType::EXP);
        m_dfa.AddTransition(TokenType::EXP_INVALID, '+', TokenType::EXP);
    }

    std::vector<Token> Lex(std::string input)
    {
        std::vector<Token> result;
        std::string lexeme;
        m_dfa.Reset();
        std::stack<char> parenStack;

        m_currPos = m_startPos = m_currLine = m_startLine = 1;

        for (auto i = input.begin(); i < input.end() - 1; i++)
        {
            lexeme += *i;
            m_dfa.Input(*i);

            auto currState = m_dfa.State();
            if (currState != TokenType::COMMENT && currState != TokenType::MULTILINE_COMMENT_INCOMPLETE && currState != TokenType::STRING_LITERAL_INCOMPLETE)
            {
                if (*i == '(' || *i == '{')
                {
                    parenStack.push(*i);
                }
                else if (*i == ')' || *i == '}')
                {
                    if (!parenStack.empty() && ((parenStack.top() == '(' && *i == ')') ||
                        (parenStack.top() == '{' && *i == '}')))
                    {
                        parenStack.pop();
                    }
                }
            }

            char next = *(i + 1);
            if (m_dfa.IsAccepting() && !m_dfa.IsAccepting(next))
            {
                InsertToken(result, lexeme);
                m_startLine = m_currLine;
                m_startPos = m_currPos;
            }
            else
            {
                m_currPos++;
            }
        }

        InsertToken(result, lexeme);

        if (!parenStack.empty())
        {
            Token errorToken;
            errorToken.type = TokenType::ERROR;
            errorToken.lexeme = string("Error this scope is not closed: ") + parenStack.top();
            errorToken.pos = m_currPos;
            errorToken.line = m_currLine;
            result.push_back(errorToken);
		}

        std::cout << "-----ID-----" << std::endl;
        for (const auto& tk : result)
        {
            if (tk.type == TokenType::ID)
            {
                std::cout << "[" << tk.line << ":" << tk.pos << "] " << tk.lexeme << std::endl;
            }
        }
        std::cout << "------------" << std::endl;

		return result;
	}

private:
	DFA<char> m_dfa;
    size_t m_currLine, m_currPos, m_startLine, m_startPos;

    void InsertToken(std::vector<Token>& result, std::string& lexeme)
    {
        Token tk;
        tk.lexeme = lexeme;
        tk.type = (TokenType)m_dfa.State();
        tk.pos = m_startPos;
        tk.line = m_startLine;

        if (tk.type == TokenType::ID && IsReservedWord(tk.lexeme))
        {
            tk.type = getReservedWordToken(tk.lexeme);
        }
        else if (tk.type == TokenType::EOL)
        {
            tk.lexeme = "\\n";
        }

        if ((tk.type == TokenType::ID || tk.type == TokenType::INTEGER || tk.type == TokenType::FLOAT) && tk.lexeme.length() > 50)
        {
            tk.type = TokenType::LENGTH_ERROR;
            tk.lexeme = tk.lexeme;
            result.push_back(tk);
            lexeme.clear();
            m_dfa.Reset();
            return;
        }

        if (tk.type >= TokenType::COMMENT_INVALID)
        {
            tk.lexeme = string("Error [") + std::to_string(tk.type) + ']';
            tk.type = TokenType::ERROR;
        }

        if (tk.type != TokenType::WHITESPACE && tk.type != TokenType::EOL && tk.type != TokenType::COMMENT && tk.type != TokenType::MULTILINE_COMMENT)
        {
            result.push_back(tk);
        }
        if (tk.type == TokenType::EOL)
        {
            m_currLine++;
            m_currPos = 1;
        }
        else
        {
            m_currPos++;
        }
        lexeme.clear();
        m_dfa.Reset();
    }
};