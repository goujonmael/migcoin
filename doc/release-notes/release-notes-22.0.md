22.0 Release Notes
==================

Migcoin Core version 22.0 is now available from:

  <https://migcoincore.org/bin/migcoin-core-22.0/>

This release includes new features, various bug fixes and performance
improvements, as well as updated translations.

Please report bugs using the issue tracker at GitHub:

  <https://github.com/migcoin/migcoin/issues>

To receive security and update notifications, please subscribe to:

  <https://migcoincore.org/en/list/announcements/join/>

How to Upgrade
==============

If you are running an older version, shut it down. Wait until it has completely
shut down (which might take a few minutes in some cases), then run the
installer (on Windows) or just copy over `/Applications/Migcoin-Qt` (on Mac)
or `migcoind`/`migcoin-qt` (on Linux).

Upgrading directly from a version of Migcoin Core that has reached its EOL is
possible, but it might take some time if the data directory needs to be migrated. Old
wallet versions of Migcoin Core are generally supported.

Compatibility
==============

Migcoin Core is supported and extensively tested on operating systems
using the Linux kernel, macOS 10.14+, and Windows 7 and newer.  Migcoin
Core should also work on most other Unix-like systems but is not as
frequently tested on them.  It is not recommended to use Migcoin Core on
unsupported systems.

From Migcoin Core 22.0 onwards, macOS versions earlier than 10.14 are no longer supported.

Notable changes
===============

