#ifndef __OBJECTLAYOUTMANAGER_H__
#define __OBJECTLAYOUTMANAGER_H__

#include <map>
#include <string>

class ObjectLayout;
class CompilationTable;

class ObjectLayoutManager {
    private:
        std::map<CompilationTable*, ObjectLayout*> layouts;

        void createLayoutForClass(CompilationTable*);
    public:
        ObjectLayoutManager(std::map<std::string, CompilationTable*>&);
        ~ObjectLayoutManager();
        ObjectLayout* getLayoutForClass(CompilationTable*);
};

#endif
