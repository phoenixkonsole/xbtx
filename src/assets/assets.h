// Copyright (c) 2017 The BitcoinSubsidium Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.


#ifndef BitcoinSubsidiumCOIN_ASSET_PROTOCOL_H
#define BitcoinSubsidiumCOIN_ASSET_PROTOCOL_H

#include "amount.h"
#include "tinyformat.h"
#include "assettypes.h"

#include <string>
#include <set>
#include <map>
#include <unordered_map>
#include <list>

#define XBTX_R 114
#define XBTX_V 118
#define XBTX_N 110
#define XBTX_Q 113
#define XBTX_T 116
#define XBTX_O 111

#define DEFAULT_UNITS 0
#define DEFAULT_REISSUABLE 1
#define DEFAULT_HAS_IPFS 0
#define DEFAULT_IPFS ""
#define MIN_ASSET_LENGTH 3
#define MAX_ASSET_LENGTH 32
#define OWNER_TAG "!"
#define OWNER_LENGTH 1
#define OWNER_UNITS 0
#define OWNER_ASSET_AMOUNT 1 * COIN
#define UNIQUE_ASSET_AMOUNT 1 * COIN
#define UNIQUE_ASSET_UNITS 0
#define UNIQUE_ASSETS_REISSUABLE 0

#define RESTRICTED_CHAR '$'
#define QUALIFIER_CHAR '#'

#define QUALIFIER_ASSET_MIN_AMOUNT 1 * COIN
#define QUALIFIER_ASSET_MAX_AMOUNT 10 * COIN
#define QUALIFIER_ASSET_UNITS 0

#define ASSET_TRANSFER_STRING "transfer_asset"
#define ASSET_NEW_STRING "new_asset"
#define ASSET_REISSUE_STRING "reissue_asset"

class CScript;
class CDataStream;
class CTransaction;
class CTxOut;
class Coin;
class CWallet;
class CReserveKey;
class CWalletTx;
struct CAssetOutputEntry;
class CCoinControl;
struct CBlockAssetUndo;
class COutput;

// 2500 * 82 Bytes == 205 KB (kilobytes) of memory
#define MAX_CACHE_ASSETS_SIZE 2500

// Create map that store that state of current reissued transaction that the mempool as accepted.
// If an asset name is in this map, any other reissue transactions wont be accepted into the mempool
extern std::map<uint256, std::string> mapReissuedTx;
extern std::map<std::string, uint256> mapReissuedAssets;

class CAssets {
public:
    std::map<std::pair<std::string, std::string>, CAmount> mapAssetsAddressAmount; // pair < Asset Name , Address > -> Quantity of tokens in the address

    // Dirty, Gets wiped once flushed to database
    std::map<std::string, CNewAsset> mapReissuedAssetData; // Asset Name -> New Asset Data

    CAssets(const CAssets& assets) {
        this->mapAssetsAddressAmount = assets.mapAssetsAddressAmount;
        this->mapReissuedAssetData = assets.mapReissuedAssetData;
    }

    CAssets& operator=(const CAssets& other) {
        mapAssetsAddressAmount = other.mapAssetsAddressAmount;
        mapReissuedAssetData = other.mapReissuedAssetData;
        return *this;
    }

    CAssets() {
        SetNull();
    }

    void SetNull() {
        mapAssetsAddressAmount.clear();
        mapReissuedAssetData.clear();
    }
};

struct ErrorReport {

    enum ErrorType {
        NotSetError = 0,
        InvalidQualifierName = 1,
        EmptyString = 2,
        LengthToLarge = 3,
        InvalidSubExpressionFormula = 4,
        InvalidSyntax = 5,
        AssetDoesntExist = 6,
        FailedToVerifyAgainstAddress = 7,
        EmptySubExpression = 8,
        UnknownOperator = 9,
        ParenthesisParity = 10,
        VariableNotFound = 11
    };

    ErrorType type = ErrorType::NotSetError;
    std::string strDevData;
    std::vector<std::string> vecUserData;
};

std::string GetUserErrorString(const ErrorReport& report);

