/*
 * Copyright (C) 2015 Yi Pan, Prayush Kumar, Stas Babak
 * 
 * This program is free software; you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
 * 
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 * 
 * You should have received a copy of the GNU General Public License along with
 * with program; see the file COPYING. If not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/*
 * Coefficients of the so-called "deltalm" terms are given by Damour et al.
 * PRD 79, 064004 (2009) and Pan et al. PRD 83, 064003 (2011), henceforth DIN
 * and PBFRT.
 * 
 * The coefficients are used to compute the factorized waveform
 */





#ifndef _LALSIMIMRSPINEOBFACTORIZEDWAVEFORMCOEFFICIENTS_C
#define _LALSIMIMRSPINEOBFACTORIZEDWAVEFORMCOEFFICIENTS_C


#include <complex.h>
#include <lal/LALSimInspiral.h>
#include <lal/LALSimIMR.h>

#include "LALSimIMREOBNRv2.h"
#include "LALSimIMRSpinEOB.h"

/*--------------------------------------------------------------*/
/**
 * Spin Factors
 */

UNUSED static int 
XLALSimIMREOBCalcPrecNoSpinFacWaveformCoefficients(
					   FacWaveformCoeffs * const coeffs,
						   const REAL8 m1,
						   const REAL8 m2,
						   const REAL8 eta,
						   const REAL8 a,
						   const REAL8 chiS,
						   const REAL8 chiA,
					   const UINT4 SpinAlignedEOBversion
);

UNUSED static int 
XLALSimIMREOBCalcPrecSpinFacWaveformCoefficients(
					   FacWaveformCoeffs * const coeffs,
						 const REAL8 m1,
						 const REAL8 m2,
						 const REAL8 eta,
						 const REAL8 a,
						 const REAL8 chiS,
						 const REAL8 chiA,
					   const UINT4 SpinAlignedEOBversion
);

static int 
XLALSimIMREOBCalcSpinFacWaveformCoefficients(
					   FacWaveformCoeffs * const coeffs,	/**< OUTPUT, pre-computed waveform coefficients */
					     const REAL8 m1,	/**< mass 1 */
					     const REAL8 m2,	/**< mass 2 */
					     const REAL8 eta,	/**< symmetric mass ratio */
					     const REAL8 a,	/**< Kerr spin parameter for test-particle terms */
					     const REAL8 chiS,	/**< (chi1+chi2)/2 */
					     const REAL8 chiA,	/**< (chi1-chi2)/2 */
					   const UINT4 SpinAlignedEOBversion	/**< 1 for SEOBNRv1; 2 for SEOBNRv2 */
);


/****************************************************
 *    Definition of Functions
 *    **********************************************/


/**
 * This function calculates coefficients for hlm mode factorized-resummed waveform.
 * The coefficients are pre-computed and stored in the SpinEOBParams structure.
 * Appendix of the paper, and papers DIN (PRD 79, 064004 (2009)) and PBFRT (PRD 83, 064003 (2011)).
 */

