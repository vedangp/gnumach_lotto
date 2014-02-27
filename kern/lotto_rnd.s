/*
 * Project:	Mach Lottery Scheduler (Kernel Modifications)
 * Module:	Fast Random Number Generator
 * Author:	Carl Waldspurger
 * History:
 *
 *	27-Apr-94	Copied from PSG runtime system.  I originally
 *			developed this code for the runtime scheduler
 *			way back in 1991.
 *
 * Overview:
 *
 *   This module is a MIPS assembly-language implementation of the ACM
 *   "Minimal Standard Random Number Generator".  The implementation
 *   is based on:
 *	
 *	Park, S.K. and Miller, K.W.
 *      "Random Number Generators: Good Ones Are Hard To Find",
 *      CACM vol. 31, no. 10, October 1988.
 *
 *	Carta, D.G.
 *      "Two Fast Implementations of the 'Minimal Standard' Random Number Gen",
 *      CACM vol. 33, no. 1, January 1990.
 *
 */

/*
 * Called from C as:
 *
 *    unsigned vfast_random(unsigned state);
 *
 *       For details of the underlying implementation, see the articles
 *       cited in the overview.  Basically, the following code implements
 *       the multiplicative linear congruential form: 
 *    
 *                    S' = 16807 * S mod (2^31 - 1)
 *            
 */

      .globl  vfast_random
      .align  2
      .ent    vfast_random
      .set    noat

vfast_random:
      
      move    $2, $4                  /* load old S into $2 */
      li      $8, 33614               /* $8 = 2 x constant A */
      multu   $2, $8                  /* LO, HI = A x S */
      mflo    $9                      /* $9  = Q = bits00..31 of A x S */
      srl     $9, $9, 1               
      mfhi    $10                     /* $10 = P = bits32..63 of A x S */
      addu    $2, $9, $10             /* $2  = new S = P + Q */
      bltz    $2, voverflow
      j       $31                     /* return (result in $2) */

voverflow:
      sll     $2, $2, 1               /* zero high order bit  of $2 */
      srl     $2, $2, 1
      addiu   $2, 1                   /* add 1 */
      j       $31                     /* return (result in $2) */

      .end    vfast_random
