/**
* @file       STEntry.h
* @brief      TableList中TableEntries数据中元素
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

#ifndef RIPPLE_PROTOCOL_STENTRY_H_INCLUDED
#define RIPPLE_PROTOCOL_STENTRY_H_INCLUDED

#include <ripple/protocol/PublicKey.h>
#include <ripple/protocol/SecretKey.h>
#include <ripple/protocol/STObject.h>
#include <ripple/app/tx/impl/ApplyContext.h>
#include <peersafe/protocol/TableDefines.h>
#include <cstdint>
#include <memory>

namespace ripple { 
    class STEntry final
        : public STObject
        , public CountedObject <STEntry>
    {
    public:
        static char const* getCountedObjectName() { return "STEntry"; }

        using pointer = std::shared_ptr<STEntry>;
        using ref = const std::shared_ptr<STEntry>&;

        enum
        {
            kFullFlag = 0x1
        };


        STEntry();  
		/**
		* 初始化TableList中TableEntries数据中元素各字段的值
		*
		* @param tableName 表名
		* @param nameInDB 对应底层数据库中实际的表名（LedgerSequence+OwnerAccountID+表名）
		* @param deleted 标示表是否被删除
		* @param createLgrSeq  本交易的ledger序列号-1
		* @param createdLedgerHash  本次交易的ledger HASH
		* @param createdTxnHash TransactionID
		* @param txnLedgerSequence  本交易的ledger序列号
		* @param txnLedgerhash  本次交易的ledger HASH
		* @param prevTxnLedgerSequence 上次交易的ledger序列号
		* @param prevTxnLedgerhash 上次交易的ledger HASH
		* @param txCheckhash 校验hash
		* @param users 授权用户列表
		*/
		void init(ripple::Blob tableName, uint160 nameInDB, uint8 deleted, uint32 createLgrSeq, uint256 createdLedgerHash, uint256 createdTxnHash, uint32 txnLedgerSequence, uint256 txnLedgerhash, uint32 prevTxnLedgerSequence, uint256 prevTxnLedgerhash, uint256 txCheckhash, STArray users);

		/**
		* 初始化行级控制规则
		*
		* @param operationRule 行级控制规则字符串
		*/
		void initOperationRule(ripple::Blob operationRule);

		/**
		* 获取操作类型对应的行级控制规则
		*
		* @param opType 操作类型
		*/
		std::string getOperationRule(TableOpType opType) const;

		/**
		* 判断账户是否有某种操作权限
		*
		* @param account 账户地址
		* @param flag 操作类型
		*/
		bool hasAuthority(const AccountID& account, TableRoleFlags flag);
		/// 判断是否是加密模式
		bool isConfidential();

        STBase*
            copy(std::size_t n, void* buf) const override
        {
            return emplace(n, buf, *this);
        }

        STBase*
            move(std::size_t n, void* buf) override
        {
            return emplace(n, buf, std::move(*this));
        }
    private:
        static SOTemplate const& getFormat();
    };

} // ripple

#endif