class CAssetsCache : public CAssets
{
private:
    bool AddBackSpentAsset(const Coin& coin, const std::string& assetName, const std::string& address, const CAmount& nAmount, const COutPoint& out);
    void AddToAssetBalance(const std::string& strName, const std::string& address, const CAmount& nAmount);
    bool UndoTransfer(const CAssetTransfer& transfer, const std::string& address, const COutPoint& outToRemove);
public :
    //! These are memory only containers that show dirty entries that will be databased when flushed
    std::vector<CAssetCacheUndoAssetAmount> vUndoAssetAmount;
    std::vector<CAssetCacheSpendAsset> vSpentAssets;

    // New Assets Caches
    std::set<CAssetCacheNewAsset> setNewAssetsToRemove;
    std::set<CAssetCacheNewAsset> setNewAssetsToAdd;

    // New Reissue Caches
    std::set<CAssetCacheReissueAsset> setNewReissueToRemove;
    std::set<CAssetCacheReissueAsset> setNewReissueToAdd;

    // Ownership Assets Caches
    std::set<CAssetCacheNewOwner> setNewOwnerAssetsToAdd;
    std::set<CAssetCacheNewOwner> setNewOwnerAssetsToRemove;

    // Transfer Assets Caches
    std::set<CAssetCacheNewTransfer> setNewTransferAssetsToAdd;
    std::set<CAssetCacheNewTransfer> setNewTransferAssetsToRemove;

    //! Qualfier Address Asset Caches
    std::set<CAssetCacheQualifierAddress> setNewQualifierAddressToAdd;
    std::set<CAssetCacheQualifierAddress> setNewQualifierAddressToRemove;

    //! Restricted Address Asset Caches
    std::set<CAssetCacheRestrictedAddress> setNewRestrictedAddressToAdd;
    std::set<CAssetCacheRestrictedAddress> setNewRestrictedAddressToRemove;

    //! Restricted Global Asset Caches
    std::set<CAssetCacheRestrictedGlobal> setNewRestrictedGlobalToAdd;
    std::set<CAssetCacheRestrictedGlobal> setNewRestrictedGlobalToRemove;

    //! Restricted Assets Verifier Caches
    std::set<CAssetCacheRestrictedVerifiers> setNewRestrictedVerifierToAdd;
    std::set<CAssetCacheRestrictedVerifiers> setNewRestrictedVerifierToRemove;

    //! Root Qualifier Address Map
    std::map<CAssetCacheRootQualifierChecker, std::set<std::string> > mapRootQualifierAddressesAdd;
    std::map<CAssetCacheRootQualifierChecker, std::set<std::string> > mapRootQualifierAddressesRemove;

    CAssetsCache() : CAssets()
    {
        SetNull();
        ClearDirtyCache();
    }

    CAssetsCache(const CAssetsCache& cache) : CAssets(cache)
    {
        // Copy dirty cache also
        this->vSpentAssets = cache.vSpentAssets;
        this->vUndoAssetAmount = cache.vUndoAssetAmount;

        // Transfer Caches
        this->setNewTransferAssetsToAdd = cache.setNewTransferAssetsToAdd;
        this->setNewTransferAssetsToRemove = cache.setNewTransferAssetsToRemove;

        // Issue Caches
        this->setNewAssetsToRemove = cache.setNewAssetsToRemove;
        this->setNewAssetsToAdd = cache.setNewAssetsToAdd;

        // Reissue Caches
        this->setNewReissueToRemove = cache.setNewReissueToRemove;
        this->setNewReissueToAdd = cache.setNewReissueToAdd;

        // Owner Caches
        this->setNewOwnerAssetsToAdd = cache.setNewOwnerAssetsToAdd;
        this->setNewOwnerAssetsToRemove = cache.setNewOwnerAssetsToRemove;

        //! Qualifier Caches
        this->setNewQualifierAddressToAdd = cache.setNewQualifierAddressToAdd;
        this->setNewQualifierAddressToRemove = cache.setNewQualifierAddressToRemove;

        //! Restricted Address Caches
        this->setNewRestrictedAddressToAdd = cache.setNewRestrictedAddressToAdd;
        this->setNewRestrictedAddressToRemove = cache.setNewRestrictedAddressToRemove;

        //! Restricted Global Caches
        this->setNewRestrictedGlobalToAdd = cache.setNewRestrictedGlobalToAdd;
        this->setNewRestrictedGlobalToRemove = cache.setNewRestrictedGlobalToRemove;

        //! Restricted Verifier Caches
        this->setNewRestrictedVerifierToAdd = cache.setNewRestrictedVerifierToAdd;
        this->setNewRestrictedVerifierToRemove = cache.setNewRestrictedVerifierToRemove;

        //! Root Qualifier Address Map
        this->mapRootQualifierAddressesAdd = cache.mapRootQualifierAddressesAdd;
        this->mapRootQualifierAddressesRemove = cache.mapRootQualifierAddressesRemove;
    }