P2P and network changes
-----------------------
- Added support for running Migcoin Core as an
  [I2P (Invisible Internet Project)](https://en.wikipedia.org/wiki/I2P) service
  and connect to such services. See [i2p.md](https://github.com/migcoin/migcoin/blob/22.x/doc/i2p.md) for details. (#20685)
- This release removes support for Tor version 2 hidden services in favor of Tor
  v3 only, as the Tor network [dropped support for Tor
  v2](https://blog.torproject.org/v2-deprecation-timeline) with the release of
  Tor version 0.4.6.  Henceforth, Migcoin Core ignores Tor v2 addresses; it
  neither rumors them over the network to other peers, nor stores them in memory
  or to `peers.dat`.  (#22050)

- Added NAT-PMP port mapping support via
  [`libnatpmp`](https://miniupnp.tuxfamily.org/libnatpmp.html). (#18077)

New and Updated RPCs
--------------------

- Due to [BIP 350](https://github.com/migcoin/bips/blob/master/bip-0350.mediawiki)
  being implemented, behavior for all RPCs that accept addresses is changed when
  a native witness version 1 (or higher) is passed. These now require a Bech32m
  encoding instead of a Bech32 one, and Bech32m encoding will be used for such
  addresses in RPC output as well. No version 1 addresses should be created
  for mainnet until consensus rules are adopted that give them meaning
  (as will happen through [BIP 341](https://github.com/migcoin/bips/blob/master/bip-0341.mediawiki)).
  Once that happens, Bech32m is expected to be used for them, so this shouldn't
  affect any production systems, but may be observed on other networks where such
  addresses already have meaning (like signet). (#20861)

- The `getpeerinfo` RPC returns two new boolean fields, `bip152_hb_to` and
  `bip152_hb_from`, that respectively indicate whether we selected a peer to be
  in compact blocks high-bandwidth mode or whether a peer selected us as a
  compact blocks high-bandwidth peer. High-bandwidth peers send new block
  announcements via a `cmpctblock` message rather than the usual inv/headers
  announcements. See BIP 152 for more details. (#19776)

- `getpeerinfo` no longer returns the following fields: `addnode`, `banscore`,
  and `whitelisted`, which were previously deprecated in 0.21. Instead of
  `addnode`, the `connection_type` field returns manual. Instead of
  `whitelisted`, the `permissions` field indicates if the peer has special
  privileges. The `banscore` field has simply been removed. (#20755)

- The following RPCs:  `gettxout`, `getrawtransaction`, `decoderawtransaction`,
  `decodescript`, `gettransaction`, and REST endpoints: `/rest/tx`,
  `/rest/getutxos`, `/rest/block` deprecated the following fields (which are no
  longer returned in the responses by default): `addresses`, `reqSigs`.
  The `-deprecatedrpc=addresses` flag must be passed for these fields to be
  included in the RPC response. This flag/option will be available only for this major release, after which
  the deprecation will be removed entirely. Note that these fields are attributes of
  the `scriptPubKey` object returned in the RPC response. However, in the response
  of `decodescript` these fields are top-level attributes, and included again as attributes
  of the `scriptPubKey` object. (#20286)

- When creating a hex-encoded migcoin transaction using the `migcoin-tx` utility
  with the `-json` option set, the following fields: `addresses`, `reqSigs` are no longer
  returned in the tx output of the response. (#20286)

- The `listbanned` RPC now returns two new numeric fields: `ban_duration` and `time_remaining`.
  Respectively, these new fields indicate the duration of a ban and the time remaining until a ban expires,
  both in seconds. Additionally, the `ban_created` field is repositioned to come before `banned_until`. (#21602)

- The `setban` RPC can ban onion addresses again. This fixes a regression introduced in version 0.21.0. (#20852)

- The `getnodeaddresses` RPC now returns a "network" field indicating the
  network type (ipv4, ipv6, onion, or i2p) for each address.  (#21594)

- `getnodeaddresses` now also accepts a "network" argument (ipv4, ipv6, onion,
  or i2p) to return only addresses of the specified network.  (#21843)

- The `testmempoolaccept` RPC now accepts multiple transactions (still experimental at the moment,
  API may be unstable). This is intended for testing transaction packages with dependency
  relationships; it is not recommended for batch-validating independent transactions. In addition to
  mempool policy, package policies apply: the list cannot contain more than 25 transactions or have a
  total size exceeding 101K virtual bytes, and cannot conflict with (spend the same inputs as) each other or
  the mempool, even if it would be a valid BIP125 replace-by-fee. There are some known limitations to
  the accuracy of the test accept: it's possible for `testmempoolaccept` to return "allowed"=True for a
  group of transactions, but "too-long-mempool-chain" if they are actually submitted. (#20833)

- `addmultisigaddress` and `createmultisig` now support up to 20 keys for
  Segwit addresses. (#20867)

Changes to Wallet or GUI related RPCs can be found in the GUI or Wallet section below.

Build System
------------

- Release binaries are now produced using the new `guix`-based build system.
  The [/doc/release-process.md](/doc/release-process.md) document has been updated accordingly.

Files
-----

- The list of banned hosts and networks (via `setban` RPC) is now saved on disk
  in JSON format in `banlist.json` instead of `banlist.dat`. `banlist.dat` is
  only read on startup if `banlist.json` is not present. Changes are only written to the new
  `banlist.json`. A future version of Migcoin Core may completely ignore
  `banlist.dat`. (#20966)

New settings
------------

- The `-natpmp` option has been added to use NAT-PMP to map the listening port.
  If both UPnP and NAT-PMP are enabled, a successful allocation from UPnP
  prevails over one from NAT-PMP. (#18077)

Updated settings
----------------

Changes to Wallet or GUI related settings can be found in the GUI or Wallet section below.

- Passing an invalid `-rpcauth` argument now cause migcoind to fail to start.  (#20461)

Tools and Utilities
-------------------

- A new CLI `-addrinfo` command returns the number of addresses known to the
  node per network type (including Tor v2 versus v3) and total. This can be
  useful to see if the node knows enough addresses in a network to use options
  like `-onlynet=<network>` or to upgrade to this release of Migcoin Core 22.0
  that supports Tor v3 only.  (#21595)

- A new `-rpcwaittimeout` argument to `migcoin-cli` sets the timeout
  in seconds to use with `-rpcwait`. If the timeout expires,
  `migcoin-cli` will report a failure. (#21056)

Wallet
------

- External signers such as hardware wallets can now be used through the new RPC methods `enumeratesigners` and `displayaddress`. Support is also added to the `send` RPC call. This feature is experimental. See [external-signer.md](https://github.com/migcoin/migcoin/blob/22.x/doc/external-signer.md) for details. (#16546)

- A new `listdescriptors` RPC is available to inspect the contents of descriptor-enabled wallets.
  The RPC returns public versions of all imported descriptors, including their timestamp and flags.
  For ranged descriptors, it also returns the range boundaries and the next index to generate addresses from. (#20226)

- The `bumpfee` RPC is not available with wallets that have private keys
  disabled. `psbtbumpfee` can be used instead. (#20891)

- The `fundrawtransaction`, `send` and `walletcreatefundedpsbt` RPCs now support an `include_unsafe` option
  that when `true` allows using unsafe inputs to fund the transaction.
  Note that the resulting transaction may become invalid if one of the unsafe inputs disappears.
  If that happens, the transaction must be funded with different inputs and republished. (#21359)

- We now support up to 20 keys in `multi()` and `sortedmulti()` descriptors
  under `wsh()`. (#20867)

- Taproot descriptors can be imported into the wallet only after activation has occurred on the network (e.g. mainnet, testnet, signet) in use. See [descriptors.md](https://github.com/migcoin/migcoin/blob/22.x/doc/descriptors.md) for supported descriptors.

GUI changes
-----------

- External signers such as hardware wallets can now be used. These require an external tool such as [HWI](https://github.com/migcoin-core/HWI) to be installed and configured under Options -> Wallet. When creating a new wallet a new option "External signer" will appear in the dialog. If the device is detected, its name is suggested as the wallet name. The watch-only keys are then automatically imported. Receive addresses can be verified on the device. The send dialog will automatically use the connected device. This feature is experimental and the UI may freeze for a few seconds when performing these actions.

Low-level changes
=================

RPC
---

- The RPC server can process a limited number of simultaneous RPC requests.
  Previously, if this limit was exceeded, the RPC server would respond with
  [status code 500 (`HTTP_INTERNAL_SERVER_ERROR`)](https://en.wikipedia.org/wiki/List_of_HTTP_status_codes#5xx_server_errors).
  Now it returns status code 503 (`HTTP_SERVICE_UNAVAILABLE`). (#18335)

- Error codes have been updated to be more accurate for the following error cases (#18466):
  - `signmessage` now returns RPC_INVALID_ADDRESS_OR_KEY (-5) if the
    passed address is invalid. Previously returned RPC_TYPE_ERROR (-3).
  - `verifymessage` now returns RPC_INVALID_ADDRESS_OR_KEY (-5) if the
    passed address is invalid. Previously returned RPC_TYPE_ERROR (-3).
  - `verifymessage` now returns RPC_TYPE_ERROR (-3) if the passed signature
    is malformed. Previously returned RPC_INVALID_ADDRESS_OR_KEY (-5).

Tests
-----

22.0 change log
===============

A detailed list of changes in this version follows. To keep the list to a manageable length, small refactors and typo fixes are not included, and similar changes are sometimes condensed into one line.

### Consensus
- migcoin/migcoin#19438 Introduce deploymentstatus (ajtowns)
- migcoin/migcoin#20207 Follow-up extra comments on taproot code and tests (sipa)
- migcoin/migcoin#21330 Deal with missing data in signature hashes more consistently (sipa)

### Policy
- migcoin/migcoin#18766 Disable fee estimation in blocksonly mode (by removing the fee estimates global) (darosior)
- migcoin/migcoin#20497 Add `MAX_STANDARD_SCRIPTSIG_SIZE` to policy (sanket1729)
- migcoin/migcoin#20611 Move `TX_MAX_STANDARD_VERSION` to policy (MarcoFalke)

### Mining
- migcoin/migcoin#19937, migcoin/migcoin#20923 Signet mining utility (ajtowns)

### Block and transaction handling
- migcoin/migcoin#14501 Fix possible data race when committing block files (luke-jr)
- migcoin/migcoin#15946 Allow maintaining the blockfilterindex when using prune (jonasschnelli)
- migcoin/migcoin#18710 Add local thread pool to CCheckQueue (hebasto)
- migcoin/migcoin#19521 Coinstats Index (fjahr)
- migcoin/migcoin#19806 UTXO snapshot activation (jamesob)
- migcoin/migcoin#19905 Remove dead CheckForkWarningConditionsOnNewFork (MarcoFalke)
- migcoin/migcoin#19935 Move SaltedHashers to separate file and add some new ones (achow101)
- migcoin/migcoin#20054 Remove confusing and useless "unexpected version" warning (MarcoFalke)
- migcoin/migcoin#20519 Handle rename failure in `DumpMempool(…)` by using the `RenameOver(…)` return value (practicalswift)
- migcoin/migcoin#20749, migcoin/migcoin#20750, migcoin/migcoin#21055, migcoin/migcoin#21270, migcoin/migcoin#21525, migcoin/migcoin#21391, migcoin/migcoin#21767, migcoin/migcoin#21866 Prune `g_chainman` usage (dongcarl)
- migcoin/migcoin#20833 rpc/validation: enable packages through testmempoolaccept (glozow)
- migcoin/migcoin#20834 Locks and docs in ATMP and CheckInputsFromMempoolAndCache (glozow)
- migcoin/migcoin#20854 Remove unnecessary try-block (amitiuttarwar)
- migcoin/migcoin#20868 Remove redundant check on pindex (jarolrod)
- migcoin/migcoin#20921 Don't try to invalidate genesis block in CChainState::InvalidateBlock (theStack)
- migcoin/migcoin#20972 Locks: Annotate CTxMemPool::check to require `cs_main` (dongcarl)
- migcoin/migcoin#21009 Remove RewindBlockIndex logic (dhruv)
- migcoin/migcoin#21025 Guard chainman chainstates with `cs_main` (dongcarl)
- migcoin/migcoin#21202 Two small clang lock annotation improvements (amitiuttarwar)
- migcoin/migcoin#21523 Run VerifyDB on all chainstates (jamesob)
- migcoin/migcoin#21573 Update libsecp256k1 subtree to latest master (sipa)
- migcoin/migcoin#21582, migcoin/migcoin#21584, migcoin/migcoin#21585 Fix assumeutxo crashes (MarcoFalke)
- migcoin/migcoin#21681 Fix ActivateSnapshot to use hardcoded nChainTx (jamesob)
- migcoin/migcoin#21796 index: Avoid async shutdown on init error (MarcoFalke)
- migcoin/migcoin#21946 Document and test lack of inherited signaling in RBF policy (ariard)
- migcoin/migcoin#22084 Package testmempoolaccept followups (glozow)
- migcoin/migcoin#22102 Remove `Warning:` from warning message printed for unknown new rules (prayank23)
- migcoin/migcoin#22112 Force port 0 in I2P (vasild)
- migcoin/migcoin#22135 CRegTestParams: Use `args` instead of `gArgs` (kiminuo)
- migcoin/migcoin#22146 Reject invalid coin height and output index when loading assumeutxo (MarcoFalke)
- migcoin/migcoin#22253 Distinguish between same tx and same-nonwitness-data tx in mempool (glozow)
- migcoin/migcoin#22261 Two small fixes to node broadcast logic (jnewbery)
- migcoin/migcoin#22415 Make `m_mempool` optional in CChainState (jamesob)
- migcoin/migcoin#22499 Update assumed chain params (sriramdvt)
- migcoin/migcoin#22589 net, doc: update I2P hardcoded seeds and docs for 22.0 (jonatack)

### P2P protocol and network code
- migcoin/migcoin#18077 Add NAT-PMP port forwarding support (hebasto)
- migcoin/migcoin#18722 addrman: improve performance by using more suitable containers (vasild)
- migcoin/migcoin#18819 Replace `cs_feeFilter` with simple std::atomic (MarcoFalke)
- migcoin/migcoin#19203 Add regression fuzz harness for CVE-2017-18350. Add FuzzedSocket (practicalswift)
- migcoin/migcoin#19288 fuzz: Add fuzzing harness for TorController (practicalswift)
- migcoin/migcoin#19415 Make DNS lookup mockable, add fuzzing harness (practicalswift)
- migcoin/migcoin#19509 Per-Peer Message Capture (troygiorshev)
- migcoin/migcoin#19763 Don't try to relay to the address' originator (vasild)
- migcoin/migcoin#19771 Replace enum CConnMan::NumConnections with enum class ConnectionDirection (luke-jr)
- migcoin/migcoin#19776 net, rpc: expose high bandwidth mode state via getpeerinfo (theStack)
- migcoin/migcoin#19832 Put disconnecting logs into BCLog::NET category (hebasto)
- migcoin/migcoin#19858 Periodically make block-relay connections and sync headers (sdaftuar)
- migcoin/migcoin#19884 No delay in adding fixed seeds if -dnsseed=0 and peers.dat is empty (dhruv)
- migcoin/migcoin#20079 Treat handshake misbehavior like unknown message (MarcoFalke)
- migcoin/migcoin#20138 Assume that SetCommonVersion is called at most once per peer (MarcoFalke)
- migcoin/migcoin#20162 p2p: declare Announcement::m_state as uint8_t, add getter/setter (jonatack)
- migcoin/migcoin#20197 Protect onions in AttemptToEvictConnection(), add eviction protection test coverage (jonatack)
- migcoin/migcoin#20210 assert `CNode::m_inbound_onion` is inbound in ctor, add getter, unit tests (jonatack)
- migcoin/migcoin#20228 addrman: Make addrman a top-level component (jnewbery)
- migcoin/migcoin#20234 Don't bind on 0.0.0.0 if binds are restricted to Tor (vasild)
- migcoin/migcoin#20477 Add unit testing of node eviction logic (practicalswift)
- migcoin/migcoin#20516 Well-defined CAddress disk serialization, and addrv2 anchors.dat (sipa)
- migcoin/migcoin#20557 addrman: Fix new table bucketing during unserialization (jnewbery)
- migcoin/migcoin#20561 Periodically clear `m_addr_known` (sdaftuar)
- migcoin/migcoin#20599 net processing: Tolerate sendheaders and sendcmpct messages before verack (jnewbery)
- migcoin/migcoin#20616 Check CJDNS address is valid (lontivero)
- migcoin/migcoin#20617 Remove `m_is_manual_connection` from CNodeState (ariard)
- migcoin/migcoin#20624 net processing: Remove nStartingHeight check from block relay (jnewbery)
- migcoin/migcoin#20651 Make p2p recv buffer timeout 20 minutes for all peers (jnewbery)
- migcoin/migcoin#20661 Only select from addrv2-capable peers for torv3 address relay (sipa)
- migcoin/migcoin#20685 Add I2P support using I2P SAM (vasild)
- migcoin/migcoin#20690 Clean up logging of outbound connection type (sdaftuar)
- migcoin/migcoin#20721 Move ping data to `net_processing` (jnewbery)
- migcoin/migcoin#20724 Cleanup of -debug=net log messages (ajtowns)
- migcoin/migcoin#20747 net processing: Remove dropmessagestest (jnewbery)
- migcoin/migcoin#20764 cli -netinfo peer connections dashboard updates 🎄 ✨ (jonatack)
- migcoin/migcoin#20788 add RAII socket and use it instead of bare SOCKET (vasild)
- migcoin/migcoin#20791 remove unused legacyWhitelisted in AcceptConnection() (jonatack)
- migcoin/migcoin#20816 Move RecordBytesSent() call out of `cs_vSend` lock (jnewbery)
- migcoin/migcoin#20845 Log to net debug in MaybeDiscourageAndDisconnect except for noban and manual peers (MarcoFalke)
- migcoin/migcoin#20864 Move SocketSendData lock annotation to header (MarcoFalke)
- migcoin/migcoin#20965 net, rpc:  return `NET_UNROUTABLE` as `not_publicly_routable`, automate helps (jonatack)
- migcoin/migcoin#20966 banman: save the banlist in a JSON format on disk (vasild)
- migcoin/migcoin#21015 Make all of `net_processing` (and some of net) use std::chrono types (dhruv)
- migcoin/migcoin#21029 migcoin-cli: Correct docs (no "generatenewaddress" exists) (luke-jr)
- migcoin/migcoin#21148 Split orphan handling from `net_processing` into txorphanage (ajtowns)
- migcoin/migcoin#21162 Net Processing: Move RelayTransaction() into PeerManager (jnewbery)
- migcoin/migcoin#21167 make `CNode::m_inbound_onion` public, initialize explicitly (jonatack)
- migcoin/migcoin#21186 net/net processing: Move addr data into `net_processing` (jnewbery)
- migcoin/migcoin#21187 Net processing: Only call PushAddress() from `net_processing` (jnewbery)
- migcoin/migcoin#21198 Address outstanding review comments from PR20721 (jnewbery)
- migcoin/migcoin#21222 log: Clarify log message when file does not exist (MarcoFalke)
- migcoin/migcoin#21235 Clarify disconnect log message in ProcessGetBlockData, remove send bool (MarcoFalke)
- migcoin/migcoin#21236 Net processing: Extract `addr` send functionality into MaybeSendAddr() (jnewbery)
- migcoin/migcoin#21261 update inbound eviction protection for multiple networks, add I2P peers (jonatack)
- migcoin/migcoin#21328 net, refactor: pass uint16 CService::port as uint16 (jonatack)
- migcoin/migcoin#21387 Refactor sock to add I2P fuzz and unit tests (vasild)
- migcoin/migcoin#21395 Net processing: Remove unused CNodeState.address member (jnewbery)
- migcoin/migcoin#21407 i2p: limit the size of incoming messages (vasild)
- migcoin/migcoin#21506 p2p, refactor: make NetPermissionFlags an enum class (jonatack)
- migcoin/migcoin#21509 Don't send FEEFILTER in blocksonly mode (mzumsande)
- migcoin/migcoin#21560 Add Tor v3 hardcoded seeds (laanwj)
- migcoin/migcoin#21563 Restrict period when `cs_vNodes` mutex is locked (hebasto)
- migcoin/migcoin#21564 Avoid calling getnameinfo when formatting IPv4 addresses in CNetAddr::ToStringIP (practicalswift)
- migcoin/migcoin#21631 i2p: always check the return value of Sock::Wait() (vasild)
- migcoin/migcoin#21644 p2p, bugfix: use NetPermissions::HasFlag() in CConnman::Bind() (jonatack)
- migcoin/migcoin#21659 flag relevant Sock methods with [[nodiscard]] (vasild)
- migcoin/migcoin#21750 remove unnecessary check of `CNode::cs_vSend` (vasild)
- migcoin/migcoin#21756 Avoid calling `getnameinfo` when formatting IPv6 addresses in `CNetAddr::ToStringIP` (practicalswift)
- migcoin/migcoin#21775 Limit `m_block_inv_mutex` (MarcoFalke)
- migcoin/migcoin#21825 Add I2P hardcoded seeds (jonatack)
- migcoin/migcoin#21843 p2p, rpc: enable GetAddr, GetAddresses, and getnodeaddresses by network (jonatack)
- migcoin/migcoin#21845 net processing: Don't require locking `cs_main` before calling RelayTransactions() (jnewbery)
- migcoin/migcoin#21872 Sanitize message type for logging (laanwj)
- migcoin/migcoin#21914 Use stronger AddLocal() for our I2P address (vasild)
- migcoin/migcoin#21985 Return IPv6 scope id in `CNetAddr::ToStringIP()` (laanwj)
- migcoin/migcoin#21992 Remove -feefilter option (amadeuszpawlik)
- migcoin/migcoin#21996 Pass strings to NetPermissions::TryParse functions by const ref (jonatack)
- migcoin/migcoin#22013 ignore block-relay-only peers when skipping DNS seed (ajtowns)
- migcoin/migcoin#22050 Remove tor v2 support (jonatack)
- migcoin/migcoin#22096 AddrFetch - don't disconnect on self-announcements (mzumsande)
- migcoin/migcoin#22141 net processing: Remove hash and fValidatedHeaders from QueuedBlock (jnewbery)
- migcoin/migcoin#22144 Randomize message processing peer order (sipa)
- migcoin/migcoin#22147 Protect last outbound HB compact block peer (sdaftuar)
- migcoin/migcoin#22179 Torv2 removal followups (vasild)
- migcoin/migcoin#22211 Relay I2P addresses even if not reachable (by us) (vasild)
- migcoin/migcoin#22284 Performance improvements to ProtectEvictionCandidatesByRatio() (jonatack)
- migcoin/migcoin#22387 Rate limit the processing of rumoured addresses (sipa)
- migcoin/migcoin#22455 addrman: detect on-disk corrupted nNew and nTried during unserialization (vasild)

### Wallet
- migcoin/migcoin#15710 Catch `ios_base::failure` specifically (Bushstar)
- migcoin/migcoin#16546 External signer support - Wallet Box edition (Sjors)
- migcoin/migcoin#17331 Use effective values throughout coin selection (achow101)
- migcoin/migcoin#18418 Increase `OUTPUT_GROUP_MAX_ENTRIES` to 100 (fjahr)
- migcoin/migcoin#18842 Mark replaced tx to not be in the mempool anymore (MarcoFalke)
- migcoin/migcoin#19136 Add `parent_desc` to `getaddressinfo` (achow101)
- migcoin/migcoin#19137 wallettool: Add dump and createfromdump commands (achow101)
- migcoin/migcoin#19651 `importdescriptor`s update existing (S3RK)
- migcoin/migcoin#20040 Refactor OutputGroups to handle fees and spending eligibility on grouping (achow101)
- migcoin/migcoin#20202 Make BDB support optional (achow101)
- migcoin/migcoin#20226, migcoin/migcoin#21277, - migcoin/migcoin#21063 Add `listdescriptors` command (S3RK)
- migcoin/migcoin#20267 Disable and fix tests for when BDB is not compiled (achow101)
- migcoin/migcoin#20275 List all wallets in non-SQLite and non-BDB builds (ryanofsky)
- migcoin/migcoin#20365 wallettool: Add parameter to create descriptors wallet (S3RK)
- migcoin/migcoin#20403 `upgradewallet` fixes, improvements, test coverage (jonatack)
- migcoin/migcoin#20448 `unloadwallet`: Allow specifying `wallet_name` param matching RPC endpoint wallet (luke-jr)
- migcoin/migcoin#20536 Error with "Transaction too large" if the funded tx will end up being too large after signing (achow101)
- migcoin/migcoin#20687 Add missing check for -descriptors wallet tool option (MarcoFalke)
- migcoin/migcoin#20952 Add BerkeleyDB version sanity check at init time (laanwj)
- migcoin/migcoin#21127 Load flags before everything else (Sjors)
- migcoin/migcoin#21141 Add new format string placeholders for walletnotify (maayank)
- migcoin/migcoin#21238 A few descriptor improvements to prepare for Taproot support (sipa)
- migcoin/migcoin#21302 `createwallet` examples for descriptor wallets (S3RK)
- migcoin/migcoin#21329 descriptor wallet: Cache last hardened xpub and use in normalized descriptors (achow101)
- migcoin/migcoin#21365 Basic Taproot signing support for descriptor wallets (sipa)
- migcoin/migcoin#21417 Misc external signer improvement and HWI 2 support (Sjors)
- migcoin/migcoin#21467 Move external signer out of wallet module (Sjors)
- migcoin/migcoin#21572 Fix wrong wallet RPC context set after #21366 (ryanofsky)
- migcoin/migcoin#21574 Drop JSONRPCRequest constructors after #21366 (ryanofsky)
- migcoin/migcoin#21666 Miscellaneous external signer changes (fanquake)
- migcoin/migcoin#21759 Document coin selection code (glozow)
- migcoin/migcoin#21786 Ensure sat/vB feerates are in range (mantissa of 3) (jonatack)
- migcoin/migcoin#21944 Fix issues when `walletdir` is root directory (prayank23)
- migcoin/migcoin#22042 Replace size/weight estimate tuple with struct for named fields (instagibbs)
- migcoin/migcoin#22051 Basic Taproot derivation support for descriptors (sipa)
- migcoin/migcoin#22154 Add OutputType::BECH32M and related wallet support for fetching bech32m addresses (achow101)
- migcoin/migcoin#22156 Allow tr() import only when Taproot is active (achow101)
- migcoin/migcoin#22166 Add support for inferring tr() descriptors (sipa)
- migcoin/migcoin#22173 Do not load external signers wallets when unsupported (achow101)
- migcoin/migcoin#22308 Add missing BlockUntilSyncedToCurrentChain (MarcoFalke)
- migcoin/migcoin#22334 Do not spam about non-existent spk managers (S3RK)
- migcoin/migcoin#22379 Erase spkmans rather than setting to nullptr (achow101)
- migcoin/migcoin#22421 Make IsSegWitOutput return true for taproot outputs (sipa)
- migcoin/migcoin#22461 Change ScriptPubKeyMan::Upgrade default to True (achow101)
- migcoin/migcoin#22492 Reorder locks in dumpwallet to avoid lock order assertion (achow101)
- migcoin/migcoin#22686 Use GetSelectionAmount in ApproximateBestSubset (achow101)

### RPC and other APIs
- migcoin/migcoin#18335, migcoin/migcoin#21484 cli: Print useful error if migcoind rpc work queue exceeded (LarryRuane)
- migcoin/migcoin#18466 Fix invalid parameter error codes for `{sign,verify}message` RPCs (theStack)
- migcoin/migcoin#18772 Calculate fees in `getblock` using BlockUndo data (robot-visions)
- migcoin/migcoin#19033 http: Release work queue after event base finish (promag)
- migcoin/migcoin#19055 Add MuHash3072 implementation (fjahr)
- migcoin/migcoin#19145 Add `hash_type` MUHASH for gettxoutsetinfo (fjahr)
- migcoin/migcoin#19847 Avoid duplicate set lookup in `gettxoutproof` (promag)
- migcoin/migcoin#20286 Deprecate `addresses` and `reqSigs` from RPC outputs (mjdietzx)
- migcoin/migcoin#20459 Fail to return undocumented return values (MarcoFalke)
- migcoin/migcoin#20461 Validate `-rpcauth` arguments (promag)
- migcoin/migcoin#20556 Properly document return values (`submitblock`, `gettxout`, `getblocktemplate`, `scantxoutset`) (MarcoFalke)
- migcoin/migcoin#20755 Remove deprecated fields from `getpeerinfo` (amitiuttarwar)
- migcoin/migcoin#20832 Better error messages for invalid addresses (eilx2)
- migcoin/migcoin#20867 Support up to 20 keys for multisig under Segwit context (darosior)
- migcoin/migcoin#20877 cli: `-netinfo` user help and argument parsing improvements (jonatack)
- migcoin/migcoin#20891 Remove deprecated bumpfee behavior (achow101)
- migcoin/migcoin#20916 Return wtxid from `testmempoolaccept` (MarcoFalke)
- migcoin/migcoin#20917 Add missing signet mentions in network name lists (theStack)
- migcoin/migcoin#20941 Document `RPC_TRANSACTION_ALREADY_IN_CHAIN` exception (jarolrod)
- migcoin/migcoin#20944 Return total fee in `getmempoolinfo` (MarcoFalke)
- migcoin/migcoin#20964 Add specific error code for "wallet already loaded" (laanwj)
- migcoin/migcoin#21053 Document {previous,next}blockhash as optional (theStack)
- migcoin/migcoin#21056 Add a `-rpcwaittimeout` parameter to limit time spent waiting (cdecker)
- migcoin/migcoin#21192 cli: Treat high detail levels as maximum in `-netinfo` (laanwj)
- migcoin/migcoin#21311 Document optional fields for `getchaintxstats` result (theStack)
- migcoin/migcoin#21359 `include_unsafe` option for fundrawtransaction (t-bast)
- migcoin/migcoin#21426 Remove `scantxoutset` EXPERIMENTAL warning (jonatack)
- migcoin/migcoin#21544 Missing doc updates for bumpfee psbt update (MarcoFalke)
- migcoin/migcoin#21594 Add `network` field to `getnodeaddresses` (jonatack)
- migcoin/migcoin#21595, migcoin/migcoin#21753 cli: Create `-addrinfo` (jonatack)
- migcoin/migcoin#21602 Add additional ban time fields to `listbanned` (jarolrod)
- migcoin/migcoin#21679 Keep default argument value in correct type (promag)
- migcoin/migcoin#21718 Improve error message for `getblock` invalid datatype (klementtan)
- migcoin/migcoin#21913 RPCHelpMan fixes (kallewoof)
- migcoin/migcoin#22021 `bumpfee`/`psbtbumpfee` fixes and updates (jonatack)
- migcoin/migcoin#22043 `addpeeraddress` test coverage, code simplify/constness (jonatack)
- migcoin/migcoin#22327 cli: Avoid truncating `-rpcwaittimeout` (MarcoFalke)

### GUI
- migcoin/migcoin#18948 Call setParent() in the parent's context (hebasto)
- migcoin/migcoin#20482 Add depends qt fix for ARM macs (jonasschnelli)
- migcoin/migcoin#21836 scripted-diff: Replace three dots with ellipsis in the ui strings (hebasto)
- migcoin/migcoin#21935 Enable external signer support for GUI builds (Sjors)
- migcoin/migcoin#22133 Make QWindowsVistaStylePlugin available again (regression) (hebasto)
- migcoin-core/gui#4 UI external signer support (e.g. hardware wallet) (Sjors)
- migcoin-core/gui#13 Hide peer detail view if multiple are selected (promag)
- migcoin-core/gui#18 Add peertablesortproxy module (hebasto)
- migcoin-core/gui#21 Improve pruning tooltip (fluffypony, MigcoinErrorLog)
- migcoin-core/gui#72 Log static plugins meta data and used style (hebasto)
- migcoin-core/gui#79 Embed monospaced font (hebasto)
- migcoin-core/gui#85 Remove unused "What's This" button in dialogs on Windows OS (hebasto)
- migcoin-core/gui#115 Replace "Hide tray icon" option with positive "Show tray icon" one (hebasto)
- migcoin-core/gui#118 Remove BDB version from the Information tab (hebasto)
- migcoin-core/gui#121 Early subscribe core signals in transaction table model (promag)
- migcoin-core/gui#123 Do not accept command while executing another one (hebasto)
- migcoin-core/gui#125 Enable changing the autoprune block space size in intro dialog (luke-jr)
- migcoin-core/gui#138 Unlock encrypted wallet "OK" button bugfix (mjdietzx)
- migcoin-core/gui#139 doc: Improve gui/src/qt README.md (jarolrod)
- migcoin-core/gui#154 Support macOS Dark mode (goums, Uplab)
- migcoin-core/gui#162 Add network to peers window and peer details (jonatack)
- migcoin-core/gui#163, migcoin-core/gui#180 Peer details: replace Direction with Connection Type (jonatack)
- migcoin-core/gui#164 Handle peer addition/removal in a right way (hebasto)
- migcoin-core/gui#165 Save QSplitter state in QSettings (hebasto)
- migcoin-core/gui#173 Follow Qt docs when implementing rowCount and columnCount (hebasto)
- migcoin-core/gui#179 Add Type column to peers window, update peer details name/tooltip (jonatack)
- migcoin-core/gui#186 Add information to "Confirm fee bump" window (prayank23)
- migcoin-core/gui#189 Drop workaround for QTBUG-42503 which was fixed in Qt 5.5.0 (prusnak)
- migcoin-core/gui#194 Save/restore RPCConsole geometry only for window (hebasto)
- migcoin-core/gui#202 Fix right panel toggle in peers tab (RandyMcMillan)
- migcoin-core/gui#203 Display plain "Inbound" in peer details (jonatack)
- migcoin-core/gui#204 Drop buggy TableViewLastColumnResizingFixer class (hebasto)
- migcoin-core/gui#205, migcoin-core/gui#229 Save/restore TransactionView and recentRequestsView tables column sizes (hebasto)
- migcoin-core/gui#206 Display fRelayTxes and `bip152_highbandwidth_{to, from}` in peer details (jonatack)
- migcoin-core/gui#213 Add Copy Address Action to Payment Requests (jarolrod)
- migcoin-core/gui#214 Disable requests context menu actions when appropriate (jarolrod)
- migcoin-core/gui#217 Make warning label look clickable (jarolrod)
- migcoin-core/gui#219 Prevent the main window popup menu (hebasto)
- migcoin-core/gui#220 Do not translate file extensions (hebasto)
- migcoin-core/gui#221 RPCConsole translatable string fixes and improvements (jonatack)
- migcoin-core/gui#226 Add "Last Block" and "Last Tx" rows to peer details area (jonatack)
- migcoin-core/gui#233 qt test: Don't bind to regtest port (achow101)
- migcoin-core/gui#243 Fix issue when disabling the auto-enabled blank wallet checkbox (jarolrod)
- migcoin-core/gui#246 Revert "qt: Use "fusion" style on macOS Big Sur with old Qt" (hebasto)
- migcoin-core/gui#248 For values of "Bytes transferred" and "Bytes/s" with 1000-based prefix names use 1000-based divisor instead of 1024-based (wodry)
- migcoin-core/gui#251 Improve URI/file handling message (hebasto)
- migcoin-core/gui#256 Save/restore column sizes of the tables in the Peers tab (hebasto)
- migcoin-core/gui#260 Handle exceptions isntead of crash (hebasto)
- migcoin-core/gui#263 Revamp context menus (hebasto)
- migcoin-core/gui#271 Don't clear console prompt when font resizing (jarolrod)
- migcoin-core/gui#275 Support runtime appearance adjustment on macOS (hebasto)
- migcoin-core/gui#276 Elide long strings in their middle in the Peers tab (hebasto)
- migcoin-core/gui#281 Set shortcuts for console's resize buttons (jarolrod)
- migcoin-core/gui#293 Enable wordWrap for Services (RandyMcMillan)
- migcoin-core/gui#296 Do not use QObject::tr plural syntax for numbers with a unit symbol (hebasto)
- migcoin-core/gui#297 Avoid unnecessary translations (hebasto)
- migcoin-core/gui#298 Peertableview alternating row colors (RandyMcMillan)
- migcoin-core/gui#300 Remove progress bar on modal overlay (brunoerg)
- migcoin-core/gui#309 Add access to the Peers tab from the network icon (hebasto)
- migcoin-core/gui#311 Peers Window rename 'Peer id' to 'Peer' (jarolrod)
- migcoin-core/gui#313 Optimize string concatenation by default (hebasto)
- migcoin-core/gui#325 Align numbers in the "Peer Id" column to the right (hebasto)
- migcoin-core/gui#329 Make console buttons look clickable (jarolrod)
- migcoin-core/gui#330 Allow prompt icon to be colorized (jarolrod)
- migcoin-core/gui#331 Make RPC console welcome message translation-friendly (hebasto)
- migcoin-core/gui#332 Replace disambiguation strings with translator comments (hebasto)
- migcoin-core/gui#335 test: Use QSignalSpy instead of QEventLoop (jarolrod)
- migcoin-core/gui#343 Improve the GUI responsiveness when progress dialogs are used (hebasto)
- migcoin-core/gui#361 Fix GUI segfault caused by migcoin/migcoin#22216 (ryanofsky)
- migcoin-core/gui#362 Add keyboard shortcuts to context menus (luke-jr)
- migcoin-core/gui#366 Dark Mode fixes/portability (luke-jr)
- migcoin-core/gui#375 Emit dataChanged signal to dynamically re-sort Peers table (hebasto)
- migcoin-core/gui#393 Fix regression in "Encrypt Wallet" menu item (hebasto)
- migcoin-core/gui#396 Ensure external signer option remains disabled without signers (achow101)
- migcoin-core/gui#406 Handle new added plurals in `migcoin_en.ts` (hebasto)

### Build system
- migcoin/migcoin#17227 Add Android packaging support (icota)
- migcoin/migcoin#17920 guix: Build support for macOS (dongcarl)
- migcoin/migcoin#18298 Fix Qt processing of configure script for depends with DEBUG=1 (hebasto)
- migcoin/migcoin#19160 multiprocess: Add basic spawn and IPC support (ryanofsky)
- migcoin/migcoin#19504 Bump minimum python version to 3.6 (ajtowns)
- migcoin/migcoin#19522 fix building libconsensus with reduced exports for Darwin targets (fanquake)
- migcoin/migcoin#19683 Pin clang search paths for darwin host (dongcarl)
- migcoin/migcoin#19764 Split boost into build/host packages + bump + cleanup (dongcarl)
- migcoin/migcoin#19817 libtapi 1100.0.11 (fanquake)
- migcoin/migcoin#19846 enable unused member function diagnostic (Zero-1729)
- migcoin/migcoin#19867 Document and cleanup Qt hacks (fanquake)
- migcoin/migcoin#20046 Set `CMAKE_INSTALL_RPATH` for native packages (ryanofsky)
- migcoin/migcoin#20223 Drop the leading 0 from the version number (achow101)
- migcoin/migcoin#20333 Remove `native_biplist` dependency (fanquake)
- migcoin/migcoin#20353 configure: Support -fdebug-prefix-map and -fmacro-prefix-map (ajtowns)
- migcoin/migcoin#20359 Various config.site.in improvements and linting (dongcarl)
- migcoin/migcoin#20413 Require C++17 compiler (MarcoFalke)
- migcoin/migcoin#20419 Set minimum supported macOS to 10.14 (fanquake)
- migcoin/migcoin#20421 miniupnpc 2.2.2 (fanquake)
- migcoin/migcoin#20422 Mac deployment unification (fanquake)
- migcoin/migcoin#20424 Update univalue subtree (MarcoFalke)
- migcoin/migcoin#20449 Fix Windows installer build (achow101)
- migcoin/migcoin#20468 Warn when generating man pages for binaries built from a dirty branch (tylerchambers)
- migcoin/migcoin#20469 Avoid secp256k1.h include from system (dergoegge)
- migcoin/migcoin#20470 Replace genisoimage with xorriso (dongcarl)
- migcoin/migcoin#20471 Use C++17 in depends (fanquake)
- migcoin/migcoin#20496 Drop unneeded macOS framework dependencies (hebasto)
- migcoin/migcoin#20520 Do not force Precompiled Headers (PCH) for building Qt on Linux (hebasto)
- migcoin/migcoin#20549 Support make src/migcoin-node and src/migcoin-gui (promag)
- migcoin/migcoin#20565 Ensure PIC build for bdb on Android (BlockMechanic)
- migcoin/migcoin#20594 Fix getauxval calls in randomenv.cpp (jonasschnelli)
- migcoin/migcoin#20603 Update crc32c subtree (MarcoFalke)
- migcoin/migcoin#20609 configure: output notice that test binary is disabled by fuzzing (apoelstra)
- migcoin/migcoin#20619 guix: Quality of life improvements (dongcarl)
- migcoin/migcoin#20629 Improve id string robustness (dongcarl)
- migcoin/migcoin#20641 Use Qt top-level build facilities (hebasto)
- migcoin/migcoin#20650 Drop workaround for a fixed bug in Qt build system (hebasto)
- migcoin/migcoin#20673 Use more legible qmake commands in qt package (hebasto)
- migcoin/migcoin#20684 Define .INTERMEDIATE target once only (hebasto)
- migcoin/migcoin#20720 more robustly check for fcf-protection support (fanquake)
- migcoin/migcoin#20734 Make platform-specific targets available for proper platform builds only (hebasto)
- migcoin/migcoin#20936 build fuzz tests by default (danben)
- migcoin/migcoin#20937 guix: Make nsis reproducible by respecting SOURCE-DATE-EPOCH (dongcarl)
- migcoin/migcoin#20938 fix linking against -latomic when building for riscv (fanquake)
- migcoin/migcoin#20939 fix `RELOC_SECTION` security check for migcoin-util (fanquake)
- migcoin/migcoin#20963 gitian-linux: Build binaries for 64-bit POWER (continued) (laanwj)
- migcoin/migcoin#21036 gitian: Bump descriptors to focal for 22.0 (fanquake)
- migcoin/migcoin#21045 Adds switch to enable/disable randomized base address in MSVC builds (EthanHeilman)
- migcoin/migcoin#21065 make macOS HOST in download-osx generic (fanquake)
- migcoin/migcoin#21078 guix: only download sources for hosts being built (fanquake)
- migcoin/migcoin#21116 Disable --disable-fuzz-binary for gitian/guix builds (hebasto)
- migcoin/migcoin#21182 remove mostly pointless `BOOST_PROCESS` macro (fanquake)
- migcoin/migcoin#21205 actually fail when Boost is missing (fanquake)
- migcoin/migcoin#21209 use newer source for libnatpmp (fanquake)
- migcoin/migcoin#21226 Fix fuzz binary compilation under windows (danben)
- migcoin/migcoin#21231 Add /opt/homebrew to path to look for boost libraries (fyquah)
- migcoin/migcoin#21239 guix: Add codesignature attachment support for osx+win (dongcarl)
- migcoin/migcoin#21250 Make `HAVE_O_CLOEXEC` available outside LevelDB (bugfix) (theStack)
- migcoin/migcoin#21272 guix: Passthrough `SDK_PATH` into container (dongcarl)
- migcoin/migcoin#21274 assumptions:  Assume C++17 (fanquake)
- migcoin/migcoin#21286 Bump minimum Qt version to 5.9.5 (hebasto)
- migcoin/migcoin#21298 guix: Bump time-machine, glibc, and linux-headers (dongcarl)
- migcoin/migcoin#21304 guix: Add guix-clean script + establish gc-root for container profiles (dongcarl)
- migcoin/migcoin#21320 fix libnatpmp macos cross compile (fanquake)
- migcoin/migcoin#21321 guix: Add curl to required tool list (hebasto)
- migcoin/migcoin#21333 set Unicode true for NSIS installer (fanquake)
- migcoin/migcoin#21339 Make `AM_CONDITIONAL([ENABLE_EXTERNAL_SIGNER])` unconditional (hebasto)
- migcoin/migcoin#21349 Fix fuzz-cuckoocache cross-compiling with DEBUG=1 (hebasto)
- migcoin/migcoin#21354 build, doc: Drop no longer required packages from macOS cross-compiling dependencies (hebasto)
- migcoin/migcoin#21363 build, qt: Improve Qt static plugins/libs check code (hebasto)
- migcoin/migcoin#21375 guix: Misc feedback-based fixes + hier restructuring (dongcarl)
- migcoin/migcoin#21376 Qt 5.12.10 (fanquake)
- migcoin/migcoin#21382 Clean remnants of QTBUG-34748 fix (hebasto)
- migcoin/migcoin#21400 Fix regression introduced in #21363 (hebasto)
- migcoin/migcoin#21403 set --build when configuring packages in depends (fanquake)
- migcoin/migcoin#21421 don't try and use -fstack-clash-protection on Windows (fanquake)
- migcoin/migcoin#21423 Cleanups and follow ups after bumping Qt to 5.12.10 (hebasto)
- migcoin/migcoin#21427 Fix `id_string` invocations (dongcarl)
- migcoin/migcoin#21430 Add -Werror=implicit-fallthrough compile flag (hebasto)
- migcoin/migcoin#21457 Split libtapi and clang out of `native_cctools` (fanquake)
- migcoin/migcoin#21462 guix: Add guix-{attest,verify} scripts (dongcarl)
- migcoin/migcoin#21495 build, qt: Fix static builds on macOS Big Sur (hebasto)
- migcoin/migcoin#21497 Do not opt-in unused CoreWLAN stuff in depends for macOS (hebasto)
- migcoin/migcoin#21543 Enable safe warnings for msvc builds (hebasto)
- migcoin/migcoin#21565 Make `migcoin_qt.m4` more generic (fanquake)
- migcoin/migcoin#21610 remove -Wdeprecated-register from NOWARN flags (fanquake)
- migcoin/migcoin#21613 enable -Wdocumentation (fanquake)
- migcoin/migcoin#21629 Fix configuring when building depends with `NO_BDB=1` (fanquake)
- migcoin/migcoin#21654 build, qt: Make Qt rcc output always deterministic (hebasto)
- migcoin/migcoin#21655 build, qt: No longer need to set `QT_RCC_TEST=1` for determinism (hebasto)
- migcoin/migcoin#21658 fix make deploy for arm64-darwin (sgulls)
- migcoin/migcoin#21694 Use XLIFF file to provide more context to Transifex translators (hebasto)
- migcoin/migcoin#21708, migcoin/migcoin#21593 Drop pointless sed commands (hebasto)
- migcoin/migcoin#21731 Update msvc build to use Qt5.12.10 binaries (sipsorcery)
- migcoin/migcoin#21733 Re-add command to install vcpkg (dplusplus1024)
- migcoin/migcoin#21793 Use `-isysroot` over `--sysroot` on macOS (fanquake)
- migcoin/migcoin#21869 Add missing `-D_LIBCPP_DEBUG=1` to debug flags (MarcoFalke)
- migcoin/migcoin#21889 macho: check for control flow instrumentation (fanquake)
- migcoin/migcoin#21920 Improve macro for testing -latomic requirement (MarcoFalke)
- migcoin/migcoin#21991 libevent 2.1.12-stable (fanquake)
- migcoin/migcoin#22054 Bump Qt version to 5.12.11 (hebasto)
- migcoin/migcoin#22063 Use Qt archive of the same version as the compiled binaries (hebasto)
- migcoin/migcoin#22070 Don't use cf-protection when targeting arm-apple-darwin (fanquake)
- migcoin/migcoin#22071 Latest config.guess and config.sub (fanquake)
- migcoin/migcoin#22075 guix: Misc leftover usability improvements (dongcarl)
- migcoin/migcoin#22123 Fix qt.mk for mac arm64 (promag)
- migcoin/migcoin#22174 build, qt: Fix libraries linking order for Linux hosts (hebasto)
- migcoin/migcoin#22182 guix: Overhaul how guix-{attest,verify} works and hierarchy (dongcarl)
- migcoin/migcoin#22186 build, qt: Fix compiling qt package in depends with GCC 11 (hebasto)
- migcoin/migcoin#22199 macdeploy: minor fixups and simplifications (fanquake)
- migcoin/migcoin#22230 Fix MSVC linker /SubSystem option for migcoin-qt.exe (hebasto)
- migcoin/migcoin#22234 Mark print-% target as phony (dgoncharov)
- migcoin/migcoin#22238 improve detection of eBPF support (fanquake)
- migcoin/migcoin#22258 Disable deprecated-copy warning only when external warnings are enabled (MarcoFalke)
- migcoin/migcoin#22320 set minimum required Boost to 1.64.0 (fanquake)
- migcoin/migcoin#22348 Fix cross build for Windows with Boost Process (hebasto)
- migcoin/migcoin#22365 guix: Avoid relying on newer symbols by rebasing our cross toolchains on older glibcs (dongcarl)
- migcoin/migcoin#22381 guix: Test security-check sanity before performing them (with macOS) (fanquake)
- migcoin/migcoin#22405 Remove --enable-glibc-back-compat from Guix build (fanquake)
- migcoin/migcoin#22406 Remove --enable-determinism configure option (fanquake)
- migcoin/migcoin#22410 Avoid GCC 7.1 ABI change warning in guix build (sipa)
- migcoin/migcoin#22436 use aarch64 Clang if cross-compiling for darwin on aarch64 (fanquake)
- migcoin/migcoin#22465 guix: Pin kernel-header version, time-machine to upstream 1.3.0 commit (dongcarl)
- migcoin/migcoin#22511 guix: Silence `getent(1)` invocation, doc fixups (dongcarl)
- migcoin/migcoin#22531 guix: Fixes to guix-{attest,verify} (achow101)
- migcoin/migcoin#22642 release: Release with separate sha256sums and sig files (dongcarl)
- migcoin/migcoin#22685 clientversion: No suffix `#if CLIENT_VERSION_IS_RELEASE` (dongcarl)
- migcoin/migcoin#22713 Fix build with Boost 1.77.0 (sizeofvoid)

### Tests and QA
- migcoin/migcoin#14604 Add test and refactor `feature_block.py` (sanket1729)
- migcoin/migcoin#17556 Change `feature_config_args.py` not to rely on strange regtest=0 behavior (ryanofsky)
- migcoin/migcoin#18795 wallet issue with orphaned rewards (domob1812)
- migcoin/migcoin#18847 compressor: Use a prevector in CompressScript serialization (jb55)
- migcoin/migcoin#19259 fuzz: Add fuzzing harness for LoadMempool(…) and DumpMempool(…) (practicalswift)
- migcoin/migcoin#19315 Allow outbound & block-relay-only connections in functional tests. (amitiuttarwar)
- migcoin/migcoin#19698 Apply strict verification flags for transaction tests and assert backwards compatibility (glozow)
- migcoin/migcoin#19801 Check for all possible `OP_CLTV` fail reasons in `feature_cltv.py` (BIP 65) (theStack)
- migcoin/migcoin#19893 Remove or explain syncwithvalidationinterfacequeue (MarcoFalke)
- migcoin/migcoin#19972 fuzz: Add fuzzing harness for node eviction logic (practicalswift)
- migcoin/migcoin#19982 Fix inconsistent lock order in `wallet_tests/CreateWallet` (hebasto)
- migcoin/migcoin#20000 Fix creation of "std::string"s with \0s (vasild)
- migcoin/migcoin#20047 Use `wait_for_{block,header}` helpers in `p2p_fingerprint.py` (theStack)
- migcoin/migcoin#20171 Add functional test `test_txid_inv_delay` (ariard)
- migcoin/migcoin#20189 Switch to BIP341's suggested scheme for outputs without script (sipa)
- migcoin/migcoin#20248 Fix length of R check in `key_signature_tests` (dgpv)
- migcoin/migcoin#20276, migcoin/migcoin#20385, migcoin/migcoin#20688, migcoin/migcoin#20692 Run various mempool tests even with wallet disabled (mjdietzx)
- migcoin/migcoin#20323 Create or use existing properly initialized NodeContexts (dongcarl)
- migcoin/migcoin#20354 Add `feature_taproot.py --previous_release` (MarcoFalke)
- migcoin/migcoin#20370 fuzz: Version handshake (MarcoFalke)
- migcoin/migcoin#20377 fuzz: Fill various small fuzzing gaps (practicalswift)
- migcoin/migcoin#20425 fuzz: Make CAddrMan fuzzing harness deterministic (practicalswift)
- migcoin/migcoin#20430 Sanitizers: Add suppression for unsigned-integer-overflow in libstdc++ (jonasschnelli)
- migcoin/migcoin#20437 fuzz: Avoid time-based "non-determinism" in fuzzing harnesses by using mocked GetTime() (practicalswift)
- migcoin/migcoin#20458 Add `is_bdb_compiled` helper (Sjors)
- migcoin/migcoin#20466 Fix intermittent `p2p_fingerprint` issue (MarcoFalke)
- migcoin/migcoin#20472 Add testing of ParseInt/ParseUInt edge cases with leading +/-/0:s (practicalswift)
- migcoin/migcoin#20507 sync: print proper lock order location when double lock is detected (vasild)
- migcoin/migcoin#20522 Fix sync issue in `disconnect_p2ps` (amitiuttarwar)
- migcoin/migcoin#20524 Move `MIN_VERSION_SUPPORTED` to p2p.py (jnewbery)
- migcoin/migcoin#20540 Fix `wallet_multiwallet` issue on windows (MarcoFalke)
- migcoin/migcoin#20560 fuzz: Link all targets once (MarcoFalke)
- migcoin/migcoin#20567 Add option to git-subtree-check to do full check, add help (laanwj)
- migcoin/migcoin#20569 Fix intermittent `wallet_multiwallet` issue with `got_loading_error` (MarcoFalke)
- migcoin/migcoin#20613 Use Popen.wait instead of RPC in `assert_start_raises_init_error` (MarcoFalke)
- migcoin/migcoin#20663 fuzz: Hide `script_assets_test_minimizer` (MarcoFalke)
- migcoin/migcoin#20674 fuzz: Call SendMessages after ProcessMessage to increase coverage (MarcoFalke)
- migcoin/migcoin#20683 Fix restart node race (MarcoFalke)
- migcoin/migcoin#20686 fuzz: replace CNode code with fuzz/util.h::ConsumeNode() (jonatack)
- migcoin/migcoin#20733 Inline non-member functions with body in fuzzing headers (pstratem)
- migcoin/migcoin#20737 Add missing assignment in `mempool_resurrect.py` (MarcoFalke)
- migcoin/migcoin#20745 Correct `epoll_ctl` data race suppression (hebasto)
- migcoin/migcoin#20748 Add race:SendZmqMessage tsan suppression (MarcoFalke)
- migcoin/migcoin#20760 Set correct nValue for multi-op-return policy check (MarcoFalke)
- migcoin/migcoin#20761 fuzz: Check that `NULL_DATA` is unspendable (MarcoFalke)
- migcoin/migcoin#20765 fuzz: Check that certain script TxoutType are nonstandard (mjdietzx)
- migcoin/migcoin#20772 fuzz: Bolster ExtractDestination(s) checks (mjdietzx)
- migcoin/migcoin#20789 fuzz: Rework strong and weak net enum fuzzing (MarcoFalke)
- migcoin/migcoin#20828 fuzz: Introduce CallOneOf helper to replace switch-case (MarcoFalke)
- migcoin/migcoin#20839 fuzz: Avoid extraneous copy of input data, using Span<> (MarcoFalke)
- migcoin/migcoin#20844 Add sanitizer suppressions for AMD EPYC CPUs (MarcoFalke)
- migcoin/migcoin#20857 Update documentation in `feature_csv_activation.py` (PiRK)
- migcoin/migcoin#20876 Replace getmempoolentry with testmempoolaccept in MiniWallet (MarcoFalke)
- migcoin/migcoin#20881 fuzz: net permission flags in net processing (MarcoFalke)
- migcoin/migcoin#20882 fuzz: Add missing muhash registration (MarcoFalke)
- migcoin/migcoin#20908 fuzz: Use mocktime in `process_message*` fuzz targets (MarcoFalke)
- migcoin/migcoin#20915 fuzz: Fail if message type is not fuzzed (MarcoFalke)
- migcoin/migcoin#20946 fuzz: Consolidate fuzzing TestingSetup initialization (dongcarl)
- migcoin/migcoin#20954 Declare `nodes` type `in test_framework.py` (kiminuo)
- migcoin/migcoin#20955 Fix `get_previous_releases.py` for aarch64 (MarcoFalke)
- migcoin/migcoin#20969 check that getblockfilter RPC fails without block filter index (theStack)
- migcoin/migcoin#20971 Work around libFuzzer deadlock (MarcoFalke)
- migcoin/migcoin#20993 Store subversion (user agent) as string in `msg_version` (theStack)
- migcoin/migcoin#20995 fuzz: Avoid initializing version to less than `MIN_PEER_PROTO_VERSION` (MarcoFalke)
- migcoin/migcoin#20998 Fix BlockToJsonVerbose benchmark (martinus)
- migcoin/migcoin#21003 Move MakeNoLogFileContext to `libtest_util`, and use it in bench (MarcoFalke)
- migcoin/migcoin#21008 Fix zmq test flakiness, improve speed (theStack)
- migcoin/migcoin#21023 fuzz: Disable shuffle when merge=1 (MarcoFalke)
- migcoin/migcoin#21037 fuzz: Avoid designated initialization (C++20) in fuzz tests (practicalswift)
- migcoin/migcoin#21042 doc, test: Improve `setup_clean_chain` documentation (fjahr)
- migcoin/migcoin#21080 fuzz: Configure check for main function (take 2) (MarcoFalke)
- migcoin/migcoin#21084 Fix timeout decrease in `feature_assumevalid` (brunoerg)
- migcoin/migcoin#21096 Re-add dead code detection (flack)
- migcoin/migcoin#21100 Remove unused function `xor_bytes` (theStack)
- migcoin/migcoin#21115 Fix Windows cross build (hebasto)
- migcoin/migcoin#21117 Remove `assert_blockchain_height` (MarcoFalke)
- migcoin/migcoin#21121 Small unit test improvements, including helper to make mempool transaction (amitiuttarwar)
- migcoin/migcoin#21124 Remove unnecessary assignment in bdb (brunoerg)
- migcoin/migcoin#21125 Change `BOOST_CHECK` to `BOOST_CHECK_EQUAL` for paths (kiminuo)
- migcoin/migcoin#21142, migcoin/migcoin#21512 fuzz: Add `tx_pool` fuzz target (MarcoFalke)
- migcoin/migcoin#21165 Use mocktime in `test_seed_peers` (dhruv)
- migcoin/migcoin#21169 fuzz: Add RPC interface fuzzing. Increase fuzzing coverage from 65% to 70% (practicalswift)
- migcoin/migcoin#21170 bench: Add benchmark to write json into a string (martinus)
- migcoin/migcoin#21178 Run `mempool_reorg.py` even with wallet disabled (DariusParvin)
- migcoin/migcoin#21185 fuzz: Remove expensive and redundant muhash from crypto fuzz target (MarcoFalke)
- migcoin/migcoin#21200 Speed up `rpc_blockchain.py` by removing miniwallet.generate() (MarcoFalke)
- migcoin/migcoin#21211 Move `P2WSH_OP_TRUE` to shared test library (MarcoFalke)
- migcoin/migcoin#21228 Avoid comparision of integers with different signs (jonasschnelli)
- migcoin/migcoin#21230 Fix `NODE_NETWORK_LIMITED_MIN_BLOCKS` disconnection (MarcoFalke)
- migcoin/migcoin#21252 Add missing wait for sync to `feature_blockfilterindex_prune` (MarcoFalke)
- migcoin/migcoin#21254 Avoid connecting to real network when running tests (MarcoFalke)
- migcoin/migcoin#21264 fuzz: Two scripted diff renames (MarcoFalke)
- migcoin/migcoin#21280 Bug fix in `transaction_tests` (glozow)
- migcoin/migcoin#21293 Replace accidentally placed bit-OR with logical-OR (hebasto)
- migcoin/migcoin#21297 `feature_blockfilterindex_prune.py` improvements (jonatack)
- migcoin/migcoin#21310 zmq test: fix sync-up by matching notification to generated block (theStack)
- migcoin/migcoin#21334 Additional BIP9 tests (Sjors)
- migcoin/migcoin#21338 Add functional test for anchors.dat (brunoerg)
- migcoin/migcoin#21345 Bring `p2p_leak.py` up to date (mzumsande)
- migcoin/migcoin#21357 Unconditionally check for fRelay field in test framework (jarolrod)
- migcoin/migcoin#21358 fuzz: Add missing include (`test/util/setup_common.h`) (MarcoFalke)
- migcoin/migcoin#21371 fuzz: fix gcc Woverloaded-virtual build warnings (jonatack)
- migcoin/migcoin#21373 Generate fewer blocks in `feature_nulldummy` to fix timeouts, speed up (jonatack)
- migcoin/migcoin#21390 Test improvements for UTXO set hash tests (fjahr)
- migcoin/migcoin#21410 increase `rpc_timeout` for fundrawtx `test_transaction_too_large` (jonatack)
- migcoin/migcoin#21411 add logging, reduce blocks, move `sync_all` in `wallet_` groups (jonatack)
- migcoin/migcoin#21438 Add ParseUInt8() test coverage (jonatack)
- migcoin/migcoin#21443 fuzz: Implement `fuzzed_dns_lookup_function` as a lambda (practicalswift)
- migcoin/migcoin#21445 cirrus: Use SSD cluster for speedup (MarcoFalke)
- migcoin/migcoin#21477 Add test for CNetAddr::ToString IPv6 address formatting (RFC 5952) (practicalswift)
- migcoin/migcoin#21487 fuzz: Use ConsumeWeakEnum in addrman for service flags (MarcoFalke)
- migcoin/migcoin#21488 Add ParseUInt16() unit test and fuzz coverage (jonatack)
- migcoin/migcoin#21491 test: remove duplicate assertions in util_tests (jonatack)
- migcoin/migcoin#21522 fuzz: Use PickValue where possible (MarcoFalke)
- migcoin/migcoin#21531 remove qt byteswap compattests (fanquake)
- migcoin/migcoin#21557 small cleanup in RPCNestedTests tests (fanquake)
- migcoin/migcoin#21586 Add missing suppression for signed-integer-overflow:txmempool.cpp (MarcoFalke)
- migcoin/migcoin#21592 Remove option to make TestChain100Setup non-deterministic (MarcoFalke)
- migcoin/migcoin#21597 Document `race:validation_chainstatemanager_tests` suppression (MarcoFalke)
- migcoin/migcoin#21599 Replace file level integer overflow suppression with function level suppression (practicalswift)
- migcoin/migcoin#21604 Document why no symbol names can be used for suppressions (MarcoFalke)
- migcoin/migcoin#21606 fuzz: Extend psbt fuzz target a bit (MarcoFalke)
- migcoin/migcoin#21617 fuzz: Fix uninitialized read in i2p test (MarcoFalke)
- migcoin/migcoin#21630 fuzz: split FuzzedSock interface and implementation (vasild)
- migcoin/migcoin#21634 Skip SQLite fsyncs while testing (achow101)
- migcoin/migcoin#21669 Remove spurious double lock tsan suppressions by bumping to clang-12 (MarcoFalke)
- migcoin/migcoin#21676 Use mocktime to avoid intermittent failure in `rpc_tests` (MarcoFalke)
- migcoin/migcoin#21677 fuzz: Avoid use of low file descriptor ids (which may be in use) in FuzzedSock (practicalswift)
- migcoin/migcoin#21678 Fix TestPotentialDeadLockDetected suppression (hebasto)
- migcoin/migcoin#21689 Remove intermittently failing and not very meaningful `BOOST_CHECK` in `cnetaddr_basic` (practicalswift)
- migcoin/migcoin#21691 Check that no versionbits are re-used (MarcoFalke)
- migcoin/migcoin#21707 Extend functional tests for addr relay (mzumsande)
- migcoin/migcoin#21712 Test default `include_mempool` value of gettxout (promag)
- migcoin/migcoin#21738 Use clang-12 for ASAN, Add missing suppression (MarcoFalke)
- migcoin/migcoin#21740 add new python linter to check file names and permissions (windsok)
- migcoin/migcoin#21749 Bump shellcheck version (hebasto)
- migcoin/migcoin#21754 Run `feature_cltv` with MiniWallet (MarcoFalke)
- migcoin/migcoin#21762 Speed up `mempool_spend_coinbase.py` (MarcoFalke)
- migcoin/migcoin#21773 fuzz: Ensure prevout is consensus-valid (MarcoFalke)
- migcoin/migcoin#21777 Fix `feature_notifications.py` intermittent issue (MarcoFalke)
- migcoin/migcoin#21785 Fix intermittent issue in `p2p_addr_relay.py` (MarcoFalke)
- migcoin/migcoin#21787 Fix off-by-ones in `rpc_fundrawtransaction` assertions (jonatack)
- migcoin/migcoin#21792 Fix intermittent issue in `p2p_segwit.py` (MarcoFalke)
- migcoin/migcoin#21795 fuzz: Terminate immediately if a fuzzing harness tries to perform a DNS lookup (belt and suspenders) (practicalswift)
- migcoin/migcoin#21798 fuzz: Create a block template in `tx_pool` targets (MarcoFalke)
- migcoin/migcoin#21804 Speed up `p2p_segwit.py` (jnewbery)
- migcoin/migcoin#21810 fuzz: Various RPC fuzzer follow-ups (practicalswift)
- migcoin/migcoin#21814 Fix `feature_config_args.py` intermittent issue (MarcoFalke)
- migcoin/migcoin#21821 Add missing test for empty P2WSH redeem (MarcoFalke)
- migcoin/migcoin#21822 Resolve bug in `interface_migcoin_cli.py` (klementtan)
- migcoin/migcoin#21846 fuzz: Add `-fsanitize=integer` suppression needed for RPC fuzzer (`generateblock`) (practicalswift)
- migcoin/migcoin#21849 fuzz: Limit toxic test globals to their respective scope (MarcoFalke)
- migcoin/migcoin#21867 use MiniWallet for `p2p_blocksonly.py` (theStack)
- migcoin/migcoin#21873 minor fixes & improvements for files linter test (windsok)
- migcoin/migcoin#21874 fuzz: Add `WRITE_ALL_FUZZ_TARGETS_AND_ABORT` (MarcoFalke)
- migcoin/migcoin#21884 fuzz: Remove unused --enable-danger-fuzz-link-all option (MarcoFalke)
- migcoin/migcoin#21890 fuzz: Limit ParseISO8601DateTime fuzzing to 32-bit (MarcoFalke)
- migcoin/migcoin#21891 fuzz: Remove strprintf test cases that are known to fail (MarcoFalke)
- migcoin/migcoin#21892 fuzz: Avoid excessively large min fee rate in `tx_pool` (MarcoFalke)
- migcoin/migcoin#21895 Add TSA annotations to the WorkQueue class members (hebasto)
- migcoin/migcoin#21900 use MiniWallet for `feature_csv_activation.py` (theStack)
- migcoin/migcoin#21909 fuzz: Limit max insertions in timedata fuzz test (MarcoFalke)
- migcoin/migcoin#21922 fuzz: Avoid timeout in EncodeBase58 (MarcoFalke)
- migcoin/migcoin#21927 fuzz: Run const CScript member functions only once (MarcoFalke)
- migcoin/migcoin#21929 fuzz: Remove incorrect float round-trip serialization test (MarcoFalke)
- migcoin/migcoin#21936 fuzz: Terminate immediately if a fuzzing harness tries to create a TCP socket (belt and suspenders) (practicalswift)
- migcoin/migcoin#21941 fuzz: Call const member functions in addrman fuzz test only once (MarcoFalke)
- migcoin/migcoin#21945 add P2PK support to MiniWallet (theStack)
- migcoin/migcoin#21948 Fix off-by-one in mockscheduler test RPC (MarcoFalke)
- migcoin/migcoin#21953 fuzz: Add `utxo_snapshot` target (MarcoFalke)
- migcoin/migcoin#21970 fuzz: Add missing CheckTransaction before CheckTxInputs (MarcoFalke)
- migcoin/migcoin#21989 Use `COINBASE_MATURITY` in functional tests (kiminuo)
- migcoin/migcoin#22003 Add thread safety annotations (ajtowns)
- migcoin/migcoin#22004 fuzz: Speed up transaction fuzz target (MarcoFalke)
- migcoin/migcoin#22005 fuzz: Speed up banman fuzz target (MarcoFalke)
- migcoin/migcoin#22029 [fuzz] Improve transport deserialization fuzz test coverage (dhruv)
- migcoin/migcoin#22048 MiniWallet: introduce enum type for output mode (theStack)
- migcoin/migcoin#22057 use MiniWallet (P2PK mode) for `feature_dersig.py` (theStack)
- migcoin/migcoin#22065 Mark `CheckTxInputs` `[[nodiscard]]`. Avoid UUM in fuzzing harness `coins_view` (practicalswift)
- migcoin/migcoin#22069 fuzz: don't try and use fopencookie() when building for Android (fanquake)
- migcoin/migcoin#22082 update nanobench from release 4.0.0 to 4.3.4 (martinus)
- migcoin/migcoin#22086 remove BasicTestingSetup from unit tests that don't need it (fanquake)
- migcoin/migcoin#22089 MiniWallet: fix fee calculation for P2PK and check tx vsize (theStack)
- migcoin/migcoin#21107, migcoin/migcoin#22092 Convert documentation into type annotations (fanquake)
- migcoin/migcoin#22095 Additional BIP32 test vector for hardened derivation with leading zeros (kristapsk)
- migcoin/migcoin#22103 Fix IPv6 check on BSD systems (n-thumann)
- migcoin/migcoin#22118 check anchors.dat when node starts for the first time (brunoerg)
- migcoin/migcoin#22120 `p2p_invalid_block`: Check that a block rejected due to too-new tim… (willcl-ark)
- migcoin/migcoin#22153 Fix `p2p_leak.py` intermittent failure (mzumsande)
- migcoin/migcoin#22169 p2p, rpc, fuzz: various tiny follow-ups (jonatack)
- migcoin/migcoin#22176 Correct outstanding -Werror=sign-compare errors (Empact)
- migcoin/migcoin#22180 fuzz: Increase branch coverage of the float fuzz target (MarcoFalke)
- migcoin/migcoin#22187 Add `sync_blocks` in `wallet_orphanedreward.py` (domob1812)
- migcoin/migcoin#22201 Fix TestShell to allow running in Jupyter Notebook (josibake)
- migcoin/migcoin#22202 Add temporary coinstats suppressions (MarcoFalke)
- migcoin/migcoin#22203 Use ConnmanTestMsg from test lib in `denialofservice_tests` (MarcoFalke)
- migcoin/migcoin#22210 Use MiniWallet in `test_no_inherited_signaling` RBF test (MarcoFalke)
- migcoin/migcoin#22224 Update msvc and appveyor builds to use Qt5.12.11 binaries (sipsorcery)
- migcoin/migcoin#22249 Kill process group to avoid dangling processes when using `--failfast` (S3RK)
- migcoin/migcoin#22267 fuzz: Speed up crypto fuzz target (MarcoFalke)
- migcoin/migcoin#22270 Add migcoin-util tests (+refactors) (MarcoFalke)
- migcoin/migcoin#22271 fuzz: Assert roundtrip equality for `CPubKey` (theStack)
- migcoin/migcoin#22279 fuzz: add missing ECCVerifyHandle to `base_encode_decode` (apoelstra)
- migcoin/migcoin#22292 bench, doc: benchmarking updates and fixups (jonatack)
- migcoin/migcoin#22306 Improvements to `p2p_addr_relay.py` (amitiuttarwar)
- migcoin/migcoin#22310 Add functional test for replacement relay fee check (ariard)
- migcoin/migcoin#22311 Add missing syncwithvalidationinterfacequeue in `p2p_blockfilters` (MarcoFalke)
- migcoin/migcoin#22313 Add missing `sync_all` to `feature_coinstatsindex` (MarcoFalke)
- migcoin/migcoin#22322 fuzz: Check banman roundtrip (MarcoFalke)
- migcoin/migcoin#22363 Use `script_util` helpers for creating P2{PKH,SH,WPKH,WSH} scripts (theStack)
- migcoin/migcoin#22399 fuzz: Rework CTxDestination fuzzing (MarcoFalke)
- migcoin/migcoin#22408 add tests for `bad-txns-prevout-null` reject reason (theStack)
- migcoin/migcoin#22445 fuzz: Move implementations of non-template fuzz helpers from util.h to util.cpp (sriramdvt)
- migcoin/migcoin#22446 Fix `wallet_listdescriptors.py` if bdb is not compiled (hebasto)
- migcoin/migcoin#22447 Whitelist `rpc_rawtransaction` peers to speed up tests (jonatack)
- migcoin/migcoin#22742 Use proper target in `do_fund_send` (S3RK)

### Miscellaneous
- migcoin/migcoin#19337 sync: Detect double lock from the same thread (vasild)
- migcoin/migcoin#19809 log: Prefix log messages with function name and source code location if -logsourcelocations is set (practicalswift)
- migcoin/migcoin#19866 eBPF Linux tracepoints (jb55)
- migcoin/migcoin#20024 init: Fix incorrect warning "Reducing -maxconnections from N to N-1, because of system limitations" (practicalswift)
- migcoin/migcoin#20145 contrib: Add getcoins.py script to get coins from (signet) faucet (kallewoof)
- migcoin/migcoin#20255 util: Add assume() identity function (MarcoFalke)
- migcoin/migcoin#20288 script, doc: Contrib/seeds updates (jonatack)
- migcoin/migcoin#20358 src/randomenv.cpp: Fix build on uclibc (ffontaine)
- migcoin/migcoin#20406 util: Avoid invalid integer negation in formatmoney and valuefromamount (practicalswift)
- migcoin/migcoin#20434 contrib: Parse elf directly for symbol and security checks (laanwj)
- migcoin/migcoin#20451 lint: Run mypy over contrib/devtools (fanquake)
- migcoin/migcoin#20476 contrib: Add test for elf symbol-check (laanwj)
- migcoin/migcoin#20530 lint: Update cppcheck linter to c++17 and improve explicit usage (fjahr)
- migcoin/migcoin#20589 log: Clarify that failure to read/write `fee_estimates.dat` is non-fatal (MarcoFalke)
- migcoin/migcoin#20602 util: Allow use of c++14 chrono literals (MarcoFalke)
- migcoin/migcoin#20605 init: Signal-safe instant shutdown (laanwj)
- migcoin/migcoin#20608 contrib: Add symbol check test for PE binaries (fanquake)
- migcoin/migcoin#20689 contrib: Replace binary verification script verify.sh with python rewrite (theStack)
- migcoin/migcoin#20715 util: Add argsmanager::getcommand() and use it in migcoin-wallet (MarcoFalke)
- migcoin/migcoin#20735 script: Remove outdated extract-osx-sdk.sh (hebasto)
- migcoin/migcoin#20817 lint: Update list of spelling linter false positives, bump to codespell 2.0.0 (theStack)
- migcoin/migcoin#20884 script: Improve robustness of migcoind.service on startup (hebasto)
- migcoin/migcoin#20906 contrib: Embed c++11 patch in `install_db4.sh` (gruve-p)
- migcoin/migcoin#21004 contrib: Fix docker args conditional in gitian-build (setpill)
- migcoin/migcoin#21007 migcoind: Add -daemonwait option to wait for initialization (laanwj)
- migcoin/migcoin#21041 log: Move "Pre-allocating up to position 0x[…] in […].dat" log message to debug category (practicalswift)
- migcoin/migcoin#21059 Drop boost/preprocessor dependencies (hebasto)
- migcoin/migcoin#21087 guix: Passthrough `BASE_CACHE` into container (dongcarl)
- migcoin/migcoin#21088 guix: Jump forwards in time-machine and adapt (dongcarl)
- migcoin/migcoin#21089 guix: Add support for powerpc64{,le} (dongcarl)
- migcoin/migcoin#21110 util: Remove boost `posix_time` usage from `gettime*` (fanquake)
- migcoin/migcoin#21111 Improve OpenRC initscript (parazyd)
- migcoin/migcoin#21123 code style: Add EditorConfig file (kiminuo)
- migcoin/migcoin#21173 util: Faster hexstr => 13% faster blocktojson (martinus)
- migcoin/migcoin#21221 tools: Allow argument/parameter bin packing in clang-format (jnewbery)
- migcoin/migcoin#21244 Move GetDataDir to ArgsManager (kiminuo)
- migcoin/migcoin#21255 contrib: Run test-symbol-check for risc-v (fanquake)
- migcoin/migcoin#21271 guix: Explicitly set umask in build container (dongcarl)
- migcoin/migcoin#21300 script: Add explanatory comment to tc.sh (dscotese)
- migcoin/migcoin#21317 util: Make assume() usable as unary expression (MarcoFalke)
- migcoin/migcoin#21336 Make .gitignore ignore src/test/fuzz/fuzz.exe (hebasto)
- migcoin/migcoin#21337 guix: Update darwin native packages dependencies (hebasto)
- migcoin/migcoin#21405 compat: remove memcpy -> memmove backwards compatibility alias (fanquake)
- migcoin/migcoin#21418 contrib: Make systemd invoke dependencies only when ready (laanwj)
- migcoin/migcoin#21447 Always add -daemonwait to known command line arguments (hebasto)
- migcoin/migcoin#21471 bugfix: Fix `bech32_encode` calls in `gen_key_io_test_vectors.py` (sipa)
- migcoin/migcoin#21615 script: Add trusted key for hebasto (hebasto)
- migcoin/migcoin#21664 contrib: Use lief for macos and windows symbol & security checks (fanquake)
- migcoin/migcoin#21695 contrib: Remove no longer used contrib/migcoin-qt.pro (hebasto)
- migcoin/migcoin#21711 guix: Add full installation and usage documentation (dongcarl)
- migcoin/migcoin#21799 guix: Use `gcc-8` across the board (dongcarl)
- migcoin/migcoin#21802 Avoid UB in util/asmap (advance a dereferenceable iterator outside its valid range) (MarcoFalke)
- migcoin/migcoin#21823 script: Update reviewers (jonatack)
- migcoin/migcoin#21850 Remove `GetDataDir(net_specific)` function (kiminuo)
- migcoin/migcoin#21871 scripts: Add checks for minimum required os versions (fanquake)
- migcoin/migcoin#21966 Remove double serialization; use software encoder for fee estimation (sipa)
- migcoin/migcoin#22060 contrib: Add torv3 seed nodes for testnet, drop v2 ones (laanwj)
- migcoin/migcoin#22244 devtools: Correctly extract symbol versions in symbol-check (laanwj)
- migcoin/migcoin#22533 guix/build: Remove vestigial SKIPATTEST.TAG (dongcarl)
- migcoin/migcoin#22643 guix-verify: Non-zero exit code when anything fails (dongcarl)
- migcoin/migcoin#22654 guix: Don't include directory name in SHA256SUMS (achow101)

### Documentation
- migcoin/migcoin#15451 clarify getdata limit after #14897 (HashUnlimited)
- migcoin/migcoin#15545 Explain why CheckBlock() is called before AcceptBlock (Sjors)
- migcoin/migcoin#17350 Add developer documentation to isminetype (HAOYUatHZ)
- migcoin/migcoin#17934 Use `CONFIG_SITE` variable instead of --prefix option (hebasto)
- migcoin/migcoin#18030 Coin::IsSpent() can also mean never existed (Sjors)
- migcoin/migcoin#18096 IsFinalTx comment about nSequence & `OP_CLTV` (nothingmuch)
- migcoin/migcoin#18568 Clarify developer notes about constant naming (ryanofsky)
- migcoin/migcoin#19961 doc: tor.md updates (jonatack)
- migcoin/migcoin#19968 Clarify CRollingBloomFilter size estimate (robot-dreams)
- migcoin/migcoin#20200 Rename CODEOWNERS to REVIEWERS (adamjonas)
- migcoin/migcoin#20329 docs/descriptors.md: Remove hardened marker in the path after xpub (dgpv)
- migcoin/migcoin#20380 Add instructions on how to fuzz the P2P layer using Honggfuzz NetDriver (practicalswift)
- migcoin/migcoin#20414 Remove generated manual pages from master branch (laanwj)
- migcoin/migcoin#20473 Document current boost dependency as 1.71.0 (laanwj)
- migcoin/migcoin#20512 Add bash as an OpenBSD dependency (emilengler)
- migcoin/migcoin#20568 Use FeeModes doc helper in estimatesmartfee (MarcoFalke)
- migcoin/migcoin#20577 libconsensus: add missing error code description, fix NMigcoin link (theStack)
- migcoin/migcoin#20587 Tidy up Tor doc (more stringent) (wodry)
- migcoin/migcoin#20592 Update wtxidrelay documentation per BIP339 (jonatack)
- migcoin/migcoin#20601 Update for FreeBSD 12.2, add GUI Build Instructions (jarolrod)
- migcoin/migcoin#20635 fix misleading comment about call to non-existing function (pox)
- migcoin/migcoin#20646 Refer to BIPs 339/155 in feature negotiation (jonatack)
- migcoin/migcoin#20653 Move addr relay comment in net to correct place (MarcoFalke)
- migcoin/migcoin#20677 Remove shouty enums in `net_processing` comments (sdaftuar)
- migcoin/migcoin#20741 Update 'Secure string handling' (prayank23)
- migcoin/migcoin#20757 tor.md and -onlynet help updates (jonatack)
- migcoin/migcoin#20829 Add -netinfo help (jonatack)
- migcoin/migcoin#20830 Update developer notes with signet (jonatack)
- migcoin/migcoin#20890 Add explicit macdeployqtplus dependencies install step (hebasto)
- migcoin/migcoin#20913 Add manual page generation for migcoin-util (laanwj)
- migcoin/migcoin#20985 Add xorriso to macOS depends packages (fanquake)
- migcoin/migcoin#20986 Update developer notes to discourage very long lines (jnewbery)
- migcoin/migcoin#20987 Add instructions for generating RPC docs (ben-kaufman)
- migcoin/migcoin#21026 Document use of make-tag script to make tags (laanwj)
- migcoin/migcoin#21028 doc/bips: Add BIPs 43, 44, 49, and 84 (luke-jr)
- migcoin/migcoin#21049 Add release notes for listdescriptors RPC (S3RK)
- migcoin/migcoin#21060 More precise -debug and -debugexclude doc (wodry)
- migcoin/migcoin#21077 Clarify -timeout and -peertimeout config options (glozow)
- migcoin/migcoin#21105 Correctly identify script type (niftynei)
- migcoin/migcoin#21163 Guix is shipped in Debian and Ubuntu (MarcoFalke)
- migcoin/migcoin#21210 Rework internal and external links (MarcoFalke)
- migcoin/migcoin#21246 Correction for VerifyTaprootCommitment comments (roconnor-blockstream)
- migcoin/migcoin#21263 Clarify that squashing should happen before review (MarcoFalke)
- migcoin/migcoin#21323 guix, doc: Update default HOSTS value (hebasto)
- migcoin/migcoin#21324 Update build instructions for Fedora (hebasto)
- migcoin/migcoin#21343 Revamp macOS build doc (jarolrod)
- migcoin/migcoin#21346 install qt5 when building on macOS (fanquake)
- migcoin/migcoin#21384 doc: add signet to migcoin.conf documentation (jonatack)
- migcoin/migcoin#21394 Improve comment about protected peers (amitiuttarwar)
- migcoin/migcoin#21398 Update fuzzing docs for afl-clang-lto (MarcoFalke)
- migcoin/migcoin#21444 net, doc: Doxygen updates and fixes in netbase.{h,cpp} (jonatack)
- migcoin/migcoin#21481 Tell howto install clang-format on Debian/Ubuntu (wodry)
- migcoin/migcoin#21567 Fix various misleading comments (glozow)
- migcoin/migcoin#21661 Fix name of script guix-build (Emzy)
- migcoin/migcoin#21672 Remove boostrap info from `GUIX_COMMON_FLAGS` doc (fanquake)
- migcoin/migcoin#21688 Note on SDK for macOS depends cross-compile (jarolrod)
- migcoin/migcoin#21709 Update reduce-memory.md and migcoin.conf -maxconnections info (jonatack)
- migcoin/migcoin#21710 update helps for addnode rpc and -addnode/-maxconnections config options (jonatack)
- migcoin/migcoin#21752 Clarify that feerates are per virtual size (MarcoFalke)
- migcoin/migcoin#21811 Remove Visual Studio 2017 reference from readme (sipsorcery)
- migcoin/migcoin#21818 Fixup -coinstatsindex help, update migcoin.conf and files.md (jonatack)
- migcoin/migcoin#21856 add OSS-Fuzz section to fuzzing.md doc (adamjonas)
- migcoin/migcoin#21912 Remove mention of priority estimation (MarcoFalke)
- migcoin/migcoin#21925 Update bips.md for 0.21.1 (MarcoFalke)
- migcoin/migcoin#21942 improve make with parallel jobs description (klementtan)
- migcoin/migcoin#21947 Fix OSS-Fuzz links (MarcoFalke)
- migcoin/migcoin#21988 note that brew installed qt is not supported (jarolrod)
- migcoin/migcoin#22056 describe in fuzzing.md how to reproduce a CI crash (jonatack)
- migcoin/migcoin#22080 add maxuploadtarget to migcoin.conf example (jarolrod)
- migcoin/migcoin#22088 Improve note on choosing posix mingw32 (jarolrod)
- migcoin/migcoin#22109 Fix external links (IRC, …) (MarcoFalke)
- migcoin/migcoin#22121 Various validation doc fixups (MarcoFalke)
- migcoin/migcoin#22172 Update tor.md, release notes with removal of tor v2 support (jonatack)
- migcoin/migcoin#22204 Remove obsolete `okSafeMode` RPC guideline from developer notes (theStack)
- migcoin/migcoin#22208 Update `REVIEWERS` (practicalswift)
- migcoin/migcoin#22250 add basic I2P documentation (vasild)
- migcoin/migcoin#22296 Final merge of release notes snippets, mv to wiki (MarcoFalke)
- migcoin/migcoin#22335 recommend `--disable-external-signer` in OpenBSD build guide (theStack)
- migcoin/migcoin#22339 Document minimum required libc++ version (hebasto)
- migcoin/migcoin#22349 Repository IRC updates (jonatack)
- migcoin/migcoin#22360 Remove unused section from release process (MarcoFalke)
- migcoin/migcoin#22369 Add steps for Transifex to release process (jonatack)
- migcoin/migcoin#22393 Added info to migcoin.conf doc (bliotti)
- migcoin/migcoin#22402 Install Rosetta on M1-macOS for qt in depends (hebasto)
- migcoin/migcoin#22432 Fix incorrect `testmempoolaccept` doc (glozow)
- migcoin/migcoin#22648 doc, test: improve i2p/tor docs and i2p reachable unit tests (jonatack)

Credits
=======

Thanks to everyone who directly contributed to this release:

- Aaron Clauson
- Adam Jonas
- amadeuszpawlik
- Amiti Uttarwar
- Andrew Chow
- Andrew Poelstra
- Anthony Towns
- Antoine Poinsot
- Antoine Riard
- apawlik
- apitko
- Ben Carman
- Ben Woosley
- benk10
- Bezdrighin
- Block Mechanic
- Brian Liotti
- Bruno Garcia
- Carl Dong
- Christian Decker
- coinforensics
- Cory Fields
- Dan Benjamin
- Daniel Kraft
- Darius Parvin
- Dhruv Mehta
- Dmitry Goncharov
- Dmitry Petukhov
- dplusplus1024
- dscotese
- Duncan Dean
- Elle Mouton
- Elliott Jin
- Emil Engler
- Ethan Heilman
- eugene
- Evan Klitzke
- Fabian Jahr
- Fabrice Fontaine
- fanquake
- fdov
- flack
- Fotis Koutoupas
- Fu Yong Quah
- fyquah
- glozow
- Gregory Sanders
- Guido Vranken
- Gunar C. Gessner
- h
- HAOYUatHZ
- Hennadii Stepanov
- Igor Cota
- Ikko Ashimine
- Ivan Metlushko
- jackielove4u
- James O'Beirne
- Jarol Rodriguez
- Joel Klabo
- John Newbery
- Jon Atack
- Jonas Schnelli
- João Barbosa
- Josiah Baker
- Karl-Johan Alm
- Kiminuo
- Klement Tan
- Kristaps Kaupe
- Larry Ruane
- lisa neigut
- Lucas Ontivero
- Luke Dashjr
- Maayan Keshet
- MarcoFalke
- Martin Ankerl
- Martin Zumsande
- Michael Dietz
- Michael Polzer
- Michael Tidwell
- Niklas Gögge
- nthumann
- Oliver Gugger
- parazyd
- Patrick Strateman
- Pavol Rusnak
- Peter Bushnell
- Pierre K
- Pieter Wuille
- PiRK
- pox
- practicalswift
- Prayank
- R E Broadley
- Rafael Sadowski
- randymcmillan
- Raul Siles
- Riccardo Spagni
- Russell O'Connor
- Russell Yanofsky
- S3RK
- saibato
- Samuel Dobson
- sanket1729
- Sawyer Billings
- Sebastian Falbesoner
- setpill
- sgulls
- sinetek
- Sjors Provoost
- Sriram
- Stephan Oeste
- Suhas Daftuar
- Sylvain Goumy
- t-bast
- Troy Giorshev
- Tushar Singla
- Tyler Chambers
- Uplab
- Vasil Dimov
- W. J. van der Laan
- willcl-ark
- William Bright
- William Casarin
- windsok
- wodry
- Yerzhan Mazhkenov
- Yuval Kogman
- Zero

As well as to everyone that helped with translations on
[Transifex](https://www.transifex.com/migcoin/migcoin/).
