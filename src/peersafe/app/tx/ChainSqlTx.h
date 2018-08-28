/**

* @file       ChainSqlTx.h

* @brief      chainSQL 交易基础类.

* @details	chainSQL 交易基础类

* @author     peersafe

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

#ifndef RIPPLE_APP_TX_CHAINSQLTX_H_INCLUDED
#define RIPPLE_APP_TX_CHAINSQLTX_H_INCLUDED

#include <ripple/app/tx/impl/Transactor.h>

namespace ripple {
	/**

	* chainSQL 交易基础类

	* chainSQL 交易基础类

	*/
    class ChainSqlTx : public Transactor
    {   
    protected:        
        explicit ChainSqlTx(ApplyContext& ctx);

        static TER preflight(PreflightContext const& ctx);
        static TER preclaim(PreclaimContext const& ctx);

		/** 处理交易
		*     处理交易
		*     @param void
		*     @return    TER
		*/
		TER doApply();	

		/** 判断交易是否能处理
		*     判断交易是否能处理
		*     @param ctxa chainSQL 交易对象上下文信息 
		*     @return    true 交易能被处理， false 交易无法处理
		*/
		static bool canDispose(ApplyContext& ctx);

		/** 获取数据库相关信息
		*     获取数据库相关信息
		*     @param ctx chainSQL 交易对象上下文信息
		*     @return    std::pair 第一个元素为数据连接信息，第二个元素为交易对象
		*/
		static std::pair<TxStoreDBConn*, TxStore*> getTransactionDBEnv(ApplyContext& ctx);
	public:
		virtual std::pair<TER, std::string> dispose(TxStore& txStore, const STTx& tx);
    };
}

#endif
