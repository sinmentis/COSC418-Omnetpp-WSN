//
// Generated file, do not edit! Created by nedtool 5.5 from SignalStart.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#if defined(__clang__)
#  pragma clang diagnostic ignored "-Wshadow"
#  pragma clang diagnostic ignored "-Wconversion"
#  pragma clang diagnostic ignored "-Wunused-parameter"
#  pragma clang diagnostic ignored "-Wc++98-compat"
#  pragma clang diagnostic ignored "-Wunreachable-code-break"
#  pragma clang diagnostic ignored "-Wold-style-cast"
#elif defined(__GNUC__)
#  pragma GCC diagnostic ignored "-Wshadow"
#  pragma GCC diagnostic ignored "-Wconversion"
#  pragma GCC diagnostic ignored "-Wunused-parameter"
#  pragma GCC diagnostic ignored "-Wold-style-cast"
#  pragma GCC diagnostic ignored "-Wsuggest-attribute=noreturn"
#  pragma GCC diagnostic ignored "-Wfloat-conversion"
#endif

#include <iostream>
#include <sstream>
#include "SignalStart_m.h"

namespace omnetpp {

// Template pack/unpack rules. They are declared *after* a1l type-specific pack functions for multiple reasons.
// They are in the omnetpp namespace, to allow them to be found by argument-dependent lookup via the cCommBuffer argument

// Packing/unpacking an std::vector
template<typename T, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::vector<T,A>& v)
{
    int n = v.size();
    doParsimPacking(buffer, n);
    for (int i = 0; i < n; i++)
        doParsimPacking(buffer, v[i]);
}

template<typename T, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::vector<T,A>& v)
{
    int n;
    doParsimUnpacking(buffer, n);
    v.resize(n);
    for (int i = 0; i < n; i++)
        doParsimUnpacking(buffer, v[i]);
}

// Packing/unpacking an std::list
template<typename T, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::list<T,A>& l)
{
    doParsimPacking(buffer, (int)l.size());
    for (typename std::list<T,A>::const_iterator it = l.begin(); it != l.end(); ++it)
        doParsimPacking(buffer, (T&)*it);
}

template<typename T, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::list<T,A>& l)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i=0; i<n; i++) {
        l.push_back(T());
        doParsimUnpacking(buffer, l.back());
    }
}

// Packing/unpacking an std::set
template<typename T, typename Tr, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::set<T,Tr,A>& s)
{
    doParsimPacking(buffer, (int)s.size());
    for (typename std::set<T,Tr,A>::const_iterator it = s.begin(); it != s.end(); ++it)
        doParsimPacking(buffer, *it);
}

template<typename T, typename Tr, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::set<T,Tr,A>& s)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i=0; i<n; i++) {
        T x;
        doParsimUnpacking(buffer, x);
        s.insert(x);
    }
}

// Packing/unpacking an std::map
template<typename K, typename V, typename Tr, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::map<K,V,Tr,A>& m)
{
    doParsimPacking(buffer, (int)m.size());
    for (typename std::map<K,V,Tr,A>::const_iterator it = m.begin(); it != m.end(); ++it) {
        doParsimPacking(buffer, it->first);
        doParsimPacking(buffer, it->second);
    }
}

template<typename K, typename V, typename Tr, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::map<K,V,Tr,A>& m)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i=0; i<n; i++) {
        K k; V v;
        doParsimUnpacking(buffer, k);
        doParsimUnpacking(buffer, v);
        m[k] = v;
    }
}

// Default pack/unpack function for arrays
template<typename T>
void doParsimArrayPacking(omnetpp::cCommBuffer *b, const T *t, int n)
{
    for (int i = 0; i < n; i++)
        doParsimPacking(b, t[i]);
}

template<typename T>
void doParsimArrayUnpacking(omnetpp::cCommBuffer *b, T *t, int n)
{
    for (int i = 0; i < n; i++)
        doParsimUnpacking(b, t[i]);
}

// Default rule to prevent compiler from choosing base class' doParsimPacking() function
template<typename T>
void doParsimPacking(omnetpp::cCommBuffer *, const T& t)
{
    throw omnetpp::cRuntimeError("Parsim error: No doParsimPacking() function for type %s", omnetpp::opp_typename(typeid(t)));
}

