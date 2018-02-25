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

/* RCSINFO $Id: fct_mpf.h,v 1.3 2003/11/05 16:57:39 meven Exp $ */
#ifndef mpf___FUNCTIONS_H
#define mpf___FUNCTIONS_H

#include "basicdefs.h"
#include "lpdefs_mpf.h"


int mpf_ILLfct_compute_zA (
	mpf_lpinfo * lp,
	mpf_svector * z,
	mpf_svector * zA),
  mpf_ILLfct_compute_wz (
	mpf_lpinfo * lp,
	mpf_t * wz),
  mpf_ILLfct_adjust_viol_bounds (
	mpf_lpinfo * lp),
  mpf_ILLfct_perturb_bounds (
	mpf_lpinfo * lp),
  mpf_ILLfct_perturb_phaseI_bounds (
	mpf_lpinfo * lp),
  mpf_ILLfct_bound_shift (
	mpf_lpinfo * lp,
	int col,
	int bndtype,
	mpf_t newbnd),
  mpf_ILLfct_adjust_viol_coefs (
	mpf_lpinfo * lp),
  mpf_ILLfct_perturb_coefs (
	mpf_lpinfo * lp),
  mpf_ILLfct_coef_shift (
	mpf_lpinfo * lp,
	int col,
	mpf_t newcoef),
  mpf_ILLfct_test_pivot (
	mpf_lpinfo * lp,
	int indx,
	int indxtype,
	mpf_t piv_val);

