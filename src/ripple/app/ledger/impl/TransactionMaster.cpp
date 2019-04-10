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
#include <ripple/app/ledger/TransactionMaster.h>
#include <ripple/app/misc/Transaction.h>
#include <ripple/app/main/Application.h>
#include <ripple/protocol/STTx.h>
#include <ripple/basics/Log.h>
#include <ripple/basics/chrono.h>
#include <ripple/app/ledger/OpenLedger.h>		
#include <ripple/app/misc/TxQ.h>	

namespace ripple {

TransactionMaster::TransactionMaster (Application& app)
    : mApp (app)
    , mCache ("TransactionCache", 65536, 1800, stopwatch(),
        mApp.journal("TaggedCache"))
	, mCacheSeq("AccountSeqCache", 10240, 10, stopwatch(), mApp.journal("TaggedCache"))
    , m_pClientTxStoreDBConn(std::make_unique<TxStoreDBConn>(app.config()))
    , m_pClientTxStoreDB(std::make_unique<TxStore>(m_pClientTxStoreDBConn->GetDBConn(), app.config(), app.logs().journal("TxStore")))
    , m_pConsensusTxStoreDBConn(std::make_unique<TxStoreDBConn>(app.config()))
    , m_pConsensusTxStoreDB(std::make_unique<TxStore>(m_pConsensusTxStoreDBConn->GetDBConn(), app.config(), app.logs().journal("TxStore")))
{
}

bool TransactionMaster::inLedger (uint256 const& hash, std::uint32_t ledger)
{
    auto txn = mCache.fetch (hash);

    if (!txn)
        return false;

    txn->setStatus (COMMITTED, ledger);
    return true;
}

int TransactionMaster::getTxCount(bool chainsql)
{
	std::string sql = "SELECT COUNT(*) FROM Transactions WHERE Status='V'";
	if (chainsql)
	{
		sql += " AND (TransType = 'SQLTransaction' or TransType = 'TableListSet' or TransType = 'SQLStatement')";
	}
	sql += ";";

	boost::optional<int> txCount;
	{
		auto db = mApp.getTxnDB().checkoutDb();
		*db << sql,
			soci::into(txCount);

		if (!db->got_data() || !txCount)
			return 0;
	}

	return *txCount;
}

int TransactionMaster::getAccountSequence(AccountID const& accountId)
{
	std::lock_guard<std::mutex> lock(mMutexSeq);
	auto seq = mCacheSeq.fetch(accountId);
	if (seq)
	{
		mCacheSeq.refreshIfPresent(accountId);
		return ++(*seq);
	}
	else
	{
		auto const& ledger = mApp.openLedger().current();
		auto sle = ledger->read(keylet::account(accountId));
		if (!sle)
		{
			// did not find account, error.
			auto j = mApp.journal("TransactionMaster");
			JLOG(j.debug())
				<< "transactionSign: getAccountSequence Failed to find account "
				<< "in current ledger: "
				<< toBase58(accountId);

			return 0;
		}

		auto seq = (*sle)[sfSequence];
		auto const queued = mApp.getTxQ().getAccountTxs(accountId, *ledger);
		// If the account has any txs in the TxQ, skip those sequence
		// numbers (accounting for possible gaps).
		for (auto const& tx : queued)
		{
			if (tx.first == seq)
				++seq;
			else if (tx.first > seq)
				break;
		}

		auto pInfo = std::make_shared<int>(seq);
		mCacheSeq.canonicalize(accountId, pInfo);
	}
	return 0;
}

std::shared_ptr<Transaction>
TransactionMaster::fetch (uint256 const& txnID, bool checkDisk)
{
    auto txn = mCache.fetch (txnID);

    if (!checkDisk || txn)
        return txn;

    txn = Transaction::load (txnID, mApp);

    if (!txn)
        return txn;

    mCache.canonicalize (txnID, txn);

    return txn;
}

std::shared_ptr<STTx const>
TransactionMaster::fetch (std::shared_ptr<SHAMapItem> const& item,
    SHAMapTreeNode::TNType type,
        bool checkDisk, std::uint32_t uCommitLedger)
{
    std::shared_ptr<STTx const>  txn;
    auto iTx = fetch (item->key(), false);

    if (!iTx)
    {

        if (type == SHAMapTreeNode::tnTRANSACTION_NM)
        {
            SerialIter sit (item->slice());
            txn = std::make_shared<STTx const> (std::ref (sit));
        }
        else if (type == SHAMapTreeNode::tnTRANSACTION_MD)
        {
            auto blob = SerialIter{item->data(), item->size()}.getVL();
            txn = std::make_shared<STTx const>(SerialIter{blob.data(), blob.size()});
        }
    }
    else
    {
        if (uCommitLedger)
            iTx->setStatus (COMMITTED, uCommitLedger);

        txn = iTx->getSTransaction ();
    }

    return txn;
}

TxStoreDBConn& TransactionMaster::getClientTxStoreDBConn()
{
    return *m_pClientTxStoreDBConn;
}

TxStore& TransactionMaster::getClientTxStore()
{
    return *m_pClientTxStoreDB;
}

TxStoreDBConn& TransactionMaster::getConsensusTxStoreDBConn()
{
    return *m_pConsensusTxStoreDBConn;
}

TxStore& TransactionMaster::getConsensusTxStore()
{
    return *m_pConsensusTxStoreDB;
}

void
TransactionMaster::canonicalize(std::shared_ptr<Transaction>* pTransaction)
{
    uint256 const tid = (*pTransaction)->getID();
    if (tid != zero)
    {
        auto txn = *pTransaction;
        // VFALCO NOTE canonicalize can change the value of txn!
        mCache.canonicalize(tid, txn);
        *pTransaction = txn;
    }
}

void TransactionMaster::sweep (void)
{
    mCache.sweep ();
	mCacheSeq.sweep();
}

TaggedCache <uint256, Transaction>& TransactionMaster::getCache()
{
    return mCache;
}

} // ripple
