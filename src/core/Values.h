#ifndef VALUES_H
#define VALUES_H

#include "Units.h"
#include "core/OriFloatingPoint.h"

namespace Z {

class Value
{
public:
    Value() {}
    Value(double value, Unit unit): _value(value), _unit(unit) {}
    Value(const Value& other): _value(other.value()), _unit(other.unit()) {}

    const double& value() const { return _value; }
    Unit unit() const { return _unit; }

    QString str() const;

    /// Returns string representation suitable for displaying to user.
    /// It should not be such as `0.776242642387489237489 mm`
    /// but more like to `0.776 mm` depending on application preferences.
    /// It uses @a Z::format() method to prepare value.
    QString displayStr() const;

    double toSi() const { return _unit->toSi(_value); }

    void operator = (const Value& v) { _value = v._value, _unit = v._unit; }
    bool operator == (const Value& v) const { return Double(toSi()).is(v.toSi()); }
    bool operator == (const double& v) const { return Double(toSi()).is(v); }
    bool operator != (const Value& v) const { return Double(toSi()).isNot(v.toSi()); }
    bool operator != (const double& v) const { return Double(toSi()).isNot(v); }
    bool operator > (const Value& v) const { return toSi() > v.toSi(); }
    bool operator > (const double& v) const { return toSi() > v; }
    bool operator >= (const Value& v) const { return toSi() >= v.toSi(); }
    bool operator >= (const double& v) const { return toSi() >= v; }
    bool operator < (const Value& v) const { return toSi() < v.toSi(); }
    bool operator < (const double& v) const { return toSi() < v; }
    bool operator <= (const Value& v) const { return toSi() <= v.toSi(); }
    bool operator <= (const double& v) const { return toSi() <= v; }

    QString toStoredStr() const;
    bool fromStoredStr(const QString& s);

private:
    double _value;
    Unit _unit;
};

//------------------------------------------------------------------------------

class ValueTS
{
    double _valueT, _valueS;
    Unit _unit;
};

//------------------------------------------------------------------------------

class PrefixedValue
{
public:
    PrefixedValue(double value, Unit unit);

    QString str() const;

private:
    double _value;
    Unit _unit;
    Z::Units::Prefix _prefix;
};

//------------------------------------------------------------------------------

template <typename TValue>
struct PairTS
{
    TValue T {};
    TValue S {};

    PairTS() {}
    PairTS(const TValue& t, const TValue& s) : T(t), S(s) {}
    PairTS(const PairTS& other) : T(other.T), S(other.S) {}
    PairTS(const PairTS* other) : T(other->T), S(other->S) {}

    void operator =(const TValue& v) { T = v, S = v; }

    void set(const TValue& t, const TValue& s) { T = t, S = s; }

    QString str() const { return QString(); }
};

using PointTS = PairTS<double>;

//------------------------------------------------------------------------------

struct DoublePoint
{
    double X;
    double Y;

    QString str() const;
};

//------------------------------------------------------------------------------

} // namespace Z

#endif // VALUES_H