    CAssetsCache& operator=(const CAssetsCache& cache)
    {
        this->mapAssetsAddressAmount = cache.mapAssetsAddressAmount;
        this->mapReissuedAssetData = cache.mapReissuedAssetData;

        // Copy dirty cache also
        this->vSpentAssets = cache.vSpentAssets;
        this->vUndoAssetAmount = cache.vUndoAssetAmount;

        // Transfer Caches
        this->setNewTransferAssetsToAdd = cache.setNewTransferAssetsToAdd;
        this->setNewTransferAssetsToRemove = cache.setNewTransferAssetsToRemove;

        // Issue Caches
        this->setNewAssetsToRemove = cache.setNewAssetsToRemove;
        this->setNewAssetsToAdd = cache.setNewAssetsToAdd;

        // Reissue Caches
        this->setNewReissueToRemove = cache.setNewReissueToRemove;
        this->setNewReissueToAdd = cache.setNewReissueToAdd;

        // Owner Caches
        this->setNewOwnerAssetsToAdd = cache.setNewOwnerAssetsToAdd;
        this->setNewOwnerAssetsToRemove = cache.setNewOwnerAssetsToRemove;

        //! Qualifier Caches
        this->setNewQualifierAddressToAdd = cache.setNewQualifierAddressToAdd;
        this->setNewQualifierAddressToRemove = cache.setNewQualifierAddressToRemove;

        //! Restricted Address Caches
        this->setNewRestrictedAddressToAdd = cache.setNewRestrictedAddressToAdd;
        this->setNewRestrictedAddressToRemove = cache.setNewRestrictedAddressToRemove;

        //! Restricted Global Caches
        this->setNewRestrictedGlobalToAdd = cache.setNewRestrictedGlobalToAdd;
        this->setNewRestrictedGlobalToRemove = cache.setNewRestrictedGlobalToRemove;

        //! Restricted Verifier Caches
        this->setNewRestrictedVerifierToAdd = cache.setNewRestrictedVerifierToAdd;
        this->setNewRestrictedVerifierToRemove = cache.setNewRestrictedVerifierToRemove;

        //! Root Qualifier Address Map
        this->mapRootQualifierAddressesAdd = cache.mapRootQualifierAddressesAdd;
        this->mapRootQualifierAddressesRemove = cache.mapRootQualifierAddressesRemove;

        return *this;
    }

    // Cache only undo functions
    bool RemoveNewAsset(const CNewAsset& asset, const std::string address);
    bool RemoveTransfer(const CAssetTransfer& transfer, const std::string& address, const COutPoint& out);
    bool RemoveOwnerAsset(const std::string& assetsName, const std::string address);
    bool RemoveReissueAsset(const CReissueAsset& reissue, const std::string address, const COutPoint& out, const std::vector<std::pair<std::string, CBlockAssetUndo> >& vUndoIPFS);
    bool UndoAssetCoin(const Coin& coin, const COutPoint& out);
    bool RemoveQualifierAddress(const std::string& assetName, const std::string& address, const QualifierType type);
    bool RemoveRestrictedAddress(const std::string& assetName, const std::string& address, const RestrictedType type);
    bool RemoveGlobalRestricted(const std::string& assetName, const RestrictedType type);
    bool RemoveRestrictedVerifier(const std::string& assetName, const std::string& verifier, const bool fUndoingReissue = false);

