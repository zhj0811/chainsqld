/**

* @file       SQLDataType.h

* @brief      定义 chainSQL 协议层的数据类型过渡到数据库表字段类型.

* @details	详细定义了 chainSQL 过渡到后端数据库 (MYSQL) 表字段类型映射。其中定义了 InnerDateTime,InnerDate,InnerDecimal 等

* @author     dbliu

* @date       2017/12/09

* @version v1.0

* @par Copyright (c):

*      Copyright (c) 2016-2018 Peersafe Technology Co., Ltd.

* @par History:

*   v1.0: dbliu, 2017/12/09, originator\n

*/

//------------------------------------------------------------------------------
/*
 This file is part of chainsqld: https://github.com/chainsql/chainsqld
 Copyright (c) 2016-2018 Peersafe Technology Co., Ltd.
 
	chainsqld is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.
 
	chainsqld is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.
	You should have received a copy of the GNU General Public License
	along with cpp-ethereum.  If not, see <http://www.gnu.org/licenses/>.
 */
//==============================================================================

#ifndef RIPPLE_APP_MISC_SQLDATATYPE_H_INCLUDED
#define RIPPLE_APP_MISC_SQLDATATYPE_H_INCLUDED

#include <string>
#include <cstring> // std::strcmp
#include <cassert> // assert

namespace ripple {

/**

* only for holding space, no sense.

* 略

*/
class InnerDateTime
{
public:
	InnerDateTime() {}
	~InnerDateTime() {}
};

/**

* only for holding space, no sense.

* 略

*/
class InnerDate {
public:
	InnerDate() {}
	~InnerDate() {}
};

/**

* Deciaml 类型

* 将创建表的字段类型解析成 innerDecimal 类型，这种类型和double、float最大的区别是可以定义精确度

*/

class InnerDecimal
{
public:
	InnerDecimal(int length, int accuracy)
		: length_(length)
		, accuracy_(accuracy) {
	}

	~InnerDecimal() {}
	const int length() {
		return length_;
	}

	const int length() const {
		return length_;
	}

	const int accuracy() {
		return accuracy_;
	}

	const int accuracy() const {
		return accuracy_;
	}

	void update(const InnerDecimal& v) {
		length_ = v.length_;
		accuracy_ = v.accuracy_;
	}

	InnerDecimal& operator =(const InnerDecimal& value) {
		length_ = value.length_;
		accuracy_ = value.accuracy_;
		return *this;
	}

	bool operator ==(const InnerDecimal& value) {
		return length_ == value.length_ && accuracy_ == value.accuracy_;
	}

private:
	InnerDecimal() {}
	int length_;	///< length of decimal
	int accuracy_;	///< accuracy of decimal
};

/**

* 创建表时，用于解析具体字段类型

* chainSQL 创建的表的时候，应用层会根据 Raw 协议定义表字段的各种类型，这个类用于解析表字段的名称、类型 

*/
class FieldValue {
public:
	/** FieldValue 构造函数
	*
	*    FieldValue 默认构造函数 
	*/
	explicit FieldValue()
		: value_type_(INNER_UNKOWN) {};

	/** FieldValue 构造函数
	*
	*    根据类型为 string 的值构造 FieldValue
	*	@param value 字段值
	*/
	explicit FieldValue(const std::string& value)
		: value_type_(STRING) {
		value_.str = new std::string;
		if (value_.str) {
			value_.str->assign(value);
		}
	}

	enum { fVARCHAR, fCHAR, fTEXT, fBLOB };
	/** FieldValue 构造函数
	*
	*    根据类型为 string 的值和类型为 int 的值构造 FieldValue			
	*	@param value 字段值
	*	@param flag	字段类型
	*/
	explicit FieldValue(const std::string& value, int flag)
		: value_type_(STRING) {

		if (flag == fVARCHAR)
			value_type_ = VARCHAR;
		else if (flag == fCHAR)
			value_type_ = CHAR;
		else if (flag == fTEXT)
			value_type_ = TEXT;
		else if (flag == fBLOB)
			value_type_ = BLOB;

		value_.str = new std::string;
		if (value_.str) {
			value_.str->assign(value);
		}
	}

