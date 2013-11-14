#include "IMsg.h"

BOOST_SERIALIZATION_ASSUME_ABSTRACT(IMsg)

IMsg::~IMsg()
{
}

std::string IMsg::ClassName() const
{
    return "IMsg";
}
