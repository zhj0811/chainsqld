/**
* @file       TableUtils.h
* @brief      TableEntry获取；ChainSql基本交易类型判断；包含错误信息字段的json对象生成等。
* @date       20171209
* @version	  V1.0
* @par Copyright (c):
*      2016-2018 Peersafe Technology Co., Ltd.
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

#ifndef RIPPLE_RPC_TABLE_UTILS_H_INCLUDED
#define RIPPLE_RPC_TABLE_UTILS_H_INCLUDED


#include <ripple/json/json_value.h>
#include <peersafe/protocol/STEntry.h>

namespace ripple {
	/**
	*     用字符串（rpc/websocket）错误信息生成对应的json格式错误信息
	*
	*     @param errMsg 错误信息
	*     @param ws true：websocket错误信息； false：rpc错误信息
	*     @return    json格式的错误信息
	*/
	Json::Value generateError(const std::string& errMsg, bool ws = false);
	/**
	*     从TableEntries中找到对应表名的TableEntry
	*
	*     @param aTables TableEntries
	*     @param sCheckName 被获取的TableEntry的表名
	*     @return    TableEntry
	*/
	STEntry * getTableEntry(const STArray & aTables, std::string sCheckName);
	/**
	*     从ApplyView中找到交易拥有者账户或发送交易账户（onwer/）表数据中对应表名的TableEntry
	*
	*     @param view Writeable view to a ledger, for applying a transaction.
	*     @param tx 交易
	*     @return    TableEntry
	*/
	STEntry *getTableEntry(ApplyView& view, const STTx& tx);
	/**
	*     从TableEntries中找到对应表名的TableEntry
	*
	*     @param aTables TableEntries
	*     @param vCheckName 被获取的TableEntry的表名
	*     @return    TableEntry
	*/
	STEntry *getTableEntry(const STArray & aTables, Blob& vCheckName);
	/**
	*     判断交易是否属于Chainsql基本交易类型
	*
	*     @param transactionType 交易类型
	*     @return   true：Chainsql基本交易类型；false：不是Chainsql基本交易类型
	*/
	bool isChainSqlBaseType(const std::string& transactionType);
}

#endif
