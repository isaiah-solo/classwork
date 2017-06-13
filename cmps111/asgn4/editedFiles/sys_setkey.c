#include <sys/sysproto.h>
#include <sys/proc.h>
#include <sys/vnode.h>
#include <sys/types.h>
#include <sys/systm.h>

#define KEYBITS 128
#define KEYLENGTH(keybits) ((keybits)/8)

#ifndef _SYS_SYSPROTO_H_
struct setkey_args {
  unsigned int k0;
  unsigned int k1;
};
#endif

int sys_setkey(struct thread *td, struct setkey_args *args)
{
  struct ucred *cred = td->td_proc->p_ucred;
  uid_t id = cred->cr_uid;
  unsigned int k0 = args->k0;
  unsigned int k1 = args->k1;
  unsigned char key[KEYLENGTH(KEYBITS)];
  bzero (key, sizeof (key));
  bcopy (&k0, &(key[0]), sizeof (k0));
  bcopy (&k1, &(key[sizeof(k0)]), sizeof (k1));

  /** Both keys supplied were 0, so disable */
  if (k0 == 0 && k1 == 0) {
    cred->k0 = 0;
    cred->k1 = 0;
    return 0;
  }

  /** Sets key */
  cred->k0 = k0;
  cred->k1 = k1;

  printf("Key = ");
  for (int i = 0; i < sizeof (key); i++) {
    printf ("%02x", key[sizeof(key)-i-1]);
  }
  printf("\nUser id = %d\n", id);

  return 0;
}