    // Cache only add asset functions
    bool AddNewAsset(const CNewAsset& asset, const std::string address, const int& nHeight, const uint256& blockHash);
    bool AddTransferAsset(const CAssetTransfer& transferAsset, const std::string& address, const COutPoint& out, const CTxOut& txOut);
    bool AddOwnerAsset(const std::string& assetsName, const std::string address);
    bool AddReissueAsset(const CReissueAsset& reissue, const std::string address, const COutPoint& out);
    bool AddQualifierAddress(const std::string& assetName, const std::string& address, const QualifierType type);
    bool AddRestrictedAddress(const std::string& assetName, const std::string& address, const RestrictedType type);
    bool AddGlobalRestricted(const std::string& assetName, const RestrictedType type);
    bool AddRestrictedVerifier(const std::string& assetName, const std::string& verifier);

    // Cache only validation functions
    bool TrySpendCoin(const COutPoint& out, const CTxOut& coin);

    // Help functions
    bool ContainsAsset(const CNewAsset& asset);
    bool ContainsAsset(const std::string& assetName);

    bool CheckIfAssetExists(const std::string& name, bool fForceDuplicateCheck = true);
    bool GetAssetMetaDataIfExists(const std::string &name, CNewAsset &asset, int& nHeight, uint256& blockHash);
    bool GetAssetMetaDataIfExists(const std::string &name, CNewAsset &asset);

    //! Returns true if the Asset Verifier String was found for an asset_name, if fSkipTempCache is true, it will only search passets pointer and databases
    bool GetAssetVerifierStringIfExists(const std::string &name, CNullAssetTxVerifierString& verifier, bool fSkipTempCache = false);

    //! Return true if the address has the given qualifier assigned to it
    bool CheckForAddressQualifier(const std::string &qualifier_name, const std::string& address, bool fSkipTempCache = false);

    //! Return true if the address is marked as frozen
    bool CheckForAddressRestriction(const std::string &restricted_name, const std::string& address, bool fSkipTempCache = false);

    //! Return true if the restricted asset is globally freezing trading
    bool CheckForGlobalRestriction(const std::string &restricted_name, bool fSkipTempCache = false);

    //! Calculate the size of the CAssets (in bytes)
    size_t DynamicMemoryUsage() const;

    //! Get the size of the none databased cache
    size_t GetCacheSize() const;
    size_t GetCacheSizeV2() const;

    //! Flush all new cache entries into the passets global cache
    bool Flush();

    //! Write asset cache data to database
    bool DumpCacheToDatabase();

    void ClearDirtyCache() {

        vUndoAssetAmount.clear();
        vSpentAssets.clear();

        setNewAssetsToRemove.clear();
        setNewAssetsToAdd.clear();

        setNewReissueToAdd.clear();
        setNewReissueToRemove.clear();

        setNewTransferAssetsToAdd.clear();
        setNewTransferAssetsToRemove.clear();

        setNewOwnerAssetsToAdd.clear();
        setNewOwnerAssetsToRemove.clear();

        mapReissuedAssetData.clear();
        mapAssetsAddressAmount.clear();

        setNewQualifierAddressToAdd.clear();
        setNewQualifierAddressToRemove.clear();

        setNewRestrictedAddressToAdd.clear();
        setNewRestrictedAddressToRemove.clear();

        setNewRestrictedGlobalToAdd.clear();
        setNewRestrictedGlobalToRemove.clear();

        setNewRestrictedVerifierToAdd.clear();
        setNewRestrictedVerifierToRemove.clear();

        mapRootQualifierAddressesAdd.clear();
        mapRootQualifierAddressesRemove.clear();
    }

