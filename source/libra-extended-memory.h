/* SPDX-License-Identifier: AGPL-3.0-or-later */
#ifndef LIBRA_EXTENDED_MEMORY_H
#define LIBRA_EXTENDED_MEMORY_H

/* Custom retro_get_memory_data IDs >= 0x100 are out of band of standard
 * libretro and will not collide with future RETRO_MEMORY_* additions.
 * Used by rtissera/libra and the Megadrive Studio MCP server to surface
 * VDP debug state that the standard memory IDs do not cover.
 *
 * Standard libretro frontends ignore unknown memory IDs, so this is a
 * non-breaking extension. Debug-aware frontends (rtissera/libra) can
 * read these to display CRAM palettes, VSRAM scroll values, the live
 * VDP_State struct, and the live Clown68000_State register file.
 *
 * Each ID maps to a typed pointer into the live ClownMDEmu instance
 * (no copy, no allocation). Sizes are sizeof() of the underlying
 * struct so consumers can lay out the bytes against the matching
 * upstream header.
 */
#define LIBRA_MEMORY_CRAM      0x100  /* clownmdemu.vdp.state.cram     */
#define LIBRA_MEMORY_VSRAM     0x101  /* clownmdemu.vdp.state.vsram    */
#define LIBRA_MEMORY_VDP_STATE 0x102  /* full VDP_State struct (typed) */
#define LIBRA_MEMORY_M68K      0x103  /* full Clown68000_State struct  */
#define LIBRA_MEMORY_Z80       0x104  /* full ClownZ80_State struct    */
#define LIBRA_MEMORY_Z80_RAM   0x105  /* 8 KiB Z80 RAM ($A00000-$A01FFF) */
#define LIBRA_MEMORY_Z80_BUS   0x108  /* packed bus arbitration flags  */
#define LIBRA_MEMORY_DEBUG_API 0x109  /* libra_md_debug_api fn-pointer table */

/* LIBRA_MEMORY_Z80_BUS layout (size = 1 byte, refreshed on each call):
 *   bit 0 = bus_requested  (68k holds Z80 BUSREQ)
 *   bit 1 = reset_held     (Z80 in reset)
 * Mirrors clownmdemu.state.z80.{bus_requested,reset_held}.
 */

/* LIBRA_MEMORY_DEBUG_API: pointer to a static struct exposing the M4
 * debug entry points to external libretro frontends without requiring
 * a new RETRO_ENVIRONMENT extension. Returned size is sizeof the struct.
 *
 * The function pointers are stable for the lifetime of the loaded core.
 * All callbacks are installed onto the *currently running* ClownMDEmu's
 * 68k state — there is exactly one such state per core instance.
 */
struct libra_md_debug_api {
	/* Cooperative halt: ask the 68k dispatch loop to bail at the next
	   instruction boundary. Thread-safe (volatile flag). */
	void (*request_halt)(void);
	void (*clear_halt_request)(void);
	/* Install/remove an execution breakpoint callback. Callback receives
	   (userdata, pc) before each instruction; non-zero return triggers halt.
	   Pass cb=NULL to detach. */
	void (*set_breakpoint_callback)(int (*cb)(void *userdata, unsigned long pc), void *userdata);
	/* Install/remove a memory watchpoint callback fired on every 68k bus
	   read/write. Callback receives (userdata, addr, size, is_write, value);
	   non-zero return triggers halt. Pass cb=NULL to detach. */
	void (*set_watchpoint_callback)(int (*cb)(void *userdata, unsigned long addr, unsigned char size, int is_write, unsigned long value), void *userdata);
};

#endif /* LIBRA_EXTENDED_MEMORY_H */
