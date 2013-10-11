#ifndef _IMSG_H_
#define _IMSG_H_

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/string.hpp>
#include <boost/type_traites/is_abstract.hpp>
#include <string>

class IMsg
{
public:
    IMsg() = default;
    virtual ~IMsg();
    IMsg(const IMsg &) = default;
    IMsg& operator=(const IMsg &) = default;
public:
    virtual std::string GetClassName() const;
    virtual IMsg * Clone() const = 0;
private:
    std::string strNetID;
private:
    friend class boost::serialization::access;
    template <typename Archive>
    void serialize(Archive &ar, const unsigned version)
	{
	}
};

#endif
