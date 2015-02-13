#include "name.h"

Name::Name(Name *qualifier, Identifier *id){
    this->qualifier = qualifier;
    this->id = id;
}

Name::Name(Identifier *id){
    this->qualifier = NULL;
    this->id = id;
}

Identifier *Name::GetId(){
    return this->id;
}

Name *Name::GetQualifier(){
    return this->qualifier;
}