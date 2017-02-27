//
// Generated file, do not edit! Created by nedtool 5.0 from veins/modules/Prext/base/messages/WAVEBeacon.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#include <iostream>
#include <sstream>
#include "WAVEBeacon_m.h"

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
    throw omnetpp::cRuntimeError("Parsim error: no doParsimPacking() function for type %s", omnetpp::opp_typename(typeid(t)));
}

template<typename T>
void doParsimUnpacking(omnetpp::cCommBuffer *, T& t)
{
    throw omnetpp::cRuntimeError("Parsim error: no doParsimUnpacking() function for type %s", omnetpp::opp_typename(typeid(t)));
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

Register_Class(WAVEBeacon);

WAVEBeacon::WAVEBeacon(const char *name, int kind) : ::WaveShortMessage(name,kind)
{
    this->senderPsynm = 0;
    this->senderAngle = 0;
    this->CPN_readyFlag = false;
    this->isEncrypted = false;
}

WAVEBeacon::WAVEBeacon(const WAVEBeacon& other) : ::WaveShortMessage(other)
{
    copy(other);
}

WAVEBeacon::~WAVEBeacon()
{
}

WAVEBeacon& WAVEBeacon::operator=(const WAVEBeacon& other)
{
    if (this==&other) return *this;
    ::WaveShortMessage::operator=(other);
    copy(other);
    return *this;
}

void WAVEBeacon::copy(const WAVEBeacon& other)
{
    this->senderPsynm = other.senderPsynm;
    this->senderVel = other.senderVel;
    this->senderAngle = other.senderAngle;
    this->CPN_readyFlag = other.CPN_readyFlag;
    this->isEncrypted = other.isEncrypted;
}

void WAVEBeacon::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::WaveShortMessage::parsimPack(b);
    doParsimPacking(b,this->senderPsynm);
    doParsimPacking(b,this->senderVel);
    doParsimPacking(b,this->senderAngle);
    doParsimPacking(b,this->CPN_readyFlag);
    doParsimPacking(b,this->isEncrypted);
}

void WAVEBeacon::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::WaveShortMessage::parsimUnpack(b);
    doParsimUnpacking(b,this->senderPsynm);
    doParsimUnpacking(b,this->senderVel);
    doParsimUnpacking(b,this->senderAngle);
    doParsimUnpacking(b,this->CPN_readyFlag);
    doParsimUnpacking(b,this->isEncrypted);
}

uint64_t WAVEBeacon::getSenderPsynm() const
{
    return this->senderPsynm;
}

void WAVEBeacon::setSenderPsynm(uint64_t senderPsynm)
{
    this->senderPsynm = senderPsynm;
}

Coord& WAVEBeacon::getSenderVel()
{
    return this->senderVel;
}

void WAVEBeacon::setSenderVel(const Coord& senderVel)
{
    this->senderVel = senderVel;
}

double WAVEBeacon::getSenderAngle() const
{
    return this->senderAngle;
}

void WAVEBeacon::setSenderAngle(double senderAngle)
{
    this->senderAngle = senderAngle;
}

bool WAVEBeacon::getCPN_readyFlag() const
{
    return this->CPN_readyFlag;
}

void WAVEBeacon::setCPN_readyFlag(bool CPN_readyFlag)
{
    this->CPN_readyFlag = CPN_readyFlag;
}

bool WAVEBeacon::getIsEncrypted() const
{
    return this->isEncrypted;
}

void WAVEBeacon::setIsEncrypted(bool isEncrypted)
{
    this->isEncrypted = isEncrypted;
}

class WAVEBeaconDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertynames;
  public:
    WAVEBeaconDescriptor();
    virtual ~WAVEBeaconDescriptor();

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

    virtual std::string getFieldValueAsString(void *object, int field, int i) const override;
    virtual bool setFieldValueAsString(void *object, int field, int i, const char *value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual void *getFieldStructValuePointer(void *object, int field, int i) const override;
};

Register_ClassDescriptor(WAVEBeaconDescriptor);

WAVEBeaconDescriptor::WAVEBeaconDescriptor() : omnetpp::cClassDescriptor("WAVEBeacon", "WaveShortMessage")
{
    propertynames = nullptr;
}

