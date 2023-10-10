# Libraries

| Name                     | Description |
|--------------------------|-------------|
| *libmigcoin_cli*         | RPC client functionality used by *migcoin-cli* executable |
| *libmigcoin_common*      | Home for common functionality shared by different executables and libraries. Similar to *libmigcoin_util*, but higher-level (see [Dependencies](#dependencies)). |
| *libmigcoin_consensus*   | Stable, backwards-compatible consensus functionality used by *libmigcoin_node* and *libmigcoin_wallet* and also exposed as a [shared library](../shared-libraries.md). |
| *libmigcoinconsensus*    | Shared library build of static *libmigcoin_consensus* library |
| *libmigcoin_kernel*      | Consensus engine and support library used for validation by *libmigcoin_node* and also exposed as a [shared library](../shared-libraries.md). |
| *libmigcoinqt*           | GUI functionality used by *migcoin-qt* and *migcoin-gui* executables |
| *libmigcoin_ipc*         | IPC functionality used by *migcoin-node*, *migcoin-wallet*, *migcoin-gui* executables to communicate when [`--enable-multiprocess`](multiprocess.md) is used. |
| *libmigcoin_node*        | P2P and RPC server functionality used by *migcoind* and *migcoin-qt* executables. |
| *libmigcoin_util*        | Home for common functionality shared by different executables and libraries. Similar to *libmigcoin_common*, but lower-level (see [Dependencies](#dependencies)). |
| *libmigcoin_wallet*      | Wallet functionality used by *migcoind* and *migcoin-wallet* executables. |
| *libmigcoin_wallet_tool* | Lower-level wallet functionality used by *migcoin-wallet* executable. |
| *libmigcoin_zmq*         | [ZeroMQ](../zmq.md) functionality used by *migcoind* and *migcoin-qt* executables. |

## Conventions

- Most libraries are internal libraries and have APIs which are completely unstable! There are few or no restrictions on backwards compatibility or rules about external dependencies. Exceptions are *libmigcoin_consensus* and *libmigcoin_kernel* which have external interfaces documented at [../shared-libraries.md](../shared-libraries.md).

- Generally each library should have a corresponding source directory and namespace. Source code organization is a work in progress, so it is true that some namespaces are applied inconsistently, and if you look at [`libmigcoin_*_SOURCES`](../../src/Makefile.am) lists you can see that many libraries pull in files from outside their source directory. But when working with libraries, it is good to follow a consistent pattern like:

  - *libmigcoin_node* code lives in `src/node/` in the `node::` namespace
  - *libmigcoin_wallet* code lives in `src/wallet/` in the `wallet::` namespace
  - *libmigcoin_ipc* code lives in `src/ipc/` in the `ipc::` namespace
  - *libmigcoin_util* code lives in `src/util/` in the `util::` namespace
  - *libmigcoin_consensus* code lives in `src/consensus/` in the `Consensus::` namespace

## Dependencies

- Libraries should minimize what other libraries they depend on, and only reference symbols following the arrows shown in the dependency graph below:

<table><tr><td>

```mermaid

%%{ init : { "flowchart" : { "curve" : "basis" }}}%%

graph TD;

migcoin-cli[migcoin-cli]-->libmigcoin_cli;

migcoind[migcoind]-->libmigcoin_node;
migcoind[migcoind]-->libmigcoin_wallet;

migcoin-qt[migcoin-qt]-->libmigcoin_node;
migcoin-qt[migcoin-qt]-->libmigcoinqt;
migcoin-qt[migcoin-qt]-->libmigcoin_wallet;

migcoin-wallet[migcoin-wallet]-->libmigcoin_wallet;
migcoin-wallet[migcoin-wallet]-->libmigcoin_wallet_tool;

libmigcoin_cli-->libmigcoin_util;
libmigcoin_cli-->libmigcoin_common;

libmigcoin_common-->libmigcoin_consensus;
libmigcoin_common-->libmigcoin_util;

libmigcoin_kernel-->libmigcoin_consensus;
libmigcoin_kernel-->libmigcoin_util;

libmigcoin_node-->libmigcoin_consensus;
libmigcoin_node-->libmigcoin_kernel;
libmigcoin_node-->libmigcoin_common;
libmigcoin_node-->libmigcoin_util;

libmigcoinqt-->libmigcoin_common;
libmigcoinqt-->libmigcoin_util;

libmigcoin_wallet-->libmigcoin_common;
libmigcoin_wallet-->libmigcoin_util;

libmigcoin_wallet_tool-->libmigcoin_wallet;
libmigcoin_wallet_tool-->libmigcoin_util;

classDef bold stroke-width:2px, font-weight:bold, font-size: smaller;
class migcoin-qt,migcoind,migcoin-cli,migcoin-wallet bold
```
</td></tr><tr><td>

**Dependency graph**. Arrows show linker symbol dependencies. *Consensus* lib depends on nothing. *Util* lib is depended on by everything. *Kernel* lib depends only on consensus and util.

</td></tr></table>

- The graph shows what _linker symbols_ (functions and variables) from each library other libraries can call and reference directly, but it is not a call graph. For example, there is no arrow connecting *libmigcoin_wallet* and *libmigcoin_node* libraries, because these libraries are intended to be modular and not depend on each other's internal implementation details. But wallet code is still able to call node code indirectly through the `interfaces::Chain` abstract class in [`interfaces/chain.h`](../../src/interfaces/chain.h) and node code calls wallet code through the `interfaces::ChainClient` and `interfaces::Chain::Notifications` abstract classes in the same file. In general, defining abstract classes in [`src/interfaces/`](../../src/interfaces/) can be a convenient way of avoiding unwanted direct dependencies or circular dependencies between libraries.

- *libmigcoin_consensus* should be a standalone dependency that any library can depend on, and it should not depend on any other libraries itself.

- *libmigcoin_util* should also be a standalone dependency that any library can depend on, and it should not depend on other internal libraries.

- *libmigcoin_common* should serve a similar function as *libmigcoin_util* and be a place for miscellaneous code used by various daemon, GUI, and CLI applications and libraries to live. It should not depend on anything other than *libmigcoin_util* and *libmigcoin_consensus*. The boundary between _util_ and _common_ is a little fuzzy but historically _util_ has been used for more generic, lower-level things like parsing hex, and _common_ has been used for migcoin-specific, higher-level things like parsing base58. The difference between util and common is mostly important because *libmigcoin_kernel* is not supposed to depend on *libmigcoin_common*, only *libmigcoin_util*. In general, if it is ever unclear whether it is better to add code to *util* or *common*, it is probably better to add it to *common* unless it is very generically useful or useful particularly to include in the kernel.


- *libmigcoin_kernel* should only depend on *libmigcoin_util* and *libmigcoin_consensus*.

- The only thing that should depend on *libmigcoin_kernel* internally should be *libmigcoin_node*. GUI and wallet libraries *libmigcoinqt* and *libmigcoin_wallet* in particular should not depend on *libmigcoin_kernel* and the unneeded functionality it would pull in, like block validation. To the extent that GUI and wallet code need scripting and signing functionality, they should be get able it from *libmigcoin_consensus*, *libmigcoin_common*, and *libmigcoin_util*, instead of *libmigcoin_kernel*.

- GUI, node, and wallet code internal implementations should all be independent of each other, and the *libmigcoinqt*, *libmigcoin_node*, *libmigcoin_wallet* libraries should never reference each other's symbols. They should only call each other through [`src/interfaces/`](`../../src/interfaces/`) abstract interfaces.

## Work in progress

- Validation code is moving from *libmigcoin_node* to *libmigcoin_kernel* as part of [The libmigcoinkernel Project #24303](https://github.com/migcoin/migcoin/issues/24303)
- Source code organization is discussed in general in [Library source code organization #15732](https://github.com/migcoin/migcoin/issues/15732)
