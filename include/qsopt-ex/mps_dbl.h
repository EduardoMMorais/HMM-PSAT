/* WARNING! This file was autogenerated from template */
/****************************************************************************/
/*                                                                          */
/*  This file is part of QSopt_ex.                                          */
/*                                                                          */
/*  (c) Copyright 2006 by David Applegate, William Cook, Sanjeeb Dash,      */
/*  and Daniel Espinoza                                                     */
/*                                                                          */
/*  Sanjeeb Dash ownership of copyright in QSopt_ex is derived from his     */
/*  copyright in QSopt.                                                     */
/*                                                                          */
/*  This code may be used under the terms of the GNU General Public License */
/*  (Version 2.1 or later) as published by the Free Software Foundation.    */
/*                                                                          */
/*  Alternatively, use is granted for research purposes only.               */
/*                                                                          */
/*  It is your choice of which of these two licenses you are operating      */
/*  under.                                                                  */
/*                                                                          */
/*  We make no guarantees about the correctness or usefulness of this code. */
/*                                                                          */
/****************************************************************************/

/* RCSINFO $Id: mps_dbl.h,v 1.2 2003/11/05 16:57:39 meven Exp $ */
#ifndef dbl_MPS_H
#define dbl_MPS_H

#include "readline_dbl.h"
#include "format_dbl.h"

/****************************************************************************/
/*                                                                          */
/*              Routines to support Reading and Writing MPS Files           */
/*                                                                          */
/****************************************************************************/
#include "basicdefs.h"
extern const char *dbl_ILLmps_section_name[ILL_MPS_N_SECTIONS + 2];

#include "lpdata_dbl.h"
#include "rawlp_dbl.h"
#include "read_mps_dbl.h"

extern int dbl_ILLread_mps (
	dbl_qsline_reader * file,
	const char *filename,
	dbl_rawlpdata * lp);

extern int dbl_ILLwrite_mps (
	dbl_ILLlpdata * lp,
	dbl_qserror_collector * collector);

				/* use lp->reporter for output */

#endif