static int 
XLALSimIMREOBCalcSpinFacWaveformCoefficients(
					   FacWaveformCoeffs * const coeffs,	/**< OUTPUT, pre-computed waveform coefficients */
					     const REAL8 m1,	/**< mass 1 */
					     const REAL8 m2,	/**< mass 2 */
					     const REAL8 eta,	/**< symmetric mass ratio */
					     const REAL8 tmpa,	/**< Kerr spin parameter for test-particle terms */
					     const REAL8 chiS,	/**< (chi1+chi2)/2 */
					     const REAL8 chiA,	/**< (chi1-chi2)/2 */
					   const UINT4 SpinAlignedEOBversion	/**< 1 for SEOBNRv1; 2 for SEOBNRv2 */
)
{
	int		debugPK = 0;
	if (debugPK) {
		printf("In XLALSimIMREOBCalcSpinFacWaveformCoefficients: Renewing hlm coefficients.\n");
		//FIXME
			printf("PK:: chiS = %.12e, chiA = %.12e, a = %.12e (UNUSED), EOBVERSION = %d\n",
			       chiS, chiA, tmpa, SpinAlignedEOBversion);
	}
	REAL8		a = tmpa;
	REAL8		eta2 = eta * eta;
	REAL8		eta3 = eta2 * eta;

	REAL8		dM      , dM2;
	//dM3;
	REAL8		aDelta  , a2, a3;

	/* Combination which appears a lot */
	REAL8		m1Plus3eta, m1Plus3eta2, m1Plus3eta3;

	dM2 = 1. - 4. * eta;

	//printf("****************************** a = %e *********************************\n", a);

	/* Check that deltaM has a reasonable value */
	if (dM2 < 0) {
		XLALPrintError("XLAL Error - %s: eta seems to be < 0.25 - this isn't allowed!\n", __func__);
		XLAL_ERROR(XLAL_EINVAL);
	}
	dM = sqrt(dM2);
	if (m1 < m2) {
		dM = -dM;
	}
	//dM3 = dM2 * dM;

	aDelta = 0.;
	//a value in delta_lm is 0 in both SEOBNRv1 and SEOBNRv2
		a2 = a * a;
	a3 = a2 * a;

	m1Plus3eta = -1. + 3. * eta;
	m1Plus3eta2 = m1Plus3eta * m1Plus3eta;
	m1Plus3eta3 = m1Plus3eta * m1Plus3eta2;

	/* Initialize all coefficients to zero */
	/* This is important, as we will not set some if dM is zero */
	memset(coeffs, 0, sizeof(FacWaveformCoeffs));


	/*
	 * l = 2, Eqs. A8a and A8b for rho, Eq. A15a for f, Eqs. 20 and 21 of
	 * DIN and Eqs. 27a and 27b of PBFRT for delta
	 */

	coeffs->delta22vh3 = 7. / 3.;
	coeffs->delta22vh6 = (-4. * aDelta) / 3. + (428. * LAL_PI) / 105.;
	coeffs->delta22v8 = (20. * aDelta) / 63.;
	coeffs->delta22vh9 = -2203. / 81. + (1712. * LAL_PI * LAL_PI) / 315.;
	coeffs->delta22v5 = -24. * eta;
	coeffs->delta22v6 = 0.0;
	if (SpinAlignedEOBversion == 2 && chiS + chiA * dM / (1. - 2. * eta) > 0.) {
		coeffs->delta22v6 = -540. * eta * (chiS + chiA * dM / (1. - 2. * eta));
//        double chi = (chiS + chiA * dM / (1. - 2. * eta));
//        coeffs->delta22v6 = eta*(1./4.*(1. - 1080.*chi - sqrt((1. - 1080.*chi)*(1. - 1080*chi) + 8.*(13.5 +270.*chi +13.5*chi*chi))));
        }
    if (SpinAlignedEOBversion == 3 /*&& chiS + chiA * dM / (1. - 2. * eta) > 0.*/) {
//        		coeffs->delta22v6 = -540. * eta * (chiS + chiA * dM / (1. - 2. * eta));
        double chi = (chiS + chiA * dM / (1. - 2. * eta));
        coeffs->delta22v6 = eta*(1./4.*(1. - 1080.*chi - sqrt((1. - 1080.*chi)*(1. - 1080*chi) + 8.*(13.5 +270.*chi +13.5*chi*chi))));
    }
	coeffs->rho22v2 = -43. / 42. + (55. * eta) / 84.;
	coeffs->rho22v3 = (-2. * (chiS + chiA * dM - chiS * eta)) / 3.;
	switch (SpinAlignedEOBversion) {
	case 1:
		coeffs->rho22v4 = -20555. / 10584. + 0.5 * a2
			- (33025. * eta) / 21168. + (19583. * eta2) / 42336.;
		break;
	case 2:
		coeffs->rho22v4 = -20555. / 10584. + 0.5 * (chiS + chiA * dM) * (chiS + chiA * dM)
			- (33025. * eta) / 21168. + (19583. * eta2) / 42336.;
		break;
    case 3:
        coeffs->rho22v4 = -20555. / 10584. + 0.5 * (chiS + chiA * dM) * (chiS + chiA * dM)
        - (33025. * eta) / 21168. + (19583. * eta2) / 42336.;
        break;
	default:
		XLALPrintError("XLAL Error - %s: wrong SpinAlignedEOBversion value, must be 1 or 2!\n", __func__);
		XLAL_ERROR(XLAL_EINVAL);
		break;
	}
	coeffs->rho22v5 = (-34. * a) / 21.;
	coeffs->rho22v6 = 1556919113. / 122245200. + (89. * a2) / 252. - (48993925. * eta) / 9779616.
		- (6292061. * eta2) / 3259872. + (10620745. * eta3) / 39118464.
		+ (41. * eta * LAL_PI * LAL_PI) / 192.;
	coeffs->rho22v6l = -428. / 105.;
	coeffs->rho22v7 = (18733. * a) / 15876. + a * a2 / 3.;
	switch (SpinAlignedEOBversion) {
	case 1:
		coeffs->rho22v8 = eta * (-5.6 - 117.6 * eta) - 387216563023. / 160190110080. + (18353. * a2) / 21168. - a2 * a2 / 8.;
		break;
	case 2:
		coeffs->rho22v8 = -387216563023. / 160190110080. + (18353. * a2) / 21168. - a2 * a2 / 8.;
		break;
    case 3:
        coeffs->rho22v8 = -387216563023. / 160190110080. + (18353. * a2) / 21168. - a2 * a2 / 8.;
        break;
	default:
		XLALPrintError("XLAL Error - %s: wrong SpinAlignedEOBversion value, must be 1 or 2!\n", __func__);
		XLAL_ERROR(XLAL_EINVAL);
		break;
	}
	coeffs->rho22v8l = 9202. / 2205.;
	coeffs->rho22v10 = -16094530514677. / 533967033600.;
	coeffs->rho22v10l = 439877. / 55566.;

	if (debugPK) {
		printf("\nPK:: dM, eta, chiS, chiA while renewing hlm coeffs: %e, %e, %e, %e\n",
		       dM, eta, chiS, chiA);
		printf("PK:: Renewed rho-lm coeffs: v2 = %.16e, v3 = %.16e, v4 = %.16e, v5 = %.16e\nv6 = %.16e, v6l = %.16e v7 = %.16e v8 = %.16e, v8l = %.16e v10 = %.16e v10l = %.16e\n",
		       coeffs->rho22v2, coeffs->rho22v3, coeffs->rho22v4, coeffs->rho22v5,
		       coeffs->rho22v6, coeffs->rho22v6l, coeffs->rho22v7, coeffs->rho22v8,
		     coeffs->rho22v8l, coeffs->rho22v10, coeffs->rho22v10l);
	}
	if (dM2) {
		coeffs->delta21vh3 = 2. / 3.;
		coeffs->delta21vh6 = (-17. * aDelta) / 35. + (107. * LAL_PI) / 105.;
		coeffs->delta21vh7 = (3. * aDelta * aDelta) / 140.;
		coeffs->delta21vh9 = -272. / 81. + (214. * LAL_PI * LAL_PI) / 315.;
		coeffs->delta21v5 = -493. * eta / 42.;

		//coeffs->rho21v1 = (-3. * (chiS + chiA / dM)) / (4.);
		coeffs->rho21v1 = 0.0;
		//coeffs->rho21v2 = -59. / 56 - (9. * chiAPlusChiSdM * chiAPlusChiSdM) / (32. * dM2) + (23. * eta) / 84.;
		switch (SpinAlignedEOBversion) {
		case 1:
			coeffs->rho21v2 = -59. / 56 + (23. * eta) / 84. - 9. / 32. * a2;
			coeffs->rho21v3 = 1177. / 672. * a - 27. / 128. * a3;
			break;
		case 2:
			coeffs->rho21v2 = -59. / 56 + (23. * eta) / 84.;
			coeffs->rho21v3 = 0.0;
			break;
        case 3:
            coeffs->rho21v2 = -59. / 56 + (23. * eta) / 84.;
            coeffs->rho21v3 = 0.0;
            break;
		default:
			XLALPrintError("XLAL Error - %s: wrong SpinAlignedEOBversion value, must be 1 or 2!\n", __func__);
			XLAL_ERROR(XLAL_EINVAL);
			break;
		}
		/*
		 * coeffs->rho21v3   = (-567.*chiA*chiA*chiA -
		 * 1701.*chiA*chiA*chiS*dM + chiA*(-4708. + 1701.*chiS*chiS -
		 * 2648.*eta)*(-1. + 4.*eta) + chiS* dM3 *(4708. -
		 * 567.*chiS*chiS + 1816.*eta))/(2688.*dM3);
		 */
		coeffs->rho21v4 = -47009. / 56448. - (865. * a2) / 1792. - (405. * a2 * a2) / 2048. - (10993. * eta) / 14112.
			+ (617. * eta2) / 4704.;
		coeffs->rho21v5 = (-98635. * a) / 75264. + (2031. * a * a2) / 7168. - (1701. * a2 * a3) / 8192.;
		coeffs->rho21v6 = 7613184941. / 2607897600. + (9032393. * a2) / 1806336. + (3897. * a2 * a2) / 16384.
			- (15309. * a3 * a3) / 65536.;
		coeffs->rho21v6l = -107. / 105.;
		coeffs->rho21v7 = (-3859374457. * a) / 1159065600. - (55169. * a3) / 16384.
			+ (18603. * a2 * a3) / 65536. - (72171. * a2 * a2 * a3) / 262144.;
		coeffs->rho21v7l = 107. * a / 140.;
		coeffs->rho21v8 = -1168617463883. / 911303737344.;
		coeffs->rho21v8l = 6313. / 5880.;
		coeffs->rho21v10 = -63735873771463. / 16569158860800.;
		coeffs->rho21v10l = 5029963. / 5927040.;

		coeffs->f21v1 = (-3. * (chiS + chiA / dM)) / (2.);
		switch (SpinAlignedEOBversion) {
		case 1:
			coeffs->f21v3 = 0.0;
			break;
		case 2:
			coeffs->f21v3 = (chiS * dM * (427. + 79. * eta) + chiA * (147. + 280. * dM * dM + 1251. * eta)) / 84. / dM;
			break;
        case 3:
            coeffs->f21v3 = (chiS * dM * (427. + 79. * eta) + chiA * (147. + 280. * dM * dM + 1251. * eta)) / 84. / dM;
            break;
		default:
			XLALPrintError("XLAL Error - %s: wrong SpinAlignedEOBversion value, must be 1 or 2!\n", __func__);
			XLAL_ERROR(XLAL_EINVAL);
			break;
		}
	} else {
		coeffs->f21v1 = -3. * chiA / 2.;
		switch (SpinAlignedEOBversion) {
		case 1:
			coeffs->f21v3 = 0.0;
			break;
		case 2:
			coeffs->f21v3 = (chiS * dM * (427. + 79. * eta) + chiA * (147. + 280. * dM * dM + 1251. * eta)) / 84.;
			break;
        case 3:
            coeffs->f21v3 = (chiS * dM * (427. + 79. * eta) + chiA * (147. + 280. * dM * dM + 1251. * eta)) / 84.;
            break;
		default:
			XLALPrintError("XLAL Error - %s: wrong SpinAlignedEOBversion value, must be 1 or 2!\n", __func__);
			XLAL_ERROR(XLAL_EINVAL);
			break;
		}
	}

	/*
	 * l = 3, Eqs. A9a - A9c for rho, Eqs. A15b and A15c for f, Eqs. 22 -
	 * 24 of DIN and Eqs. 27c - 27e of PBFRT for delta
	 */
	if (dM2) {
		coeffs->delta33vh3 = 13. / 10.;
		coeffs->delta33vh6 = (-81. * aDelta) / 20. + (39. * LAL_PI) / 7.;
		coeffs->delta33vh9 = -227827. / 3000. + (78. * LAL_PI * LAL_PI) / 7.;
		coeffs->delta33v5 = -80897. * eta / 2430.;

		coeffs->rho33v2 = -7. / 6. + (2. * eta) / 3.;
		//coeffs->rho33v3 = (chiS * dM * (-4. + 5. * eta) + chiA * (-4. + 19. * eta)) / (6. * dM);
		coeffs->rho33v3 = 0.0;
		coeffs->rho33v4 = -6719. / 3960. + a2 / 2. - (1861. * eta) / 990. + (149. * eta2) / 330.;
		coeffs->rho33v5 = (-4. * a) / 3.;
		coeffs->rho33v6 = 3203101567. / 227026800. + (5. * a2) / 36.;
		coeffs->rho33v6l = -26. / 7.;
		coeffs->rho33v7 = (5297. * a) / 2970. + a * a2 / 3.;
		coeffs->rho33v8 = -57566572157. / 8562153600.;
		coeffs->rho33v8l = 13. / 3.;

		coeffs->f33v3 = (chiS * dM * (-4. + 5. * eta) + chiA * (-4. + 19. * eta)) / (2. * dM);
	} else {
		coeffs->f33v3 = chiA * 3. / 8.;
	}

	coeffs->delta32vh3 = (10. + 33. * eta) / (-15. * m1Plus3eta);
	coeffs->delta32vh4 = 4. * aDelta;
	coeffs->delta32vh6 = (-136. * aDelta) / 45. + (52. * LAL_PI) / 21.;
	coeffs->delta32vh9 = -9112. / 405. + (208. * LAL_PI * LAL_PI) / 63.;

	coeffs->rho32v = (4. * chiS * eta) / (-3. * m1Plus3eta);
	coeffs->rho32v2 = (-4. * a2 * eta2) / (9. * m1Plus3eta2) + (328. - 1115. * eta
				       + 320. * eta2) / (270. * m1Plus3eta);
	coeffs->rho32v3 = (2. * (45. * a * m1Plus3eta3
	     - a * eta * (328. - 2099. * eta + 5. * (733. + 20. * a2) * eta2
			  - 960. * eta3))) / (405. * m1Plus3eta3);
	coeffs->rho32v3 = 2. / 9. * a;
	coeffs->rho32v4 = a2 / 3. + (-1444528.
		       + 8050045. * eta - 4725605. * eta2 - 20338960. * eta3
		       + 3085640. * eta2 * eta2) / (1603800. * m1Plus3eta2);
	coeffs->rho32v5 = (-2788. * a) / 1215.;
	coeffs->rho32v6 = 5849948554. / 940355325. + (488. * a2) / 405.;
	coeffs->rho32v6l = -104. / 63.;
	coeffs->rho32v8 = -10607269449358. / 3072140846775.;
	coeffs->rho32v8l = 17056. / 8505.;

	if (dM2) {
		coeffs->delta31vh3 = 13. / 30.;
		coeffs->delta31vh6 = (61. * aDelta) / 20. + (13. * LAL_PI) / 21.;
		coeffs->delta31vh7 = (-24. * aDelta * aDelta) / 5.;
		coeffs->delta31vh9 = -227827. / 81000. + (26. * LAL_PI * LAL_PI) / 63.;
		coeffs->delta31v5 = -17. * eta / 10.;

		coeffs->rho31v2 = -13. / 18. - (2. * eta) / 9.;
		//coeffs->rho31v3 = (chiA * (-4. + 11. * eta) + chiS * dM * (-4. + 13. * eta)) / (6. * dM);
		coeffs->rho31v3 = 0.0;
		coeffs->rho31v4 = 101. / 7128.
			- (5. * a2) / 6. - (1685. * eta) / 1782. - (829. * eta2) / 1782.;
		coeffs->rho31v5 = (4. * a) / 9.;
		coeffs->rho31v6 = 11706720301. / 6129723600. - (49. * a2) / 108.;
		coeffs->rho31v6l = -26. / 63.;
		coeffs->rho31v7 = (-2579. * a) / 5346. + a * a2 / 9.;
		coeffs->rho31v8 = 2606097992581. / 4854741091200.;
		coeffs->rho31v8l = 169. / 567.;

		coeffs->f31v3 = (chiA * (-4. + 11. * eta) + chiS * dM * (-4. + 13. * eta)) / (2. * dM);
	} else {
		coeffs->f31v3 = -chiA * 5. / 8.;
	}

	/*
	 * l = 4, Eqs. A10a - A10d for delta, Eq. A15d for f Eqs. 25 - 28 of
	 * DIN and Eqs. 27f - 27i of PBFRT for delta
	 */

	coeffs->delta44vh3 = (112. + 219. * eta) / (-120. * m1Plus3eta);
	coeffs->delta44vh6 = (-464. * aDelta) / 75. + (25136. * LAL_PI) / 3465.;

	coeffs->rho44v2 = (1614. - 5870. * eta + 2625. * eta2) / (1320. * m1Plus3eta);
	coeffs->rho44v3 = (chiA * (10. - 39. * eta) * dM + chiS * (10. - 41. * eta
					+ 42. * eta2)) / (15. * m1Plus3eta);
	coeffs->rho44v4 = a2 / 2. + (-511573572.
		+ 2338945704. * eta - 313857376. * eta2 - 6733146000. * eta3
		  + 1252563795. * eta2 * eta2) / (317116800. * m1Plus3eta2);
	coeffs->rho44v5 = (-69. * a) / 55.;
	coeffs->rho44v6 = 16600939332793. / 1098809712000. + (217. * a2) / 3960.;
	coeffs->rho44v6l = -12568. / 3465.;

	if (dM2) {
		coeffs->delta43vh3 = (486. + 4961. * eta) / (810. * (1. - 2. * eta));
		coeffs->delta43vh4 = (11. * aDelta) / 4.;
		coeffs->delta43vh6 = 1571. * LAL_PI / 385.;

		//coeffs->rho43v = (5. * (chiA - chiS * dM) * eta) / (8. * dM * (-1. + 2. * eta));
		coeffs->rho43v = 0.0;
		coeffs->rho43v2 = (222. - 547. * eta + 160. * eta2) / (176. * (-1. + 2. * eta));
		coeffs->rho43v4 = -6894273. / 7047040. + (3. * a2) / 8.;
		coeffs->rho43v5 = (-12113. * a) / 6160.;
		coeffs->rho43v6 = 1664224207351. / 195343948800.;
		coeffs->rho43v6l = -1571. / 770.;

		coeffs->f43v = (5. * (chiA - chiS * dM) * eta) / (2. * dM * (-1. + 2. * eta));
	} else {
		coeffs->f43v = -5. * chiA / 4.;
	}

	coeffs->delta42vh3 = (7. * (1. + 6. * eta)) / (-15. * m1Plus3eta);
	coeffs->delta42vh6 = (212. * aDelta) / 75. + (6284. * LAL_PI) / 3465.;

	coeffs->rho42v2 = (1146. - 3530. * eta + 285. * eta2) / (1320. * m1Plus3eta);
	coeffs->rho42v3 = (chiA * (10. - 21. * eta) * dM + chiS * (10. - 59. * eta
					+ 78. * eta2)) / (15. * m1Plus3eta);
	coeffs->rho42v4 = a2 / 2. + (-114859044. + 295834536. * eta + 1204388696. * eta2 - 3047981160. * eta3
		   - 379526805. * eta2 * eta2) / (317116800. * m1Plus3eta2);
	coeffs->rho42v5 = (-7. * a) / 110.;
	coeffs->rho42v6 = 848238724511. / 219761942400. + (2323. * a2) / 3960.;
	coeffs->rho42v6l = -3142. / 3465.;

	if (dM2) {
		coeffs->delta41vh3 = (2. + 507. * eta) / (10. * (1. - 2. * eta));
		coeffs->delta41vh4 = (11. * aDelta) / 12.;
		coeffs->delta41vh6 = 1571. * LAL_PI / 3465.;

		//coeffs->rho41v = (5. * (chiA - chiS * dM) * eta) / (8. * dM * (-1. + 2. * eta));
		coeffs->rho41v = 0.0;
		coeffs->rho41v2 = (602. - 1385. * eta + 288. * eta2) / (528. * (-1. + 2. * eta));
		coeffs->rho41v4 = -7775491. / 21141120. + (3. * a2) / 8.;
		coeffs->rho41v5 = (-20033. * a) / 55440. - (5 * a * a2) / 6.;
		coeffs->rho41v6 = 1227423222031. / 1758095539200.;
		coeffs->rho41v6l = -1571. / 6930.;

		coeffs->f41v = (5. * (chiA - chiS * dM) * eta) / (2. * dM * (-1. + 2. * eta));
	} else {
		coeffs->f41v = -5. * chiA / 4.;
	}

	/*
	 * l = 5, Eqs. A11a - A11e for rho, Eq. 29 of DIN and Eqs. E1a and
	 * E1b of PBFRT for delta
	 */
	if (dM2) {
		coeffs->delta55vh3 = (96875. + 857528. * eta) / (131250. * (1 - 2 * eta));

		coeffs->rho55v2 = (487. - 1298. * eta + 512. * eta2) / (390. * (-1. + 2. * eta));
		coeffs->rho55v3 = (-2. * a) / 3.;
		coeffs->rho55v4 = -3353747. / 2129400. + a2 / 2.;
		coeffs->rho55v5 = -241. * a / 195.;
	}
	coeffs->delta54vh3 = 8. / 15.;
	coeffs->delta54vh4 = 12. * aDelta / 5.;

	coeffs->rho54v2 = (-17448. + 96019. * eta - 127610. * eta2
		  + 33320. * eta3) / (13650. * (1. - 5. * eta + 5. * eta2));
	coeffs->rho54v3 = (-2. * a) / 15.;
	coeffs->rho54v4 = -16213384. / 15526875. + (2. * a2) / 5.;

	if (dM2) {
		coeffs->delta53vh3 = 31. / 70.;

		coeffs->rho53v2 = (375. - 850. * eta + 176. * eta2) / (390. * (-1. + 2. * eta));
		coeffs->rho53v3 = (-2. * a) / 3.;
		coeffs->rho53v4 = -410833. / 709800. + a2 / 2.;
		coeffs->rho53v5 = -103. * a / 325.;
	}
	coeffs->delta52vh3 = 4. / 15.;
	coeffs->delta52vh4 = 6. * aDelta / 5.;

	coeffs->rho52v2 = (-15828. + 84679. * eta - 104930. * eta2
		  + 21980. * eta3) / (13650. * (1. - 5. * eta + 5. * eta2));
	coeffs->rho52v3 = (-2. * a) / 15.;
	coeffs->rho52v4 = -7187914. / 15526875. + (2. * a2) / 5.;

	if (dM2) {
		coeffs->delta51vh3 = 31. / 210.;

		coeffs->rho51v2 = (319. - 626. * eta + 8. * eta2) / (390. * (-1. + 2. * eta));
		coeffs->rho51v3 = (-2. * a) / 3.;
		coeffs->rho51v4 = -31877. / 304200. + a2 / 2.;
		coeffs->rho51v5 = 139. * a / 975.;
	}
	/*
	 * l = 6, Eqs. A12a - A12f for rho, Eqs. E1c and E1d of PBFRT for
	 * delta
	 */

	coeffs->delta66vh3 = 43. / 70.;

	coeffs->rho66v2 = (-106. + 602. * eta - 861. * eta2
		       + 273. * eta3) / (84. * (1. - 5. * eta + 5. * eta2));
	coeffs->rho66v3 = (-2. * a) / 3.;
	coeffs->rho66v4 = -1025435. / 659736. + a2 / 2.;

	if (dM2) {
		coeffs->delta65vh3 = 10. / 21.;

		coeffs->rho65v2 = (-185. + 838. * eta - 910. * eta2
				+ 220. * eta3) / (144. * (dM2 + 3. * eta2));
		coeffs->rho65v3 = -2. * a / 9.;
	}
	coeffs->delta64vh3 = 43. / 105.;

	coeffs->rho64v2 = (-86. + 462. * eta - 581. * eta2
		       + 133. * eta3) / (84. * (1. - 5. * eta + 5. * eta2));
	coeffs->rho64v3 = (-2. * a) / 3.;
	coeffs->rho64v4 = -476887. / 659736. + a2 / 2.;

	if (dM2) {
		coeffs->delta63vh3 = 2. / 7.;

		coeffs->rho63v2 = (-169. + 742. * eta - 750. * eta2
				+ 156. * eta3) / (144. * (dM2 + 3. * eta2));
		coeffs->rho63v3 = -2. * a / 9.;
	}
	coeffs->delta62vh3 = 43. / 210.;

	coeffs->rho62v2 = (-74. + 378. * eta - 413. * eta2
			+ 49. * eta3) / (84. * (1. - 5. * eta + 5. * eta2));
	coeffs->rho62v3 = (-2. * a) / 3.;
	coeffs->rho62v4 = -817991. / 3298680. + a2 / 2.;

	if (dM2) {
		coeffs->delta61vh3 = 2. / 21.;

		coeffs->rho61v2 = (-161. + 694. * eta - 670. * eta2
				+ 124. * eta3) / (144. * (dM2 + 3. * eta2));
		coeffs->rho61v3 = -2. * a / 9.;
	}
	/*
	 * l = 7, Eqs. A13a - A13g for rho, Eqs. E1e and E1f of PBFRT for
	 * delta
	 */
	if (dM2) {
		coeffs->delta77vh3 = 19. / 36.;

		coeffs->rho77v2 = (-906. + 4246. * eta - 4963. * eta2
			       + 1380. * eta3) / (714. * (dM2 + 3. * eta2));
		coeffs->rho77v3 = -2. * a / 3.;
	}
	coeffs->rho76v2 = (2144. - 16185. * eta + 37828. * eta2 - 29351. * eta3
		 + 6104. * eta2 * eta2) / (1666. * (-1 + 7 * eta - 14 * eta2
						    + 7 * eta3));

	if (dM2) {
		coeffs->delta75vh3 = 95. / 252.;

		coeffs->rho75v2 = (-762. + 3382. * eta - 3523. * eta2
				+ 804. * eta3) / (714. * (dM2 + 3. * eta2));
		coeffs->rho75v3 = -2. * a / 3.;
	}
	coeffs->rho74v2 = (17756. - 131805. * eta + 298872. * eta2 - 217959. * eta3
	    + 41076. * eta2 * eta2) / (14994. * (-1. + 7. * eta - 14. * eta2
						 + 7. * eta3));

	if (dM2) {
		coeffs->delta73vh3 = 19. / 84.;

		coeffs->rho73v2 = (-666. + 2806. * eta - 2563. * eta2
				+ 420. * eta3) / (714. * (dM2 + 3. * eta2));
		coeffs->rho73v3 = -2. * a / 3.;
	}
	coeffs->rho72v2 = (16832. - 123489. * eta + 273924. * eta2 - 190239. * eta3
	    + 32760. * eta2 * eta2) / (14994. * (-1. + 7. * eta - 14. * eta2
						 + 7. * eta3));

	if (dM2) {
		coeffs->delta71vh3 = 19. / 252.;

		coeffs->rho71v2 = (-618. + 2518. * eta - 2083. * eta2
				+ 228. * eta3) / (714. * (dM2 + 3. * eta2));
		coeffs->rho71v3 = -2. * a / 3.;
	}
	/* l = 8, Eqs. A14a - A14h */

	coeffs->rho88v2 = (3482. - 26778. * eta + 64659. * eta2 - 53445. * eta3
	     + 12243. * eta2 * eta2) / (2736. * (-1. + 7. * eta - 14. * eta2
						 + 7. * eta3));

	if (dM2) {
		coeffs->rho87v2 = (23478. - 154099. * eta + 309498. * eta2 - 207550. * eta3
		+ 38920 * eta2 * eta2) / (18240. * (-1 + 6 * eta - 10 * eta2
						    + 4 * eta3));
	}
	coeffs->rho86v2 = (1002. - 7498. * eta + 17269. * eta2 - 13055. * eta3
	       + 2653. * eta2 * eta2) / (912. * (-1. + 7. * eta - 14. * eta2
						 + 7. * eta3));

	if (dM2) {
		coeffs->rho85v2 = (4350. - 28055. * eta + 54642. * eta2 - 34598. * eta3
				   + 6056. * eta2 * eta2) / (3648. * (-1. + 6. * eta - 10. * eta2
							      + 4. * eta3));
	}
	coeffs->rho84v2 = (2666. - 19434. * eta + 42627. * eta2 - 28965. * eta3
	      + 4899. * eta2 * eta2) / (2736. * (-1. + 7. * eta - 14. * eta2
						 + 7. * eta3));

	if (dM2) {
		coeffs->rho83v2 = (20598. - 131059. * eta + 249018. * eta2 - 149950. * eta3
				   + 24520. * eta2 * eta2) / (18240. * (-1. + 6. * eta - 10. * eta2
							      + 4. * eta3));
	}
	coeffs->rho82v2 = (2462. - 17598. * eta + 37119. * eta2 - 22845. * eta3
	      + 3063. * eta2 * eta2) / (2736. * (-1. + 7. * eta - 14. * eta2
						 + 7. * eta3));

	if (dM2) {
		coeffs->rho81v2 = (20022. - 126451. * eta + 236922. * eta2 - 138430. * eta3
				   + 21640. * eta2 * eta2) / (18240. * (-1. + 6. * eta - 10. * eta2
							      + 4. * eta3));
	}
	/* All relevant coefficients should be set, so we return */

	return XLAL_SUCCESS;
}


