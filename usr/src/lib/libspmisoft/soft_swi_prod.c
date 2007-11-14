/*
 * CDDL HEADER START
 *
 * The contents of this file are subject to the terms of the
 * Common Development and Distribution License (the "License").
 * You may not use this file except in compliance with the License.
 *
 * You can obtain a copy of the license at usr/src/OPENSOLARIS.LICENSE
 * or http://www.opensolaris.org/os/licensing.
 * See the License for the specific language governing permissions
 * and limitations under the License.
 *
 * When distributing Covered Code, include this CDDL HEADER in each
 * file and include the License file at usr/src/OPENSOLARIS.LICENSE.
 * If applicable, add the following below this CDDL HEADER, with the
 * fields enclosed by brackets "[]" replaced with your own identifying
 * information: Portions Copyright [yyyy] [name of copyright owner]
 *
 * CDDL HEADER END
 */

/*
 * Copyright 2007 Sun Microsystems, Inc.  All rights reserved.
 * Use is subject to license terms.
 */

#pragma ident	"@(#)soft_swi_prod.c	1.2	07/11/12 SMI"

#include "spmisoft_api.h"
#include "sw_swi.h"

char *
get_clustertoc_path(Module * mod)
{
	char *c;

	enter_swlib("get_clustertoc_path");
	c = swi_get_clustertoc_path(mod);
	exit_swlib();
	return (c);
}

void
media_category(Module * mod)
{
	enter_swlib("media_category");
	swi_media_category(mod);
	exit_swlib();
	return;
}