	/** FieldValue 构造函数
	*
	*    根据类型为 int 的值构造 FieldValue
	*	@param value 字段值
	*/
	explicit FieldValue(const int value)
		: value_type_(INT) {
		value_.i = value;
	}

	/** FieldValue 构造函数
	*
	*    根据类型为 unsigned int 的值构造 FieldValue
	*	@param value 字段值
	*/
	explicit FieldValue(const unsigned int value)
		: value_type_(UINT) {
		value_.ui = value;
	}

	/** FieldValue 构造函数
	*
	*    根据类型为 float 的值构造 FieldValue
	*	@param f 字段值
	*/
	explicit FieldValue(const float f)
		: value_type_(FLOAT) {
		value_.f = f;
	}

	/** FieldValue 构造函数
	*
	*    根据类型为 double 的值构造 FieldValue
	*	@param d 字段值
	*/
	explicit FieldValue(const double d)
		: value_type_(DOUBLE) {
		value_.d = d;
	}

	/** FieldValue 构造函数
	*
	*    根据类型为 int64_t 的值构造 FieldValue
	*	@param value 字段值
	*/
	explicit FieldValue(const int64_t value)
		: value_type_(LONG64) {
		value_.i64 = value;
	}

	/** FieldValue 构造函数
	*
	*    根据类型为 InnerDateTime 的值构造 FieldValueE
	*	@param datetime 字段值
	*/
	explicit FieldValue(const InnerDateTime& datetime)
		: value_type_(DATETIME) {
		value_.datetime = nullptr;
	}

	/** FieldValue 构造函数
	*
	*    根据类型为 InnerDate 的值构造 FieldValue
	*	@param date 字段值
	*/
	explicit FieldValue(const InnerDate& date)
		: value_type_(DATE) {
		value_.date = nullptr;
	}

	/** FieldValue 构造函数
	*
	*    根据类型为 InnerDecimal 的值构造 FieldValueL
	*	@param d 字段值
	*/
	explicit FieldValue(const InnerDecimal& d)
		: value_type_(DECIMAL) {
		value_.decimal = new InnerDecimal(d.length(), d.accuracy());
	}

	/** FieldValue 拷贝构造函数
	*
	*    根据类型为 FieldValue 的值构造 FieldValue
	*	@param value 字段值
	*/
	explicit FieldValue(const FieldValue& value)
		: value_type_(value.value_type_) {
		assign(value);
	}

	/** FieldValue 赋值运算符
	*
	*    根据右值类型为 FieldValue 的值生成 FieldValue 对象
	*	@param value 右值
	*	@return 返回 FieldValue 类型对象 
	*/
	FieldValue& operator =(const FieldValue& value) {
		value_type_ = value.value_type_;
		assign(value);
		return *this;
	}

	/** 赋值实现
	*
	*    将 value 对象的值和类型赋值给新对象
	*	@param value 需要赋值的对象
	*/
	void assign(const FieldValue& value) {
		if (value_type_ == INT) {
			value_.i = value.value_.i;
		}
		else if (value_type_ == UINT) {
			value_.ui = value.value_.ui;
		}
		else if (value_type_ == STRING || value_type_ == VARCHAR
			|| value_type_ == TEXT || value_type_ == BLOB
			|| value_type_ == CHAR) {

			value_.str = new std::string;
			if (value_.str) {
				value_.str->assign(value.value_.str->c_str());
			}
		}
		else if (value_type_ == DATETIME) {
			value_.datetime = value.value_.datetime;
		}
		else if (value_type_ == DATE) {
			value_.date = value.value_.date;
		} else if (value_type_ == LONG64) {
			value_.i64 = value.value_.i64;
		}
		else if (value_type_ == FLOAT) {
			value_.f = value.value_.f;
		}
		else if (value_type_ == DOUBLE) {
			value_.d = value.value_.d;
		}
		else if (value_type_ == DECIMAL) {
			value_.decimal = new InnerDecimal(value.value_.decimal->length(),
				value.value_.decimal->accuracy());
		}
	}

