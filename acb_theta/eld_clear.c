
#include "acb_theta.h"

void acb_theta_eld_clear(acb_theta_eld_t E)
{
  slong k;
  slong nr = acb_theta_eld_nr(E);
  slong nl = acb_theta_eld_nl(E);
  
  if (nr > 0)
    {
      for (k = 0; k < nr; k++) acb_theta_eld_clear(acb_theta_eld_rchild(E, k));
      flint_free(E->rchildren);
    }
  if (nl > 0)
    {
      for (k = 0; k < nl; k++) acb_theta_eld_clear(acb_theta_eld_lchild(E, k));
      flint_free(E->lchildren);
    }

  flint_free(E->last_coords);
  _arb_vec_clear(acb_theta_eld_offset(E), acb_theta_eld_dim(E));
  arb_clear(acb_theta_eld_normsqr(E));
  arb_clear(acb_theta_eld_rad(E));
  arb_clear(acb_theta_eld_ctr(E));
  flint_free(E->box);
}