   std::string CacheToString() const {

       return strprintf(
               "vNewAssetsToRemove size : %d, vNewAssetsToAdd size : %d, vNewTransfer size : %d, vSpentAssets : %d\n"
               "setNewQualifierAddressToAdd size : %d, setNewQualifierAddressToRemove size : %d, setNewRestrictedAddressToAdd size : %d\n"
               "setNewRestrictedAddressToRemove size : %d, setNewRestrictedGlobalToAdd size : %d, setNewRestrictedGlobalToRemove : %d",
               setNewAssetsToRemove.size(), setNewAssetsToAdd.size(), setNewTransferAssetsToAdd.size(),
               vSpentAssets.size(), setNewQualifierAddressToAdd.size(), setNewQualifierAddressToRemove.size(), setNewRestrictedAddressToAdd.size(),
               setNewRestrictedAddressToRemove.size(), setNewRestrictedGlobalToAdd.size(), setNewRestrictedGlobalToRemove.size());
   }
};

// Functions to be used to get access to the current burn amount required for specific asset issuance transactions
CAmount GetIssueAssetBurnAmount();
CAmount GetReissueAssetBurnAmount();
CAmount GetIssueSubAssetBurnAmount();
CAmount GetIssueUniqueAssetBurnAmount();
CAmount GetIssueQualifierAssetBurnAmount();
CAmount GetIssueSubQualifierAssetBurnAmount();
CAmount GetIssueRestrictedAssetBurnAmount();
CAmount GetAddNullQualifierTagBurnAmount();
CAmount GetBurnAmount(const AssetType type);
CAmount GetBurnAmount(const int nType);
std::string GetBurnAddress(const AssetType type);
std::string GetBurnAddress(const int nType);

void GetTxOutAssetTypes(const std::vector<CTxOut>& vout, int& issues, int& reissues, int& transfers, int& owners);

bool IsAssetNameValid(const std::string& name);
bool IsAssetNameValid(const std::string& name, AssetType& assetType);
bool IsAssetNameValid(const std::string& name, AssetType& assetType, std::string& error);
bool IsUniqueTagValid(const std::string& tag);
bool IsAssetNameAnOwner(const std::string& name);
bool IsAssetNameAnRestricted(const std::string& name);
bool IsAssetNameAQualifier(const std::string& name, bool fOnlyQualifiers = false);
bool IsAssetNameASubQualifier(const std::string& name);
bool IsAssetNameARoot(const std::string& name);
std::string GetParentName(const std::string& name); // Gets the parent name of a subasset TEST/TESTSUB would return TEST
std::string RestrictedNameToOwnerName(const std::string& name);
std::string GetUniqueAssetName(const std::string& parent, const std::string& tag);

bool IsTypeCheckNameValid(const AssetType type, const std::string& name, std::string& error);

bool IsAssetUnitsValid(const CAmount& units);

bool AssetFromTransaction(const CTransaction& tx, CNewAsset& asset, std::string& strAddress);
bool OwnerFromTransaction(const CTransaction& tx, std::string& ownerName, std::string& strAddress);
bool ReissueAssetFromTransaction(const CTransaction& tx, CReissueAsset& reissue, std::string& strAddress);
bool UniqueAssetFromTransaction(const CTransaction& tx, CNewAsset& asset, std::string& strAddress);
bool QualifierAssetFromTransaction(const CTransaction& tx, CNewAsset& asset, std::string& strAddress);
bool RestrictedAssetFromTransaction(const CTransaction& tx, CNewAsset& asset, std::string& strAddress);

bool TransferAssetFromScript(const CScript& scriptPubKey, CAssetTransfer& assetTransfer, std::string& strAddress);
bool AssetFromScript(const CScript& scriptPubKey, CNewAsset& asset, std::string& strAddress);
bool OwnerAssetFromScript(const CScript& scriptPubKey, std::string& assetName, std::string& strAddress);
bool ReissueAssetFromScript(const CScript& scriptPubKey, CReissueAsset& reissue, std::string& strAddress);
bool QualifierAssetFromScript(const CScript& scriptPubKey, CNewAsset& asset, std::string& strAddress);
bool RestrictedAssetFromScript(const CScript& scriptPubKey, CNewAsset& asset, std::string& strAddress);
bool AssetNullDataFromScript(const CScript& scriptPubKey, CNullAssetTxData& assetData, std::string& strAddress);
bool AssetNullVerifierDataFromScript(const CScript& scriptPubKey, CNullAssetTxVerifierString& verifierData);
bool GlobalAssetNullDataFromScript(const CScript& scriptPubKey, CNullAssetTxData& assetData);