template<typename T>
void doParsimUnpacking(omnetpp::cCommBuffer *, T& t)
{
    throw omnetpp::cRuntimeError("Parsim error: No doParsimUnpacking() function for type %s", omnetpp::opp_typename(typeid(t)));
}

}  // namespace omnetpp


// forward
template<typename T, typename A>
std::ostream& operator<<(std::ostream& out, const std::vector<T,A>& vec);

// Template rule which fires if a struct or class doesn't have operator<<
template<typename T>
inline std::ostream& operator<<(std::ostream& out,const T&) {return out;}

// operator<< for std::vector<T>
template<typename T, typename A>
inline std::ostream& operator<<(std::ostream& out, const std::vector<T,A>& vec)
{
    out.put('{');
    for(typename std::vector<T,A>::const_iterator it = vec.begin(); it != vec.end(); ++it)
    {
        if (it != vec.begin()) {
            out.put(','); out.put(' ');
        }
        out << *it;
    }
    out.put('}');
    
    char buf[32];
    sprintf(buf, " (size=%u)", (unsigned int)vec.size());
    out.write(buf, strlen(buf));
    return out;
}

Register_Class(SignalStart)

SignalStart::SignalStart(const char *name, short kind) : ::omnetpp::cPacket(name,kind)
{
    this->transmitPowerDBm = 0;
    this->positionX = 0;
    this->positionY = 0;
    this->identifier = 0;
    this->collidedFlag = false;
    this->pathLossExponent = 0;
}

SignalStart::SignalStart(const SignalStart& other) : ::omnetpp::cPacket(other)
{
    copy(other);
}

SignalStart::~SignalStart()
{
}

SignalStart& SignalStart::operator=(const SignalStart& other)
{
    if (this==&other) return *this;
    ::omnetpp::cPacket::operator=(other);
    copy(other);
    return *this;
}

void SignalStart::copy(const SignalStart& other)
{
    this->transmitPowerDBm = other.transmitPowerDBm;
    this->positionX = other.positionX;
    this->positionY = other.positionY;
    this->identifier = other.identifier;
    this->collidedFlag = other.collidedFlag;
    this->pathLossExponent = other.pathLossExponent;
}

void SignalStart::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::omnetpp::cPacket::parsimPack(b);
    doParsimPacking(b,this->transmitPowerDBm);
    doParsimPacking(b,this->positionX);
    doParsimPacking(b,this->positionY);
    doParsimPacking(b,this->identifier);
    doParsimPacking(b,this->collidedFlag);
    doParsimPacking(b,this->pathLossExponent);
}

void SignalStart::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::omnetpp::cPacket::parsimUnpack(b);
    doParsimUnpacking(b,this->transmitPowerDBm);
    doParsimUnpacking(b,this->positionX);
    doParsimUnpacking(b,this->positionY);
    doParsimUnpacking(b,this->identifier);
    doParsimUnpacking(b,this->collidedFlag);
    doParsimUnpacking(b,this->pathLossExponent);
}

int SignalStart::getTransmitPowerDBm() const
{
    return this->transmitPowerDBm;
}

void SignalStart::setTransmitPowerDBm(int transmitPowerDBm)
{
    this->transmitPowerDBm = transmitPowerDBm;
}

double SignalStart::getPositionX() const
{
    return this->positionX;
}

void SignalStart::setPositionX(double positionX)
{
    this->positionX = positionX;
}

double SignalStart::getPositionY() const
{
    return this->positionY;
}

void SignalStart::setPositionY(double positionY)
{
    this->positionY = positionY;
}

int SignalStart::getIdentifier() const
{
    return this->identifier;
}

void SignalStart::setIdentifier(int identifier)
{
    this->identifier = identifier;
}

bool SignalStart::getCollidedFlag() const
{
    return this->collidedFlag;
}

void SignalStart::setCollidedFlag(bool collidedFlag)
{
    this->collidedFlag = collidedFlag;
}

int SignalStart::getPathLossExponent() const
{
    return this->pathLossExponent;
}

void SignalStart::setPathLossExponent(int pathLossExponent)
{
    this->pathLossExponent = pathLossExponent;
}

class SignalStartDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertynames;
  public:
    SignalStartDescriptor();
    virtual ~SignalStartDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyname) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyname) const override;
    virtual int getFieldArraySize(void *object, int field) const override;

    virtual const char *getFieldDynamicTypeString(void *object, int field, int i) const override;
    virtual std::string getFieldValueAsString(void *object, int field, int i) const override;
    virtual bool setFieldValueAsString(void *object, int field, int i, const char *value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual void *getFieldStructValuePointer(void *object, int field, int i) const override;
};

