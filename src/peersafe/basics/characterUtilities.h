/**

* @file       characterUtilities.h

* @brief      字符处理类工具.

* @details	字符处理类工具

* @author     peersafe

* @date       2017/12/09

* @version v1.0

* @par Copyright (c):

*      Copyright (c) 2016-2018 Peersafe Technology Co., Ltd.

* @par History:

*   v1.0: dbliu, 2017/12/09, originator\n

*//**

* @file       characterUtilities.h

* @brief      字符处理类工具.

* @details	字符处理类工具

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

#ifndef RIPPLE_BASICS_CHARACTERUTILITIES_H_INCLUDED
#define RIPPLE_BASICS_CHARACTERUTILITIES_H_INCLUDED

#include <string>
namespace ripple {
	/** 字符集转换函数
	*     GBK、UFT8字符集转互换
	*     @param sSrc 
	*     @param sDest 
	*     @param bFromGBK2UTF8 
	*     @return    
	*/
    bool  TransGBK_UTF8(const std::string &sSrc, std::string &sDest, bool bFromGBK2UTF8);    
}
#endif
