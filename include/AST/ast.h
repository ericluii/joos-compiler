#ifndef __AST_H__
#define __AST_H__

class Ast {
    protected:
        int rule;
    public:
        virtual bool isEpsilon() { return false; }
        virtual ~Ast() {}
        void setRule(int set) { rule = set; }
        int getRule() { return rule; }
};

#endif