bool CheckIssueBurnTx(const CTxOut& txOut, const AssetType& type, const int numberIssued);
bool CheckIssueBurnTx(const CTxOut& txOut, const AssetType& type);
bool CheckReissueBurnTx(const CTxOut& txOut);

bool CheckIssueDataTx(const CTxOut& txOut);
bool CheckOwnerDataTx(const CTxOut& txOut);
bool CheckReissueDataTx(const CTxOut& txOut);
bool CheckTransferOwnerTx(const CTxOut& txOut);

bool CheckEncodedIPFS(const std::string& hash, std::string& strError);

//! Check the Encoded hash and make sure it is either an IPFS hash or a OIP hash
bool CheckEncoded(const std::string& hash, std::string& strError);

bool CheckAmountWithUnits(const CAmount& nAmount, const int8_t nUnits);

bool IsScriptNewAsset(const CScript& scriptPubKey, int& nStartingIndex);
bool IsScriptNewUniqueAsset(const CScript& scriptPubKey, int& nStartingIndex);
bool IsScriptOwnerAsset(const CScript& scriptPubKey, int& nStartingIndex);
bool IsScriptReissueAsset(const CScript& scriptPubKey, int& nStartingIndex);
bool IsScriptTransferAsset(const CScript& scriptPubKey, int& nStartingIndex);
bool IsScriptNewQualifierAsset(const CScript& scriptPubKey);
bool IsScriptNewQualifierAsset(const CScript &scriptPubKey, int &nStartingIndex);
bool IsScriptNewRestrictedAsset(const CScript& scriptPubKey);
bool IsScriptNewRestrictedAsset(const CScript &scriptPubKey, int &nStartingIndex);
bool IsScriptNewAsset(const CScript& scriptPubKey);
bool IsScriptNewUniqueAsset(const CScript& scriptPubKey);
bool IsScriptOwnerAsset(const CScript& scriptPubKey);
bool IsScriptReissueAsset(const CScript& scriptPubKey);
bool IsScriptTransferAsset(const CScript& scriptPubKey);

bool IsNewOwnerTxValid(const CTransaction& tx, const std::string& assetName, const std::string& address, std::string& errorMsg);

void GetAllAdministrativeAssets(CWallet *pwallet, std::vector<std::string> &names, int nMinConf = 1);
void GetAllMyAssets(CWallet* pwallet, std::vector<std::string>& names, int nMinConf = 1, bool fIncludeAdministrator = false, bool fOnlyAdministrator = false);

bool GetAssetInfoFromCoin(const Coin& coin, std::string& strName, CAmount& nAmount);
bool GetAssetInfoFromScript(const CScript& scriptPubKey, std::string& strName, CAmount& nAmount);

bool GetAssetData(const CScript& script, CAssetOutputEntry& data);

bool GetBestAssetAddressAmount(CAssetsCache& cache, const std::string& assetName, const std::string& address);

std::string DecodeAssetData(std::string encoded);
std::string EncodeAssetData(std::string decoded);
std::string DecodeIPFS(std::string encoded);
std::string EncodeIPFS(std::string decoded);

bool GetAllMyAssetBalances(std::map<std::string, std::vector<COutput> >& outputs, std::map<std::string, CAmount>& amounts, const int confirmations = 0, const std::string& prefix = "");

/** Verifies that this wallet owns the give asset */
bool VerifyWalletHasAsset(const std::string& asset_name, std::pair<int, std::string>& pairError);

std::string DecodeIPFS(std::string encoded);
std::string EncodeIPFS(std::string decoded);

bool CreateAssetTransaction(CWallet* pwallet, CCoinControl& coinControl, const CNewAsset& asset, const std::string& address, std::pair<int, std::string>& error, CWalletTx& wtxNew, CReserveKey& reservekey, CAmount& nFeeRequired, std::string* verifier_string = nullptr);
bool CreateAssetTransaction(CWallet* pwallet, CCoinControl& coinControl, const std::vector<CNewAsset> assets, const std::string& address, std::pair<int, std::string>& error, CWalletTx& wtxNew, CReserveKey& reservekey, CAmount& nFeeRequired, std::string* verifier_string = nullptr);

