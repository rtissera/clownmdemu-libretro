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

#endif /* LIBRA_EXTENDED_MEMORY_H */
