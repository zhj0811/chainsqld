/**

* @file       SQLConditionTree.h

* @brief      条件语法树，用于生成 SQL 语句的条件部分.

* @details	此类会根据 Raw 协议将条件生成一个二叉树，然后用中序遍历算法将整个表达式输出

* @author     dbliu

* @date       2017/12/09

* @version v1.0

* @par Copyright (c):

*      Copyright (c) 2016-2018 Peersafe Technology Co., Ltd

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

#ifndef RIPPLE_APP_MISC_SQLCONDITIONTREE_H_INCLUDED
#define RIPPLE_APP_MISC_SQLCONDITIONTREE_H_INCLUDED

#include <vector>
#include <functional>
#include <tuple>

#include <peersafe/app/sql/SQLDataType.h>

#include <ripple/json/json_value.h>
#include<ripple/core/SociDB.h> // soci::details::once_temp_type

namespace ripple {

typedef FieldValue BindValue;

/**

* 条件语法树

* chainSQL 对标进行查询和更改的时候，应用层会根据 Raw 协议定义各种查询条件，这个类会将条件创建一颗二叉树，然后利用中序遍历将表达式输出

*/
class conditionTree {
public:
	typedef std::vector<conditionTree>::iterator iterator;	///< 树节点指针
	typedef std::vector<conditionTree>::const_iterator const_iterator; ///< 树节点指针
	typedef std::tuple<std::string /*key*/, std::string /*operator*/, std::vector<BindValue> /*key's value*/> expression_result; ///< 表达式

	/**
	* 语法树节点类型

	* 忽略.
	*/
	typedef enum {
		Logical_And,	///< 逻辑与表达式, eg: value = 1 and id = 1
		Logical_Or,		///< 逻辑或表达式, eg: value = 1 or id = 1
		Expression		///< 普通表达式, eg: value = 1
	} NodeType;

	conditionTree(NodeType);
	conditionTree(const conditionTree& t);
	~conditionTree();

	conditionTree& operator = (const conditionTree& rhs) {
		type_ = rhs.type_;
		bind_values_index_ = rhs.bind_values_index_;
		expression_ = rhs.expression_;
		children_ = rhs.children_;
		bind_values_ = rhs.bind_values_;
		return *this;
	}

	/** 创建语法树根节点
	*
	*    静态函数，根据 Raw 协议创建语法树根节点
	*	@param conditions Raw 语法协议 
	*	@return 返回 std::pair 类型，std::pair 类型的第一个元素值为 0 表示生成根节点成功，否则生成根节点失败；std::pair 类型的第二个元素表示根节点对象
	*/
	static std::pair<int, conditionTree> createRoot(const Json::Value& conditions);

	/** 获取语法树节点类型
	*
	*    略
	*	@param void
	*	@return 返回树节点类型
	*/
	const NodeType node_type() const {
		return type_;
	}

	/** 设置语法树节点表达式
	*
	*    略
	*	@param express 表达式值， json 格式，如 {k = v}
	*	@return void
	*/
	void set_expression(const Json::Value& express) {
		expression_ = express;
	}

	expression_result parse_expression() {
		return parse_expression(expression_);
	}

	void set_bind_values_index(int fromindex) {
		bind_values_index_ = fromindex;
	}

	const Json::Value& expression() const {
		return expression_;
	}

	/** 添加语法树节点
	*
	*    略
	*	@param child 树节点
	*	@return void
	*/
	void add_child(const conditionTree& child) {
		children_.push_back(child);
	}

	iterator begin() {
		return children_.begin();
	}

	const_iterator begin() const {
		return children_.begin();
	}

	iterator end() {
		return children_.end();
	}

	const_iterator end() const {
		return children_.end();
	}

	size_t size() const {
		return children_.size();
	}

	/** 将语法树以字符串形式输出
	*
	*    将语法树以字符串形式输出, eg: `id > 10 and name = 'chainsql'`
	*	@param void
	*	@return string 
	*/
	const std::string asString() const;

	/** 表达语法树
	*
	*    将语法树表达成 std::pair<int,std::string> 对象
	*	@param void
	*	@return std::pair<int, std::string> std::pair 的第一个元素为 0 表示语法树表达成功，否则失败；std::pair 第二个元素在成功的时候字符串格式如：`id > :id and name = :name`
	*/
	const std::pair<int, std::string> asConditionString() const;
	// bind once_temp_type with values,return {0, "success"} if success,otherwise return {-1, "bind value unsuccessfully"}
	const std::pair<int, std::string> bind_value(soci::details::once_temp_type& t);
private:
	int format_conditions(int style, std::string& conditions) const;
	int format_value(const BindValue& value, std::string& result) const;
	// bind values
	int bind_value(const BindValue& value, soci::details::once_temp_type& t);
	int bind_array(const std::vector<BindValue>& values, soci::details::once_temp_type& t);
	// parse values
	int parse_array(const Json::Value& j, std::vector<BindValue>& v);
	int parse_value(const Json::Value& j, BindValue& v);
	expression_result parse_expression(const Json::Value& e);

	NodeType type_;
	mutable int	bind_values_index_; // binding same fields may be failure in sqlite
	Json::Value expression_;
	std::vector<conditionTree> children_;
	mutable std::vector<std::vector<BindValue>> bind_values_;
};

/**

* 解析语法树的 helper 函数

* 略

*/
namespace conditionParse {

	/** 解析 Raw 协议并且生成语法树
	*
	*    略
	*	@param raw_value Raw 协议中的语法条件
	*	@param root 语法树根节点，用 conditionTree::createRoot 函数生成
	*	@return std::pair的第一个元素为 0 表示语法树生成成功，否则失败；std::pair 第二个元素表示失败的信息
	*/
	std::pair<int, std::string> parse_conditions(const Json::Value& raw_value, conditionTree& root);

	typedef std::function<bool(const conditionTree::expression_result&)> handlevaluecb;							///< 处理语法树的叶子节点
	typedef std::function<int(const conditionTree&, int /*0-starting,1-processing,2-end*/)> handlenodecb;		///< 处理语法树的非叶子节点
	bool judge(conditionTree& root, handlevaluecb cb);

	/** 遍历语法树
	*
	*    略
	*	@param root 语法树根节点
	*	@param cb 处理语法树叶子节点的回调函数
	*	@param cb 处理语法树非叶子节点的回调函数
	*	@return void
	*/
	void traverse(conditionTree& root, handlevaluecb cb, handlenodecb ncb);
} // namespace helper

} // namespace ripple

#endif // RIPPLE_APP_MISC_SQLCONDITIONTREE_H_INCLUDED