Register_ClassDescriptor(SignalStartDescriptor)

SignalStartDescriptor::SignalStartDescriptor() : omnetpp::cClassDescriptor("SignalStart", "omnetpp::cPacket")
{
    propertynames = nullptr;
}

SignalStartDescriptor::~SignalStartDescriptor()
{
    delete[] propertynames;
}

bool SignalStartDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<SignalStart *>(obj)!=nullptr;
}

const char **SignalStartDescriptor::getPropertyNames() const
{
    if (!propertynames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
        const char **basenames = basedesc ? basedesc->getPropertyNames() : nullptr;
        propertynames = mergeLists(basenames, names);
    }
    return propertynames;
}

const char *SignalStartDescriptor::getProperty(const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : nullptr;
}

int SignalStartDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 6+basedesc->getFieldCount() : 6;
}

unsigned int SignalStartDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeFlags(field);
        field -= basedesc->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
    };
    return (field>=0 && field<6) ? fieldTypeFlags[field] : 0;
}

const char *SignalStartDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldName(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldNames[] = {
        "transmitPowerDBm",
        "positionX",
        "positionY",
        "identifier",
        "collidedFlag",
        "pathLossExponent",
    };
    return (field>=0 && field<6) ? fieldNames[field] : nullptr;
}

int SignalStartDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount() : 0;
    if (fieldName[0]=='t' && strcmp(fieldName, "transmitPowerDBm")==0) return base+0;
    if (fieldName[0]=='p' && strcmp(fieldName, "positionX")==0) return base+1;
    if (fieldName[0]=='p' && strcmp(fieldName, "positionY")==0) return base+2;
    if (fieldName[0]=='i' && strcmp(fieldName, "identifier")==0) return base+3;
    if (fieldName[0]=='c' && strcmp(fieldName, "collidedFlag")==0) return base+4;
    if (fieldName[0]=='p' && strcmp(fieldName, "pathLossExponent")==0) return base+5;
    return basedesc ? basedesc->findField(fieldName) : -1;
}

const char *SignalStartDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeString(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "int",
        "double",
        "double",
        "int",
        "bool",
        "int",
    };
    return (field>=0 && field<6) ? fieldTypeStrings[field] : nullptr;
}

const char **SignalStartDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldPropertyNames(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *SignalStartDescriptor::getFieldProperty(int field, const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldProperty(field, propertyname);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int SignalStartDescriptor::getFieldArraySize(void *object, int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldArraySize(object, field);
        field -= basedesc->getFieldCount();
    }
    SignalStart *pp = (SignalStart *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

const char *SignalStartDescriptor::getFieldDynamicTypeString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldDynamicTypeString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    SignalStart *pp = (SignalStart *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string SignalStartDescriptor::getFieldValueAsString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldValueAsString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    SignalStart *pp = (SignalStart *)object; (void)pp;
    switch (field) {
        case 0: return long2string(pp->getTransmitPowerDBm());
        case 1: return double2string(pp->getPositionX());
        case 2: return double2string(pp->getPositionY());
        case 3: return long2string(pp->getIdentifier());
        case 4: return bool2string(pp->getCollidedFlag());
        case 5: return long2string(pp->getPathLossExponent());
        default: return "";
    }
}

bool SignalStartDescriptor::setFieldValueAsString(void *object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->setFieldValueAsString(object,field,i,value);
        field -= basedesc->getFieldCount();
    }
    SignalStart *pp = (SignalStart *)object; (void)pp;
    switch (field) {
        case 0: pp->setTransmitPowerDBm(string2long(value)); return true;
        case 1: pp->setPositionX(string2double(value)); return true;
        case 2: pp->setPositionY(string2double(value)); return true;
        case 3: pp->setIdentifier(string2long(value)); return true;
        case 4: pp->setCollidedFlag(string2bool(value)); return true;
        case 5: pp->setPathLossExponent(string2long(value)); return true;
        default: return false;
    }
}

const char *SignalStartDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructName(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    };
}

void *SignalStartDescriptor::getFieldStructValuePointer(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructValuePointer(object, field, i);
        field -= basedesc->getFieldCount();
    }
    SignalStart *pp = (SignalStart *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}


