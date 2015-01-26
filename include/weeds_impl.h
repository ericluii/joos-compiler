#ifndef __WEEDS_IMPL_H__
#define __WEEDS_IMPL_H__

class Weed
{
    public:
        std::string rule;
        virtual int check(ParseTree* node) = 0;

        virtual ~Weed() {}
};


class DemoWeed final : public Weed
{
    public:
        DemoWeed()
        {
            rule = "banana republic";
        }

        int check(ParseTree* node)
        {
            //TODO: Some kind of checking logic
            return 0;
        }

};

class DemoWeed2 final : public Weed
{
    public:
        DemoWeed2()
        {
            rule = "banana republic eh";
        }

        int check(ParseTree* node)
        {
            //TODO: Some kind of checking logic
            return 1;
        }
};

#endif
