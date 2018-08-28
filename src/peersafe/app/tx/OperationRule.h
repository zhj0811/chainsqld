/**

* @file       OperationRule.h

* @brief      表行级控制规则

* @details	表行级控制规则

* @author     peersafe

* @date       2017/12/09

* @version v1.0

* @par Copyright (c):

*      Copyright (c) 2016-2018 Peersafe Technology Co., Ltd.

* @par History:

*   v1.0: dbliu, 2017/12/09, originator\n

*/
#ifndef RIPPLE_TX_OPERATION_RULE_H_INCLUDED
#define RIPPLE_TX_OPERATION_RULE_H_INCLUDED

#include <ripple/app/tx/impl/Transactor.h>
#include <ripple/basics/Log.h>
#include <ripple/protocol/TxFlags.h>
#include <peersafe/app/tx/ChainSqlTx.h>

namespace ripple {

/**

* 表行级控制规则

* 表行级控制规则

*/
class OperationRule {
public:
	//for TableListSet
	static TER dealWithTableListSetRule(ApplyContext& ctx, const STTx& tx);
	//check field in update/delete/get is right
	static bool checkRuleFields(std::vector<std::string>& vecFields,Json::Value condition);

	//for SqlStatement
	static TER dealWithSqlStatementRule(ApplyContext& ctx, const STTx& tx);

	static std::string getOperationRule(ApplyView& view, const STTx& tx);
	static bool hasOperationRule(ApplyView& view, const STTx& tx);

	static TER adjustInsertCount(ApplyContext& ctx, const STTx& tx, DatabaseCon* pConn);
};

} // ripple
#endif