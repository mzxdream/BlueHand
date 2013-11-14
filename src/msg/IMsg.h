#ifndef _IMSG_H_
#define _IMSG_H_

#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/string.hpp>
#include <boost/type_traits/is_abstract.hpp>
#include <boost/serialization/export.hpp>
#include <string>

class IMsg
{
public:
    IMsg() = default;
    virtual ~IMsg();
    IMsg(const IMsg &) = default;
    IMsg& operator=(const IMsg &) = default;
public:
    virtual std::string ClassName() const;
    virtual IMsg* Clone() const = 0;
private:
    friend class boost::serialization::access;
    template<typename Archive>
    void serialize(Archive &ar, const unsigned version);
};

template<typename Archive>
void IMsg::serialize(Archive &ar, const unsigned version)
{
}
#endif
