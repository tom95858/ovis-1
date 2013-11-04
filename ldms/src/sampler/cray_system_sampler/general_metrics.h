/* -*- c-basic-offset: 8 -*-
 * Copyright (c) 2013 Open Grid Computing, Inc. All rights reserved.
 * Copyright (c) 2013 Sandia Corporation. All rights reserved.
 * Under the terms of Contract DE-AC04-94AL85000, there is a non-exclusive
 * license for use of this work by or on behalf of the U.S. Government.
 * Export of this program may require a license from the United States
 * Government.
 *
 * This software is available to you under a choice of one of two
 * licenses.  You may choose to be licensed under the terms of the GNU
 * General Public License (GPL) Version 2, available from the file
 * COPYING in the main directory of this source tree, or the BSD-type
 * license below:
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *      Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 *
 *      Redistributions in binary form must reproduce the above
 *      copyright notice, this list of conditions and the following
 *      disclaimer in the documentation and/or other materials provided
 *      with the distribution.
 *
 *      Neither the name of Sandia nor the names of any contributors may
 *      be used to endorse or promote products derived from this software
 *      without specific prior written permission.
 *
 *      Neither the name of Open Grid Computing nor the names of any
 *      contributors may be used to endorse or promote products derived
 *      from this software without specific prior written permission.
 *
 *      Modified source versions must be plainly marked as such, and
 *      must not be misrepresented as being the original software.
 *
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */


/**
 * \file general_metrics.h non-gpcdr metrics
 */

#ifndef __GENERAL_METRICS_H_
#define __GENERAL_METRICS_H_

#define _GNU_SOURCE
#include <inttypes.h>
#include <unistd.h>
#include <sys/errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <pthread.h>
#include <sys/types.h>
#include <ctype.h>
#include <wordexp.h>

#include "../lustre/lustre_sampler.h"

#define VMSTAT_FILE "/proc/vmstat"
#define LOADAVG_FILE "/proc/loadavg"
#define CURRENT_FREEMEM_FILE "/proc/current_freemem"
#define KGNILND_FILE  "/proc/kgnilnd/stats"

/* CURRENT_FREEMEM Specific */
FILE *cf_f;
static char* CURRENT_FREEMEM_METRICS[] = {"current_freemem"};
#define NUM_CURRENT_FREEMEM_METRICS (sizeof(CURRENT_FREEMEM_METRICS)/sizeof(CURRENT_FREEMEM_METRICS[0]))
ldms_metric_t* metric_table_current_freemem;

/* VMSTAT Specific */
FILE *v_f;
static char* VMSTAT_METRICS[] = {"nr_dirty", "nr_writeback"};
#define NUM_VMSTAT_METRICS (sizeof(VMSTAT_METRICS)/sizeof(VMSTAT_METRICS[0]))
ldms_metric_t* metric_table_vmstat;

/* LOADAVG Specific */
FILE *l_f;
static char* LOADAVG_METRICS[] = {"loadavg_latest(*100)",
				  "loadavg_5min(*100)",
				  "loadavg_running_processes",
				  "loadavg_total_processes"};
#define NUM_LOADAVG_METRICS (sizeof(LOADAVG_METRICS)/sizeof(LOADAVG_METRICS[0]))
ldms_metric_t *metric_table_loadavg;

/* KGNILND Specific */
FILE *k_f;
static char* KGNILND_METRICS[] = {"SMSG_ntx",
				  "SMSG_tx_bytes",
				  "SMSG_nrx",
				  "SMSG_rx_bytes",
				  "RDMA_ntx",
				  "RDMA_tx_bytes",
				  "RDMA_nrx",
				  "RDMA_rx_bytes"
};
#define NUM_KGNILND_METRICS (sizeof(KGNILND_METRICS)/sizeof(KGNILND_METRICS[0]))
ldms_metric_t* metric_table_kgnilnd;

/* LUSTRE Specific */
/**
 * This is for single llite.
 * The real metrics will contain all llites.
 */
static char *LUSTRE_METRICS[] = {
	/* file operation */
	"dirty_pages_hits",
	"dirty_pages_misses",
	"writeback_from_writepage",
	"writeback_from_pressure",
	"writeback_ok_pages",
	"writeback_failed_pages",
	"read_bytes",
	"write_bytes",
	"brw_read",
	"brw_write",
	"ioctl",
	"open",
	"close",
	"mmap",
	"seek",
	"fsync",
	/* inode operation */
	"setattr",
	"truncate",
	"lockless_truncate",
	"flock",
	"getattr",
	/* special inode operation */
	"statfs",
	"alloc_inode",
	"setxattr",
	"getxattr",
	"listxattr",
	"removexattr",
	"inode_permission",
	"direct_read",
	"direct_write",
	"lockless_read_bytes",
	"lockless_write_bytes",
};
#define LUSTRE_METRICS_LEN (sizeof(LUSTRE_METRICS)/sizeof(LUSTRE_METRICS[0]))
#define LLITE_PREFIX "/proc/fs/lustre/llite"

/* Lustre specific vars */
/**
 * str<->idx in LUSTRE_METRICS.
 */
extern struct lustre_svc_stats_head lustre_svc_head;
extern struct str_map *lustre_idx_map;

/** get metric size */
int get_metric_size_lustre(size_t *m_sz, size_t *d_sz,
			   ldmsd_msg_log_f msglog);


/** add metrics */
int add_metrics_lustre(ldms_set_t set, int comp_id,
			      ldmsd_msg_log_f msglog);

/** helpers */
int handle_llite(const char *llite);


/** sample */
int sample_metrics_vmstat(ldmsd_msg_log_f msglog);
int sample_metrics_kgnilnd(ldmsd_msg_log_f msglog);
int sample_metrics_current_freemem(ldmsd_msg_log_f msglog);
int sample_metrics_loadavg(ldmsd_msg_log_f msglog);
int sample_metrics_lustre(ldmsd_msg_log_f msglog);

#endif