	/** FieldValue 赋值运算符
	*
	*    根据右值类型为 string 的值生成 FieldValue 对象
	*	@param value 右值
	*	@return 返回 FieldValue 类型对象
	*/
	FieldValue& operator =(const std::string& value) {
		value_type_ = STRING;
		value_.str = new std::string;
		if (value_.str) {
			value_.str->assign(value);
		}
		return *this;
	}

	/** FieldValue 赋值运算符
	*
	*    根据右值类型为 int 的值生成 FieldValue 对象
	*	@param value 右值
	*	@return 返回 FieldValue 类型对象
	*/
	FieldValue& operator =(const int value) {
		value_type_ = INT;
		value_.i = value;
		return *this;
	}

	/** FieldValue 赋值运算符
	*
	*    根据右值类型为 unsigned int 的值生成 FieldValue 对象
	*	@param value 右值
	*	@return 返回 FieldValue 类型对象
	*/
	FieldValue& operator =(const unsigned int value) {
		value_type_ = UINT;
		value_.ui = value;
		return *this;
	}

	/** FieldValue 赋值运算符
	*
	*    根据右值类型为 float 的值生成 FieldValue 对象
	*	@param value 右值
	*	@return 返回 FieldValue 类型对象
	*/
	FieldValue& operator =(const float value) {
		value_type_ = FLOAT;
		value_.f = value;
		return *this;
	}

	/** FieldValue 赋值运算符
	*
	*    根据右值类型为 double 的值生成 FieldValue 对象
	*	@param value 右值
	*	@return 返回 FieldValue 类型对象
	*/
	FieldValue& operator =(const double value) {
		value_type_ = DOUBLE;
		value_.d = value;
		return *this;
	}

	/** FieldValue 赋值运算符
	*
	*    根据右值类型为 InnerDateTime 的值生成 FieldValue 对象
	*	@param value 右值
	*	@return 返回 FieldValue 类型对象
	*/
	FieldValue& operator =(const InnerDateTime& value) {
		value_type_ = DATETIME;
		value_.datetime = nullptr;
		return *this;
	}

	/** FieldValue 赋值运算符
	*
	*    根据右值类型为 InnerDate 的值生成 FieldValue 对象
	*	@param value 右值
	*	@return 返回 FieldValue 类型对象
	*/
	FieldValue& operator =(const InnerDate& value) {
		value_type_ = DATE;
		value_.date = nullptr;
		return *this;
	}

	/** FieldValue 赋值运算符
	*
	*    根据右值类型为 InnerDecimal 的值生成 FieldValue 对象
	*	@param value 右值
	*	@return 返回 FieldValue 类型对象
	*/
	FieldValue& operator =(const InnerDecimal& value) {
		value_type_ = DECIMAL;
		value_.decimal = new InnerDecimal(value.length(), value.accuracy());
		return *this;
	}

	/** FieldValue 赋值运算符
	*
	*    根据右值类型为 int64_t 的值生成 FieldValue 对象
	*	@param value 右值
	*	@return 返回 FieldValue 类型对象
	*/
	FieldValue& operator =(const int64_t value) {
		value_type_ = LONG64;
		value_.i64 = value;
		return *this;
	}

