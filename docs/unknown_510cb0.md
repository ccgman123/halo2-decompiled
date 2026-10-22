# One 0xdcd8-byte object at 0x510cb0, in many repo pieces (unknown_510cb0)

Retail data covered: `0x510cb0`–`0x51e987`, 0xdcd8 bytes of `.data`. This
document is about data, not one function. It is offered to lane H
(`0x190000`–`0x19ffff`, issue #9), two of whose drafts are disabled because of
it (`src/unknown_1967d0.cpp`, in the `#if 0` block at `:1388`–`:1634`):

- `0x195a40` (1,100 bytes, `todo`, no source in `config/functions.csv`):
  "Disabled: the contiguous 0xdcd8-byte results object still has split shared
  bindings" (`:1527`–`1528`).
- `refresh_results_195ed0`, a second draft of `0x195ed0` (1,210 bytes,
  `todo`): "Disabled: the contiguous results object still has split shared
  bindings" (`:1436`). The live draft of `0x195ed0` (`:916`) is compiled and
  uses the split names.

In short:

1. Retail treats `0x510cb0`–`0x51e987` as **one object**. `0x195a40` clears it
   with one `rep stosd`, six retail functions load its base `0x510cb0` as an
   immediate, and the matched `function_b3e90` packs all of it (see
   [Boundary](#boundary)).
2. The repo **already has a type for the whole object**, `s_grs_source`
   (`src/unknown_1c5e10.cpp:596`), with a field at every offset retail uses.
   But it **defines the object as thirteen globals in four files**, plus five
   `#define`d names into them, and reads it through eight more partial views
   (see [The repo's pieces](#the-repos-pieces)).
3. One piece disagrees with retail's layout: `g_511000` starts 0x20 bytes
   inside a copied string and its records 4 bytes early. And `g_510cb0` is a
   `byte` whose address is passed as the whole object (see
   [Findings](#findings)).
4. Every piece is at the right address, so one object of `s_grs_source`'s
   shape, 4 bytes longer, could replace them, with the existing names as
   `#define`s (see [A possible binding](#a-possible-binding)).

**Analysis only:** this document adds no source, and nothing in it has been
built or checked against retail with the original compiler. Names are
provisional; offsets are relative to `0x510cb0` unless an absolute address is
given.

## Boundary

- **The clear.** `0x195a40` begins `xor eax, eax; mov ecx, 0x3736;
  mov edi, 0x510cb0; rep stosd` (`0x195a44`–`0x195a50`): 0x3736 dwords, 0xdcd8
  bytes, to `0x51e987`.
- **Before it** lie four separate globals that the same function writes on
  their own: `0x510ca0` and `0x510ca1` (bytes, `0x195e6f`, `0x195e76`),
  `0x510ca4` (`0x195a53`; the sample count, `src/unknown_196d20.cpp:20`) and
  `0x510ca8` (`0x195a5d`, the result of the `time` call at `0x195a58`). They
  are not cleared, and nothing in `0x510cac`–`0x510caf` is referenced.
- **After it**, `0x51e98c` is `real g_51e98c` (`src/unknown_1a1c3e.cpp:140`,
  written at `:499` and `:501`). `0x195ed0` (`0x19637a`) and `0x1994a0`
  (`0x1994b9`) compare a pointer with `0x51e98c` only as the end of a loop over
  the address entries' `+8` byte (see the table). Nothing references
  `0x51e984`–`0x51e98b`.
- **The base as a pointer.** Retail loads `0x510cb0` as an immediate in
  `0x195a40` (`0x195a4b`, `0x195ba1`, `0x195bb6`), `0x195ed0` (`0x1962f6`),
  `0x196390` (`0x1963a4`, `0x1963bc`, `0x1963e3`, `0x196415`), `0x196430`
  (`0x196431`, `0x19645d`), `0x196470` (`0x196766`) and `0x199460`
  (`0x199461`). The repo's sources pass `&g_510cb0` at
  `src/unknown_1967d0.cpp:989` (in the `todo` `0x195ed0`) and at `:1037`,
  `:1042`, `:1046`, `:1057`, `:1105`, `:1133`, `:1139` and `:1320` (in the
  matched `0x196390`, `0x196430`, `0x196470` and `0x199460`).
- **The whole object packed.** `function_b3e90` (matched,
  `src/unknown_0b3d30.cpp:474`–`479`) receives `&g_510cb0` (`:1057`), passes it
  to `packed_grs_write` as `s_grs_source const *` and sends the packed record
  as `"application/x-halo2pcr"`.

## The layout

From every retail instruction whose absolute address operand falls in the
block (210 references in 47 functions, 41 of them `matched`; see
[Evidence](#evidence)), and from `0x195a40`'s copies. "Global" is the repo's
defined global or `#define` at that address; every row also has an
`s_grs_source` field at the same offset (listed after the table).

| Offset | Address | Size | What it holds | Retail evidence | Global |
| --- | --- | --- | --- | --- | --- |
| `+0` | `0x510cb0` | 1 | Valid flag; set last by `0x195a40` | `mov byte ptr [0x510cb0], 1` at `0x195e60` | `g_510cb0` (`src/globals.cpp:39`) |
| `+1` | `0x510cb1` | 1 | Final flag | referenced by 26 functions; written by `0x196390` (`0x196395`) | `g_510cb1` (`src/globals.cpp:40`) |
| `+2` | `0x510cb2` | 1 | A flag | 2 functions | `g_510cb2` (`src/unknown_1967d0.cpp:835`) |
| `+3` | `0x510cb3` | 1 | Not referenced | — | — |
| `+4` | `0x510cb4` | 4 | The options' `+0x138`, which `function_192e60` takes | `0x195aa2` | — |
| `+8` | `0x510cb8` | 9 × 0x20 | Nine UTF-16 strings of 16 characters, from the variant's nine 0x10-byte strings at `+0xc` | the loop `0x195b07`–`0x195b25` (`function_140440`, end `0x510dd8`) | — |
| `+0x128` | `0x510dd8` | 3 × 1 | Three flags from the variant's first dword (5, 2 or 4; 5, 3 or 4; 5) | `0x195ade`, `0x195afe`, `0x195abe` | — |
| `+0x12b` | `0x510ddb` | 1 | Team mode: `+0x180` bit 0 | `0x195be1`; written also by `0x195ed0` (`0x195f22`) | `g_510ddb` (`src/unknown_1967d0.cpp:850`) |
| `+0x12c` | `0x510ddc` | 4 | Not referenced | — | — |
| `+0x130` | `0x510de0` | 4 | The options' `id_a` (`+0x10`) | `0x195a89`, `0x195b2a` | `g_510de0` (`src/unknown_1967d0.cpp:836`) |
| `+0x134` | `0x510de4` | 4 | The options' `id_b` (`+0x14`) | `0x195a92`, `0x195b33` | `g_510de4` (`:837`) |
| `+0x138` | `0x510de8` | 0x130 | A copy of the options' `+0x13c`–`+0x26b` | `rep movsd`, `ecx` = 0x4c, `0x195b38`–`0x195b48` | — |
| `+0x17c` | `0x510e2c` | 4 | (in that copy) the options' `mode_180` | read at `0x196aa6` (in `0x196a90`) | `g_510e2c` (`src/globals.cpp:41`) |
| `+0x180` | `0x510e30` | 4 | (in that copy) the options' `flags184` | `0x195bda` | `g_510e30` (`src/globals.cpp:42`) |
| `+0x268` | `0x510f18` | 4 | The options' `position_b` (`+0x20`) | `0x195b4d` | — |
| `+0x26c` | `0x510f1c` | 0x104 | A copy of the options' `+0x24`–`+0x127` (`name[0x100]` and 4 bytes) | `rep movsd`, `ecx` = 0x41, `0x195b53`–`0x195b60` | inside `g_511000` from `0x511000` (see [Findings](#findings)) |
| `+0x370` | `0x511020` | 8 | A flag (byte) and a dword | 4 and 6 functions | `g_511020` (`include/input_record.h:136`) |
| `+0x378` | `0x511028` | 8 | A flag (byte) and a dword | 3 functions | `g_511028` (`include/input_record.h:137`) |
| `+0x380` | `0x511030` | 4 | Bytes `+0x380`–`+0x382`, read only through a pointer, by the matched `0x1c59a0` (`0x1c5a9f`, `0x1c5aab`, `0x1c5ab7`) | — | inside `g_511000.entries[0]` |
| `+0x384` | `0x511034` | 16 × 0xa4 | Player records: `+0` active, `+1` address index, `+0x8c` team, `+0xa0` a byte | `mov ebp, 0x511035` with `[ebp - 1]` (`0x195b62`, `0x195b7a`); `mov esi, 0x511034` (`0x1965bd`, `0x1973a6`); the team at `0x195c0b` | `g_511000.entries` (4 bytes early), `input_device()` |
| `+0xdc4` | `0x511a74` | 16 × 0x18 | Team entries: `+0` active, `+1`, `+2` short, `+4` short, `+8` dword, `+0xc` 12 bytes | `[eax*8 + 0x511a74]` … `+0x511a80` (`0x195c1b`–`0x195caa`), with `eax` = team × 3 (`lea eax, [ecx + ecx*2]` at `0x195c18`) | `g_511a74[16]` (`src/game_results_queries.cpp:16`) |
| `+0xf44` | `0x511bf4` | 0x4040 | Two-byte counters: 16 × 0x1b5 per player, then 16 × 16 × 2, then 16 × 45 | `mov esi, 0x511bf4` (`0x196674`); `add esi, 0x511bf4` (`0x157dab`, `0x157fc7`) | `g_511bf4` (`src/globals.cpp:43`) |
| `+0x45e4` | `0x515294` | 0x400 | (in the counters) the 16 × 16 × 2 block | `mov esi, 0x515294` (`0x1966cd`) | `g_515294` (`include/globals.h:754`) |
| `+0x49e4` | `0x515694` | 0x5a0 | (in the counters) the 16 × 45 block | `mov eax, 0x515694` (`0x158204`) | — (`include/globals.h:736` mentions it) |
| `+0x4f84` | `0x515c34` | 1000 × 0x24 | Samples (`g_510ca4` counts them) | `lea edi, [edi*4 + 0x515c34]` at `0x19713e` (in `0x197120`) | `g_515c34[1000]` (`src/unknown_196d20.cpp:21`) |
| `+0xdc24` | `0x51e8d4` | 16 × 11 | Machine addresses: 6-byte address, `+6` used, `+7`–`+10` flags | `mov ebp, 0x51e8db` (`0x195d25`); loops from `0x51e8dc` to `0x51e98c` (`0x196300`/`0x19637a`, `0x1994a2`/`0x1994b9`) | `g_51e8d4[16]` (`src/unknown_1967d0.cpp:14`) |
| `+0xdcd4` | `0x51e984` | 4 | Not referenced; cleared by `0x195a40` | — | — |

Notes on the table:

- **The copies' sources** are `g_4e6948` (`s_game_options_view`,
  `include/globals.h:43`); `ebx` holds it from `0x195a80`. The variant is what
  `function_192e60` returns for the value at the options' `+0x138`
  (`0x195aa9`–`0x195aaf`).
- **`+0x17c` and `+0x180`** fall inside the 0x130-byte copy, at its `+0x44` and
  `+0x48`, so they are the options' `+0x180` (`mode_180`) and `+0x184`
  (`flags184`). `0x195a40` reads `+0x180`'s low byte back for the team mode
  (`mov al, byte ptr [0x510e30]` at `0x195bda`, `and al, 1` at `0x195bdf`,
  stored at `0x195be1`); `game_results_queries.cpp:62` reads `+0x17c` as the
  game type.
- **The counters' parts** are at the offsets `s_input_counters` gives them
  (`include/globals.h:737`–`754`): `0x511bf4 + 16 × 0x1b5 × 2 = 0x515294`, and
  `0x515294 + 0x400 = 0x515694`. `0x515694 + 16 × 45 × 2 = 0x515c34`, where the
  samples begin.
- **`s_grs_source`'s fields** at those offsets (`src/unknown_1c5e10.cpp:596`):
  `unknown0000[4]` (`+0`–`+3`), `unknown0004`, `unknown0008` (`s_block20x9`),
  `unknown0128`–`unknown012a`, `unknown012b[5]`, `unknown0130`, `unknown0134`,
  `unknown0138` (`s_vr_source`, whose `unknown44` and `unknown48` are `+0x17c`
  and `+0x180`) and `unknown0210`, `unknown0268`, `unknown026c`
  (`s_block104`, 0x41 dwords), `unknown0370`–`unknown037c`,
  `unknown0380`–`unknown0383`, `unknown0384[16]` (`s_pgd_source`),
  `unknown0dc4[16]` (`s_grs_entry_source`), `unknown0f44` (`s_sta_source`),
  `unknown4f84` (`s_block8ca0`, 0x2328 dwords) and `unknowndc24[16]`
  (`s_grs_mac_source`, 11 bytes). It ends at `+0xdcd4`, 4 bytes before the end
  of retail's clear.

## The repo's pieces

**Defined globals (thirteen, in four files):**

| Global | Declared as | Defined at | Covers |
| --- | --- | --- | --- |
| `g_510cb0` | `byte` | `src/globals.cpp:39` | `+0` |
| `g_510cb1` | `byte` | `src/globals.cpp:40` | `+1` |
| `g_510cb2` | `byte` | `src/unknown_1967d0.cpp:835` | `+2` |
| `g_510de0`, `g_510de4` | `long` | `src/unknown_1967d0.cpp:836`–`837` | `+0x130`, `+0x134` |
| `g_510ddb` | `byte` | `src/unknown_1967d0.cpp:850` | `+0x12b` |
| `g_510e2c`, `g_510e30` | `dword` | `src/globals.cpp:41`–`42` | `+0x17c`, `+0x180` |
| `g_511000` | `s_input_device_globals` (0xa74 bytes) | `src/globals.cpp:44` | `+0x350`–`+0xdc3` |
| `g_511a74` | `s_input_entry[16]` | `src/game_results_queries.cpp:16` | `+0xdc4`–`+0xf43` |
| `g_511bf4` | `s_input_counters` (0x4040 bytes) | `src/globals.cpp:43` | `+0xf44`–`+0x4f83` |
| `g_515c34` | `s_196d20_sample[1000]` | `src/unknown_196d20.cpp:21` | `+0x4f84`–`+0xdc23` |
| `g_51e8d4` | `s_input_address[16]` | `src/unknown_1967d0.cpp:14` | `+0xdc24`–`+0xdcd3` |

**`#define`d names into them:** `g_511020` and `g_511028` into `g_511000`
(`include/input_record.h:136`–`137`), and `g_511c4e`, `g_511c90` and
`g_515294` into `g_511bf4` (`include/globals.h:752`–`754`).

**Views of the object through a pointer:**

| View | Where | Used by | What it names |
| --- | --- | --- | --- |
| `s_grs_source` | `src/unknown_1c5e10.cpp:596` | `packed_grs_write` (`0x1c59a0`), through `function_b3e90`'s cast (`src/unknown_0b3d30.cpp:477`) | Every section, to `+0xdcd4` |
| `s_match_player_rating_collection` | `src/unknown_072db0.cpp:539` | `function_741f0`, `function_74800` | `+4` `key`, `+0x12b` `team_mode`, `+0x384` `players[16]` (`s_match_player_rating_entry`, 0xa4 bytes, `team` at `+0x8c`), `+0xdc4` `teams[16]` (`s_match_rating_entry`, 0x18 bytes) |
| `s_match_rating_collection` | `src/unknown_072db0.cpp:463` | `function_74720` | `+0xdc4` `entries[16]` |
| `s_results_globals_view` | `src/unknown_1967d0.cpp:722` | `function_199290` | `+0xdc24` `addresses[16]` |
| `s_results_players_view` | `src/unknown_1967d0.cpp:757` | `function_199310` | `+0x384` `players[16]` (`s_results_player_slot`, `:750`) |
| `s_address_table` | `src/unknown_1967d0.cpp:663` | `function_1991f0`, `function_199250`, `function_199310`; cast by `function_73ca0` and `function_741f0` (`src/unknown_072db0.cpp:1793`, `:1395`) | `+0xdc24` `entries[16]` of 11 bytes |
| raw `byte *results` offsets | `function_73ca0` (`src/unknown_072db0.cpp:1779`) and the disabled `function_195a40` | — | `results + 4`, `+ 0x130`, `+ 0x134`, `results[2]`, … |
| `s_results_update_storage` | `src/unknown_1967d0.cpp:1401` (in the disabled block) | the disabled `refresh_results_195ed0` | The whole object, to `+0xdcd4` |

**Views of one record:** `s_input_device_view` (`include/globals.h:715`,
through `input_device()`, `:765`) and `s_results_player_195ed0`
(`src/unknown_1967d0.cpp:875`), both 0xa4 bytes from `+0x384`.
`s_input_record` (`include/input_record.h:40`) is not a view of this object
but a 0x4cc0-byte record that mirrors four of its sections (16 records, 16
team entries, the 0x4040 bytes of counters, 16 addresses).

## Findings

- **`s_input_device_globals` (`g_511000`) does not match retail's layout.**
  Retail references nothing in `0x511000`–`0x51101f`: those 0x20 bytes are the
  end of the 0x104-byte copy at `+0x26c`, which runs to `0x51101f`
  (`0x195b53`–`0x195b60`). Its `entries[16]` (0xa4 bytes from `+0x30`,
  `include/globals.h:755`–`761`) start at `0x511030`, 4 bytes before retail's
  first record at `0x511034`, so its `unknowna70[4]` is the last 4 bytes of
  record 15. `input_device()` adds the 4 back (`include/globals.h:765`–`768`);
  `s_grs_source`, `s_match_player_rating_collection` and
  `s_results_players_view` put the records at their true offset.
- **`g_510cb0` is a `byte` used as the whole object.** Nine call sites pass
  `&g_510cb0` to functions that read up to `+0xdc24` + 16 × 11 through it
  (`src/unknown_1967d0.cpp:1042` and `:1139` cast it to `s_address_table *`,
  `src/unknown_0b3d30.cpp:477` to `s_grs_source const *`). DECOMPILING.md asks
  for no "reaching neighbouring globals by offsetting one global's address"
  (`docs/DECOMPILING.md:434`–`435`); here the neighbours are in fact fields of
  one object.
- **The same fields have different names in different files**: `+0x12b` is
  `g_510ddb`, `team_mode` and `s_grs_source::unknown012b[0]`; `+4` is `key`,
  `unknown0004` and `results + 4` (which `function_73ca0` passes to
  `function_192e60`; `0x195a40` stores that value at `+4`, `0x195aa2`, and
  passes the same value from the options, `0x195aa9`); the records are `players[16]`,
  `unknown0384`, `g_511000.entries` (shifted) and `input_device()`; the team
  entries are `teams[16]`, `entries[16]`, `unknown0dc4` and `g_511a74`; the
  addresses are `s_address_table::entries`, `addresses[16]`, `unknowndc24` and
  `g_51e8d4`.
- **Three namings of the object** coexist: the `s_input_*` names
  (`include/globals.h:686`, `include/input_record.h:1`) call it input-device
  state; `src/game_results_queries.cpp:7`–`13` and the `s_results_*` views call
  it the game results that the postgame carnage report reads; and
  `s_grs_source` is what `function_b3e90` packs and sends as
  `"application/x-halo2pcr"` (`src/unknown_0b3d30.cpp:477`–`478`). The retail facts above (a copy of the game
  options and variant, per-player and per-team entries, counters and machine
  addresses, cleared at once by `0x195a40`) fit the second and third.
- **The 4-byte tail.** `s_grs_source` and `s_results_update_storage` end at
  `+0xdcd4`; retail clears to `+0xdcd8`, and nothing references
  `+0xdcd4`–`+0xdcd7`. Either a trailing field reached only through a pointer,
  or the type's size is rounded up by an 8-byte member; this document does not
  settle which.
- **Other unreferenced bytes**: `+3` and `+0x12c`–`+0x12f` have no absolute
  reference (`+0x380`–`+0x382` do have pointer readers, above).

## What the two drafts need

- **`0x195a40`** writes `+0`, `+4`, `+8`–`+0x12b`, `+0x130`–`+0x36f` (through
  the copies; `+0x370`–`+0x37f` are only cleared), the records' `+0`–`+0x9f`,
  the team entries and the addresses' `+7`–`+9`, and passes the base to
  `function_199250` and `function_1991f0` (`mov edx, 0x510cb0` at `0x195ba1`
  and `0x195bb6`). Its draft already uses these offsets on a `byte *results`
  parameter that retail does not have; with one object at `0x510cb0`, the
  parameter goes away.
- **`0x195ed0`** reads `+1` (`0x195ee0`) and `+0x370` (`0x195eed`). It writes
  `+0x12b` (`0x195f22`); the team entries' `+0` (`0x195f16`, in a loop from
  `mov esi, 0x511a74` at `0x195f01` to `0x511bf4` at `0x195f2d`) and their
  `+1` and `+2` word (in the player loop, `0x195fac`–`0x195fb5` and its
  unrolled copies such as `0x196039`–`0x196042`, and in the ranking loop,
  `0x1961f9` and `0x1962ae`); the records'
  `+1` and `+0x10`–`+0x9f` (from `mov edx, 0x511035` at `0x195f48`) and their
  `+0xa0` (`mov byte ptr [edx + 0x5110d4], al` at `0x1962ee`); counter 0 of
  every player, in a loop unrolled four times around `0x511f5e`
  (`+0xf44` + 0x36a; `0x195f40`, used at `0x195fbf` and `0x19604c`); and the
  addresses' `+7` and `+8` (`0x196300`–`0x19637a`). It pushes the base at
  `0x1962f6`. The live draft (`src/unknown_1967d0.cpp:916`) reaches all of
  these through the split names; the disabled second draft reaches them
  through `s_results_update_storage`.

## A possible binding

Unverified: it has not been compiled, and changing a matched function's
global into a field can change its code. The repo already uses `#define`d
names into a larger global (`include/input_record.h:136`–`137`,
`include/globals.h:752`–`754`), and the checker verifies a global by its
`g_<address>` name (`docs/DECOMPILING.md`, "What a match means"), so the
object would be `g_510cb0`:

1. Give `s_grs_source` named fields for `+0`–`+3` (today `unknown0000[4]`),
   move it to a shared header, and add the tail, so that its size is 0xdcd8.
2. Define `s_grs_source g_510cb0;` once, and turn eleven of the other
   globals and two of the `#define`s into `#define`s of its fields: `g_510cb1` → `+1`, `g_510cb2` → `+2`,
   `g_510ddb` → `unknown012b[0]`, `g_510de0` → `unknown0130`, `g_510de4` →
   `unknown0134`, `g_510e2c` → `unknown0138.unknown44`, `g_510e30` →
   `unknown0138.unknown48`, `g_511a74` → `unknown0dc4`, `g_511bf4` →
   `unknown0f44`, `g_515c34` → `unknown4f84`, `g_51e8d4` → `unknowndc24`;
   `g_511020` and `g_511028` → `unknown0370` and `unknown0378`.
3. Retire `g_511000`: its users reach the records through `unknown0384` (or
   `input_device()` rebased on it).
4. The dozen or so uses of `g_510cb0` as the valid byte (for example
   `src/game_results_queries.cpp:24`) become the `+0` field.

The types would need reconciling first:

- Same sizes, different element types: `s_input_entry` and
  `s_grs_entry_source` (0x18 bytes), `s_input_address` and `s_grs_mac_source`
  (11), `s_input_counters` and `s_sta_source` (0x4040), and
  `s_196d20_sample[1000]` and `s_block8ca0` (0x8ca0, `dword[0x2328]`).
- `g_511020` and `g_511028` are `s_flagged_value`s (8 bytes, used as `.flag`
  and `.value`, for example `src/unknown_1967d0.cpp:921`), but `s_grs_source`
  splits each into a `byte` and a `long` (`unknown0370`/`unknown0374`,
  `unknown0378`/`unknown037c`); `s_grs_source` would take
  `s_flagged_value` there.
- `g_510e30` is a `dword`, but `s_vr_source::unknown48` is `byte[4]`
  (`src/unknown_1c5e10.cpp:20`).

## Evidence

- A capstone scan of every inventory entry for absolute memory operands and
  immediates in `0x510c90`–`0x51e98f` found 389 references in 128 functions;
  210 of them, in 47 functions (41 `matched`), are inside
  `0x510cb0`–`0x51e987`. The rest are the separate globals at `0x510c90`–
  `0x510ca8` (used across the game, not by this object) and `0x51e98c`. The
  scan script and its output are in the workspace (`analysis/blockrefs.py`,
  `analysis/refs_510cb0.txt`), not in the repo. Pointer accesses, such as
  `0x1c59a0`'s, are not in the count.
- `0x195a40` (retail): the clear at `0x195a44`–`0x195a50`; the copies with
  `ecx` = 0x4c (`0x195b3e`) and 0x41 (`0x195b56`); the string loop
  `0x195b10`–`0x195b25`; the record loop from `0x195b62`, with the 12-byte
  copy to `+2` (`0x195b85`–`0x195b98`) and the 0x24-dword copy to `+0x10`
  (`0x195b9b`–`0x195ba6`).
- `0x1c59a0` (`packed_grs_write`, matched) reads the source at `+0x380`–`+0x382`
  (`0x1c5a9f`, `0x1c5aab`, `0x1c5ab7`), `+0x384` (`0x1c5ac3`), `+0xf44`
  (`0x1c5b61`) and `+0x4f84` (`0x1c5b6d`).
- Repo sizes: `s_input_device_globals` is asserted 0xa74 bytes
  (`include/globals.h:761`); `s_input_counters` is `s_input_counter all[0x2020]`
  (0x4040 bytes, `include/globals.h:741`); `s_196d20_sample` is 9 dwords
  (`src/unknown_196d20.cpp:11`–`14`); `s_input_address` is 11 bytes
  (`include/input_record.h:34`–`37`).
- Line numbers are at `7364a06c`.
