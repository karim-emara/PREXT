//
// Generated file, do not edit! Created by nedtool 5.0 from veins/modules/Prext/base/messages/MixZoneAd.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#include <iostream>
#include <sstream>
#include "MixZoneAd_m.h"

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

Register_Class(MixZoneAd);

MixZoneAd::MixZoneAd(const char *name, int kind) : ::WaveShortMessage(name,kind)
{
    this->zoneType = 1;
    this->circularRange = 0;
    zonePolygon_arraysize = 0;
    this->zonePolygon = 0;
}

MixZoneAd::MixZoneAd(const MixZoneAd& other) : ::WaveShortMessage(other)
{
    zonePolygon_arraysize = 0;
    this->zonePolygon = 0;
    copy(other);
}

MixZoneAd::~MixZoneAd()
{
    delete [] this->zonePolygon;
}

MixZoneAd& MixZoneAd::operator=(const MixZoneAd& other)
{
    if (this==&other) return *this;
    ::WaveShortMessage::operator=(other);
    copy(other);
    return *this;
}

void MixZoneAd::copy(const MixZoneAd& other)
{
    this->zoneType = other.zoneType;
    this->circularRange = other.circularRange;
    delete [] this->zonePolygon;
    this->zonePolygon = (other.zonePolygon_arraysize==0) ? nullptr : new Convex[other.zonePolygon_arraysize];
    zonePolygon_arraysize = other.zonePolygon_arraysize;
    for (unsigned int i=0; i<zonePolygon_arraysize; i++)
        this->zonePolygon[i] = other.zonePolygon[i];
}

void MixZoneAd::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::WaveShortMessage::parsimPack(b);
    doParsimPacking(b,this->zoneType);
    doParsimPacking(b,this->circularRange);
    b->pack(zonePolygon_arraysize);
    doParsimArrayPacking(b,this->zonePolygon,zonePolygon_arraysize);
}

void MixZoneAd::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::WaveShortMessage::parsimUnpack(b);
    doParsimUnpacking(b,this->zoneType);
    doParsimUnpacking(b,this->circularRange);
    delete [] this->zonePolygon;
    b->unpack(zonePolygon_arraysize);
    if (zonePolygon_arraysize==0) {
        this->zonePolygon = 0;
    } else {
        this->zonePolygon = new Convex[zonePolygon_arraysize];
        doParsimArrayUnpacking(b,this->zonePolygon,zonePolygon_arraysize);
    }
}

int MixZoneAd::getZoneType() const
{
    return this->zoneType;
}

void MixZoneAd::setZoneType(int zoneType)
{
    this->zoneType = zoneType;
}

int MixZoneAd::getCircularRange() const
{
    return this->circularRange;
}

void MixZoneAd::setCircularRange(int circularRange)
{
    this->circularRange = circularRange;
}

void MixZoneAd::setZonePolygonArraySize(unsigned int size)
{
    Convex *zonePolygon2 = (size==0) ? nullptr : new Convex[size];
    unsigned int sz = zonePolygon_arraysize < size ? zonePolygon_arraysize : size;
    for (unsigned int i=0; i<sz; i++)
        zonePolygon2[i] = this->zonePolygon[i];
    zonePolygon_arraysize = size;
    delete [] this->zonePolygon;
    this->zonePolygon = zonePolygon2;
}

unsigned int MixZoneAd::getZonePolygonArraySize() const
{
    return zonePolygon_arraysize;
}

Convex& MixZoneAd::getZonePolygon(unsigned int k)
{
    if (k>=zonePolygon_arraysize) throw omnetpp::cRuntimeError("Array of size %d indexed by %d", zonePolygon_arraysize, k);
    return this->zonePolygon[k];
}

void MixZoneAd::setZonePolygon(unsigned int k, const Convex& zonePolygon)
{
    if (k>=zonePolygon_arraysize) throw omnetpp::cRuntimeError("Array of size %d indexed by %d", zonePolygon_arraysize, k);
    this->zonePolygon[k] = zonePolygon;
}

class MixZoneAdDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertynames;
  public:
    MixZoneAdDescriptor();
    virtual ~MixZoneAdDescriptor();

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

Register_ClassDescriptor(MixZoneAdDescriptor);

MixZoneAdDescriptor::MixZoneAdDescriptor() : omnetpp::cClassDescriptor("MixZoneAd", "WaveShortMessage")
{
    propertynames = nullptr;
}

MixZoneAdDescriptor::~MixZoneAdDescriptor()
{
    delete[] propertynames;
}

bool MixZoneAdDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<MixZoneAd *>(obj)!=nullptr;
}

const char **MixZoneAdDescriptor::getPropertyNames() const
{
    if (!propertynames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
        const char **basenames = basedesc ? basedesc->getPropertyNames() : nullptr;
        propertynames = mergeLists(basenames, names);
    }
    return propertynames;
}

const char *MixZoneAdDescriptor::getProperty(const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : nullptr;
}

int MixZoneAdDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 3+basedesc->getFieldCount() : 3;
}

unsigned int MixZoneAdDescriptor::getFieldTypeFlags(int field) const
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
        FD_ISARRAY | FD_ISCOMPOUND,
    };
    return (field>=0 && field<3) ? fieldTypeFlags[field] : 0;
}

const char *MixZoneAdDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldName(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldNames[] = {
        "zoneType",
        "circularRange",
        "zonePolygon",
    };
    return (field>=0 && field<3) ? fieldNames[field] : nullptr;
}

int MixZoneAdDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount() : 0;
    if (fieldName[0]=='z' && strcmp(fieldName, "zoneType")==0) return base+0;
    if (fieldName[0]=='c' && strcmp(fieldName, "circularRange")==0) return base+1;
    if (fieldName[0]=='z' && strcmp(fieldName, "zonePolygon")==0) return base+2;
    return basedesc ? basedesc->findField(fieldName) : -1;
}

const char *MixZoneAdDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeString(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "int",
        "int",
        "Convex",
    };
    return (field>=0 && field<3) ? fieldTypeStrings[field] : nullptr;
}

const char **MixZoneAdDescriptor::getFieldPropertyNames(int field) const
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

const char *MixZoneAdDescriptor::getFieldProperty(int field, const char *propertyname) const
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

int MixZoneAdDescriptor::getFieldArraySize(void *object, int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldArraySize(object, field);
        field -= basedesc->getFieldCount();
    }
    MixZoneAd *pp = (MixZoneAd *)object; (void)pp;
    switch (field) {
        case 2: return pp->getZonePolygonArraySize();
        default: return 0;
    }
}

std::string MixZoneAdDescriptor::getFieldValueAsString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldValueAsString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    MixZoneAd *pp = (MixZoneAd *)object; (void)pp;
    switch (field) {
        case 0: return long2string(pp->getZoneType());
        case 1: return long2string(pp->getCircularRange());
        case 2: {std::stringstream out; out << pp->getZonePolygon(i); return out.str();}
        default: return "";
    }
}

bool MixZoneAdDescriptor::setFieldValueAsString(void *object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->setFieldValueAsString(object,field,i,value);
        field -= basedesc->getFieldCount();
    }
    MixZoneAd *pp = (MixZoneAd *)object; (void)pp;
    switch (field) {
        case 0: pp->setZoneType(string2long(value)); return true;
        case 1: pp->setCircularRange(string2long(value)); return true;
        default: return false;
    }
}

const char *MixZoneAdDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructName(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        case 2: return omnetpp::opp_typename(typeid(Convex));
        default: return nullptr;
    };
}

void *MixZoneAdDescriptor::getFieldStructValuePointer(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructValuePointer(object, field, i);
        field -= basedesc->getFieldCount();
    }
    MixZoneAd *pp = (MixZoneAd *)object; (void)pp;
    switch (field) {
        case 2: return (void *)(&pp->getZonePolygon(i)); break;
        default: return nullptr;
    }
}