/**
 * This function calculates coefficients for hlm mode factorized-resummed
 * waveform, without their spin-dependent parts.
 * The coefficients are pre-computed and stored in the SpinEOBParams structure.
 * Appendix of the paper, and papers DIN (PRD 79, 064004 (2009)) and PBFRT (PRD 83, 064003 (2011)).
 */

UNUSED static int 
XLALSimIMREOBCalcPrecNoSpinFacWaveformCoefficients(
					   FacWaveformCoeffs * const coeffs,	/**< OUTPUT, pre-computed waveform coefficients */
						   const REAL8 m1,	/**< mass 1 */
						   const REAL8 m2,	/**< mass 2 */
						   const REAL8 eta,	/**< symmetric mass ratio */
						   UNUSED const REAL8 a,	/**< Kerr spin parameter for test-particle terms */
						   const REAL8 chiS,	/**< (chi1+chi2)/2 */
						   const REAL8 chiA,	/**< (chi1-chi2)/2 */
					   const UINT4 SpinAlignedEOBversion	/**< 1 for SEOBNRv1; 2 for SEOBNRv2 */
)
{
	REAL8		eta2 = eta * eta;
	REAL8		eta3 = eta2 * eta;

	REAL8		dM      , dM2;
	//dM3;
	//REAL8 a2, a3;

	/* Combination which appears a lot */
	REAL8		m1Plus3eta, m1Plus3eta2;
	//m1Plus3eta3;

	dM2 = 1. - 4. * eta;

	//printf("****************************** a = %e *********************************\n", a);

	/* Check that deltaM has a reasonable value */
	if (dM2 < 0) {
		XLALPrintError("XLAL Error - %s: eta seems to be < 0.25 - this isn't allowed!\n", __func__);
		XLAL_ERROR(XLAL_EINVAL);
	}
	dM = sqrt(dM2);
	if (m1 < m2) {
		dM = -dM;
	}
	//dM3 = dM2 * dM;
	REAL8		temp = chiS + chiA;
	temp *= 2;
	//a2 = a * a;
	//a3 = a2 * a;

	m1Plus3eta = -1. + 3. * eta;
	m1Plus3eta2 = m1Plus3eta * m1Plus3eta;
	//m1Plus3eta3 = m1Plus3eta * m1Plus3eta2;

	/* Initialize all coefficients to zero */
	/* This is important, as we will not set some if dM is zero */
	memset(coeffs, 0, sizeof(FacWaveformCoeffs));


	/*
	 * l = 2, Eqs. A8a and A8b for rho, Eq. A15a for f, Eqs. 20 and 21 of
	 * DIN and Eqs. 27a and 27b of PBFRT for delta
	 */

	coeffs->delta22vh3 = 7. / 3.;
	coeffs->delta22vh6 = (428. * LAL_PI) / 105.;
	//+(-4. * a) / 3.;
	coeffs->delta22v8 = 0.;
	//(20. * a) / 63.;
	coeffs->delta22vh9 = -2203. / 81. + (1712. * LAL_PI * LAL_PI) / 315.;
	coeffs->delta22v5 = -24. * eta;
	coeffs->delta22v6 = 0.0;
    if (SpinAlignedEOBversion == 2){
		//&&chiS + chiA * dM / (1. - 2. * eta) > 0.) {
		coeffs->delta22v6 = 0.;
		//-580. * eta * (chiS + chiA * dM / (1. - 2. * eta));
		}

	coeffs->rho22v2 = -43. / 42. + (55. * eta) / 84.;
	coeffs->rho22v3 = 0.;
	//(-2. * (chiS + chiA * dM - chiS * eta)) / 3.;
	switch (SpinAlignedEOBversion) {
	case 1:
		coeffs->rho22v4 = -20555. / 10584. // +0.5 * a2
			- (33025. * eta) / 21168. + (19583. * eta2) / 42336.;
		break;
	case 2:
		coeffs->rho22v4 = -20555. / 10584. // +0.5 * (chiS + chiA * dM) * (chiS + chiA * dM)
			- (33025. * eta) / 21168. + (19583. * eta2) / 42336.;
		break;
	default:
		XLALPrintError("XLAL Error - %s: wrong SpinAlignedEOBversion value, must be 1 or 2!\n", __func__);
		XLAL_ERROR(XLAL_EINVAL);
		break;
	}
	coeffs->rho22v5 = 0.;
	//(-34. * a) / 21.;
	coeffs->rho22v6 = 1556919113. / 122245200. // +(89. * a2) / 252.
		- (48993925. * eta) / 9779616.
		- (6292061. * eta2) / 3259872. + (10620745. * eta3) / 39118464.
		+ (41. * eta * LAL_PI * LAL_PI) / 192.;
	coeffs->rho22v6l = -428. / 105.;
	coeffs->rho22v7 = 0.;
	//(18733. * a) / 15876. + a * a2 / 3.;
	switch (SpinAlignedEOBversion) {
	case 1:
		coeffs->rho22v8 = eta * (-5.6 - 117.6 * eta) - 387216563023. / 160190110080.;
		//+(18353. * a2) / 21168. - a2 * a2 / 8.;
		break;
	case 2:
		coeffs->rho22v8 = -387216563023. / 160190110080.;
		//+(18353. * a2) / 21168. - a2 * a2 / 8.;
		break;
	}
	coeffs->rho22v8l = 9202. / 2205.;
	coeffs->rho22v10 = -16094530514677. / 533967033600.;
	coeffs->rho22v10l = 439877. / 55566.;

	/*
	 * printf( "v2 = %.16e, v3 = %.16e, v4 = %.16e, v5 = %.16e\n" "v6 =
	 * %.16e, v6l = %.16e v7 = %.16e v8 = %.16e, v8l = %.16e v10 = %.16e
	 * v10l = %.16e\n", coeffs->rho22v2, coeffs->rho22v3,
	 * coeffs->rho22v4, coeffs->rho22v5, coeffs->rho22v6,
	 * coeffs->rho22v6l, coeffs->rho22v7, coeffs->rho22v8,
	 * coeffs->rho22v8l, coeffs->rho22v10, coeffs->rho22v10l );
	 */

	if (dM2) {
		coeffs->delta21vh3 = 2. / 3.;
		coeffs->delta21vh6 = /* (-17.*a)/35. + */ (107. * LAL_PI) / 105.;
		coeffs->delta21vh7 = 0.;
		//(3. * a2) / 140.;
		coeffs->delta21vh9 = -272. / 81. + (214. * LAL_PI * LAL_PI) / 315.;
		coeffs->delta21v5 = -493. * eta / 42.;

		//coeffs->rho21v1 = (-3. * (chiS + chiA / dM)) / (4.);
		coeffs->rho21v1 = 0.0;
		//coeffs->rho21v2 = -59. / 56 - (9. * chiAPlusChiSdM * chiAPlusChiSdM) / (32. * dM2) + (23. * eta) / 84.;
		switch (SpinAlignedEOBversion) {
		case 1:
			coeffs->rho21v2 = -59. / 56 + (23. * eta) / 84.;
			//-9. / 32. * a2;
			coeffs->rho21v3 = 0.;
			//1177. / 672. * a - 27. / 128. * a3;
			break;
		case 2:
			coeffs->rho21v2 = -59. / 56 + (23. * eta) / 84.;
			coeffs->rho21v3 = 0.0;
			break;
		default:
			XLALPrintError("XLAL Error - %s: wrong SpinAlignedEOBversion value, must be 1 or 2!\n", __func__);
			XLAL_ERROR(XLAL_EINVAL);
			break;
		}
		/*
		 * coeffs->rho21v3   = (-567.*chiA*chiA*chiA -
		 * 1701.*chiA*chiA*chiS*dM + chiA*(-4708. + 1701.*chiS*chiS -
		 * 2648.*eta)*(-1. + 4.*eta) + chiS* dM3 *(4708. -
		 * 567.*chiS*chiS + 1816.*eta))/(2688.*dM3);
		 */
		coeffs->rho21v4 = -47009. / 56448.	/*- (865.*a2)/1792. - (405.*a2*a2)/2048.*/
			- (10993. * eta) / 14112.
			+ (617. * eta2) / 4704.;
		coeffs->rho21v5 = 0.;
		//(-98635. * a) / 75264. + (2031. * a * a2) / 7168. - (1701. * a2 * a3) / 8192.;
		coeffs->rho21v6 = 7613184941. / 2607897600.;
		//+(9032393. * a2) / 1806336. + (3897. * a2 * a2) / 16384. - (15309. * a3 * a3) / 65536.;
		coeffs->rho21v6l = -107. / 105.;
		coeffs->rho21v7 = 0.;
		//(-3859374457. * a) / 1159065600. - (55169. * a3) / 16384.
			// +(18603. * a2 * a3) / 65536. - (72171. * a2 * a2 * a3) / 262144.;
		coeffs->rho21v7l = 0.;
		//107. * a / 140.;
		coeffs->rho21v8 = -1168617463883. / 911303737344.;
		coeffs->rho21v8l = 6313. / 5880.;
		coeffs->rho21v10 = -63735873771463. / 16569158860800.;
		coeffs->rho21v10l = 5029963. / 5927040.;

		coeffs->f21v1 = 0.;
		//(-3. * (chiS + chiA / dM)) / (2.);
		switch (SpinAlignedEOBversion) {
		case 1:
			coeffs->f21v3 = 0.0;
			break;
		case 2:
			coeffs->f21v3 = 0.;
			//(chiS * dM * (427. + 79. * eta) + chiA * (147. + 280. * dM * dM + 1251. * eta)) / 84. / dM;
			break;
		default:
			XLALPrintError("XLAL Error - %s: wrong SpinAlignedEOBversion value, must be 1 or 2!\n", __func__);
			XLAL_ERROR(XLAL_EINVAL);
			break;
		}
	} else {
		coeffs->f21v1 = 0.;
		//-3. * chiA / 2.;
		coeffs->f21v3 = 0.;
		//(chiS * dM * (427. + 79. * eta) + chiA * (147. + 280. * dM * dM + 1251. * eta)) / 84.;
	}

	/*
	 * l = 3, Eqs. A9a - A9c for rho, Eqs. A15b and A15c for f, Eqs. 22 -
	 * 24 of DIN and Eqs. 27c - 27e of PBFRT for delta
	 */
	if (dM2) {
		coeffs->delta33vh3 = 13. / 10.;
		coeffs->delta33vh6 = /* (-81.*a)/20. + */ (39. * LAL_PI) / 7.;
		coeffs->delta33vh9 = -227827. / 3000. + (78. * LAL_PI * LAL_PI) / 7.;
		coeffs->delta33v5 = -80897. * eta / 2430.;

		coeffs->rho33v2 = -7. / 6. + (2. * eta) / 3.;
		//coeffs->rho33v3 = (chiS * dM * (-4. + 5. * eta) + chiA * (-4. + 19. * eta)) / (6. * dM);
		coeffs->rho33v3 = 0.0;
		coeffs->rho33v4 = -6719. / 3960. /* + a2/2. */ - (1861. * eta) / 990. + (149. * eta2) / 330.;
		coeffs->rho33v5 = 0.;
		//(-4. * a) / 3.;
		coeffs->rho33v6 = 3203101567. / 227026800.;
		//+(5. * a2) / 36.;
		coeffs->rho33v6l = -26. / 7.;
		coeffs->rho33v7 = 0.;
		//(5297. * a) / 2970. + a * a2 / 3.;
		coeffs->rho33v8 = -57566572157. / 8562153600.;
		coeffs->rho33v8l = 13. / 3.;

		coeffs->f33v3 = 0.;
		//(chiS * dM * (-4. + 5. * eta) + chiA * (-4. + 19. * eta)) / (2. * dM);
	} else {
		coeffs->f33v3 = 0.;
		//chiA * 3. / 8.;
	}

	coeffs->delta32vh3 = (10. + 33. * eta) / (-15. * m1Plus3eta);
	coeffs->delta32vh4 = 0.;
	//4. * a;
	coeffs->delta32vh6 = /* (-136.*a)/45. + */ (52. * LAL_PI) / 21.;
	coeffs->delta32vh9 = -9112. / 405. + (208. * LAL_PI * LAL_PI) / 63.;

	coeffs->rho32v = 0.;
	//(4. * chiS * eta) / (-3. * m1Plus3eta);
	coeffs->rho32v2 = /* (-4.*a2*eta2)/(9.*m1Plus3eta2) + */ (328. - 1115. * eta
				       + 320. * eta2) / (270. * m1Plus3eta);
	coeffs->rho32v3 = 0.;
	//(2. * (45. * a * m1Plus3eta3
	   // -a * eta * (328. - 2099. * eta + 5. * (733. + 20. * a2) * eta2
			  // -960. * eta3))) / (405. * m1Plus3eta3);
	coeffs->rho32v4 = /* a2/3. + */ (-1444528.
		       + 8050045. * eta - 4725605. * eta2 - 20338960. * eta3
		       + 3085640. * eta2 * eta2) / (1603800. * m1Plus3eta2);
	coeffs->rho32v5 = 0.;
	//(-2788. * a) / 1215.;
	coeffs->rho32v6 = 5849948554. / 940355325.;
	//+(488. * a2) / 405.;
	coeffs->rho32v6l = -104. / 63.;
	coeffs->rho32v8 = -10607269449358. / 3072140846775.;
	coeffs->rho32v8l = 17056. / 8505.;

	if (dM2) {
		coeffs->delta31vh3 = 13. / 30.;
		coeffs->delta31vh6 = /* (61.*a)/20. + */ (13. * LAL_PI) / 21.;
		coeffs->delta31vh7 = 0.;
		//(-24. * a2) / 5.;
		coeffs->delta31vh9 = -227827. / 81000. + (26. * LAL_PI * LAL_PI) / 63.;
		coeffs->delta31v5 = -17. * eta / 10.;

		coeffs->rho31v2 = -13. / 18. - (2. * eta) / 9.;
		//coeffs->rho31v3 = (chiA * (-4. + 11. * eta) + chiS * dM * (-4. + 13. * eta)) / (6. * dM);
		coeffs->rho31v3 = 0.0;
		coeffs->rho31v4 = 101. / 7128.	/*- (5.*a2)/6.*/
			- (1685. * eta) / 1782. - (829. * eta2) / 1782.;
		coeffs->rho31v5 = 0.;
		//(4. * a) / 9.;
		coeffs->rho31v6 = 11706720301. / 6129723600.;
		//-(49. * a2) / 108.;
		coeffs->rho31v6l = -26. / 63.;
		coeffs->rho31v7 = 0.;
		//(-2579. * a) / 5346. + a * a2 / 9.;
		coeffs->rho31v8 = 2606097992581. / 4854741091200.;
		coeffs->rho31v8l = 169. / 567.;

		coeffs->f31v3 = 0.;
		//(chiA * (-4. + 11. * eta) + chiS * dM * (-4. + 13. * eta)) / (2. * dM);
	} else {
		coeffs->f31v3 = 0.;
		//-chiA * 5. / 8.;
	}

	/*
	 * l = 4, Eqs. A10a - A10d for delta, Eq. A15d for f Eqs. 25 - 28 of
	 * DIN and Eqs. 27f - 27i of PBFRT for delta
	 */

	coeffs->delta44vh3 = (112. + 219. * eta) / (-120. * m1Plus3eta);
	coeffs->delta44vh6 = /* (-464.*a)/75. + */ (25136. * LAL_PI) / 3465.;

	coeffs->rho44v2 = (1614. - 5870. * eta + 2625. * eta2) / (1320. * m1Plus3eta);
	coeffs->rho44v3 = 0.;
	//(chiA * (10. - 39. * eta) * dM + chiS * (10. - 41. * eta
				      // +42. * eta2)) / (15. * m1Plus3eta);
	coeffs->rho44v4 = /* a2/2. + */ (-511573572.
		+ 2338945704. * eta - 313857376. * eta2 - 6733146000. * eta3
		  + 1252563795. * eta2 * eta2) / (317116800. * m1Plus3eta2);
	coeffs->rho44v5 = 0.;
	//(-69. * a) / 55.;
	coeffs->rho44v6 = 16600939332793. / 1098809712000.;
	//+(217. * a2) / 3960.;
	coeffs->rho44v6l = -12568. / 3465.;

	if (dM2) {
		coeffs->delta43vh3 = (486. + 4961. * eta) / (810. * (1. - 2. * eta));
		coeffs->delta43vh4 = 0.;
		//(11. * a) / 4.;
		coeffs->delta43vh6 = 1571. * LAL_PI / 385.;

		//coeffs->rho43v = (5. * (chiA - chiS * dM) * eta) / (8. * dM * (-1. + 2. * eta));
		coeffs->rho43v = 0.0;
		coeffs->rho43v2 = (222. - 547. * eta + 160. * eta2) / (176. * (-1. + 2. * eta));
		coeffs->rho43v4 = -6894273. / 7047040.;
		//+(3. * a2) / 8.;
		coeffs->rho43v5 = 0.;
		//(-12113. * a) / 6160.;
		coeffs->rho43v6 = 1664224207351. / 195343948800.;
		coeffs->rho43v6l = -1571. / 770.;

		coeffs->f43v = 0.;
		//(5. * (chiA - chiS * dM) * eta) / (2. * dM * (-1. + 2. * eta));
	} else {
		coeffs->f43v = 0.;
		//-5. * chiA / 4.;
	}

	coeffs->delta42vh3 = (7. * (1. + 6. * eta)) / (-15. * m1Plus3eta);
	coeffs->delta42vh6 = /* (212.*a)/75. + */ (6284. * LAL_PI) / 3465.;

	coeffs->rho42v2 = (1146. - 3530. * eta + 285. * eta2) / (1320. * m1Plus3eta);
	coeffs->rho42v3 = 0.;
	//(chiA * (10. - 21. * eta) * dM + chiS * (10. - 59. * eta
				      // +78. * eta2)) / (15. * m1Plus3eta);
	coeffs->rho42v4 = /* a2/2. + */ (-114859044. + 295834536. * eta + 1204388696. * eta2 - 3047981160. * eta3
		   - 379526805. * eta2 * eta2) / (317116800. * m1Plus3eta2);
	coeffs->rho42v5 = 0.;
	//(-7. * a) / 110.;
	coeffs->rho42v6 = 848238724511. / 219761942400.;
	//+(2323. * a2) / 3960.;
	coeffs->rho42v6l = -3142. / 3465.;

	if (dM2) {
		coeffs->delta41vh3 = (2. + 507. * eta) / (10. * (1. - 2. * eta));
		coeffs->delta41vh4 = 0.;
		//(11. * a) / 12.;
		coeffs->delta41vh6 = 1571. * LAL_PI / 3465.;

		//coeffs->rho41v = (5. * (chiA - chiS * dM) * eta) / (8. * dM * (-1. + 2. * eta));
		coeffs->rho41v = 0.0;
		coeffs->rho41v2 = (602. - 1385. * eta + 288. * eta2) / (528. * (-1. + 2. * eta));
		coeffs->rho41v4 = -7775491. / 21141120.;
		//+(3. * a2) / 8.;
		coeffs->rho41v5 = 0.;
		//(-20033. * a) / 55440. - (5 * a * a2) / 6.;
		coeffs->rho41v6 = 1227423222031. / 1758095539200.;
		coeffs->rho41v6l = -1571. / 6930.;

		coeffs->f41v = 0.;
		//(5. * (chiA - chiS * dM) * eta) / (2. * dM * (-1. + 2. * eta));
	} else {
		coeffs->f41v = 0.;
		//-5. * chiA / 4.;
	}

	/*
	 * l = 5, Eqs. A11a - A11e for rho, Eq. 29 of DIN and Eqs. E1a and
	 * E1b of PBFRT for delta
	 */
	if (dM2) {
		coeffs->delta55vh3 = (96875. + 857528. * eta) / (131250. * (1 - 2 * eta));

		coeffs->rho55v2 = (487. - 1298. * eta + 512. * eta2) / (390. * (-1. + 2. * eta));
		coeffs->rho55v3 = 0.;
		//(-2. * a) / 3.;
		coeffs->rho55v4 = -3353747. / 2129400.;
		//+a2 / 2.;
		coeffs->rho55v5 = 0.;
		//-241. * a / 195.;
	}
	coeffs->delta54vh3 = 8. / 15.;
	coeffs->delta54vh4 = 0.;
	//12. * a / 5.;

	coeffs->rho54v2 = (-17448. + 96019. * eta - 127610. * eta2
		  + 33320. * eta3) / (13650. * (1. - 5. * eta + 5. * eta2));
	coeffs->rho54v3 = 0.;
	//(-2. * a) / 15.;
	coeffs->rho54v4 = -16213384. / 15526875.;
	//+(2. * a2) / 5.;

	if (dM2) {
		coeffs->delta53vh3 = 31. / 70.;

		coeffs->rho53v2 = (375. - 850. * eta + 176. * eta2) / (390. * (-1. + 2. * eta));
		coeffs->rho53v3 = 0.;
		//(-2. * a) / 3.;
		coeffs->rho53v4 = -410833. / 709800.;
		//+a2 / 2.;
		coeffs->rho53v5 = 0.;
		//-103. * a / 325.;
	}
	coeffs->delta52vh3 = 4. / 15.;
	coeffs->delta52vh4 = 0.;
	//6. * a / 5.;

	coeffs->rho52v2 = (-15828. + 84679. * eta - 104930. * eta2
		  + 21980. * eta3) / (13650. * (1. - 5. * eta + 5. * eta2));
	coeffs->rho52v3 = 0.;
	//(-2. * a) / 15.;
	coeffs->rho52v4 = -7187914. / 15526875.;
	//+(2. * a2) / 5.;

	if (dM2) {
		coeffs->delta51vh3 = 31. / 210.;

		coeffs->rho51v2 = (319. - 626. * eta + 8. * eta2) / (390. * (-1. + 2. * eta));
		coeffs->rho51v3 = 0.;
		//(-2. * a) / 3.;
		coeffs->rho51v4 = -31877. / 304200.;
		//+a2 / 2.;
		coeffs->rho51v5 = 0.;
		//139. * a / 975.;
	}
	/*
	 * l = 6, Eqs. A12a - A12f for rho, Eqs. E1c and E1d of PBFRT for
	 * delta
	 */

	coeffs->delta66vh3 = 43. / 70.;

	coeffs->rho66v2 = (-106. + 602. * eta - 861. * eta2
		       + 273. * eta3) / (84. * (1. - 5. * eta + 5. * eta2));
	coeffs->rho66v3 = 0.;
	//(-2. * a) / 3.;
	coeffs->rho66v4 = -1025435. / 659736.;
	//+a2 / 2.;

	if (dM2) {
		coeffs->delta65vh3 = 10. / 21.;

		coeffs->rho65v2 = (-185. + 838. * eta - 910. * eta2
				+ 220. * eta3) / (144. * (dM2 + 3. * eta2));
		coeffs->rho65v3 = 0.;
		//-2. * a / 9.;
	}
	coeffs->delta64vh3 = 43. / 105.;

	coeffs->rho64v2 = (-86. + 462. * eta - 581. * eta2
		       + 133. * eta3) / (84. * (1. - 5. * eta + 5. * eta2));
	coeffs->rho64v3 = 0.;
	//(-2. * a) / 3.;
	coeffs->rho64v4 = -476887. / 659736.;
	//+a2 / 2.;

	if (dM2) {
		coeffs->delta63vh3 = 2. / 7.;

		coeffs->rho63v2 = (-169. + 742. * eta - 750. * eta2
				+ 156. * eta3) / (144. * (dM2 + 3. * eta2));
		coeffs->rho63v3 = 0.;
		//-2. * a / 9.;
	}
	coeffs->delta62vh3 = 43. / 210.;

	coeffs->rho62v2 = (-74. + 378. * eta - 413. * eta2
			+ 49. * eta3) / (84. * (1. - 5. * eta + 5. * eta2));
	coeffs->rho62v3 = 0.;
	//(-2. * a) / 3.;
	coeffs->rho62v4 = -817991. / 3298680.;
	//+a2 / 2.;

	if (dM2) {
		coeffs->delta61vh3 = 2. / 21.;

		coeffs->rho61v2 = (-161. + 694. * eta - 670. * eta2
				+ 124. * eta3) / (144. * (dM2 + 3. * eta2));
		coeffs->rho61v3 = 0.;
		//-2. * a / 9.;
	}
	/*
	 * l = 7, Eqs. A13a - A13g for rho, Eqs. E1e and E1f of PBFRT for
	 * delta
	 */
	if (dM2) {
		coeffs->delta77vh3 = 19. / 36.;

		coeffs->rho77v2 = (-906. + 4246. * eta - 4963. * eta2
			       + 1380. * eta3) / (714. * (dM2 + 3. * eta2));
		coeffs->rho77v3 = 0.;
		//-2. * a / 3.;
	}
	coeffs->rho76v2 = (2144. - 16185. * eta + 37828. * eta2 - 29351. * eta3
		 + 6104. * eta2 * eta2) / (1666. * (-1 + 7 * eta - 14 * eta2
						    + 7 * eta3));

	if (dM2) {
		coeffs->delta75vh3 = 95. / 252.;

		coeffs->rho75v2 = (-762. + 3382. * eta - 3523. * eta2
				+ 804. * eta3) / (714. * (dM2 + 3. * eta2));
		coeffs->rho75v3 = 0.;
		//-2. * a / 3.;
	}
	coeffs->rho74v2 = (17756. - 131805. * eta + 298872. * eta2 - 217959. * eta3
	    + 41076. * eta2 * eta2) / (14994. * (-1. + 7. * eta - 14. * eta2
						 + 7. * eta3));

	if (dM2) {
		coeffs->delta73vh3 = 19. / 84.;

		coeffs->rho73v2 = (-666. + 2806. * eta - 2563. * eta2
				+ 420. * eta3) / (714. * (dM2 + 3. * eta2));
		coeffs->rho73v3 = 0.;
		//-2. * a / 3.;
	}
	coeffs->rho72v2 = (16832. - 123489. * eta + 273924. * eta2 - 190239. * eta3
	    + 32760. * eta2 * eta2) / (14994. * (-1. + 7. * eta - 14. * eta2
						 + 7. * eta3));

	if (dM2) {
		coeffs->delta71vh3 = 19. / 252.;

		coeffs->rho71v2 = (-618. + 2518. * eta - 2083. * eta2
				+ 228. * eta3) / (714. * (dM2 + 3. * eta2));
		coeffs->rho71v3 = 0.;
		//-2. * a / 3.;
	}
	/* l = 8, Eqs. A14a - A14h */

	coeffs->rho88v2 = (3482. - 26778. * eta + 64659. * eta2 - 53445. * eta3
	     + 12243. * eta2 * eta2) / (2736. * (-1. + 7. * eta - 14. * eta2
						 + 7. * eta3));

	if (dM2) {
		coeffs->rho87v2 = (23478. - 154099. * eta + 309498. * eta2 - 207550. * eta3
		+ 38920 * eta2 * eta2) / (18240. * (-1 + 6 * eta - 10 * eta2
						    + 4 * eta3));
	}
	coeffs->rho86v2 = (1002. - 7498. * eta + 17269. * eta2 - 13055. * eta3
	       + 2653. * eta2 * eta2) / (912. * (-1. + 7. * eta - 14. * eta2
						 + 7. * eta3));

	if (dM2) {
		coeffs->rho85v2 = (4350. - 28055. * eta + 54642. * eta2 - 34598. * eta3
				   + 6056. * eta2 * eta2) / (3648. * (-1. + 6. * eta - 10. * eta2
							      + 4. * eta3));
	}
	coeffs->rho84v2 = (2666. - 19434. * eta + 42627. * eta2 - 28965. * eta3
	      + 4899. * eta2 * eta2) / (2736. * (-1. + 7. * eta - 14. * eta2
						 + 7. * eta3));

	if (dM2) {
		coeffs->rho83v2 = (20598. - 131059. * eta + 249018. * eta2 - 149950. * eta3
				   + 24520. * eta2 * eta2) / (18240. * (-1. + 6. * eta - 10. * eta2
							      + 4. * eta3));
	}
	coeffs->rho82v2 = (2462. - 17598. * eta + 37119. * eta2 - 22845. * eta3
	      + 3063. * eta2 * eta2) / (2736. * (-1. + 7. * eta - 14. * eta2
						 + 7. * eta3));

	if (dM2) {
		coeffs->rho81v2 = (20022. - 126451. * eta + 236922. * eta2 - 138430. * eta3
				   + 21640. * eta2 * eta2) / (18240. * (-1. + 6. * eta - 10. * eta2
							      + 4. * eta3));
	}
	/* All relevant coefficients should be set, so we return */

	return XLAL_SUCCESS;
}