	/** FieldValue 关系运算符
	*
	*    根据两个 FieldValue 对象的值和类型检查两个对象是否相等
	*	@param left 左值
	*	@param right 右值
	*	@return 如果两个类型相当则返回 TRUE, 否则返回 FALSE
	*/
	bool operator ==(const FieldValue& r) const {
		bool eq = false;
		assert(value_type_ == r.value_type_);
		if (value_type_ != r.value_type_)
			return eq;

		if (value_type_ == INT)
			eq = (value_.i == r.value_.i);
		else if (value_type_ == UINT)
			eq = (value_.ui == r.value_.ui);
		else if (value_type_ == FLOAT)
			eq = (value_.f == r.value_.f);
		else if (value_type_ == DOUBLE)
			eq = (value_.d == r.value_.d);
		else if (value_type_ == DECIMAL)
			eq = (*value_.decimal) == (*r.value_.decimal);
		else if (value_type_ == LONG64)
			eq = (value_.i64 == r.value_.i64);
		else if (value_type_ == STRING || value_type_ == VARCHAR
			|| value_type_ == TEXT || value_type_ == BLOB
			|| value_type_ == CHAR) {
			if (std::strcmp(value_.str->c_str(), r.value_.str->c_str()) == 0)
				eq = true;
		}
		else if (value_type_ == DATETIME || value_type_ == DATE) // TODO ??
			eq = false;

		return eq;
	}

	/** FieldValue 关系运算符
	*
	*    根据两个 FieldValue 对象的值和类型检查左值对象是否小于右值对象
	*	@param left 左值
	*	@param right 右值
	*	@return 如果左值小于右值则返回 TRUE, 否则返回 FALSE
	*/
	bool operator <(const FieldValue& r) const {
		bool eq = false;
		assert(value_type_ == r.value_type_);
		if (value_type_ != r.value_type_)
			return eq;

		if (value_type_ == INT)
			eq = (value_.i < r.value_.i);
		else if (value_type_ == UINT)
			eq = (value_.ui < r.value_.ui);
		else if (value_type_ == FLOAT)
			eq = (value_.f < r.value_.f);
		else if (value_type_ == DOUBLE)
			eq = (value_.d < r.value_.d);
		else if (value_type_ == LONG64)
			eq = (value_.i64 < r.value_.i64);
		else if (value_type_ == STRING || value_type_ == VARCHAR
			|| value_type_ == TEXT || value_type_ == BLOB
			|| value_type_ == CHAR) {
			if (std::strcmp(value_.str->c_str(), r.value_.str->c_str()) < 0)
				eq = true;
		}
		else if (value_type_ == DECIMAL) {
			assert(0);
		}
		else if (value_type_ == DATETIME || value_type_ == DATE) // TODO ??
			eq = false;

		return eq;
	}

	/** FieldValue 关系运算符
	*
	*    根据两个 FieldValue 对象的值和类型检查左值对象是否小于等于右值对象
	*	@param left 左值
	*	@param right 右值
	*	@return 如果左值小于等于TRUE, 否则返回 FALSE
	*/
	bool operator <=(const FieldValue& r) const {
		bool eq = false;
		assert(value_type_ == r.value_type_);
		if (value_type_ != r.value_type_)
			return eq;

		if (value_type_ == INT)
			eq = (value_.i <= r.value_.i);
		else if (value_type_ == UINT)
			eq = (value_.ui <= r.value_.ui);
		else if (value_type_ == FLOAT)
			eq = (value_.f <= r.value_.f);
		else if (value_type_ == DOUBLE)
			eq = (value_.d <= r.value_.d);
		else if (value_type_ == DECIMAL)
			assert(0);
		else if (value_type_ == LONG64)
			eq = (value_.i64 <= r.value_.i64);
		else if (value_type_ == STRING || value_type_ == VARCHAR
			|| value_type_ == TEXT || value_type_ == BLOB
			|| value_type_ == CHAR) {
			if (std::strcmp(value_.str->c_str(), r.value_.str->c_str()) < 0
				||std::strcmp(value_.str->c_str(), r.value_.str->c_str()) == 0)
				eq = true;
		}
		else if (value_type_ == DATETIME || value_type_ == DATE) // TODO ??
			eq = false;

		return eq;
	}

