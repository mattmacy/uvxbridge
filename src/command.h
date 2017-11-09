/*
 * Copyright (C) 2017 Joyent Inc.
 * Copyright (C) 2017 Matthew Macy <matt.macy@joyent.com>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *   1. Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 *   2. Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in the
 *      documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */


#ifndef _UVXCOMMAND_H_
#define _UVXCOMMAND_H_

#define	ETHERTYPE_UVXCONF		0xDECA	/* uxvxbridge config type  */
#define UVXMAGIC				0xABADCAFE
#define UVX_KEYSIZE 32

struct uvxcmd_header {
	/* arbitrary sequence number echoed in reply */
	uint16_t uh_seqno;
	/* operation code */
	uint16_t uh_op;
	/* return code -- only used in response */
	uint16_t uh_rc;
	/* magic value -- identifies proper command  */
	uint32_t uh_magic;
};

struct arp_request {
	/* protocol address */
	uint32_t ar_pa;
};

struct arp_reply {
	/* protocol address */
	uint32_t ar_pa;
	/* hardware  address */
	uint8_t ar_ha[ETHER_ADDR_LEN];
};

struct vx_arp_request {
	/* protocol address */
	uint32_t var_pa;
	/* guest vxlanid */
	uint32_t var_vxlanid;
};

struct vx_arp_reply {
	/* protocol address */
	uint32_t var_pa;
	/* guest vxlanid */
	uint32_t var_vxlanid;
	/* hardware address */
	uint8_t var_ha[ETHER_ADDR_LEN];
};

/* forwarding table entry request */
struct fte_request {
	/* guest vxlanid */
	uint32_t fr_vxlanid;
	/* destination hardware address */
	uint8_t fr_ha[ETHER_ADDR_LEN];
};

/* forwarding table entry reply */
struct fte_reply {
	/* destination protocol address */
	uint32_t fr_pa;
	/* guest vxlanid */
	uint32_t fr_vxlanid;
	/* destination hardware address */
	uint8_t fr_ha[ETHER_ADDR_LEN];
};

/* guest interface request */
struct vm_intf_request {
	/* guest interface hardware address */
	uint8_t vir_ha[ETHER_ADDR_LEN];
};

/* guest interface reply */
struct vm_intf_reply {
	/* guest interface vlanid */
	uint16_t vir_vlanid;
	/* guest interface vxlanid */
	uint32_t vir_vxlanid;
	/* guest interface hardware address */
	uint8_t	vir_ha[ETHER_ADDR_LEN];
	/* guest interface flags (firewall/encryption) */
	uint32_t vir_flags;
};

/* configure client encrypted tunnel */
struct tun_configure_client {
	/* peer protocol address */
	struct in_addr tcc_pa;
	/* peer preshared key */
	uint8_t tcc_psk[UVX_KEYSIZE];
};

/* configure uvxbridge preshared key */
struct tun_configure_server {
	uint8_t tcs_psk[UVX_KEYSIZE];
};

/* query peer preshared key value */
struct tun_query {
	/* peer protocol address */
	struct in_addr tq_pa;
	/* peer preshared key */
	uint8_t tq_psk[UVX_KEYSIZE];
};

/* configure uvxbridge route */
struct route_configure {
	/* local protocol address */
	uint32_t rc_lpa;
	/* remote protocol address (gateway) */
	uint32_t rc_rpa;
	/* mask prefix length */
	uint16_t rc_prefixlen;
	/* configuration flags (currently unused) */
	uint16_t rc_flags;
};

#define CMD_OP_NONE		0x0
#define CMD_ARP_REQUEST	0x1		/* request hardware address for IP */
#define CMD_ARP_REPLY		0x2		/* provide hardware address for IP */

#define CMD_FTE_REQUEST	0x3		/* request forwarding address for destination HA */
#define CMD_FTE_REPLY		0x4		/* provide forwarding address for destination HA */

#define CMD_VM_INTF_REQUEST	0x5		/* request vxlanid/vlanid for VM HA */
#define CMD_VM_INTF_REPLY	0x6		/* provide vxlanid/vlanid for VM HA */

#define CMD_TUN_SERVCONF	0x7		/* provide PSK for this bridge */
#define CMD_TUN_CLICONF	0x8		/* provide PSK for peer bridge */
#define CMD_TUN_QUERY		0x9		/* request PSK for this bridge */

#define CMD_ROUTE_CONFIGURE	0xA		/* provide route configuration information */
#define CMD_ROUTE_QUERY	0xB		/* request route configuration information */

#define CMD_IPFW		0xC		/* enclosed payload is IPFW configuration information */
#define CMD_HEARTBEAT		0xD		/* heartbeat to provisioning agent */

#define CMD_VX_ARP_REQUEST	0xE		/* request hardware address for IP */
#define CMD_VX_ARP_REPLY		0xF		/* provide hardware address for IP */

void uvxcmd_fill(char *txbuf, uint64_t smac, uint64_t dmac, uint16_t op, uint16_t rc, uint16_t seqno);



#endif
