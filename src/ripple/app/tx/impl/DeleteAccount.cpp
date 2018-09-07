//------------------------------------------------------------------------------
/*
This file is part of rippled: https://github.com/ripple/rippled
Copyright (c) 2012, 2013 Ripple Labs Inc.

Permission to use, copy, modify, and/or distribute this software for any
purpose  with  or without fee is hereby granted, provided that the above
copyright notice and this permission notice appear in all copies.

THE  SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
WITH  REGARD  TO  THIS  SOFTWARE  INCLUDING  ALL  IMPLIED  WARRANTIES  OF
MERCHANTABILITY  AND  FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
ANY  SPECIAL ,  DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
WHATSOEVER  RESULTING  FROM  LOSS  OF USE, DATA OR PROFITS, WHETHER IN AN
ACTION  OF  CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/
//==============================================================================

#include <BeastConfig.h>
#include <ripple/app/tx/impl/DeleteAccount.h>
#include <ripple/basics/Log.h>
#include <ripple/protocol/Feature.h>
#include <ripple/protocol/Quality.h>
#include <ripple/protocol/Indexes.h>
#include <ripple/protocol/st.h>
#include <ripple/ledger/View.h>

namespace ripple {

	TER
		DeleteAccount::preflight(PreflightContext const& ctx)
	{
		auto const ret = preflight1(ctx);
		if (!isTesSuccess(ret))
			return ret;

		auto& tx = ctx.tx;
		auto& j = ctx.j;

		return preflight2(ctx);
	}

	TER
		DeleteAccount::preclaim(PreclaimContext const& ctx)
	{
		auto const id = ctx.tx[sfAccount];
		if (to_string(id) != "zHb9CJAWyB4zj91VRWn96DkukG4bwdtyTh")
			return tefBAD_AUTH_MASTER;

		AccountID const uDstAccountID(ctx.tx[sfDestination]);

		auto const k = keylet::account(uDstAccountID);
		auto const sleDst = ctx.view.read(k);
		if (sleDst == nullptr)
			return tecNO_DST;

		return tesSUCCESS;
	}

	TER
		DeleteAccount::doApply()
	{
		TER terResult = tesSUCCESS;
		// Open a ledger for editing.
		AccountID const uDstAccountID(ctx_.tx.getAccountID(sfDestination));
		auto const k = keylet::account(uDstAccountID);
		SLE::pointer sleDst = view().peek(k);

		if (sleDst)
		{
			//AccountID const uAccountID(ctx_.tx.getAccountID(sfAccount));
			//SLE::pointer sleRoot = view().peek(keylet::account(uAccountID));
			//sleRoot->setFieldAmount(sfBalance,
			//	sleRoot->getFieldAmount(sfBalance) + sleDst->getFieldAmount(sfBalance));

			// Delete the account.
			view().erase(sleDst);
		}

		return terResult;
	}

}