void mpf_ILLfct_load_workvector (
	mpf_lpinfo * lp,
	mpf_svector * s),
  mpf_ILLfct_zero_workvector (
	mpf_lpinfo * lp),
  mpf_ILLfct_set_variable_type (
	mpf_lpinfo * lp),
  mpf_ILLfct_compute_pobj (
	mpf_lpinfo * lp),
  mpf_ILLfct_compute_dobj (
	mpf_lpinfo * lp),
  mpf_ILLfct_compute_xbz (
	mpf_lpinfo * lp),
  mpf_ILLfct_compute_piz (
	mpf_lpinfo * lp),
  mpf_ILLfct_compute_phaseI_xbz (
	mpf_lpinfo * lp),
  mpf_ILLfct_compute_phaseI_piz (
	mpf_lpinfo * lp),
  mpf_ILLfct_compute_dz (
	mpf_lpinfo * lp),
  mpf_ILLfct_compute_phaseI_dz (
	mpf_lpinfo * lp),
  mpf_ILLfct_compute_yz (
	mpf_lpinfo * lp,
	mpf_svector * yz,
	mpf_svector * updz,
	int ecol),
  mpf_ILLfct_compute_zz (
	mpf_lpinfo * lp,
	mpf_svector * zz,
	int lindex),
  mpf_ILLfct_compute_binvrow (
	mpf_lpinfo * lp,
	mpf_svector * zz,
	int row,
	mpf_t ztoler),
  mpf_ILLfct_compute_vA (
	mpf_lpinfo * lp,
	mpf_svector * v,
	mpf_t * vA),
  mpf_ILLfct_compute_psteep_upv (
	mpf_lpinfo * lp,
	mpf_svector * swz),
  mpf_ILLfct_compute_dsteep_upv (
	mpf_lpinfo * lp,
	mpf_svector * swz),
  mpf_ILLfct_update_basis_info (
	mpf_lpinfo * lp,
	int eindex,
	int lindex,
	int lvstat),
  mpf_ILLfct_update_xz (
	mpf_lpinfo * lp,
	mpf_t tz,
	int eindex,
	int lindex),
  mpf_ILLfct_update_piz (
	mpf_lpinfo * lp,
	mpf_t alpha),
  mpf_ILLfct_update_pIpiz (
	mpf_lpinfo * lp,
	mpf_svector * z,
	const mpf_t alpha),
  mpf_ILLfct_update_dz (
	mpf_lpinfo * lp,
	int eindex,
	mpf_t alpha),
  mpf_ILLfct_update_pIdz (
	mpf_lpinfo * lp,
	mpf_svector * zA,
	int eindex,
	const mpf_t alpha),
  mpf_ILLfct_unroll_bound_change (
	mpf_lpinfo * lp),
  mpf_ILLfct_unroll_coef_change (
	mpf_lpinfo * lp),
  mpf_ILLfct_check_pfeasible (
	mpf_lpinfo * lp,
	mpf_feas_info * fs,
	const mpf_t ftol),
  mpf_ILLfct_check_pIpfeasible (
	mpf_lpinfo * lp,
	mpf_feas_info * fs,
	mpf_t ftol),
  mpf_ILLfct_check_dfeasible (
	mpf_lpinfo * lp,
	mpf_feas_info * fs,
	const mpf_t ftol),
  mpf_ILLfct_dual_adjust (
	mpf_lpinfo * lp,
	const mpf_t ftol),
  mpf_ILLfct_dphaseI_simple_update (
	mpf_lpinfo * lp,
	mpf_t ftol),
  mpf_ILLfct_set_status_values (
	mpf_lpinfo * lp,
	int pstatus,
	int dstatus,
	int ptype,
	int dtype),
  mpf_ILLfct_init_counts (
	mpf_lpinfo * lp),
  mpf_ILLfct_update_counts (
	mpf_lpinfo * lp,
	int f,
	int upi,
	const mpf_t upd),
  mpf_ILLfct_print_counts (
	mpf_lpinfo * lp),
  mpf_ILLfct_check_pIdfeasible (
	mpf_lpinfo * lp,
	mpf_feas_info * fs,
	mpf_t ftol),
  mpf_ILLfct_update_pfeas (
	mpf_lpinfo * lp,
	int lindex,
	mpf_svector * srhs),
  mpf_ILLfct_compute_ppIzz (
	mpf_lpinfo * lp,
	mpf_svector * srhs,
	mpf_svector * ssoln),
  mpf_ILLfct_update_ppI_prices (
	mpf_lpinfo * lp,
	mpf_price_info * pinf,
	mpf_svector * srhs,
	mpf_svector * ssoln,
	int eindex,
	int lindex,
	const mpf_t alpha),
  mpf_ILLfct_update_dfeas (
	mpf_lpinfo * lp,
	int eindex,
	mpf_svector * srhs),
  mpf_ILLfct_compute_dpIy (
	mpf_lpinfo * lp,
	mpf_svector * srhs,
	mpf_svector * ssoln),
  mpf_ILLfct_update_dpI_prices (
	mpf_lpinfo * lp,
	mpf_price_info * pinf,
	mpf_svector * srhs,
	mpf_svector * ssoln,
	int lindex,
	mpf_t alpha),
  mpf_ILLfct_update_dIIfeas (
	mpf_lpinfo * lp,
	int eindex,
	mpf_svector * srhs),
  mpf_ILLfct_compute_dpIIy (
	mpf_lpinfo * lp,
	mpf_svector * srhs,
	mpf_svector * ssoln),
  mpf_ILLfct_update_dpII_prices (
	mpf_lpinfo * lp,
	mpf_price_info * pinf,
	mpf_svector * srhs,
	mpf_svector * ssoln,
	/*int eindex,*/
	int lindex,
	mpf_t eval,
	mpf_t alpha);

void mpf_fct_test_workvector (
	mpf_lpinfo * lp),
  mpf_fct_test_pfeasible (
	mpf_lpinfo * lp),
  mpf_fct_test_dfeasible (
	mpf_lpinfo * lp),
  mpf_fct_test_pI_x (
	mpf_lpinfo * lp,
	mpf_price_info * p),
  mpf_fct_test_pII_x (
	mpf_lpinfo * lp,
	mpf_price_info * p),
  mpf_fct_test_pI_pi_dz (
	mpf_lpinfo * lp,
	mpf_price_info * p),
  mpf_fct_test_pII_pi_dz (
	mpf_lpinfo * lp,
	mpf_price_info * p);

mpf_bndinfo *mpf_ILLfct_new_bndinfo (
	void);
void mpf_ILLfct_free_bndinfo (
	mpf_bndinfo * binfo);

#endif /* mpf___FUNCTIONS_H */