WAVEBeaconDescriptor::~WAVEBeaconDescriptor()
{
    delete[] propertynames;
}

bool WAVEBeaconDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<WAVEBeacon *>(obj)!=nullptr;
}

const char **WAVEBeaconDescriptor::getPropertyNames() const
{
    if (!propertynames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
        const char **basenames = basedesc ? basedesc->getPropertyNames() : nullptr;
        propertynames = mergeLists(basenames, names);
    }
    return propertynames;
}

const char *WAVEBeaconDescriptor::getProperty(const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : nullptr;
}

int WAVEBeaconDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 5+basedesc->getFieldCount() : 5;
}

unsigned int WAVEBeaconDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeFlags(field);
        field -= basedesc->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,
        FD_ISCOMPOUND,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
    };
    return (field>=0 && field<5) ? fieldTypeFlags[field] : 0;
}

const char *WAVEBeaconDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldName(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldNames[] = {
        "senderPsynm",
        "senderVel",
        "senderAngle",
        "CPN_readyFlag",
        "isEncrypted",
    };
    return (field>=0 && field<5) ? fieldNames[field] : nullptr;
}

int WAVEBeaconDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount() : 0;
    if (fieldName[0]=='s' && strcmp(fieldName, "senderPsynm")==0) return base+0;
    if (fieldName[0]=='s' && strcmp(fieldName, "senderVel")==0) return base+1;
    if (fieldName[0]=='s' && strcmp(fieldName, "senderAngle")==0) return base+2;
    if (fieldName[0]=='C' && strcmp(fieldName, "CPN_readyFlag")==0) return base+3;
    if (fieldName[0]=='i' && strcmp(fieldName, "isEncrypted")==0) return base+4;
    return basedesc ? basedesc->findField(fieldName) : -1;
}

const char *WAVEBeaconDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeString(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "uint64_t",
        "Coord",
        "double",
        "bool",
        "bool",
    };
    return (field>=0 && field<5) ? fieldTypeStrings[field] : nullptr;
}

const char **WAVEBeaconDescriptor::getFieldPropertyNames(int field) const
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

const char *WAVEBeaconDescriptor::getFieldProperty(int field, const char *propertyname) const
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

int WAVEBeaconDescriptor::getFieldArraySize(void *object, int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldArraySize(object, field);
        field -= basedesc->getFieldCount();
    }
    WAVEBeacon *pp = (WAVEBeacon *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string WAVEBeaconDescriptor::getFieldValueAsString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldValueAsString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    WAVEBeacon *pp = (WAVEBeacon *)object; (void)pp;
    switch (field) {
        case 0: return uint642string(pp->getSenderPsynm());
        case 1: {std::stringstream out; out << pp->getSenderVel(); return out.str();}
        case 2: return double2string(pp->getSenderAngle());
        case 3: return bool2string(pp->getCPN_readyFlag());
        case 4: return bool2string(pp->getIsEncrypted());
        default: return "";
    }
}

bool WAVEBeaconDescriptor::setFieldValueAsString(void *object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->setFieldValueAsString(object,field,i,value);
        field -= basedesc->getFieldCount();
    }
    WAVEBeacon *pp = (WAVEBeacon *)object; (void)pp;
    switch (field) {
        case 0: pp->setSenderPsynm(string2uint64(value)); return true;
        case 2: pp->setSenderAngle(string2double(value)); return true;
        case 3: pp->setCPN_readyFlag(string2bool(value)); return true;
        case 4: pp->setIsEncrypted(string2bool(value)); return true;
        default: return false;
    }
}

const char *WAVEBeaconDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructName(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        case 1: return omnetpp::opp_typename(typeid(Coord));
        default: return nullptr;
    };
}

void *WAVEBeaconDescriptor::getFieldStructValuePointer(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructValuePointer(object, field, i);
        field -= basedesc->getFieldCount();
    }
    WAVEBeacon *pp = (WAVEBeacon *)object; (void)pp;
    switch (field) {
        case 1: return (void *)(&pp->getSenderVel()); break;
        default: return nullptr;
    }
}