	/** FieldValue 关系运算符
	*
	*    根据两个 FieldValue 对象的值和类型检查左值对象是否大于右值对象
	*	@param left 左值
	*	@param right 右值
	*	@return 如果左值大于TRUE, 否则返回 FALSE
	*/
	bool operator >(const FieldValue& r) const {
		bool eq = false;
		assert(value_type_ == r.value_type_);
		if (value_type_ != r.value_type_)
			return eq;

		if (value_type_ == INT)
			eq = (value_.i > r.value_.i);
		else if (value_type_ == UINT)
			eq = (value_.ui > r.value_.ui);
		else if (value_type_ == FLOAT)
			eq = (value_.f > r.value_.f);
		else if (value_type_ == DOUBLE)
			eq = (value_.d > r.value_.d);
		else if (value_type_ == DECIMAL)
			assert(0);
		else if (value_type_ == LONG64)
			eq = (value_.i64 > r.value_.i64);
		else if (value_type_ == STRING || value_type_ == VARCHAR
			|| value_type_ == TEXT || value_type_ == BLOB
			|| value_type_ == CHAR) {
			if (std::strcmp(value_.str->c_str(), r.value_.str->c_str()) > 0)
				eq = true;
		}
		else if (value_type_ == DATETIME || value_type_ == DATE) // TODO ??
			eq = false;

		return eq;
	}

	/** FieldValue 关系运算符
	*
	*    根据两个 FieldValue 对象的值和类型检查左值对象是否大于等于右值对象
	*	@param left 左值
	*	@param right 右值
	*	@return 如果左值大于等于 TRUE, 否则返回 FALSE
	*/
	bool operator >=(const FieldValue& r) const {
		bool eq = false;
		assert(value_type_ == r.value_type_);
		if (value_type_ != r.value_type_)
			return eq;

		if (value_type_ == INT)
			eq = (value_.i >= r.value_.i);
		else if (value_type_ == UINT)
			eq = (value_.ui >= r.value_.ui);
		else if (value_type_ == FLOAT)
			eq = (value_.f >= r.value_.f);
		else if (value_type_ == DOUBLE)
			eq = (value_.d >= r.value_.d);
		else if (value_type_ == DECIMAL)
			assert(0);
		else if (value_type_ == LONG64)
			eq = (value_.i64 >= r.value_.i64);
		else if (value_type_ == STRING || value_type_ == VARCHAR
			|| value_type_ == TEXT || value_type_ == BLOB
			|| value_type_ == CHAR) {
			if (std::strcmp(value_.str->c_str(), r.value_.str->c_str()) > 0
				|| std::strcmp(value_.str->c_str(), r.value_.str->c_str()) == 0)
				eq = true;
		}
		else if (value_type_ == DATETIME || value_type_ == DATE) // TODO ??
			eq = false;

		return eq;
	}

	~FieldValue() {
		if ((value_type_ == STRING || value_type_ == VARCHAR
			|| value_type_ == TEXT || value_type_ == BLOB
			|| value_type_ == CHAR)
			&& value_.str) {
			delete value_.str;
			value_.str = nullptr;
		}
		else if (value_type_ == DECIMAL) {
			delete value_.decimal;
			value_.decimal = nullptr;
		}
	}

	bool isNumeric() {
		return (value_type_ == INT || value_type_ == UINT || value_type_ == LONG64
			|| value_type_ == FLOAT || value_type_ == DOUBLE || value_type_ == DECIMAL);
	}

	bool isNumeric() const {
		return (value_type_ == INT || value_type_ == UINT || value_type_ == LONG64
			|| value_type_ == FLOAT || value_type_ == DOUBLE || value_type_ == DECIMAL);
	}

	bool isInt() {
		return value_type_ == INT;
	}

	bool isInt() const {
		return value_type_ == INT;
	}

	bool isUint() {
		return value_type_ == UINT;
	}

	bool isUint() const {
		return value_type_ == UINT;
	}