/**
 * This function calculates coefficients for hlm mode factorized-resummed
 * waveform, i.e. only their spin-dependent parts.
 * The coefficients are pre-computed and stored in the SpinEOBParams structure.
 * Appendix of the paper, and papers DIN (PRD 79, 064004 (2009)) and PBFRT (PRD 83, 064003 (2011)).
 */

UNUSED static int 
XLALSimIMREOBCalcPrecSpinFacWaveformCoefficients(
					   FacWaveformCoeffs * const coeffs,	/**< OUTPUT, pre-computed waveform coefficients */
						 const REAL8 m1,	/**< mass 1 */
						 const REAL8 m2,	/**< mass 2 */
						 const REAL8 eta,	/**< symmetric mass ratio */
						 const REAL8 a,	/**< Kerr spin parameter for test-particle terms */
						 const REAL8 chiS,	/**< (chi1+chi2)/2 */
						 const REAL8 chiA,	/**< (chi1-chi2)/2 */
					   const UINT4 SpinAlignedEOBversion	/**< 1 for SEOBNRv1; 2 for SEOBNRv2 */
)
{
	REAL8		eta2 = eta * eta;
	REAL8		eta3 = eta2 * eta;

	REAL8		dM      , dM2;
	//dM3;
	REAL8		a2      , a3;

	/* Combination which appears a lot */
	REAL8		m1Plus3eta, m1Plus3eta2, m1Plus3eta3;

	dM2 = 1. - 4. * eta;

	//printf("****************************** a = %e *********************************\n", a);

	/* Check that deltaM has a reasonable value */
	if (dM2 < 0) {
		XLALPrintError("XLAL Error - %s: eta seems to be < 0.25 - this isn't allowed!\n", __func__);
		XLAL_ERROR(XLAL_EINVAL);
	}
	dM = sqrt(dM2);
	if (m1 < m2) {
		dM = -dM;
	}
	//dM3 = dM2 * dM;

	a2 = a * a;
	a3 = a2 * a;

	m1Plus3eta = -1. + 3. * eta;
	m1Plus3eta2 = m1Plus3eta * m1Plus3eta;
	m1Plus3eta3 = m1Plus3eta * m1Plus3eta2;

	/* Initialize all coefficients to zero */
	/* This is important, as we will not set some if dM is zero */
	memset(coeffs, 0, sizeof(FacWaveformCoeffs));


	/*
	 * l = 2, Eqs. A8a and A8b for rho, Eq. A15a for f, Eqs. 20 and 21 of
	 * DIN and Eqs. 27a and 27b of PBFRT for delta
	 */

	//coeffs->delta22vh3 = 7. / 3.;
	coeffs->delta22vh6S = /* (428.*LAL_PI)/105. + */ (-4. * a) / 3.;
	coeffs->delta22v8S = (20. * a) / 63.;
	//coeffs->delta22vh9 = -2203. / 81. + (1712. * LAL_PI * LAL_PI) / 315.;
	//coeffs->delta22v5 = -24. * eta;
	//coeffs->delta22v6 = 0.0;
	if (SpinAlignedEOBversion == 2 && chiS + chiA * dM / (1. - 2. * eta) > 0.) {
		coeffs->delta22v6S = -540. * eta * (chiS + chiA * dM / (1. - 2. * eta));
//        double chi = (chiS + chiA * dM / (1. - 2. * eta));
//        coeffs->delta22v6 = eta*(1./4.*(1. - 1080.*chi - sqrt((1. - 1080.*chi)*(1. - 1080*chi) + 8.*(13.5 +270.*chi +13.5*chi*chi))));
	}
    if (SpinAlignedEOBversion == 3 /*&& chiS + chiA * dM / (1. - 2. * eta) > 0.*/) {
//        		coeffs->delta22v6S = -540. * eta * (chiS + chiA * dM / (1. - 2. * eta));
        double chi = (chiS + chiA * dM / (1. - 2. * eta));
        coeffs->delta22v6 = eta*(1./4.*(1. - 1080.*chi - sqrt((1. - 1080.*chi)*(1. - 1080*chi) + 8.*(13.5 +270.*chi +13.5*chi*chi))));
    }	//coeffs->rho22v2 = -43. / 42. + (55. * eta) / 84.;
	coeffs->rho22v3S = (-2. * (chiS + chiA * dM - chiS * eta)) / 3.;
	switch (SpinAlignedEOBversion) {
	case 1:
		coeffs->rho22v4S = /*-20555./10584. +*/ 0.5 * a2;
		//-(33025. * eta) / 21168. + (19583. * eta2) / 42336.;
		break;
	case 2:
		coeffs->rho22v4S = /*-20555./10584. +*/ 0.5 * (chiS + chiA * dM) * (chiS + chiA * dM);
		//-(33025. * eta) / 21168. + (19583. * eta2) / 42336.;
		break;
	default:
		XLALPrintError("XLAL Error - %s: wrong SpinAlignedEOBversion value, must be 1 or 2!\n", __func__);
		XLAL_ERROR(XLAL_EINVAL);
		break;
	}
	coeffs->rho22v5S = (-34. * a) / 21.;
	coeffs->rho22v6S = /* 1556919113./122245200. + */ (89. * a2) / 252.;
	//-(48993925. * eta) / 9779616.
		// -(6292061. * eta2) / 3259872. + (10620745. * eta3) / 39118464.
		// +(41. * eta * LAL_PI * LAL_PI) / 192.;
	//coeffs->rho22v6l = -428. / 105.;
	coeffs->rho22v7S = (18733. * a) / 15876. + a * a2 / 3.;
	switch (SpinAlignedEOBversion) {
	case 1:
		coeffs->rho22v8S =	/* eta*(-5.6-117.6*eta)
			    -387216563023./160190110080. + */ (18353. * a2) / 21168. - a2 * a2 / 8.;
		break;
	case 2:
		coeffs->rho22v8S = /*-387216563023./160190110080. +*/ (18353. * a2) / 21168. - a2 * a2 / 8.;
		break;
	}
	//coeffs->rho22v8l = 9202. / 2205.;
	//coeffs->rho22v10 = -16094530514677. / 533967033600.;
	//coeffs->rho22v10l = 439877. / 55566.;

	/*
	 * printf( "v2 = %.16e, v3 = %.16e, v4 = %.16e, v5 = %.16e\n" "v6 =
	 * %.16e, v6l = %.16e v7 = %.16e v8 = %.16e, v8l = %.16e v10 = %.16e
	 * v10l = %.16e\n", coeffs->rho22v2, coeffs->rho22v3,
	 * coeffs->rho22v4, coeffs->rho22v5, coeffs->rho22v6,
	 * coeffs->rho22v6l, coeffs->rho22v7, coeffs->rho22v8,
	 * coeffs->rho22v8l, coeffs->rho22v10, coeffs->rho22v10l );
	 */

	if (dM2) {
		//coeffs->delta21vh3 = 2. / 3.;
		coeffs->delta21vh6S = (-17. * a) / 35.;
		//+(107. * LAL_PI) / 105.;
		coeffs->delta21vh7S = (3. * a2) / 140.;
		//coeffs->delta21vh9 = -272. / 81. + (214. * LAL_PI * LAL_PI) / 315.;
		//coeffs->delta21v5 = -493. * eta / 42.;

		//coeffs->rho21v1 = (-3. * (chiS + chiA / dM)) / (4.);
		//coeffs->rho21v1 = 0.0;
		//coeffs->rho21v2 = -59. / 56 - (9. * chiAPlusChiSdM * chiAPlusChiSdM) / (32. * dM2) + (23. * eta) / 84.;
		switch (SpinAlignedEOBversion) {
		case 1:
			coeffs->rho21v2S = /*-59./56 + (23.*eta)/84.;*/ -9. / 32. * a2;
			coeffs->rho21v3S = 1177. / 672. * a - 27. / 128. * a3;
			break;
		case 2:
			//coeffs->rho21v2 = -59. / 56 + (23. * eta) / 84.;
			coeffs->rho21v3S = 0.0;
			break;
		default:
			XLALPrintError("XLAL Error - %s: wrong SpinAlignedEOBversion value, must be 1 or 2!\n", __func__);
			XLAL_ERROR(XLAL_EINVAL);
			break;
		}
		/*
		 * coeffs->rho21v3   = (-567.*chiA*chiA*chiA -
		 * 1701.*chiA*chiA*chiS*dM + chiA*(-4708. + 1701.*chiS*chiS -
		 * 2648.*eta)*(-1. + 4.*eta) + chiS* dM3 *(4708. -
		 * 567.*chiS*chiS + 1816.*eta))/(2688.*dM3);
		 */
		coeffs->rho21v4S = /*- 47009./56448.*/ -(865. * a2) / 1792. - (405. * a2 * a2) / 2048.;
		//-(10993. * eta) / 14112.
			// +(617. * eta2) / 4704.;
		coeffs->rho21v5S = (-98635. * a) / 75264. + (2031. * a * a2) / 7168. - (1701. * a2 * a3) / 8192.;
		coeffs->rho21v6S = /* 7613184941./2607897600.;+ */ (9032393. * a2) / 1806336. + (3897. * a2 * a2) / 16384. - (15309. * a3 * a3) / 65536.;
		coeffs->rho21v6l = -107. / 105.;
		coeffs->rho21v7S = (-3859374457. * a) / 1159065600. - (55169. * a3) / 16384.
			+ (18603. * a2 * a3) / 65536. - (72171. * a2 * a2 * a3) / 262144.;
		coeffs->rho21v7lS = 107. * a / 140.;
		//coeffs->rho21v8 = -1168617463883. / 911303737344.;
		//coeffs->rho21v8l = 6313. / 5880.;
		//coeffs->rho21v10 = -63735873771463. / 16569158860800.;
		//coeffs->rho21v10l = 5029963. / 5927040.;

		coeffs->f21v1S = (-3. * (chiS + chiA / dM)) / (2.);
		switch (SpinAlignedEOBversion) {
		case 1:
			coeffs->f21v3S = 0.0;
			break;
		case 2:
			coeffs->f21v3S = (chiS * dM * (427. + 79. * eta) + chiA * (147. + 280. * dM * dM + 1251. * eta)) / 84. / dM;
			break;
		default:
			XLALPrintError("XLAL Error - %s: wrong SpinAlignedEOBversion value, must be 1 or 2!\n", __func__);
			XLAL_ERROR(XLAL_EINVAL);
			break;
		}
	} else {
		coeffs->f21v1S = -3. * chiA / 2.;
		switch (SpinAlignedEOBversion) {
		case 1:
			coeffs->f21v3S = 0.0;
			break;
		case 2:
			coeffs->f21v3S = (chiS * dM * (427. + 79. * eta) + chiA * (147. + 280. * dM * dM + 1251. * eta)) / 84.;
			break;
		default:
			XLALPrintError("XLAL Error - %s: wrong SpinAlignedEOBversion value, must be 1 or 2!\n", __func__);
			XLAL_ERROR(XLAL_EINVAL);
			break;
		}
	}

	/*
	 * l = 3, Eqs. A9a - A9c for rho, Eqs. A15b and A15c for f, Eqs. 22 -
	 * 24 of DIN and Eqs. 27c - 27e of PBFRT for delta
	 */
	if (dM2) {
		//coeffs->delta33vh3 = 13. / 10.;
		coeffs->delta33vh6S = (-81. * a) / 20.;
		//+(39. * LAL_PI) / 7.;
		//coeffs->delta33vh9 = -227827. / 3000. + (78. * LAL_PI * LAL_PI) / 7.;
		//coeffs->delta33v5 = -80897. * eta / 2430.;

		//coeffs->rho33v2 = -7. / 6. + (2. * eta) / 3.;
		//coeffs->rho33v3 = (chiS * dM * (-4. + 5. * eta) + chiA * (-4. + 19. * eta)) / (6. * dM);
		//coeffs->rho33v3 = 0.0;
		coeffs->rho33v4S = /*-6719./3960. +*/ a2 / 2.;
		//-(1861. * eta) / 990. + (149. * eta2) / 330.;
		coeffs->rho33v5S = (-4. * a) / 3.;
		coeffs->rho33v6S = /* 3203101567./227026800.; + */ (5. * a2) / 36.;
		//coeffs->rho33v6l = -26. / 7.;
		coeffs->rho33v7S = (5297. * a) / 2970. + a * a2 / 3.;
		//coeffs->rho33v8 = -57566572157. / 8562153600.;
		//coeffs->rho33v8l = 13. / 3.;

		coeffs->f33v3S = (chiS * dM * (-4. + 5. * eta) + chiA * (-4. + 19. * eta)) / (2. * dM);
	} else {
		coeffs->f33v3S = chiA * 3. / 8.;
	}

	//coeffs->delta32vh3 = (10. + 33. * eta) / (-15. * m1Plus3eta);
	coeffs->delta32vh4S = 4. * a;
	coeffs->delta32vh6S = (-136. * a) / 45.;
	//+(52. * LAL_PI) / 21.;
	//coeffs->delta32vh9 = -9112. / 405. + (208. * LAL_PI * LAL_PI) / 63.;

	coeffs->rho32vS = (4. * chiS * eta) / (-3. * m1Plus3eta);
	coeffs->rho32v2S = (-4. * a2 * eta2) / (9. * m1Plus3eta2);
	//+(328. - 1115. * eta
	    // +320. * eta2) / (270. * m1Plus3eta);
	coeffs->rho32v3S = (2. * (45. * a * m1Plus3eta3
	     - a * eta * (328. - 2099. * eta + 5. * (733. + 20. * a2) * eta2
			  - 960. * eta3))) / (405. * m1Plus3eta3);
	coeffs->rho32v4S = a2 / 3.;
	//+*/(-1444528.
	      // +8050045. * eta - 4725605. * eta2 - 20338960. * eta3
	      // +3085640. * eta2 * eta2) / (1603800. * m1Plus3eta2);
	coeffs->rho32v5S = (-2788. * a) / 1215.;
	coeffs->rho32v6S = /* 5849948554./940355325.;+ */ (488. * a2) / 405.;
	//coeffs->rho32v6l = -104. / 63.;
	//coeffs->rho32v8 = -10607269449358. / 3072140846775.;
	//coeffs->rho32v8l = 17056. / 8505.;

	if (dM2) {
		//coeffs->delta31vh3 = 13. / 30.;
		coeffs->delta31vh6S = (61. * a) / 20.;
		//+(13. * LAL_PI) / 21.;
		coeffs->delta31vh7S = (-24. * a2) / 5.;
		//coeffs->delta31vh9 = -227827. / 81000. + (26. * LAL_PI * LAL_PI) / 63.;
		//coeffs->delta31v5 = -17. * eta / 10.;

		//coeffs->rho31v2 = -13. / 18. - (2. * eta) / 9.;
		//coeffs->rho31v3 = (chiA * (-4. + 11. * eta) + chiS * dM * (-4. + 13. * eta)) / (6. * dM);
		//coeffs->rho31v3 = 0.0;
		coeffs->rho31v4S = /* 101./7128. - */ (5. * a2) / 6.;
		//-(1685. * eta) / 1782. - (829. * eta2) / 1782.;
		coeffs->rho31v5S = (4. * a) / 9.;
		coeffs->rho31v6S = /* 11706720301./6129723600.; - */ (49. * a2) / 108.;
		//coeffs->rho31v6l = -26. / 63.;
		coeffs->rho31v7S = (-2579. * a) / 5346. + a * a2 / 9.;
		//coeffs->rho31v8 = 2606097992581. / 4854741091200.;
		//coeffs->rho31v8l = 169. / 567.;

		coeffs->f31v3S = (chiA * (-4. + 11. * eta) + chiS * dM * (-4. + 13. * eta)) / (2. * dM);
	} else {
		coeffs->f31v3S = -chiA * 5. / 8.;
	}

	/*
	 * l = 4, Eqs. A10a - A10d for delta, Eq. A15d for f Eqs. 25 - 28 of
	 * DIN and Eqs. 27f - 27i of PBFRT for delta
	 */

	//coeffs->delta44vh3 = (112. + 219. * eta) / (-120. * m1Plus3eta);
	coeffs->delta44vh6S = (-464. * a) / 75.;
	//+(25136. * LAL_PI) / 3465.;

	//coeffs->rho44v2 = (1614. - 5870. * eta + 2625. * eta2) / (1320. * m1Plus3eta);
	coeffs->rho44v3S = (chiA * (10. - 39. * eta) * dM + chiS * (10. - 41. * eta
					+ 42. * eta2)) / (15. * m1Plus3eta);
	coeffs->rho44v4S = a2 / 2.;
	//+(-511573572.
	    // +2338945704. * eta - 313857376. * eta2 - 6733146000. * eta3
	    // +1252563795. * eta2 * eta2) / (317116800. * m1Plus3eta2);
	coeffs->rho44v5S = (-69. * a) / 55.;
	coeffs->rho44v6S = /* 16600939332793./1098809712000.; + */ (217. * a2) / 3960.;
	//coeffs->rho44v6l = -12568. / 3465.;

	if (dM2) {
		//coeffs->delta43vh3 = (486. + 4961. * eta) / (810. * (1. - 2. * eta));
		coeffs->delta43vh4S = (11. * a) / 4.;
		//coeffs->delta43vh6 = 1571. * LAL_PI / 385.;

		//coeffs->rho43v = (5. * (chiA - chiS * dM) * eta) / (8. * dM * (-1. + 2. * eta));
		//coeffs->rho43v = 0.0;
		//coeffs->rho43v2 = (222. - 547. * eta + 160. * eta2) / (176. * (-1. + 2. * eta));
		coeffs->rho43v4S = /*-6894273./7047040.; +*/ (3. * a2) / 8.;
		coeffs->rho43v5S = (-12113. * a) / 6160.;
		//coeffs->rho43v6 = 1664224207351. / 195343948800.;
		//coeffs->rho43v6l = -1571. / 770.;

		coeffs->f43vS = (5. * (chiA - chiS * dM) * eta) / (2. * dM * (-1. + 2. * eta));
	} else {
		coeffs->f43vS = -5. * chiA / 4.;
	}

	//coeffs->delta42vh3 = (7. * (1. + 6. * eta)) / (-15. * m1Plus3eta);
	coeffs->delta42vh6S = (212. * a) / 75.;
	//+(6284. * LAL_PI) / 3465.;

	//coeffs->rho42v2 = (1146. - 3530. * eta + 285. * eta2) / (1320. * m1Plus3eta);
	coeffs->rho42v3S = (chiA * (10. - 21. * eta) * dM + chiS * (10. - 59. * eta
					+ 78. * eta2)) / (15. * m1Plus3eta);
	coeffs->rho42v4S = a2 / 2.;
	//+(-114859044. + 295834536. * eta + 1204388696. * eta2 - 3047981160. * eta3
	    // -379526805. * eta2 * eta2) / (317116800. * m1Plus3eta2);
	coeffs->rho42v5S = (-7. * a) / 110.;
	coeffs->rho42v6S = /* 848238724511./219761942400.; + */ (2323. * a2) / 3960.;
	//coeffs->rho42v6l = -3142. / 3465.;

	if (dM2) {
		//coeffs->delta41vh3 = (2. + 507. * eta) / (10. * (1. - 2. * eta));
		coeffs->delta41vh4S = (11. * a) / 12.;
		//coeffs->delta41vh6 = 1571. * LAL_PI / 3465.;

		//coeffs->rho41v = (5. * (chiA - chiS * dM) * eta) / (8. * dM * (-1. + 2. * eta));
		//coeffs->rho41v = 0.0;
		//coeffs->rho41v2 = (602. - 1385. * eta + 288. * eta2) / (528. * (-1. + 2. * eta));
		coeffs->rho41v4S = /*-7775491./21141120.; +*/ (3. * a2) / 8.;
		coeffs->rho41v5S = (-20033. * a) / 55440. - (5 * a * a2) / 6.;
		//coeffs->rho41v6 = 1227423222031. / 1758095539200.;
		//coeffs->rho41v6l = -1571. / 6930.;

		coeffs->f41vS = (5. * (chiA - chiS * dM) * eta) / (2. * dM * (-1. + 2. * eta));
	} else {
		coeffs->f41vS = -5. * chiA / 4.;
	}

	/*
	 * l = 5, Eqs. A11a - A11e for rho, Eq. 29 of DIN and Eqs. E1a and
	 * E1b of PBFRT for delta
	 */
	if (dM2) {
		//coeffs->delta55vh3 = (96875. + 857528. * eta) / (131250. * (1 - 2 * eta));

		//coeffs->rho55v2 = (487. - 1298. * eta + 512. * eta2) / (390. * (-1. + 2. * eta));
		coeffs->rho55v3S = (-2. * a) / 3.;
		coeffs->rho55v4S = /*-3353747./2129400.; +*/ a2 / 2.;
		coeffs->rho55v5S = -241. * a / 195.;
	}
	coeffs->delta54vh3 = 8. / 15.;
	coeffs->delta54vh4S = 12. * a / 5.;

	//coeffs->rho54v2 = (-17448. + 96019. * eta - 127610. * eta2
		// +33320. * eta3) / (13650. * (1. - 5. * eta + 5. * eta2));
	coeffs->rho54v3S = (-2. * a) / 15.;
	coeffs->rho54v4S = /*-16213384./15526875.; +*/ (2. * a2) / 5.;

	if (dM2) {
		//coeffs->delta53vh3 = 31. / 70.;

		//coeffs->rho53v2 = (375. - 850. * eta + 176. * eta2) / (390. * (-1. + 2. * eta));
		coeffs->rho53v3S = (-2. * a) / 3.;
		coeffs->rho53v4S = /*-410833./709800.; +*/ a2 / 2.;
		coeffs->rho53v5S = -103. * a / 325.;
	}
	//coeffs->delta52vh3 = 4. / 15.;
	coeffs->delta52vh4S = 6. * a / 5.;

	//coeffs->rho52v2 = (-15828. + 84679. * eta - 104930. * eta2
		// +21980. * eta3) / (13650. * (1. - 5. * eta + 5. * eta2));
	coeffs->rho52v3S = (-2. * a) / 15.;
	coeffs->rho52v4S = /*-7187914./15526875.; +*/ (2. * a2) / 5.;

	if (dM2) {
		//coeffs->delta51vh3 = 31. / 210.;

		//coeffs->rho51v2 = (319. - 626. * eta + 8. * eta2) / (390. * (-1. + 2. * eta));
		coeffs->rho51v3S = (-2. * a) / 3.;
		coeffs->rho51v4S = /*-31877./304200.; +*/ a2 / 2.;
		coeffs->rho51v5S = 139. * a / 975.;
	}
	/*
	 * l = 6, Eqs. A12a - A12f for rho, Eqs. E1c and E1d of PBFRT for
	 * delta
	 */

	//coeffs->delta66vh3 = 43. / 70.;

	//coeffs->rho66v2 = (-106. + 602. * eta - 861. * eta2
		     // +273. * eta3) / (84. * (1. - 5. * eta + 5. * eta2));
	coeffs->rho66v3S = (-2. * a) / 3.;
	coeffs->rho66v4S = /*-1025435./659736.; +*/ a2 / 2.;

	if (dM2) {
		//coeffs->delta65vh3 = 10. / 21.;

		//coeffs->rho65v2 = (-185. + 838. * eta - 910. * eta2
			      // +220. * eta3) / (144. * (dM2 + 3. * eta2));
		coeffs->rho65v3 = -2. * a / 9.;
	}
	//coeffs->delta64vh3 = 43. / 105.;

	//coeffs->rho64v2 = (-86. + 462. * eta - 581. * eta2
		     // +133. * eta3) / (84. * (1. - 5. * eta + 5. * eta2));
	coeffs->rho64v3S = (-2. * a) / 3.;
	coeffs->rho64v4S = /*-476887./659736.; +*/ a2 / 2.;

	if (dM2) {
		//coeffs->delta63vh3 = 2. / 7.;

		//coeffs->rho63v2 = (-169. + 742. * eta - 750. * eta2
			      // +156. * eta3) / (144. * (dM2 + 3. * eta2));
		coeffs->rho63v3S = -2. * a / 9.;
	}
	//coeffs->delta62vh3 = 43. / 210.;

	//coeffs->rho62v2 = (-74. + 378. * eta - 413. * eta2
		      // +49. * eta3) / (84. * (1. - 5. * eta + 5. * eta2));
	coeffs->rho62v3S = (-2. * a) / 3.;
	coeffs->rho62v4S = /*-817991./3298680.; +*/ a2 / 2.;

	if (dM2) {
		//coeffs->delta61vh3 = 2. / 21.;

		//coeffs->rho61v2 = (-161. + 694. * eta - 670. * eta2
			      // +124. * eta3) / (144. * (dM2 + 3. * eta2));
		coeffs->rho61v3S = -2. * a / 9.;
	}
	/*
	 * l = 7, Eqs. A13a - A13g for rho, Eqs. E1e and E1f of PBFRT for
	 * delta
	 */
	if (dM2) {
		//coeffs->delta77vh3 = 19. / 36.;

		//coeffs->rho77v2 = (-906. + 4246. * eta - 4963. * eta2
			     // +1380. * eta3) / (714. * (dM2 + 3. * eta2));
		coeffs->rho77v3S = -2. * a / 3.;
	}
	//coeffs->rho76v2 = (2144. - 16185. * eta + 37828. * eta2 - 29351. * eta3
	       // +6104. * eta2 * eta2) / (1666. * (-1 + 7 * eta - 14 * eta2
						    // +7 * eta3));

	if (dM2) {
		//coeffs->delta75vh3 = 95. / 252.;

		//coeffs->rho75v2 = (-762. + 3382. * eta - 3523. * eta2
			      // +804. * eta3) / (714. * (dM2 + 3. * eta2));
		coeffs->rho75v3S = -2. * a / 3.;
	}
	//coeffs->rho74v2 = (17756. - 131805. * eta + 298872. * eta2 - 217959. * eta3
	  // +41076. * eta2 * eta2) / (14994. * (-1. + 7. * eta - 14. * eta2
						 // +7. * eta3));

	if (dM2) {
		//coeffs->delta73vh3 = 19. / 84.;

		//coeffs->rho73v2 = (-666. + 2806. * eta - 2563. * eta2
			      // +420. * eta3) / (714. * (dM2 + 3. * eta2));
		coeffs->rho73v3S = -2. * a / 3.;
	}
	//coeffs->rho72v2 = (16832. - 123489. * eta + 273924. * eta2 - 190239. * eta3
	  // +32760. * eta2 * eta2) / (14994. * (-1. + 7. * eta - 14. * eta2
						 // +7. * eta3));

	if (dM2) {
		//coeffs->delta71vh3 = 19. / 252.;

		//coeffs->rho71v2 = (-618. + 2518. * eta - 2083. * eta2
			      // +228. * eta3) / (714. * (dM2 + 3. * eta2));
		coeffs->rho71v3S = -2. * a / 3.;
	}
	/* l = 8, Eqs. A14a - A14h */
	/*
	 * coeffs->rho88v2 = (3482. - 26778.*eta + 64659.*eta2 - 53445.*eta3
	 * + 12243.*eta2*eta2) / (2736.*(-1. + 7.*eta - 14.*eta2 + 7.*eta3));
	 * 
	 * if ( dM2 ) { coeffs->rho87v2 = (23478. - 154099.*eta + 309498.*eta2 -
	 * 207550.*eta3 + 38920*eta2*eta2) / (18240.*(-1 + 6*eta - 10*eta2 +
	 * 4*eta3)); }
	 * 
	 * coeffs->rho86v2 = (1002. - 7498.*eta + 17269.*eta2 - 13055.*eta3 +
	 * 2653.*eta2*eta2) / (912.*(-1. + 7.*eta - 14.*eta2 + 7.*eta3));
	 * 
	 * if ( dM2 ) { coeffs->rho85v2 = (4350. - 28055.*eta + 54642.*eta2 -
	 * 34598.*eta3 + 6056.*eta2*eta2) / (3648.*(-1. + 6.*eta - 10.*eta2 +
	 * 4.*eta3)); }
	 * 
	 * coeffs->rho84v2 = (2666. - 19434.*eta + 42627.*eta2 - 28965.*eta3 +
	 * 4899.*eta2*eta2) / (2736.*(-1. + 7.*eta - 14.*eta2 + 7.*eta3));
	 * 
	 * if ( dM2 ) { coeffs->rho83v2 = (20598. - 131059.*eta + 249018.*eta2 -
	 * 149950.*eta3 + 24520.*eta2*eta2) / (18240.*(-1. + 6.*eta -
	 * 10.*eta2 + 4.*eta3)); }
	 * 
	 * coeffs->rho82v2 = (2462. - 17598.*eta + 37119.*eta2 - 22845.*eta3 +
	 * 3063.*eta2*eta2) / (2736.*(-1. + 7.*eta - 14.*eta2 + 7.*eta3));
	 * 
	 * if ( dM2 ) { coeffs->rho81v2 = (20022. - 126451.*eta + 236922.*eta2 -
	 * 138430.*eta3 + 21640.*eta2*eta2) / (18240.*(-1. + 6.*eta -
	 * 10.*eta2 + 4.*eta3)); }
	 */
	/* All relevant coefficients should be set, so we return */

	return XLAL_SUCCESS;
}

#endif