bool CreateReissueAssetTransaction(CWallet* pwallet, CCoinControl& coinControl, const CReissueAsset& asset, const std::string& address, std::pair<int, std::string>& error, CWalletTx& wtxNew, CReserveKey& reservekey, CAmount& nFeeRequired, std::string* verifier_string = nullptr);
bool CreateTransferAssetTransaction(CWallet* pwallet, const CCoinControl& coinControl, const std::vector< std::pair<CAssetTransfer, std::string> >vTransfers, const std::string& changeAddress, std::pair<int, std::string>& error, CWalletTx& wtxNew, CReserveKey& reservekey, CAmount& nFeeRequired, std::vector<std::pair<CNullAssetTxData, std::string> >* nullAssetTxData = nullptr, std::vector<CNullAssetTxData>* nullGlobalRestrictionData = nullptr);
bool SendAssetTransaction(CWallet* pwallet, CWalletTx& transaction, CReserveKey& reserveKey, std::pair<int, std::string>& error, std::string& txid);

/** Helper method for extracting address bytes, asset name and amount from an asset script */
bool ParseAssetScript(CScript scriptPubKey, uint160 &hashBytes, std::string &assetName, CAmount &assetAmount);

/** Helper method for extracting #TAGS from a verifier string */
void ExtractVerifierStringQualifiers(const std::string& verifier, std::set<std::string>& qualifiers);
bool CheckVerifierString(const std::string& verifier, std::set<std::string>& setFoundQualifiers, std::string& strError, ErrorReport* errorReport = nullptr);
std::string GetStrippedVerifierString(const std::string& verifier);

/** Helper methods that validate changes to null asset data transaction databases */
bool VerifyNullAssetDataFlag(const int& flag, std::string& strError);
bool VerifyQualifierChange(CAssetsCache& cache, const CNullAssetTxData& data, const std::string& address, std::string& strError);
bool VerifyRestrictedAddressChange(CAssetsCache& cache, const CNullAssetTxData& data, const std::string& address, std::string& strError);
bool VerifyGlobalRestrictedChange(CAssetsCache& cache, const CNullAssetTxData& data, std::string& strError);

//// Non Contextual Check functions
bool CheckVerifierAssetTxOut(const CTxOut& txout, std::string& strError);
bool CheckNewAsset(const CNewAsset& asset, std::string& strError);
bool CheckReissueAsset(const CReissueAsset& asset, std::string& strError);

//// Contextual Check functions
bool ContextualCheckNullAssetTxOut(const CTxOut& txout, CAssetsCache* assetCache, std::string& strError, std::vector<std::pair<std::string, CNullAssetTxData>>* myNullAssetData = nullptr);
bool ContextualCheckGlobalAssetTxOut(const CTxOut& txout, CAssetsCache* assetCache, std::string& strError);
bool ContextualCheckVerifierAssetTxOut(const CTxOut& txout, CAssetsCache* assetCache, std::string& strError);
bool ContextualCheckVerifierString(CAssetsCache* cache, const std::string& verifier, const std::string& check_address, std::string& strError, ErrorReport* errorReport = nullptr);
bool ContextualCheckNewAsset(CAssetsCache* assetCache, const CNewAsset& asset, std::string& strError, bool fCheckMempool = false);
bool ContextualCheckTransferAsset(CAssetsCache* assetCache, const CAssetTransfer& transfer, const std::string& address, std::string& strError);
bool ContextualCheckReissueAsset(CAssetsCache* assetCache, const CReissueAsset& reissue_asset, std::string& strError, const CTransaction& tx);
bool ContextualCheckReissueAsset(CAssetsCache* assetCache, const CReissueAsset& reissue_asset, std::string& strError);
bool ContextualCheckUniqueAssetTx(CAssetsCache* assetCache, std::string& strError, const CTransaction& tx);
bool ContextualCheckUniqueAsset(CAssetsCache* assetCache, const CNewAsset& unique_asset, std::string& strError);
#endif //BitcoinSubsidiumCOIN_ASSET_PROTOCOL_H