	bool isFloat() {
		return value_type_ == FLOAT;
	}

	bool isFloat() const {
		return value_type_ == FLOAT;
	}

	bool isDouble() {
		return value_type_ == DOUBLE;
	}

	bool isDouble() const {
		return value_type_ == DOUBLE;
	}

	bool isDecimal() {
		return value_type_ == DECIMAL;
	}

	bool isDecimal() const {
		return value_type_ == DECIMAL;
	}

	bool isInt64() {
		return value_type_ == LONG64;
	}

	bool isInt64() const {
		return value_type_ == LONG64;
	}

	bool isString() {
		return value_type_ == STRING;
	}

	bool isString() const {
		return value_type_ == STRING;
	}

	bool isVarchar() {
		return value_type_ == VARCHAR;
	}

	bool isVarchar() const {
		return value_type_ == VARCHAR;
	}

	bool isChar() {
		return value_type_ == CHAR;
	}

	bool isChar() const {
		return value_type_ == CHAR;
	}

	bool isText() {
		return value_type_ == TEXT;
	}

	bool isText() const {
		return value_type_ == TEXT;
	}

	bool isBlob() {
		return value_type_ == BLOB;
	}

	bool isBlob() const {
		return value_type_ == BLOB;
	}

	bool isDateTime() {
		return value_type_ == DATETIME;
	}

	bool isDateTime() const {
		return value_type_ == DATETIME;
	}

	bool isDate() {
		return value_type_ == DATE;
	}

	bool isDate() const {
		return value_type_ == DATE;
	}

	const int& asInt() {
		return value_.i;
	}

	const int& asInt() const {
		return value_.i;
	}

	const unsigned int& asUint() {
		return value_.ui;
	}

	const unsigned int& asUint() const {
		return value_.ui;
	}

	const int64_t& asInt64() {
		return value_.i64;
	}

	const int64_t& asInt64() const {
		return value_.i64;
	}

	const float& asFloat() {
		return value_.f;
	}

	const float& asFloat() const {
		return value_.f;
	}

	const double& asDouble() {
		return value_.d;
	}

	const double& asDouble() const {
		return value_.d;
	}

	InnerDecimal& asDecimal() {
		return *value_.decimal;
	}

	const InnerDecimal& asDecimal() const {
		return *value_.decimal;
	}

	const std::string& asString() {
		return *value_.str;
	}

	const std::string& asString() const {
		return *value_.str;
	}


	void update(const InnerDecimal& d) {
		assert(value_.decimal);
		value_.decimal->update(d);
	}

private:

	/**

	* chainSQL 支持的内部类型

	* 忽略.

	*/
	enum inner_type {
		INNER_UNKOWN,	///< 未知类型
		INT,			///< integer 
		UINT,			///< unsigned integer
		FLOAT,			///< float
		DOUBLE,			///< double
		LONG64,			///< long64
		DECIMAL,		///< decimal
		DATETIME,		///< datetime
		DATE,			///< date
		TEXT,			///< text
		VARCHAR,		///< varchar
		CHAR,			///< char
		BLOB,			///< blob
		STRING			///< string
	};

	int value_type_;	///< @enum inner_type 
	/**

	* chainSQL 支持的 inner_type 对应的系统类型

	* 忽略.

	*/
	union inner_value {
		int i;				///< int
		unsigned int ui;	///< unsigned int
		int64_t i64;		///< int64_t
		float f;			///< float
		double d;			///< date
		InnerDateTime *datetime;	///< @var InnerDateTime
		InnerDate *date;			///< @var InnerDate
		InnerDecimal *decimal;		///< @var InnerDateTime
		std::string *str;	///< @var inner_type::TEXT/inner_type::VARCHAR/inner_type::TEXT/inner_type::BLOB/inner_type::DECIMAL
	} value_;
};

} // namespace ripple

#endif // RIPPLE_APP_MISC_SQLDATATYPE_H_INCLUDED
