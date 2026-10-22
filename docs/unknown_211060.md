# Moving a point through a medium, with bounces (unknown_211060)

Retail range covered: `0x211060`–`0x2115ff`. This is one inventory entry, 1,428
retail bytes, `todo`, and `config/functions.csv` gives it no source. The repo
has only declarations and a stub for it (see
[Existing declarations](#existing-declarations)); no `@retail`, `Retail` or
`Disabled` marker names it. It lies in lane U's range (`0x200000`–`0x217fff`,
issue #9). Its neighbours `0x211030` (`src/unknown_211030.cpp`) and `0x211600`
(`src/unknown_211600.cpp`) are `matched`.

`function_211060` advances one point (a position and a velocity) by `dt`
seconds, using a physics definition that its two callers find by tag index:

1. It picks a medium. Unless the caller says otherwise, that is always the one
   that `0x211030` derives from `g_51e9c4->unknown8` (see
   [The medium](#the-medium-0x21108a0x2111b4)).
2. It reads a cell vector at the point (`function_211b70` or
   `function_211b30`, the cells of `src/unknown_20fe20.cpp`).
3. It adds gravity, scaled by a buoyancy factor from the definition's density
   and the medium's, and pulls the velocity toward the cell vector by a drag
   factor in `[0, 1]`. An optional force is added first.
4. Without collision it moves the point by `velocity · dt` and looks up its
   location again. With collision it casts the move with `function_1697c0`
   and on a hit reflects the velocity off the plane, places the point just off
   the surface and spends the rest of `dt`, up to three times.
5. It returns flags: the medium (1 or 2), and 4 or 8 for the kind of surface
   hit.

**Analysis only:** this document adds no source, and nothing in it has been
built or checked against retail with the original compiler. Names are
provisional. The `function_<va>` form is primary; the descriptions are offered
to lane U, or to whoever decompiles it.

## Boundary

- **The range** is one function. Its last instruction is `ret 0x28`
  (`0x2115f1`, three bytes); twelve `int3` bytes (`0xcc`) follow to
  `0x2115ff`. The previous entry, `0x211030`, ends with `ret` at
  `0x21105f`, so `0x211060` follows it directly.
- **The callers** (`tools/xrefs.py 0x211060`), both `todo`, both with an
  `@retail` draft:
  - `0x1161f0` at `0x11638b`, the flexible chain solver
    (`src/flexible_chain.cpp:235`, call at `:263`);
  - `0x17c540` at `0x17c770`, in lane R's range (`src/unknown_17b850.cpp:661`,
    call at `:725`).
- **The callees**, all with source:

  | Retail | Called at | Status | Source |
  | --- | --- | --- | --- |
  | `0x211b70` | `0x2110f0` | `matched` | `src/unknown_20fe20.cpp:1006` |
  | `0x211b30` | `0x211106` | `matched` | `src/unknown_20fe20.cpp:1041` |
  | `0x1697c0` | `0x211347` | `todo` (a draft) | `src/unknown_1689b0.cpp:826` |
  | `0x11bed0` | `0x2115b4` | `matched` | `src/unknown_11bed0.cpp:55` |

## Conventions

| Retail | Bytes | Arguments | Returns | Callers | What it does |
| --- | --- | --- | --- | --- | --- |
| `0x211060` | 1428 | `velocity` in `eax`; ten on the stack, popped by `ret 0x28` | flags in `eax` | 2 | Moves a point by `dt` through a medium, with up to three bounces |

**The frame.** No frame pointer: `sub esp, 0x8c` (`0x211066`), then `ebp`,
`esi`, `ebx` and `edi` are pushed (`0x21106c`, `0x21107d`, `0x2110c9`,
`0x2110d4`). Below, `entry−x` is relative to `esp` at entry (the return
address). One more push, the cell mode at `0x2110d9`, is an argument that
both cell calls share (see [Code-shape notes](#code-shape-notes)).

| Argument | Where | Use here |
| --- | --- | --- |
| `flags` | `entry+4` | Bit 0: the caller gives a cell index (`0x2110d1`). Bit 1: with bit 0, use the `unknown4` medium (`0x2110e3`–`0x2110ed`). Bit 2: no collision (`0x2112d0`) |
| `physics` | `entry+8` | The definition, held in `ebp` (`0x21106d`) |
| `location` | `entry+0xc` | An `s_location`, read by `function_211b30` and rewritten when a lookup gives a leaf |
| `cell_slot` | `entry+0x10` | Read only under bit 0, as `function_211b70`'s index (`0x2110e6`) |
| `position` | `entry+0x14` | A `point3f`, read and rewritten |
| `force` | `entry+0x18` | Optional `vector3f const *`; read at `0x2111fe` |
| `normal` | `entry+0x1c` | Optional `vector3f *`; receives the hit plane's normal (`0x211398`) |
| `material_type` | `entry+0x20` | Optional `short *`; receives the hit's material type (`0x2113b7`) |
| `radius` | `entry+0x24` | Squared and cubed at `0x2110a5`–`0x2110b9`; bounds the surface offset (`0x21135c`) |
| `dt` | `entry+0x28` | Tested at entry; in the bounce loop it is the time left, and retail writes it back to its own argument slot (`0x211515`) |

**Register inputs:** `eax` is `velocity` (`vector3f *`), moved to `esi` at
`0x21107e` and read and written throughout. `ecx` and `edx` are written
before they are read; `ebx`, `edi`, `esi` and `ebp` are saved first.

**Return value:** `eax`. It is 0 when `dt` is zero (`0x2115e8`, from the `ecx`
zeroed at `0x211074`). Otherwise it is 1 or 2 for the medium (`0x211198`,
`0x211154`), with 8 or-ed in for a hit of type 2 and 4 for a hit of type 1
(`0x211383`, `0x211391`). Neither caller reads it: `0x1161f0` overwrites `eax`
after the call without reading it (`lahf` at `0x116545`, then `mov` at
`0x11655f` or, past the `jp` at `0x11655d`, at `0x11657d`), and `0x17c540`
overwrites it at `0x17c77e`.

**The locals.** At depth `0x9c` (after the four saves; the shared push is
popped by the cell call's `ret 8`):

| Local | Type | What it holds |
| --- | --- | --- |
| `entry−0x8c` | long | The return flags |
| `entry−0x88` | real, then short | The unit mass, then `mass` (`0x2111c8`); from `0x2112de` the bounce counter, stored as a dword and compared as a word (`0x2112f1`) |
| `entry−0x84` | real | `radius³` (`0x2110b9`); in the loop, the surface offset (`0x211368`) |
| `entry−0x80` | `vector3f` | The cell vector (the `edi` of both cell calls, `0x2110d5`); in the loop, the displacement passed to `function_1697c0` |
| `entry−0x74` | long, then `s_location` | 0 (`0x211098`), read back as "no collision flags" at `0x2112ba`; on the no-collision path, the new location (`0x2115ab`) |
| `entry−0x70` | real | In the loop, the tangential part's `j` (`0x211431`) |
| `entry−0x68` | `s_collision_result_1697c0` | `function_1697c0`'s result; nothing else is used up to `entry−0x4`, so the frame leaves room for up to 0x64 bytes |
| `entry−0x4` | real | In the loop, `d · n.k` (`0x211418`) |

### The caller

Both calls push the same shape (`0x116350`–`0x116389`,
`0x17c72c`–`0x17c76e`): `flags` 0, the definition, the location, `NONE` as the
cell index, the position, three zeros (no force, no normal, no material
type), the radius and `dt`; `velocity` goes in `eax` last
(`lea eax, [esi + 0xc]` at `0x116386`, `lea eax, [esi + 0x28]` at
`0x17c76b`). The definition is the tag's data pointer,
`[g_4e3b44 + (index & 0xffff) · 0x10 + 8]` (`0x116372`–`0x116381`,
`0x17c757`–`0x17c766`). The radius is 0.02 for a chain node
(`push 0x3ca3d70a` at `0x116358`) and half the state's width for a contrail
point (`0x17c733`–`0x17c742`).

So neither existing caller uses `flags`, `cell_slot`, `force`, `normal` or
`material_type`, and both always take the location path for the cell vector.

## Data

**The physics definition** (`ebp`). The callers call its tag index
`physics_index` (`src/flexible_chain.cpp:49`) and `physics_tag_index`
(`src/unknown_17b850.cpp:100`). Retail's code holds no group tag or name for
it, so the names below describe what each field does here:

| Offset | Type | Read at | Use |
| --- | --- | --- | --- |
| `+0` | dword flags | `0x21109c`, `0x2111a3`, `0x2112ae` | Bit 1 (`0x2`): collide with the structure, and run `function_1697c0`'s per-cluster test (see [Collision flags](#collision-flags-0x2112ae0x2112d8)). Bit 2 (`0x4`): collide with the clusters' planes. Bits 3 and 4 (`0x8`, `0x10`): the cell mode's bits 0 and 1. Bit 5 (`0x20`): no gravity |
| `+4` | real | `0x211093` | Added to the medium's unit mass (`0x211129`, `0x211178`); read here as the point's own unit mass (an inference, see [Findings](#findings)) |
| `+0x20` | real | `0x211124`, `0x21113b`, `0x21116b`, `0x211182` | Density, compared with the medium's |
| `+0x24` | real | `0x211193` | Drag coefficient in the `unknown8` medium |
| `+0x28` | real | `0x21114c` | Drag coefficient in the `unknown4` medium |
| `+0x2c` | real | `0x211448` | Tangential loss on a bounce (`1 − x` keeps the rest) |
| `+0x30` | real | `0x211451` | Normal restitution on a bounce |

**Globals.**

| Global | What it is | Source |
| --- | --- | --- |
| `g_51e9c4` | `s_unknown_1eb550 *`: `+0` gravity (4.1712594), `+4` `unknown4` (1.0), `+8` `unknown8` (0.0011) | `src/unknown_1eb550.cpp:16`–`18`, `include/unknown_1eb550.h:10`; `+0` as gravity: `src/projectiles.cpp:863` |
| `g_4f93ac` | `unknown4 · 118613.344` | `src/unknown_211030.cpp:14` |
| `g_4f93b0` | `unknown8 · 118613.344` | `src/unknown_211030.cpp:13` |

`0x211030` is the only writer of `g_4f93ac` and `g_4f93b0`, and `0x211060` the
only other reader (`tools/xrefs.py`).

**Constants** (from the retail image): `0x45dbd8` = 0.0f, `0x45dbc0` = 1.0f,
`0x45dc0c` = 2.0f, `0x45e21c` = 0.005f, and `0x45dbe0` = 0.0 as an 8-byte
double.

## The function

### Entry (`0x211060`–`0x211084`)

`dt` is compared with 0.0f (`ucomiss`, `lahf`, `test ah, 0x44`, `jnp`): when
it is exactly zero the function returns 0 at once (`0x2115e7`). The
`test ah, 0x44` and parity pattern is `==`/`!=`; a NaN `dt` goes on.

### The cell vector (`0x21108a`–`0x21110b`)

The cell mode is built from the definition's flags: bit 0 is flag bit 3
(`movzx`, `shr eax, 3`, `and eax, 1`, `0x21109f`–`0x2110af`), and bit 1 is
set from flag bit 4 with an `or eax, 2` / `and eax, 0xfffffffd` pair
(`0x2110c1`, `0x2110c6`). The mode is pushed once (`0x2110d9`) for both
calls, and `edi` points at the cell vector (`0x2110d5`).

- **`flags` bit 0 set:** the medium flag `bl` is `flags` bit 1
  (`shr ebx, 1; and bl, 1`), and `function_211b70(cell_slot, position, mode,
  &cell)` is called with the index in `eax` and `position` pushed
  (`0x2110dc`–`0x2110f0`).
- **Otherwise:** `function_211b30(location, position, mode, &cell)` with
  `location` in `ecx` (`0x2110f7`–`0x211106`), and `bl` is its return value.
  Retail's `0x211b30` always returns 0 (`xor al, al` at `0x211b67`; the repo's
  source returns `false`, `src/unknown_20fe20.cpp:1053`). So without bit 0
  the function always uses the `unknown8` medium.

`function_211b70` (`src/unknown_20fe20.cpp:1006`–`1038`) writes the cell's
direction plus a noise term, or `*g_4687a4` when the index is out of range or
the cell is inactive. Mode bit 0 drops the `1 − noise_scale` factor from the
direction; mode bit 1 scales the result by `1 − damping`.

### The medium (`0x21110d`–`0x2111b4`)

`ecx` = `g_51e9c4` (`0x21110f`). The two branches compute the same four
values:

| Value | `bl` set (`0x211117`) | `bl` clear (`0x21115e`) |
| --- | --- | --- |
| unit mass (`entry−0x88`) | `g_4f93ac + physics[+4]` | `g_4f93b0 + physics[+4]` |
| `b` | `2 · density / (g_51e9c4->unknown4 + density)` | `2 · density / (g_51e9c4->unknown8 + density)` |
| drag coefficient (`xmm1`) | `physics[+0x28]` | `physics[+0x24]` |
| return flags | 2 | 1 |

`density` is `physics[+0x20]`. Then (`0x2111a3`–`0x2111b4`) the gravity scale
is `1 − b`, or 0 when the definition's flag bit 5 is set, and the drag
coefficient is multiplied by `radius²`.

### Mass, drag and force (`0x2111b7`–`0x21124d`)

`mass = unit mass · radius³` is computed on the x87 stack (`fld`, `fmul`,
`fst` to `entry−0x88`, `0x2111b7`–`0x2111c8`) and compared with the 8-byte 0.0
at `0x45dbe0` (`fucomip`, then the `test ah, 0x44` pattern).

- **`mass` not zero** (`0x2111dc`): `scale = dt / mass`, and
  `drag = scale · coefficient · radius²`, pinned to `[0, 1]` (`comiss` against
  0 at `0x2111ec`, against 1 at `0x2111f6`). If `force` is not null,
  `velocity += force · scale`, component by component (`0x211209`–`0x21123a`).
- **`mass` zero** (`0x211241`): `drag` is 1 when `coefficient · radius²` is
  not 0, else 0 (`ucomiss`, `test ah, 0x44`, `jp`). `force` is not read.

### Gravity and drag (`0x211250`–`0x2112a9`)

```
velocity.k += g_51e9c4->unknown0 · gravity_scale · dt;   // 0x211250–0x21126c
velocity.j += (cell.j − velocity.j) · drag;              // stored at 0x211284
velocity.i += (cell.i − velocity.i) · drag;              // stored at 0x2112a5
velocity.k += (cell.k − velocity.k) · drag;              // stored at 0x2112a9
```

With `gravity_scale = 1 − 2ρ/(ρm + ρ) = (ρm − ρ)/(ρm + ρ)`, a point denser
than the medium gains negative `k` velocity, as elsewhere in the repo where
gravity is subtracted (`src/items.cpp:967`).

### Collision flags (`0x2112ae`–`0x2112d8`)

The flags for `function_1697c0` are `0x800005` when the definition's flag
bit 1 is set, else 0 (reloaded from `entry−0x74`), with `0x800002` or-ed in
for flag bit 2. When they are 0, or when `flags` bit 2 is set, the function
takes [the no-collision path](#the-no-collision-path-0x2115770x2115e4).

In `function_1697c0`, retail tests flag bit 0 before a structure (bsp) hit,
which it records as type 1 (`test bl, 1` at `0x169889`, `mov dword ptr
[ebp], 1` at `0x16989a`), and flag bit 1 before testing the clusters'
instanced planes, recorded as type 2 (`0x169a93`, `0x169bb2`). Bit 23
(`0x800000`) is the first bit `collision_flags_to_test_flags` copies
(`src/unknown_1689b0.cpp:73`–`84`); with it set, the `0x1800000` default at
`0x1697d3`–`0x1697ee` does not apply.

`0x800005` also sets bit 2 (`0x4`). Retail tests it at `0x1699e0` and
`0x169c5a` (`test bl, 4`); in the repo's draft, bit 2 alone passes the
cluster loop's `& 0xe` gate (`src/unknown_1689b0.cpp:904`) and calls
`function_244980` for each cluster the vector crosses (`:989`–`991`). So the
definition's flag bit 1 turns on that test too, even when flag bit 2 is
clear. `0x244980` is `todo` (`src/unknown_244c10.cpp:91`), so which result
type its hits record is not known here; the bounce loop adds 8 or 4 only for
types 2 and 1, and treats any hit the same way otherwise.

### The bounce loop (`0x2112de`–`0x211528`)

The counter at `entry−0x88` starts at 0 and the loop runs while it is below 3
(`cmp word ptr`, `jge`, `0x2112f1`). Each pass:

1. `displacement = velocity · dt` into `entry−0x80` (`0x2112fd`–`0x211341`).
2. `collision.material_type = NONE` (`mov word ptr [esp + 0x70], 0xffff`,
   `0x21133a`, the result's `+0x24`); then
   `function_1697c0(collision_flags, position, &displacement, NONE, NONE,
   &collision)` (`0x211347`, `ret 0x18`).
3. **No hit** (`0x211539`): if `collision.end_location.leaf_index` is not
   `NONE`, `*location = collision.end_location`; `*position =
   collision.point`; return the flags. (`function_1697c0` sets `point` to the
   end of the vector at `t`, `src/unknown_1689b0.cpp:1049`.)
4. **A hit** (`0x211354`): the surface offset is `radius`, but at most 0.005
   (`comiss`, `ja`, `0x211365`). Type 2 adds 8 to the flags, type 1 adds 4.
   If `normal` is not null it receives `collision.plane`'s `i`, `j`, `k`
   (`0x2113a3`); if `material_type` is not null it receives
   `collision.material_type` (`0x2113c2`).
5. The velocity is reflected (`0x2113ca`–`0x2114a5`), with `n` the plane's
   normal:

   ```
   d       = v.k·n.k + v.j·n.j + v.i·n.i          // this order, 0x2113ef–0x21140a
   along   = d·n
   tangent = v − along
   v       = (1 − physics[+0x2c]) · tangent − physics[+0x30] · along
   ```

6. If `collision.end_location.leaf_index` is not `NONE`, `*location =
   collision.end_location` (`0x2114ac`).
7. `*position = collision.point + n · offset` (`0x2114bc`–`0x211510`).
8. `dt = dt − dt · collision.t`, written back to `dt`'s own slot
   (`0x2114f4`–`0x211515`); the counter is incremented (`0x2114f3`).
9. If the new `dt` is not 0 (`jp` at `0x211522`) the loop goes on at
   `0x2112e8`, which reloads `dt` before the counter test. Otherwise, or after
   three passes, it returns the flags (`0x211528`).

### The no-collision path (`0x211577`–`0x2115e4`)

`*position += velocity · dt` (`0x211577`–`0x2115af`; `xmm1` still holds the
new `velocity.i`). Then `function_11bed0(&new_location, position)`, with
`edx` = `position` (`0x211577`) and `esi` = `entry−0x74` (`0x2115ab`), called
at `0x2115b4`. If `new_location.leaf_index` is not `NONE`, `*location =
new_location` (`0x2115c6`) and the function returns the flags through its own
epilogue; otherwise it returns them through the loop's (`je 0x211528` at
`0x2115c0`).

## A possible source form

Unverified: it has not been compiled, and it follows the retail shapes noted
above. `s_point_mover_settings` stands for the definition; its field names are
the descriptions from [Data](#data).

```c
long function_211060(dword flags, s_point_mover_settings const *physics, s_location *location,
	long cell_slot, point3f *position, vector3f const *force, vector3f *normal,
	short *material_type, real radius, real dt, vector3f *velocity)
{
	if (dt == 0.0f)
		return 0;

	real radius_squared = radius * radius;
	real radius_cubed = radius_squared * radius;
	byte mode = TEST_FLAG(physics->flags, 3);
	SET_FLAG(mode, 1, TEST_FLAG(physics->flags, 4));

	vector3f cell;
	bool medium_4;
	if (TEST_FLAG(flags, 0))
	{
		medium_4 = TEST_FLAG(flags, 1);
		function_211b70((short)cell_slot, position->n, mode, &cell);
	}
	else
		medium_4 = function_211b30((byte const *)location, position->n, mode, &cell);

	long result;
	real unit_mass, buoyancy, coefficient;
	if (medium_4)
	{
		unit_mass = g_4f93ac + physics->unit_mass;
		buoyancy = physics->density * 2.0f / (g_51e9c4->unknown4 + physics->density);
		coefficient = physics->drag_4;
		result = 2;
	}
	else
	{
		unit_mass = g_4f93b0 + physics->unit_mass;
		buoyancy = physics->density * 2.0f / (g_51e9c4->unknown8 + physics->density);
		coefficient = physics->drag_8;
		result = 1;
	}
	real gravity_scale = 1.0f - buoyancy;
	if (TEST_FLAG(physics->flags, 5))
		gravity_scale = 0.0f;
	coefficient *= radius_squared;

	real drag;
	real mass = unit_mass * radius_cubed;
	if (mass != 0.0)
	{
		real scale = dt / mass;
		drag = scale * coefficient;
		if (drag < 0.0f)
			drag = 0.0f;
		else if (drag > 1.0f)
			drag = 1.0f;
		if (force)
		{
			velocity->i += force->i * scale;
			velocity->j += force->j * scale;
			velocity->k += force->k * scale;
		}
	}
	else
		drag = coefficient != 0.0f ? 1.0f : 0.0f;

	velocity->k += g_51e9c4->unknown0 * gravity_scale * dt;
	velocity->i += (cell.i - velocity->i) * drag;
	velocity->j += (cell.j - velocity->j) * drag;
	velocity->k += (cell.k - velocity->k) * drag;

	long collision_flags = TEST_FLAG(physics->flags, 1) ? 0x800005 : 0;
	if (TEST_FLAG(physics->flags, 2))
		collision_flags |= 0x800002;
	if (collision_flags == 0 || TEST_FLAG(flags, 2))
	{
		s_location new_location;
		position->x += velocity->i * dt;
		position->y += velocity->j * dt;
		position->z += velocity->k * dt;
		function_11bed0(&new_location, position);
		if (new_location.leaf_index != NONE)
			*location = new_location;
		return result;
	}

	for (short bounce = 0; bounce < 3; bounce++)
	{
		vector3f displacement = { velocity->i * dt, velocity->j * dt, velocity->k * dt };
		s_collision_result_1697c0 collision;
		collision.material_type = NONE;
		if (!function_1697c0(collision_flags, position, &displacement, NONE, NONE, &collision))
		{
			if (collision.end_location.leaf_index != NONE)
				*location = *(s_location *)&collision.end_location;
			*position = collision.point;
			return result;
		}
		real offset = radius > 0.005f ? 0.005f : radius;
		if (collision.type == 2)
			result |= 8;
		else if (collision.type == 1)
			result |= 4;
		if (normal)
			*normal = *(vector3f *)&collision.plane;
		if (material_type)
			*material_type = collision.material_type;

		real d = velocity->k * collision.plane.k + velocity->j * collision.plane.j + velocity->i * collision.plane.i;
		vector3f along = { d * collision.plane.i, d * collision.plane.j, d * collision.plane.k };
		vector3f tangent = { velocity->i - along.i, velocity->j - along.j, velocity->k - along.k };
		velocity->i = (1.0f - physics->tangential_loss) * tangent.i - physics->restitution * along.i;
		velocity->j = (1.0f - physics->tangential_loss) * tangent.j - physics->restitution * along.j;
		velocity->k = (1.0f - physics->tangential_loss) * tangent.k - physics->restitution * along.k;
		if (collision.end_location.leaf_index != NONE)
			*location = *(s_location *)&collision.end_location;
		position->x = collision.plane.i * offset + collision.point.x;
		position->y = collision.plane.j * offset + collision.point.y;
		position->z = collision.plane.k * offset + collision.point.z;
		dt -= dt * collision.t;
		if (dt == 0.0f)
			break;
	}
	return result;
}
```

### Code-shape notes

- **The mass test is a double comparison.** Retail computes
  `unit_mass · radius³` with `fld`/`fmul`, stores it with `fst` and compares
  the unrounded x87 value with the 8-byte 0.0 at `0x45dbe0`, in a function
  that otherwise uses SSE. That points at a `0.0` literal (not `0.0f`) or
  another double-typed expression in the test.
- **Four exact float tests** use `ucomiss` or `fucomip` with
  `lahf; test ah, 0x44` and a parity jump: `dt == 0` at entry (`jnp`),
  `mass != 0` (`jnp`), the zero-mass `coefficient != 0` (`jp`) and the
  loop's `dt != 0` (`jp`). The pins and the offset use `comiss`.
- **The shared cell-mode push.** Both cell calls take `position` and `mode`
  on the stack, `mode` being the second stack argument and so pushed first.
  Retail pushes `mode` once, before the `flags` bit 0 branch (`0x2110d9`),
  and each branch pushes only `position`.
- **The cell mode's bit 1** is set with `or eax, 2` or cleared with
  `and eax, 0xfffffffd` (`0x2110c1`, `0x2110c6`), although bit 1 is already
  clear. That is a `SET_FLAG`-style macro (`(value) ? (flags |= …) :
  (flags &= ~…)`, as in `src/units.cpp:46`), not a plain `|`.
- **The zero collision flags** are reloaded from `entry−0x74`
  (`0x2112ba`), which was filled from the `ecx` zeroed at entry
  (`0x211098`), rather than with `xor`.
- **The bounce counter** is stored as a dword (`0x2112de`, `0x21151e`) but
  compared as a word (`0x2112f1`), which suggests a `short`.
- **The loop's tests.** The counter test is at the top and is reached on
  entry (`jmp 0x2112f1` at `0x2112e6`); the `dt` test is at the bottom
  (`0x211522`). That is a counted loop with a `break` (or an equivalent
  `while`), not a `do`/`while`.
- **The time left** is `dt − dt · t` (`mulss`, `subss`, `0x2114f4`–`0x2114f8`),
  not `dt · (1 − t)`.
- **Four epilogues**: the loop's end (`0x211528`), the miss (`0x211552`),
  the no-collision path with a leaf (`0x2115c6`) and the zero `dt`
  (`0x2115e7`). The no-collision path without a leaf jumps to the loop's
  (`0x2115c0`).
- **Velocity store order** in the drag step is `k` (gravity), `j`, `i`, `k`;
  the source above writes `i`, `j`, `k`, which the compiler may reorder.
  Check this first if the drag block differs.
- **`function_211b30`'s `location`.** The repo declares it
  `byte const *location` and reads the cluster at `+4`
  (`src/unknown_20fe20.cpp:1041`, `:1046`), which is `s_location`'s
  `cluster_index` (`include/object_queries.h:9`–`14`).

## Callees without source

None. `0x1697c0` has a `todo` draft (`src/unknown_1689b0.cpp:826`); the three
others are `matched`.

## Existing declarations

| Where | Declaration | Retail |
| --- | --- | --- |
| `src/flexible_chain.cpp:86`, `src/unknown_17b850.cpp:20` | `void function_211060(long unknown0, void *physics, s_location *location, long unknown3, point3f *position, long unknown5, long unknown6, long unknown7, real radius, real dt, vector3f *velocity)` | Returns the flags in `eax` (neither caller reads them). `unknown0` is `flags`, `unknown3` the cell index, `unknown5` a `vector3f const *`, `unknown6` a `vector3f *`, `unknown7` a `short *`; `velocity` is in `eax` |
| `src/stubs/lane_r.cpp:26`–`27` | The same signature, `// @stub 0x211060`, empty body | — |
| `src/unknown_20fe20.cpp:1006` | `void function_211b70(short index, real const *position, byte flags, vector3f *out)` | `index` in `ax`, `out` in `edi`; `position` and `flags` on the stack, `ret 8` |
| `src/unknown_20fe20.cpp:1041` | `bool function_211b30(byte const *location, real const *position, byte flags, vector3f *out)` | `location` in `ecx`, `out` in `edi` (passed through); `position` and `flags` on the stack, `ret 8`; always returns 0 |
| `src/unknown_1689b0.cpp:827` | `bool __stdcall function_1697c0(long flags, point3f const *point, vector3f const *vector, long ignore_object_index, long ignore_unit_index, s_collision_result_1697c0 *collision)` | The same, `ret 0x18` |
| `src/unknown_11bed0.cpp:56` | `void function_11bed0(s_location *location, point3f const *point)` | `location` in `esi`, `point` in `edx` |

## Findings

- **`s_collision_result_1697c0` has two sizes in the repo.** The full
  definition in `src/unknown_1689b0.cpp:482`–`499` is 0x5c bytes (the
  comment at `:474` says so), as are those in `src/items.cpp:197`,
  `src/projectiles.cpp:997`, `src/unknown_0a76b0.cpp:4159`,
  `src/unknown_1c8df0.cpp:6` and `src/unknown_1c9290.cpp:17`;
  `include/unknown_0259a0.h:37`, `src/damage.cpp:3198` and
  `src/unknown_0350e0.cpp:2933` declare 0x4c bytes. Retail `0x1697c0` writes
  up to `+0x5b` on its structure-hit path (`mov word ptr [ebp + 0x5a], cx`
  at `0x169956`, after `+0x58` and `+0x59` at `0x169950` and `0x169953`), so
  the buffer must be 0x5c bytes. This frame does not decide it (it has room
  for up to 0x64), but the fields read here (`type`, `t`, `point`,
  `end_location`, `material_type`, `plane`) are all named in the full
  definition, so a draft can take that one.
- **`118613.344` (`src/unknown_211030.cpp:13`–`14`)** is within two float
  steps of `(4π/3) · 1000 · 3.048³` = 118613.33 (the stored float is
  118613.34375; one step here is 0.0078). If it is that product, `g_4f93ac`
  and `g_4f93b0` are the masses of a sphere of radius one world unit, in
  kilograms, of densities `unknown4` and `unknown8` in g/mL, with a world
  unit of 3.048 m. The repo already uses 3.048 for the world unit's metres
  (`SetDistanceFactor(3.048f, …)`, `src/unknown_21e4b0.cpp:110`). `unknown4`
  = 1.0 and `unknown8` = 0.0011 then read as water and air, and the
  definition's `+4` as its own unit mass, added to the medium's. This is an
  inference from the arithmetic; the repo names none of it.
- **The location path never uses the `unknown4` medium**, because retail's
  `0x211b30` always returns 0. Only a caller that sets `flags` bit 0 can
  choose it with bit 1, and neither existing caller does.
- **`function_211b30`'s name for its first argument** is
  `byte const *location`; this caller passes it the same `s_location *` it
  later rewrites.

## Evidence

- `tools/fncfg.py 0x211060`: 52 blocks, four `ret`s, the stack depth
  consistent at every join, every `ret` at depth 0. The dump is
  `analysis/cfg_211060.txt` in the workspace (not in the repo).
- `tools/xrefs.py`: `0x211060` is called at `0x11638b` (in `0x1161f0`) and
  `0x17c770` (in `0x17c540`); `0x4f93ac` and `0x4f93b0` are read or written
  only in `0x211030` and `0x211060`; the constant at `0x45ddec` (118613.344)
  only in `0x211030`.
- `0x1697c0` (retail): `test ebx, 0x1800000` at `0x1697d3`; type 1 at
  `0x16989a` after `test bl, 1` at `0x169889`; type 2 at `0x169bb2` after
  `test bl, 2` at `0x169a93`.
- `0x211b30` ends `xor al, al; ret 8` (`0x211b67`, `0x211b69`).
- `0x211b70` reads its index from `ax` (`0x211b73`), its mode from the
  second stack argument (`0x211b77`), and writes through `edi` (`0x211c18`).
- `0x11bed0` writes through `esi` (`0x11bede`–`0x11bee6`).
- Constants read from the retail image: `0x45dbd8` = 0.0f, `0x45dbc0` = 1.0f,
  `0x45dc0c` = 2.0f, `0x45e21c` = 0.005f, `0x45dbe0` = 0.0 (double),
  `0x45ddec` = 118613.34375f.
- Line numbers are at `7364a06c`.
